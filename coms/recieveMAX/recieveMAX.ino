#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "heartRate.h"

#define SD_CS 5

// Struktur Data ESP-NOW dari ESP32 #1
typedef struct struct_message {
  float temperature;
  float humidity;
} struct_message;

struct_message incomingShtData;

// Objek Sensor MAX30105
MAX30105 particleSensor;

// Variabel BPM
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
int beatAvg = 0;

unsigned long sampleCounter = 0;
unsigned long lastBeatSample = 0;
unsigned long lastValidBpmSample = 0;

// Variabel SpO2
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

// Variabel Plotter & Log Timing
float dcFilter = 0;
float wavePlot = 0;
unsigned long lastSdLogTime = 0;

// Callback Penerimaan ESP-NOW (Sesuai ESP32 Core v3.x)
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingShtData, incomingData, sizeof(incomingShtData));
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  // 1. INISIALISASI SD CARD
  Serial.println("Menginisialisasi SD Card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("Gagal Inisialisasi Modul SD Card!");
  } else {
    Serial.println("SD Card Berhasil Dihubungkan.");
    
    // Buat header CSV jika file belum ada
    if (!SD.exists("/data_kesehatan.csv")) {
      File file = SD.open("/data_kesehatan.csv", FILE_WRITE);
      if (file) {
        file.println("Waktu_ms,BPM,SpO2,Suhu_SHT31,Kelembapan_SHT31");
        file.close();
      }
    }
  }

  // 2. INISIALISASI SENSOR MAX30105
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("Sensor MAX30105 tidak terdeteksi.");
    while (1);
  }

  byte ledBrightness = 0x1F;
  byte sampleAverage = 4;
  byte ledMode = 3;
  int sampleRate = 100;
  int pulseWidth = 411;
  int adcRange = 4096;

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

  // 3. INISIALISASI ESP-NOW
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // Kunci ke Channel 1

  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal Inisialisasi ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP32 Receiver Siap Menunggu Data...");
}

void loop() {
  while (!particleSensor.available()) {
    particleSensor.check();
  }

  long currentRed = particleSensor.getRed();
  long currentIR = particleSensor.getIR();
  particleSensor.nextSample();

  sampleCounter++;

  if (currentIR > 20000) {

    // 1. DETEKSI DETAK JANTUNG (BPM)
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

    // 2. KUMPULKAN DATA SPO2
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

    // TIMEOUT MEMORY HOLD
    if (sampleCounter - lastValidBpmSample > 400) beatAvg = 0;
    if (sampleCounter - lastValidSpO2Sample > 400) lastValidSpO2 = 0;

    // 3. VISUALISASI GELOMBANG PLOTTER
    if (dcFilter == 0) dcFilter = currentIR; 
    dcFilter = (0.95 * dcFilter) + (0.05 * currentIR);
    wavePlot = currentIR - dcFilter;

    // OUTPUT SERIAL PLOTTER
    Serial.print("Gelombang:");
    Serial.print(wavePlot);
    Serial.print(",BPM:");
    Serial.print(beatAvg);
    Serial.print(",SpO2:");
    Serial.print(lastValidSpO2);
    Serial.print(",SuhuSHT:");
    Serial.print(incomingShtData.temperature);
    Serial.print(",LembapSHT:");
    Serial.println(incomingShtData.humidity);

  } else {
    // Reset parameter saat jari dilepas
    bufferIndex = 0;
    lastValidSpO2 = 0;
    beatAvg = 0;
    dcFilter = 0; 
    wavePlot = 0;
    
    Serial.print("Gelombang:0,BPM:0,SpO2:0,SuhuSHT:");
    Serial.print(incomingShtData.temperature);
    Serial.print(",LembapSHT:");
    Serial.println(incomingShtData.humidity);
  }

  // 4. PENULISAN PERIODIK KE SD CARD (Setiap 1 Detik)
  if (millis() - lastSdLogTime >= 1000) {
    lastSdLogTime = millis();

    File file = SD.open("/data_kesehatan.csv", FILE_APPEND);
    if (file) {
      String dataRow = String(millis()) + "," +
                       String(beatAvg) + "," +
                       String(lastValidSpO2) + "," +
                       String(incomingShtData.temperature, 2) + "," +
                       String(incomingShtData.humidity, 2);
      file.println(dataRow);
      file.close();
    }
  }
}