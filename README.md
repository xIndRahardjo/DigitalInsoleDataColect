# 🦶 Sistem Akuisisi Data Kaki — Diabetes Foot Ulcer Monitoring

> **Versi Pengujian (Testing Phase)**  
> Proyek ini merupakan sistem pengambilan data fisiologis pada kaki pasien **Diabetes Foot Ulcer (DFU)** menggunakan platform ESP32 dan berbagai sensor, yang bertujuan untuk mendukung analisis kondisi kesehatan kaki penderita diabetes secara non-invasif.

---

## 📋 Daftar Isi

- [Latar Belakang](#-latar-belakang)
- [Komponen Sistem](#-komponen-sistem)
- [Arsitektur Sistem](#-arsitektur-sistem)
- [Struktur Folder](#-struktur-folder)
- [Penjelasan Setiap Modul](#-penjelasan-setiap-modul)
  - [1. Pengujian Sensor FSR402](#1-fsr---pengujian-sensor-tekanan-fsr402)
  - [2. Pengujian Sensor MAX30105](#2-max3010---pengujian-sensor-oksimetri--detak-jantung)
  - [3. Pengujian Sensor SHT31](#3-sht31---pengujian-sensor-suhu--kelembapan)
  - [4. Komunikasi ESP-NOW](#4-coms---komunikasi-esp-now-antar-esp32)
- [Data yang Dihasilkan](#-data-yang-dihasilkan)
- [Dependensi & Library](#-dependensi--library)
- [Cara Penggunaan](#-cara-penggunaan)
- [Konfigurasi Hardware](#-konfigurasi-hardware)
- [Roadmap](#-roadmap)

---

## 🩺 Latar Belakang

**Diabetes Foot Ulcer (DFU)** adalah komplikasi serius pada penderita diabetes yang dapat menyebabkan amputasi jika tidak terdeteksi dini. Sistem ini dirancang untuk mengumpulkan data multiparameter dari kaki secara real-time, meliputi:

| Parameter | Sensor | Relevansi Klinis |
|-----------|--------|-----------------|
| **Tekanan Plantar** | FSR402 | Deteksi titik tekanan abnormal, mencegah ulkus baru |
| **Saturasi Oksigen (SpO2)** | MAX30105 | Indikator sirkulasi darah perifer pada kaki |
| **Detak Jantung (BPM)** | MAX30105 | Monitor kondisi kardiovaskular |
| **Suhu Permukaan Kulit** | SHT31 | Deteksi inflamasi / infeksi dini (perbedaan suhu antar kaki) |
| **Kelembapan Lokal** | SHT31 | Monitor kondisi kelembapan area luka |

---

## 🔧 Komponen Sistem

### Hardware

| Komponen | Fungsi |
|----------|--------|
| **ESP32 (x2)** | Mikrokontroler utama (pengirim & penerima) |
| **Sensor FSR402** | Sensor tekanan / gaya (Force Sensitive Resistor) |
| **Sensor MAX30105** | Sensor detak jantung & saturasi oksigen (SpO2) |
| **Sensor SHT31** | Sensor suhu & kelembapan digital (I2C) |
| **Modul SD Card** | Penyimpanan data lokal (format CSV) |
| **Resistor 10k dan 100k Ohm** | Rangkaian pembagi tegangan untuk FSR |

### Software / Platform

- **Arduino IDE** dengan board **ESP32 Arduino Core v3.x**
- Protokol komunikasi nirkabel **ESP-NOW** (tanpa router, latensi rendah)
- Penyimpanan data ke **SD Card** dalam format **CSV**

---

## 🏗️ Arsitektur Sistem

```
+-----------------------------+          +-------------------------------------+
|        ESP32 #1             |          |           ESP32 #2                  |
|      (TRANSMITTER)          |          |          (RECEIVER / LOGGER)        |
|                             |          |                                     |
|  +----------+               |  ESP-NOW |  +----------+   +---------------+  |
|  |  SHT31   |--I2C----------+--------->|  | MAX30105 |   |   SD Card     |  |
|  | Suhu +   |               |          |  |  BPM +   |   |  /data_       |  |
|  | Lembap   |               |          |  |  SpO2    |   |  kesehatan    |  |
|  +----------+               |          |  +----------+   |  .csv         |  |
|                             |          |                  +---------------+  |
|  +----------+               |          |                                     |
|  | FSR402   |--ADC(GPIO32)--+          |  Data gabungan disimpan setiap 1s  |
|  | Tekanan  |               |          +-------------------------------------+
|  +----------+               |
+-----------------------------+
```

**Alur Data:**
1. ESP32 #1 membaca **SHT31** (suhu + kelembapan) dan **FSR402** (tekanan)
2. Data SHT31 dikirimkan secara nirkabel ke ESP32 #2 via **ESP-NOW**
3. ESP32 #2 membaca **MAX30105** (BPM + SpO2) secara lokal
4. Semua data digabungkan dan disimpan ke **SD Card** setiap 1 detik

---

## 📁 Struktur Folder

```
projectCakTio/
|
+-- 📂 FSR/                         # Pengujian mandiri sensor tekanan
|   +-- FSR.ino                     # Baca ADC, hitung tegangan & resistansi FSR402
|
+-- 📂 MAX3010/                     # Pengujian mandiri sensor oksimetri
|   +-- MAX3010.ino                 # BPM + SpO2 + visualisasi gelombang Serial Plotter
|
+-- 📂 sht31/                       # Pengujian mandiri sensor suhu & kelembapan
|   +-- sht31.ino                   # Baca & tampilkan suhu (C) dan kelembapan (%RH)
|
+-- 📂 coms/                        # Modul komunikasi ESP-NOW (sistem terintegrasi)
|   +-- 📂 transmit/                # Uji koneksi dasar ESP-NOW
|   |   +-- transmit.ino            # Kirim paket dummy ke receiver
|   |
|   +-- 📂 reciever/                # Uji koneksi dasar ESP-NOW
|   |   +-- reciever.ino            # Terima & tampilkan paket dari transmitter
|   |
|   +-- 📂 transmitSHT/             # Pengirim data sensor nyata
|   |   +-- transmitSHT.ino         # Baca SHT31 lalu kirim via ESP-NOW ke ESP32 #2
|   |
|   +-- 📂 recieveMAX/              # Penerima + logger (firmware utama ESP32 #2)
|   |   +-- recieveMAX.ino          # Terima SHT31, baca MAX30105, simpan ke SD Card
|   |
|   +-- 📂 example/                 # Contoh integrasi awal (prototipe)
|       +-- example.ino             # Prototipe ESP-NOW + SD Card + MAX30105
|
+-- 📄 data_kesehatan.csv           # Contoh output data hasil pengujian sistem
+-- 📄 data.txt                     # Log waktu pengujian konektivitas
+-- 📄 README.md                    # Dokumentasi proyek ini
```

---

## 📖 Penjelasan Setiap Modul

### 1. `FSR/` — Pengujian Sensor Tekanan FSR402

**File:** `FSR/FSR.ino`

Sketch ini digunakan untuk **kalibrasi dan verifikasi** sensor tekanan FSR402 sebelum integrasi ke sistem utama.

**Cara Kerja:**
- Membaca nilai mentah ADC 12-bit dari GPIO 32
- Menghitung tegangan output (`Vout`) dari nilai ADC
- Menghitung resistansi FSR (`R_FSR`) menggunakan rumus pembagi tegangan:
  ```
  R_FSR = (VCC x R_GND / Vout) - R_SERI - R_GND
  ```

**Konfigurasi:**

| Parameter | Nilai |
|-----------|-------|
| Pin ADC | GPIO 32 |
| Tegangan Referensi (VCC) | 3.3 V |
| Resolusi ADC | 12-bit (0–4095) |
| Resistor Seri (R_SERI) | 10 kOhm |
| Resistor GND (R_GND) | 100 kOhm |
| Atenuasi ADC | 11 dB (full range ~3.3V) |
| Sampling Rate | 20 Hz (delay 50ms) |

**Output Serial Monitor:**
```
ADC_Raw:2048, Tegangan_V:1.65, R_FSR_Ohm:90000
```

---

### 2. `MAX3010/` — Pengujian Sensor Oksimetri & Detak Jantung

**File:** `MAX3010/MAX3010.ino`

Sketch ini menguji sensor **MAX30105** untuk mengukur BPM (detak jantung) dan SpO2 (saturasi oksigen) dengan akurasi tinggi.

**Cara Kerja:**
- Konfigurasi MAX30105 pada **100 Hz** (0.01 detik per sampel)
- **BPM** dihitung menggunakan `checkForBeat()` dengan counter berbasis sampel (bukan `millis()`) untuk menghindari bug timing sensor
- Filter anti double-click dengan jeda minimum 30 sampel (0.3 detik)
- **SpO2** dihitung dari buffer 100 sampel menggunakan algoritma `maxim_heart_rate_and_oxygen_saturation()`
- Buffer digeser 25 sampel setelah kalkulasi (sliding window) untuk efisiensi
- Sistem **memory-hold**: nilai valid dipertahankan hingga 400 sampel (~4 detik) setelah sinyal hilang

**Konfigurasi Sensor:**

| Parameter | Nilai |
|-----------|-------|
| LED Brightness | 0x1F |
| Sample Average | 4x |
| LED Mode | Multi-LED (Red + IR) |
| Sample Rate | 100 Hz |
| Pulse Width | 411 us |
| ADC Range | 4096 |

**Output Serial Plotter:**
```
Gelombang:-123.5, BPM:75, SpO2:98
```

---

### 3. `sht31/` — Pengujian Sensor Suhu & Kelembapan

**File:** `sht31/sht31.ino`

Sketch sederhana untuk verifikasi koneksi dan pembacaan sensor **SHT31** via I2C.

**Cara Kerja:**
- Inisialisasi sensor di alamat I2C `0x44` (default SHT31)
- Membaca suhu dan kelembapan setiap **2 detik**
- Validasi pembacaan dengan pengecekan nilai `NaN`

**Output Serial Monitor:**
```
Suhu: 22.81 C    Kelembapan: 64.07 %RH
```

> **Catatan:** Jika I2C Scanner mendeteksi alamat `0x45`, ubah parameter `sht31.begin(0x44)` menjadi `0x45`.

---

### 4. `coms/` — Komunikasi ESP-NOW Antar ESP32

Folder ini berisi firmware sistem komunikasi nirkabel menggunakan protokol **ESP-NOW** (protokol bawaan ESP32, tanpa membutuhkan router/akses poin).

---

#### `coms/transmit/` — Uji Koneksi Dasar (Pengirim)

**File:** `coms/transmit/transmit.ino`

Firmware pengujian koneksi point-to-point. Mengirimkan paket dummy berisi ID, counter, dan string pesan setiap 2 detik.

**Konfigurasi Penting:**
```cpp
// Ganti dengan MAC Address ESP32 Receiver Anda
uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0x14, 0xA1, 0x50};
```

**Struktur paket yang dikirim:**
```cpp
typedef struct struct_message {
  int id;
  int counter;
  char pesan[32];
} struct_message;
```

---

#### `coms/reciever/` — Uji Koneksi Dasar (Penerima)

**File:** `coms/reciever/reciever.ino`

Menerima paket dari transmitter dan menampilkan isinya ke Serial Monitor. Loop utama dibiarkan kosong — semua logika ada di callback `OnDataRecv()`.

**Output Serial Monitor:**
```
-> [DATA MASUK] Dari ESP32 ID: 1 | Paket #5 | Pesan: Tes Komunikasi OK
```

---

#### `coms/transmitSHT/` — Pengirim Data Sensor SHT31 (ESP32 #1)

**File:** `coms/transmitSHT/transmitSHT.ino`

**Firmware produksi untuk ESP32 #1 (Pengirim).**  
Membaca data suhu & kelembapan dari SHT31 lalu mengirimkannya via ESP-NOW ke ESP32 #2 setiap 2 detik.

**Struktur paket yang dikirim:**
```cpp
typedef struct struct_message {
  float temperature;  // Suhu dalam derajat C
  float humidity;     // Kelembapan dalam %RH
} struct_message;
```

**Alur Kerja:**
```
[SHT31] --I2C--> [ESP32 #1] --ESP-NOW (Channel 1)--> [ESP32 #2]
```

---

#### `coms/recieveMAX/` — Penerima + Logger Utama (ESP32 #2) ⭐

**File:** `coms/recieveMAX/recieveMAX.ino`

**Firmware utama dan paling lengkap — firmware produksi untuk ESP32 #2 (Penerima/Logger).**

**Fungsi Utama:**
1. **Menerima** data SHT31 (suhu + kelembapan) dari ESP32 #1 via ESP-NOW
2. **Membaca** sensor MAX30105 (BPM + SpO2) secara lokal
3. **Menampilkan** semua data ke Serial Plotter
4. **Menyimpan** data gabungan ke SD Card setiap **1 detik** dalam format CSV

**Format Data CSV yang Disimpan:**
```
Waktu_ms,BPM,SpO2,Suhu_SHT31,Kelembapan_SHT31
1000,75,98,22.81,64.07
2000,76,98,22.76,64.14
```

**Inisialisasi Sistem (dalam `setup()`):**
1. SD Card  — Buat file CSV dengan header jika belum ada
2. MAX30105 — Konfigurasi sensor pada 100 Hz
3. ESP-NOW  — Set ke Channel 1, daftarkan callback penerima

---

#### `coms/example/` — Prototipe Integrasi Awal

**File:** `coms/example/example.ino`

Prototipe awal yang menggunakan struktur data lebih sederhana (hanya suhu + kelembapan dari ESP-NOW, tanpa kalkulasi BPM/SpO2 penuh). Berguna sebagai referensi arsitektur sistem dua ESP32.

---

## 📊 Data yang Dihasilkan

### `data_kesehatan.csv`

File CSV hasil pengujian sistem terintegrasi yang tersimpan di SD Card.

| Kolom | Satuan | Keterangan |
|-------|--------|-----------|
| `Waktu_ms` | ms | Timestamp sejak boot ESP32 (millis) |
| `BPM` | bpm | Detak jantung rata-rata (moving average 4 sampel) |
| `SpO2` | % | Saturasi oksigen (algoritma Maxim) |
| `Suhu_SHT31` | C | Suhu permukaan yang diterima dari ESP32 #1 |
| `Kelembapan_SHT31` | %RH | Kelembapan relatif yang diterima dari ESP32 #1 |

**Contoh baris data:**
```csv
148263,0,76,22.96,64.28
151269,0,99,22.96,64.22
166299,0,100,22.99,64.07
```

> **Catatan pada data pengujian:** Nilai `BPM = 0` pada baris awal menunjukkan fase inisialisasi dan waktu tunggu sensor MAX30105 mengumpulkan 100 sampel pertama sebelum kalkulasi SpO2 dapat dimulai. Nilai `Suhu = 0.00` pada awal menunjukkan ESP-NOW belum menerima data dari ESP32 #1.

### `data.txt`

Log sederhana untuk memverifikasi stabilitas koneksi ESP-NOW dalam jangka waktu tertentu.

```
--- LOGGING DIMULAI ---
Waktu: 5 s | Data: OK
Waktu: 10 s | Data: OK
...
Waktu: 300 s | Data: OK
```

---

## 📦 Dependensi & Library

Pastikan library berikut sudah terpasang di **Arduino IDE** (via Library Manager atau manual):

| Library | Versi | Instalasi |
|---------|-------|-----------|
| `SparkFun MAX3010x Pulse and Proximity Sensor Library` | >= 1.1.1 | Library Manager → cari "MAX3010" |
| `Adafruit SHT31 Library` | >= 2.2.0 | Library Manager → cari "SHT31" |
| `Adafruit BusIO` | >= 1.14 | Dependensi otomatis Adafruit SHT31 |
| `ESP32 Arduino Core` | **v3.x** | Board Manager → ESP32 by Espressif |

> **Penting:** Proyek ini menggunakan **ESP32 Arduino Core v3.x**. Callback ESP-NOW telah disesuaikan dengan API baru (`const esp_now_recv_info *info` dan `const wifi_tx_info_t *info`). Jika menggunakan Core v2.x, callback perlu diubah.

---

## 🚀 Cara Penggunaan

### Langkah 1: Persiapan

1. Install **Arduino IDE** dan tambahkan board **ESP32** (Core v3.x)
2. Install semua library yang dibutuhkan (lihat tabel di atas)
3. Siapkan 2 buah ESP32, sensor FSR402, MAX30105, SHT31, dan modul SD Card

### Langkah 2: Pengujian Sensor Mandiri (Opsional tapi Direkomendasikan)

Uji setiap sensor satu per satu sebelum integrasi:

```
FSR/FSR.ino         --> Upload ke ESP32, periksa output ADC & resistansi di Serial Monitor
sht31/sht31.ino     --> Upload ke ESP32, verifikasi pembacaan suhu & kelembapan
MAX3010/MAX3010.ino --> Upload ke ESP32, tempelkan jari ke sensor, pantau BPM & SpO2
```

### Langkah 3: Uji Komunikasi ESP-NOW

1. Dapatkan MAC Address ESP32 Penerima:
   ```cpp
   // Tambahkan di setup() untuk melihat MAC Address
   Serial.println(WiFi.macAddress());
   ```

2. Upload `coms/reciever/reciever.ino` ke **ESP32 #2** (Penerima)

3. Ubah `broadcastAddress` di `coms/transmit/transmit.ino` dengan MAC Address ESP32 #2:
   ```cpp
   uint8_t broadcastAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};
   ```

4. Upload `coms/transmit/transmit.ino` ke **ESP32 #1** (Pengirim)

5. Pantau Serial Monitor kedua ESP32 untuk verifikasi koneksi

### Langkah 4: Deploy Sistem Terintegrasi

1. Ubah `broadcastAddress` di `coms/transmitSHT/transmitSHT.ino` dengan MAC Address ESP32 #2
2. Upload `coms/transmitSHT/transmitSHT.ino` ke **ESP32 #1**
3. Pasang SD Card yang sudah diformat FAT32 ke modul SD Card di **ESP32 #2**
4. Upload `coms/recieveMAX/recieveMAX.ino` ke **ESP32 #2**
5. Buka Serial Plotter di IDE untuk memantau sinyal real-time
6. Data akan otomatis tersimpan ke `/data_kesehatan.csv` di SD Card

---

## 🔌 Konfigurasi Hardware

### Pin ESP32 #1 (Pengirim)

| Pin ESP32 | Terhubung ke | Keterangan |
|-----------|-------------|-----------|
| GPIO 32 | FSR402 (output pembagi tegangan) | ADC input tekanan |
| GPIO 21 (SDA) | SHT31 SDA | I2C Data |
| GPIO 22 (SCL) | SHT31 SCL | I2C Clock |
| 3.3V | FSR402 (satu kaki) + SHT31 VCC | Tegangan |
| GND | Semua GND | Ground |

### Pin ESP32 #2 (Penerima / Logger)

| Pin ESP32 | Terhubung ke | Keterangan |
|-----------|-------------|-----------|
| GPIO 21 (SDA) | MAX30105 SDA | I2C Data |
| GPIO 22 (SCL) | MAX30105 SCL | I2C Clock |
| GPIO 5 (CS) | Modul SD Card CS | SPI Chip Select |
| GPIO 18 (CLK) | Modul SD Card CLK | SPI Clock |
| GPIO 19 (MISO) | Modul SD Card MISO | SPI Data In |
| GPIO 23 (MOSI) | Modul SD Card MOSI | SPI Data Out |
| 3.3V | MAX30105 VCC + SD Card VCC | Tegangan |
| GND | Semua GND | Ground |

### Rangkaian Pembagi Tegangan Sensor FSR402

```
VCC (3.3V) ----+---- [R_GND = 100kOhm] ---- GND
               |
               +---- GPIO 32 (ADC Input)
               |
               +---- [FSR402] ---- [R_SERI = 10kOhm] ---- GND
```

---

## 🗺️ Roadmap

### Fase 1 — Pengujian Komponen ✅ (Selesai)
- [x] Pengujian sensor tekanan FSR402
- [x] Pengujian sensor oksimetri MAX30105 (BPM + SpO2)
- [x] Pengujian sensor suhu & kelembapan SHT31
- [x] Pengujian komunikasi nirkabel ESP-NOW (transmit & receive)

### Fase 2 — Integrasi & Logging ✅ (Selesai)
- [x] Pengiriman data SHT31 via ESP-NOW
- [x] Penerimaan + pembacaan MAX30105 bersamaan
- [x] Penyimpanan data gabungan ke SD Card (format CSV)
- [x] Pengujian kestabilan sistem >= 5 menit

### Fase 3 — Pengembangan Lanjutan 🔄 (Rencana)
- [ ] Integrasi FSR402 ke sistem terintegrasi (gabungkan dengan pengiriman SHT31)
- [ ] Tambah timestamp RTC (Real Time Clock) untuk data temporal yang akurat
- [ ] Penambahan display OLED untuk monitoring tanpa PC
- [ ] Optimasi daya untuk operasi baterai (deep sleep)
- [ ] Desain PCB custom untuk form factor yang sesuai alas kaki

### Fase 4 — Analisis Data 🔬 (Rencana)
- [ ] Pipeline analisis data dengan Python (pandas, matplotlib)
- [ ] Algoritma deteksi anomali tekanan plantar
- [ ] Korelasi data suhu dengan kondisi sirkulasi darah
- [ ] Dashboard visualisasi data real-time

---

## ⚠️ Disclaimer

Sistem ini berada dalam **fase pengujian** dan belum divalidasi secara klinis. Data yang dihasilkan hanya untuk keperluan penelitian dan pengembangan, bukan untuk diagnosis medis.

---

*Dibuat untuk mendukung penelitian deteksi dini Diabetes Foot Ulcer melalui pemantauan data fisiologis multisensor.*
