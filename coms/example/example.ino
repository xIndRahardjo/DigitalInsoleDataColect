#include <WiFi.h>
#include <esp_now.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>
#include <Wire.h>
#include "MAX30105.h"

#define SD_CS 5

// Struktur data dari ESP32 #1
typedef struct struct_message {
  float temperature;
  float humidity;
} struct_message;

struct_message incomingShtData;
MAX30105 particleSensor;

bool newDataReceived = false;

// Callback fungsi saat data ESP-NOW diterima
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&incomingShtData, incomingData, sizeof(incomingShtData));
  newDataReceived = true;
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // 1. Inisialisasi SD Card
  if (!SD.begin(SD_CS)) {
    Serial.println("Gagal Inisialisasi Modul SD Card!");
    return;
  }

  // Buat Header CSV jika file belum ada
  if (!SD.exists("/data_log.csv")) {
    File file = SD.open("/data_log.csv", FILE_WRITE);
    if (file) {
      file.println("Waktu_ms,Suhu_SHT31,Kelembapan_SHT31,IR_MAX30105,Red_MAX30105");
      file.close();
    }
  }

  // 2. Inisialisasi Sensor MAX30105
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 tidak terdeteksi!");
    while (1);
  }
  particleSensor.setup(); // Konfigurasi default sensor

  // 3. Inisialisasi ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal Inisialisasi ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Jika ada data masuk dari ESP32 #1
  if (newDataReceived) {
    newDataReceived = false;

    // Baca data lokal dari MAX30105
    uint32_t irValue = particleSensor.getIR();
    uint32_t redValue = particleSensor.getRed();

    // Format baris CSV
    String dataRow = String(millis()) + "," +
                     String(incomingShtData.temperature, 2) + "," +
                     String(incomingShtData.humidity, 2) + "," +
                     String(irValue) + "," +
                     String(redValue);

    // Simpan ke SD Card
    File file = SD.open("/data_log.csv", FILE_APPEND);
    if (file) {
      file.println(dataRow);
      file.close();
      Serial.println("Data Tersimpan: " + dataRow);
    } else {
      Serial.println("Gagal menyimpan ke SD Card!");
    }
  }
}