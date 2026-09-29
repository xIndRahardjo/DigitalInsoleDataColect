#include <Wire.h>
#include "Adafruit_SHT31.h"

// Inisialisasi objek sensor SHT31
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  // Memulai komunikasi Serial
  Serial.begin(115200);
  while (!Serial) delay(10); // Tunggu hingga Serial Monitor siap
  
  Serial.println("\nPengujian Sensor SHT31");
  
  // Inisialisasi pin I2C ESP32 (SDA = 21, SCL = 22)
  Wire.begin();

  // Memulai sensor di alamat 0x44 (alamat default SHT31)
  // Jika I2C scanner mendeteksi 0x45, ubah angka di bawah menjadi 0x45
  if (!sht31.begin(0x44)) {
    Serial.println("Sensor SHT31 tidak terdeteksi. Cek kabel Anda!");
    while (1) delay(1); // Berhenti di sini jika sensor gagal diinisialisasi
  }
  
  Serial.println("SHT31 berhasil terhubung!");
}

void loop() {
  // Membaca suhu dan kelembapan
  float suhu = sht31.readTemperature();
  float kelembapan = sht31.readHumidity();

  // Mengecek apakah pembacaan suhu berhasil (bukan NaN / Not a Number)
  if (!isnan(suhu)) {
    Serial.print("Suhu: ");
    Serial.print(suhu, 2); // 2 angka di belakang koma
    Serial.print(" °C\t");
  } else {
    Serial.print("Gagal membaca suhu!\t");
  }

  // Mengecek apakah pembacaan kelembapan berhasil
  if (!isnan(kelembapan)) {
    Serial.print("Kelembapan: ");
    Serial.print(kelembapan, 2);
    Serial.println(" %RH");
  } else {
    Serial.println("Gagal membaca kelembapan!");
  }

  // Tunggu 2 detik sebelum membaca kembali
  delay(2000);
}