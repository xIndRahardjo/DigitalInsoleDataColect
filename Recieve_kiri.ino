#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "heartRate.h"

// Pin Modul SD Card Reader
#define SD_CS 5

// Pin ADC Sensor FSR402 Kaki Kiri
#define FSR_L1_PIN 32
#define FSR_L2_PIN 33
#define FSR_L3_PIN 34
#define FSR_L4_PIN 35
#define FSR_L5_PIN 36 // VP

// Struktur Data ESP-NOW dari Kaki Kanan
typedef struct struct_kaki_kanan {
  uint16_t fsr[5];
  float temperature;
  float humidity;
} struct_kaki_kanan;

struct_kaki_kanan dataKakiKanan;
volatile bool newDataFromRight = false;
unsigned long lastPacketRightTime = 0;

// Objek Sensor Lokal Kaki Kiri
Adafruit_SHT31 sht31Kiri = Adafruit_SHT31();
MAX30105 particleSensor;

// Variabel MAX30105 (BPM & SpO2)
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
int beatAvg = 0;
unsigned long sampleCounter = 0;
unsigned long lastBeatSample = 0;
unsigned long lastValidBpmSample = 0;

uint32_t irBuffer[100];
uint32_t redBuffer[100];
int32_t bufferLength = 100;
int32_t spo2 = 0;
int8_t validSPO2 = 0;
int32_t dummyHR = 0;
int8_t validDummyHR = 0;
byte bufferIndex = 0;
int32_t lastValidSpO2 = 0;
unsigned long lastValidSpO2Sample = 0;

// Timing SD Logger
unsigned long lastSdLogTime = 0;
bool sdInitialized = false;

// Callback Penerimaan ESP-NOW dari Kaki Kanan
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(dataKakiKanan)) {
    memcpy(&dataKakiKanan, incomingData, sizeof(dataKakiKanan));
    newDataFromRight = true;
    lastPacketRightTime = millis();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  Serial.println("\n=== DIAFOS V2 - RECEIVER & LOGGER KAKI KIRI ===");

  // 1. Inisialisasi SD Card Reader
  Serial.print("[SD CARD] Menginisialisasi Modul SD Card CS Pin ");
  Serial.println(SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("[ERROR] Gagal Inisialisasi SD Card! Periksa kabel/kartu SD.");
  } else {
    sdInitialized = true;
    Serial.println("[OK] SD Card Berhasil Dihubungkan.");

    // Buat header CSV jika belum ada
    if (!SD.exists("/data_pengujian_gabungan.csv")) {
      File file = SD.open("/data_pengujian_gabungan.csv", FILE_WRITE);
      if (file) {
        file.println("Millis,BPM,SpO2,Temp_L,Hum_L,FSR_L1,FSR_L2,FSR_L3,FSR_L4,FSR_L5,Temp_R,Hum_R,FSR_R1,FSR_R2,FSR_R3,FSR_R4,FSR_R5,Status_Kanan");
        file.close();
        Serial.println("[SD CARD] File '/data_pengujian_gabungan.csv' Dibuat (Header ditulis).");
      }
    }
  }

  // 2. Inisialisasi Sensor SHT31 Kaki Kiri
  if (!sht31Kiri.begin(0x44)) {
    Serial.println("[WARN] Sensor SHT31 Kaki Kiri tidak terdeteksi di 0x44!");
  } else {
    Serial.println("[OK] Sensor SHT31 Kaki Kiri Terhubung.");
  }

  // 3. Inisialisasi Sensor MAX30105
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("[WARN] Sensor MAX30105 tidak terdeteksi.");
  } else {
    byte ledBrightness = 0x1F;
    byte sampleAverage = 4;
    byte ledMode = 3;
    int sampleRate = 100;
    int pulseWidth = 411;
    int adcRange = 4096;
    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
    Serial.println("[OK] Sensor MAX30105 Berhasil Dikonfigurasi.");
  }

  // 4. Inisialisasi ESP-NOW Receiver
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // Kunci ke Channel 1

  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERROR] Gagal Inisialisasi ESP-NOW di Kaki Kiri!");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("[OK] ESP-NOW Receiver Kaki Kiri Siap Menunggu Data Kaki Kanan...\n");
}

