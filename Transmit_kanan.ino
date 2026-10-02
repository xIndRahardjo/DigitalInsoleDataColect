#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

// MAC Address ESP32 Kaki Kiri (Receiver)
uint8_t receiverMacAddress[] = {0xEC, 0xE3, 0x34, 0x14, 0xA1, 0x50}; // Silakan ganti dengan MAC Address ESP32 Kaki Kiri Anda!

// Pin ADC Sensor FSR402 Kaki Kanan
#define FSR_R1_PIN 32
#define FSR_R2_PIN 33
#define FSR_R3_PIN 34
#define FSR_R4_PIN 35
#define FSR_R5_PIN 36 // VP

// Struktur Data ESP-NOW Kaki Kanan
typedef struct struct_kaki_kanan {
  uint16_t fsr[5];    // Raw ADC FSR 1 - 5 Kaki Kanan
  float temperature;  // Suhu SHT31 Kaki Kanan (°C)
  float humidity;     // Kelembapan SHT31 Kaki Kanan (%RH)
} struct_kaki_kanan;

struct_kaki_kanan dataKakiKanan;
esp_now_peer_info_t peerInfo;

// Objek SHT31 Kaki Kanan
Adafruit_SHT31 sht31Kanan = Adafruit_SHT31();

// Callback status pengiriman ESP-NOW (Kompatibel ESP32 Core v3.x)
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("[ESP-NOW KANAN] Status Pengiriman: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("SUCCESS -> Terkirim ke Kaki Kiri");
  } else {
    Serial.println("FAIL -> Gagal (Receiver Mati/Beda Channel)");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== DIAFOS V2 - TRANSMITTER KAKI KANAN ===");

  // Inisialisasi Bus I2C & Sensor SHT31
  Wire.begin();
  if (!sht31Kanan.begin(0x44)) {
    Serial.println("[WARN] Sensor SHT31 Kaki Kanan tidak terdeteksi di 0x44!");
  } else {
    Serial.println("[OK] Sensor SHT31 Kaki Kanan Terhubung.");
  }

  // Inisialisasi Wi-Fi STA & ESP-NOW
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE); // Kunci ke Channel 1

  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERROR] Gagal Inisialisasi ESP-NOW di Kaki Kanan!");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Daftarkan Peer ESP32 Kaki Kiri
  memcpy(peerInfo.peer_addr, receiverMacAddress, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ERROR] Gagal menambahkan Peer Kaki Kiri!");
    return;
  }

  Serial.println("[OK] Kaki Kanan Siap Mengirim Data Kontinu...\n");
}

void loop() {
  // 1. Baca Sensor FSR402 Kaki Kanan (5 Titik)
  dataKakiKanan.fsr[0] = analogRead(FSR_R1_PIN);
  dataKakiKanan.fsr[1] = analogRead(FSR_R2_PIN);
  dataKakiKanan.fsr[2] = analogRead(FSR_R3_PIN);
  dataKakiKanan.fsr[3] = analogRead(FSR_R4_PIN);
  dataKakiKanan.fsr[4] = analogRead(FSR_R5_PIN);

  // 2. Baca Sensor SHT31 Kaki Kanan
  float t = sht31Kanan.readTemperature();
  float h = sht31Kanan.readHumidity();

  dataKakiKanan.temperature = isnan(t) ? 0.0 : t;
  dataKakiKanan.humidity    = isnan(h) ? 0.0 : h;

  // 3. Kirim via ESP-NOW ke Kaki Kiri
  esp_now_send(receiverMacAddress, (uint8_t *)&dataKakiKanan, sizeof(dataKakiKanan));

  // Tampilkan Log Serial Lokal
  Serial.print("FSR R:[ ");
  for(int i=0; i<5; i++){
    Serial.print(dataKakiKanan.fsr[i]); Serial.print(" ");
  }
  Serial.print("] | Suhu R: "); Serial.print(dataKakiKanan.temperature, 2);
  Serial.print("°C | Lembap R: "); Serial.print(dataKakiKanan.humidity, 2);
  Serial.println("%RH");

  delay(200); // Frekuensi pengiriman ~5Hz
}
