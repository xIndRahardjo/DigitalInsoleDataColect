#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"

// MAC Address ESP32 Receiver Anda
uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0x14, 0xA1, 0x50};

// Struktur Data Pengiriman ESP-NOW
typedef struct struct_message {
  float temperature;
  float humidity;
} struct_message;

struct_message shtData;
esp_now_peer_info_t peerInfo;

// Inisialisasi objek sensor SHT31
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Callback status pengiriman (Kompatibel ESP32 Core v3.x)
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Status ESP-NOW: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("SUCCESS (Terkirim ke Receiver)");
  } else {
    Serial.println("FAIL (Gagal Terkirim)");
  }
}

void setup() {
  // Memulai komunikasi Serial
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\nPengujian & Pengiriman Sensor SHT31 via ESP-NOW");
  
  // Inisialisasi pin I2C ESP32 (SDA = 21, SCL = 22)
  Wire.begin();

  // Memulai sensor di alamat 0x44 (alamat default SHT31)
  if (!sht31.begin(0x44)) {
    Serial.println("Sensor SHT31 tidak terdeteksi. Cek kabel Anda!");
    while (1) delay(1); // Berhenti di sini jika sensor gagal diinisialisasi
  }
  
  Serial.println("SHT31 berhasil terhubung!");

  // Set Mode Wi-Fi STA & Kunci ke Channel 1
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  // Inisialisasi ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal Inisialisasi ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // Registrasi Peer Receiver
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Gagal menambahkan Peer ESP-NOW!");
    return;
  }

  Serial.println("Sistem Pengirim Siap!\n");
}

void loop() {
  // Membaca suhu dan kelembapan
  float suhu = sht31.readTemperature();
  float kelembapan = sht31.readHumidity();

  // Mengecek apakah pembacaan suhu dan kelembapan berhasil (bukan NaN)
  if (!isnan(suhu) && !isnan(kelembapan)) {
    // Tampilkan log lokal di Serial Monitor
    Serial.print("Suhu: ");
    Serial.print(suhu, 2);
    Serial.print(" °C\t");
    Serial.print("Kelembapan: ");
    Serial.print(kelembapan, 2);
    Serial.print(" %RH\t| ");

    // Masukkan data ke struct
    shtData.temperature = suhu;
    shtData.humidity = kelembapan;

    // Kirim data via ESP-NOW ke ESP32 #2
    esp_now_send(broadcastAddress, (uint8_t *)&shtData, sizeof(shtData));
  } else {
    Serial.println("Gagal membaca suhu/kelembapan dari SHT31!");
  }

  // Tunggu 2 detik sebelum membaca & mengirim kembali
  delay(2000);
}