void loop() {
  // A. Pembacaan Sensor MAX30105 (Setiap Sample)
  if (particleSensor.available()) {
    long currentRed = particleSensor.getRed();
    long currentIR = particleSensor.getIR();
    particleSensor.nextSample();
    sampleCounter++;

    if (currentIR > 20000) {
      // Deteksi BPM
      if (checkForBeat(currentIR) == true) {
        long samplesPassed = sampleCounter - lastBeatSample;
        lastBeatSample = sampleCounter;
        if (samplesPassed > 30) {
          float bpm = 60.0 / (samplesPassed * 0.01);
          if (bpm > 40 && bpm < 160) {
            if (beatAvg == 0) {
              for (byte x = 0; x < RATE_SIZE; x++) rates[x] = (byte)bpm;
            } else {
              rates[rateSpot++] = (byte)bpm;
              rateSpot %= RATE_SIZE;
            }
            beatAvg = 0;
            for (byte x = 0; x < RATE_SIZE; x++) beatAvg += rates[x];
            beatAvg /= RATE_SIZE;
            lastValidBpmSample = sampleCounter;
          }
        }
      }

      // Deteksi SpO2
      irBuffer[bufferIndex] = currentIR;
      redBuffer[bufferIndex] = currentRed;
      bufferIndex++;

      if (bufferIndex == 100) {
        maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &dummyHR, &validDummyHR);
        for (byte i = 25; i < 100; i++) {
          irBuffer[i - 25] = irBuffer[i];
          redBuffer[i - 25] = redBuffer[i];
        }
        bufferIndex = 75;

        if (validSPO2 == 1 && spo2 > 70 && spo2 <= 100) {
          lastValidSpO2 = spo2;
          lastValidSpO2Sample = sampleCounter;
        }
      }
    } else {
      // Jari tidak menempel
      bufferIndex = 0;
      lastValidSpO2 = 0;
      beatAvg = 0;
    }

    // Timeout clearing
    if (sampleCounter - lastValidBpmSample > 400) beatAvg = 0;
    if (sampleCounter - lastValidSpO2Sample > 400) lastValidSpO2 = 0;
  }

  // B. Penulisan Log Periodik ke SD Card & Serial (Setiap 1000 ms)
  if (millis() - lastSdLogTime >= 1000) {
    lastSdLogTime = millis();

    // 1. Baca Sensor Lokal Kaki Kiri
    uint16_t fsrL[5];
    fsrL[0] = analogRead(FSR_L1_PIN);
    fsrL[1] = analogRead(FSR_L2_PIN);
    fsrL[2] = analogRead(FSR_L3_PIN);
    fsrL[3] = analogRead(FSR_L4_PIN);
    fsrL[4] = analogRead(FSR_L5_PIN);

    float tempL = sht31Kiri.readTemperature();
    float humL  = sht31Kiri.readHumidity();
    if (isnan(tempL)) tempL = 0.0;
    if (isnan(humL))  humL  = 0.0;

    // 2. Cek Koneksi Kaki Kanan (Timeout 3 Detik)
    bool isRightConnected = (millis() - lastPacketRightTime < 3000);
    String statusKanan = isRightConnected ? "ONLINE" : "OFFLINE";

    // 3. Tampilkan Summary Log di Serial Monitor
    Serial.print("[LOG 1s] ");
    Serial.print("BPM:"); Serial.print(beatAvg);
    Serial.print(" | SpO2:"); Serial.print(lastValidSpO2);
    Serial.print("% | Kiri(T:"); Serial.print(tempL, 1);
    Serial.print("C, H:"); Serial.print(humL, 1);
    Serial.print("%) | Kanan ["); Serial.print(statusKanan);
    Serial.print("](T:"); Serial.print(dataKakiKanan.temperature, 1);
    Serial.print("C, H:"); Serial.print(dataKakiKanan.humidity, 1);
    Serial.println("%)");

    // 4. Simpan ke File CSV SD Card
    if (sdInitialized) {
      File file = SD.open("/data_pengujian_gabungan.csv", FILE_APPEND);
      if (file) {
        String dataRow = String(millis()) + "," +
                         String(beatAvg) + "," +
                         String(lastValidSpO2) + "," +
                         String(tempL, 2) + "," +
                         String(humL, 2) + "," +
                         String(fsrL[0]) + "," + String(fsrL[1]) + "," + String(fsrL[2]) + "," + String(fsrL[3]) + "," + String(fsrL[4]) + "," +
                         String(dataKakiKanan.temperature, 2) + "," +
                         String(dataKakiKanan.humidity, 2) + "," +
                         String(dataKakiKanan.fsr[0]) + "," + String(dataKakiKanan.fsr[1]) + "," + String(dataKakiKanan.fsr[2]) + "," + String(dataKakiKanan.fsr[3]) + "," + String(dataKakiKanan.fsr[4]) + "," +
                         statusKanan;
        
        file.println(dataRow);
        file.close();
        Serial.println("   --> [SD CARD] Berhasil menyimpan baris log.");
      } else {
        Serial.println("   --> [ERROR SD CARD] Gagal membuka file /data_pengujian_gabungan.csv!");
      }
    }
  }
}
