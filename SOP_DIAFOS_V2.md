# SOP PENGGUNAAN ALAT DIAFOS V2
## Standar Operasional Prosedur — Sistem Monitoring Kaki Diabetik (ESP-NOW & SD Logger)

**Versi Alat:** DIAFOS V2 (Desain PCB Terpisah + ESP-NOW + SD Card Logger)  
**Dokumen:** SOP-DIAFOS-V2-002  
**Berlaku untuk:** Operator / Peneliti / Tenaga Kesehatan / Pasien  

---

## PERHATIAN PENTING

> Alat ini adalah perangkat monitoring **untuk keperluan penelitian**. Data yang dihasilkan **TIDAK** digunakan sebagai dasar diagnosis medis atau keputusan terapi klinis secara mandiri. Selalu konsultasikan hasil pengukuran dengan dokter / tenaga kesehatan profesional.

---

## 📋 Daftar Isi

1. [Deskripsi Sistem & Arsitektur ESP-NOW](#1-deskripsi-sistem--arsitektur-esp-now)
2. [Komponen Fisik Alat](#2-komponen-fisik-alat)
3. [Persiapan Sebelum Pemasangan](#3-persiapan-sebelum-pemasangan)
4. [Prosedur Pemasangan Alat pada Kaki & Betis Pasien](#4-prosedur-pemasangan-alat-pada-kaki--betis-pasien)
5. [Prosedur Pengaktifan & Inisialisasi ESP-NOW](#5-prosedur-pengaktifan--inisialisasi-esp-now)
6. [Verifikasi Koneksi ESP-NOW & SD Card Logger](#6-verifikasi-koneksi-esp-now--sd-card-logger)
7. [Prosedur & Protokol Gerakan Pasien (Panduan Pengambilan Data)](#7-prosedur--protokol-gerakan-pasien-panduan-pengambilan-data)
8. [Monitoring & Manajemen Data (SD Card CSV)](#8-monitoring--manajemen-data-sd-card-csv)
9. [Memahami Indikator Alert & Batas Kritis](#9-memahami-indikator-alert--batas-kritis)
10. [Prosedur Pelepasan & Perawatan Alat](#10-prosedur-pelepasan--perawatan-alat)
11. [Troubleshooting Cepat (ESP-NOW & SD Card)](#11-troubleshooting-cepat-esp-now--sd-card)
12. [Checklist Harian (Pra & Pasca Sesi)](#12-checklist-harian-pra--pasca-sesi)

---

## 1. Deskripsi Sistem & Arsitektur ESP-NOW

DIAFOS V2 adalah sistem akuisisi data fisiologis multiparameter pada kaki pasien **Diabetes Foot Ulcer (DFU)** yang bekerja secara terintegrasi menggunakan protokol nirkabel **ESP-NOW** (point-to-point 2.4GHz, tanpa memerlukan router WiFi atau jaringan internet/Blynk).

Sistem terdiri dari 2 unit utama:

### 1.1 Unit Pengirim (ESP32 #1 - Transmitter / Leg & Insole Unit)
- **Sol Sensor (Insole Unit):** Dimasukkan ke dalam alas kaki pasien. Berisi 5 sensor tekanan **FSR402** dan sensor suhu & kelembapan **SHT31** (I2C).
- **Casing Betis (Calf Unit):** Berisi PCB transmitter ESP32 #1 yang terpasang di betis pasien via strap velcro, menerima kabel dari sol sensor lalu mengirimkan data secara nirkabel via ESP-NOW (Channel 1).

### 1.2 Unit Penerima & Logger (ESP32 #2 - Receiver / Base Station Unit)
- Berada di meja operator / stasioner.
- Dilengkapi sensor oksimetri **MAX30105** (SpO2 & BPM) yang bersentuhan dengan kulit pergelangan/kaki.
- Menerima data nirkabel dari ESP32 #1 via ESP-NOW.
- Menggabungkan seluruh data parameter dan menyimpannya secara otomatis ke **SD Card** dalam format file `data_kesehatan.csv` setiap **1 detik**.

```
+------------------------------------+          +-------------------------------------+
|        ESP32 #1 (TRANSMITTER)      |          |       ESP32 #2 (RECEIVER / LOGGER)  |
|      Dipasang pada Betis Pasien    |          |            Stasiun Operator         |
|                                    |          |                                     |
|  +----------+                      |  ESP-NOW |  +----------+   +---------------+  |
|  |  SHT31   |--I2C-----------------+--------->|  | MAX30105 |   |   SD Card     |  |
|  | Suhu +   |                      | (Ch 1)   |  |  BPM +   |   |  /data_       |  |
|  | Lembap   |                      |          |  |  SpO2    |   |  kesehatan    |  |
|  +----------+                      |          |  +----------+   |  .csv         |  |
|                                    |          |                  +---------------+  |
|  +----------+                      |          |                                     |
|  | FSR402   |--ADC (5 Titik Sol)---+          |  Data gabungan disimpan setiap 1s  |
|  | Tekanan  |                      |          +-------------------------------------+
|  +----------+                      |
+------------------------------------+
```

---

## 2. Komponen Fisik Alat

### 2.1 Sol Sensor (Insole Unit)
| No | Komponen | Lokasi pada Sol | Fungsi |
|----|----------|----------------|--------|
| 1 | FSR402 — Sensor 1 | Tumit (heel) | Mengukur tekanan tumit |
| 2 | FSR402 — Sensor 2 | Telapak depan (metatarsal) | Mengukur tekanan bola kaki |
| 3 | FSR402 — Sensor 3 | Ibu jari kaki | Mengukur tekanan ujung kaki |
| 4 | FSR402 — Sensor 4 | Sisi luar (lateral) | Mengukur tekanan tepi luar |
| 5 | FSR402 — Sensor 5 | Tengah telapak kaki | Mengukur tekanan midfoot |
| 6 | Sensor SHT31 | Bagian atas sol (dekat kulit) | Mengukur suhu (°C) & kelembapan (%RH) |

### 2.2 Casing Betis (Calf Unit - ESP32 #1 Transmitter)
| No | Komponen | Keterangan |
|----|----------|-----------|
| 1 | Casing Proteksi | Melindungi PCB dari benturan & keringat |
| 2 | PCB ESP32 #1 | Modul pembaca FSR + SHT31 & pemancar ESP-NOW |
| 3 | Terminal / Konektor Kabel | Port penyambung bundle kabel dari sol |
| 4 | Strap Velcro | Pengikat casing di betis pasien |
| 5 | Power Supply / Baterai | LiPo / Powerbank mini pemasuk daya ESP32 #1 |

### 2.3 Unit Penerima & Data Logger (ESP32 #2 Receiver)
| No | Komponen | Keterangan |
|----|----------|-----------|
| 1 | PCB ESP32 #2 | Receiver ESP-NOW + pengolah data utama |
| 2 | Sensor MAX30105 | Sensor optik SpO2 & Heart Rate (BPM) |
| 3 | Modul SD Card + Kartu SD | Penyimpan data log CSV (`data_kesehatan.csv`) |
| 4 | Port Kabel Data USB | Dihubungkan ke PC operator (pilihan visualisasi Serial Plotter) |

---

## 3. Persiapan Sebelum Pemasangan

### 3.1 Persiapan Operator & Peralatan
- [ ] Cuci tangan dan gunakan sarung tangan medis non-steril.
- [ ] Pastikan **Kartu SD (FAT32)** telah terpasang pada Modul SD Card di ESP32 #2.
- [ ] Periksa baterai/powerbank untuk ESP32 #1 (Transmitter) dan ESP32 #2 (Receiver) dalam kondisi terisi penuh (minimal 80%).
- [ ] Pastikan MAC Address ESP32 Receiver telah dikonfigurasi dengan benar pada firmware ESP32 Transmitter (`broadcastAddress[]`).

### 3.2 Persiapan Pasien
- [ ] Minta pasien duduk santai di kursi pengujian dengan posisi kaki rileks.
- [ ] Lakukan pemeriksaan kulit kaki & betis pasien. Pastikan **TIDAK ADA LUKA TERBUKA** pada lokasi penempelan casing betis atau tempat penempatan sensor optik.
- [ ] Bersihkan telapak kaki pasien menggunakan alkohol swab / tisu basah medis, lalu keringkan.
- [ ] Berikan penjelasan singkat mengenai alur pengambilan data dan gerakan yang akan dilakukan pasien.

---

## 4. Prosedur Pemasangan Alat pada Kaki & Betis Pasien

### LANGKAH 1 — Pemasangan Sol Sensor (Insole Unit)
1. Ambil sol sensor dari tempat penyimpanan, pastikan kabel dan permukaan sensor bersih.
2. Lepaskan insole bawaan dari sepatu / sandal khusus pengujian pasien.
3. Masukkan sol sensor DIAFOS ke dalam sepatu/sandal, pastikan posisi tumit, metatarsal, dan ibu jari tepat berada di area yang sesuai.
4. Keluarkan bundle kabel sensor melalui celah tumit/samping sepatu menuju atas.

```
   Pandangan Atas Sol Sensor:
   +-----------------------------+
   |        [FSR 3]              |  <-- Ibu jari kaki
   |                             |
   |  [FSR 2]           [FSR 4] |  <-- Bola kaki & sisi luar
   |                             |
   |       [FSR 5]               |  <-- Tengah telapak
   |                             |
   |          [FSR 1]            |  <-- Tumit
   +-----------------------------+
   [SHT31] terpasang di area atas sol
```

### LANGKAH 2 — Pemasangan Casing Betis (Calf Unit)
1. Posisikan casing betis pada sisi anterior/samping betis (5–10 cm di atas mata kaki).
2. Lingkarkan strap velcro melingkari betis pasien.
3. Rekatkan strap dengan kekencangan sedang:
   - **Kriteria Tepat:** Casing tidak meluncur turun, dan operator dapat menyelipkan **1–2 jari** di antara strap dan kulit betis.
   - **Terlalu Kencang:** Menyebabkan nyeri / bekas penekanan merah.
   - **Terlalu Longgar:** Casing bergeser saat pasien berjalan.
4. Sambungkan konektor kabel dari sol ke port casing betis hingga terdengar bunyi *klik* atau kencang. Berikan sedikit kelonggaran kabel (*slack*) di area pergelangan agar kabel tidak tegang saat kaki bergerak.

### LANGKAH 3 — Pemasangan Sensor MAX30105 (Oksimetri)
1. Tempelkan modul sensor MAX30105 pada kulit pergelangan kaki / punggung kaki / jari kaki sesuai lokasi pengukuran yang ditentukan.
2. Rekatkan menggunakan tape medis hipoalergenik agar sensor bersentuhan erat dengan kulit tanpa menekan berlebihan (untuk menghindari oklusi pembuluh darah).

---

## 5. Prosedur Pengaktifan & Inisialisasi ESP-NOW

1. **Nyalakan ESP32 #2 (Receiver / Base Station):**
   - Sambungkan ke powerbank / PC operator via USB.
   - LED indikator power pada ESP32 #2 & Modul SD Card akan menyala.
2. **Nyalakan ESP32 #1 (Transmitter / Betis Pasien):**
   - Hubungkan baterai/powerbank pada casing betis.
   - ESP32 #1 akan otomatis menginisialisasi sensor SHT31, ADC FSR, dan memularkan transmisi paket data ESP-NOW pada Channel 1.
3. **Waktu Stabilisasi Sistem:**
   - Biarkan sistem menyala dalam kondisi pasien diam selama **1–2 menit** untuk inisialisasi buffer sensor MAX30105 (100 sampel awal untuk kalkulasi SpO2 & BPM) serta penyesuaian suhu sensor SHT31.

---

## 6. Verifikasi Koneksi ESP-NOW & SD Card Logger

Sebelum memulai protokol gerakan pengambilan data, operator **WAJIB** memverifikasi status koneksi pada Serial Monitor PC Operator (115200 baud):

### 6.1 Tampilan Log Inisialisasi Normal
```
╔══════════════════════════════════════════════════════╗
║   DIAFOS V2 - ESP-NOW & SD CARD LOGGER INITIALIZED   ║
╚══════════════════════════════════════════════════════╝

-> Initializing SD Card...
   ✓ SD Card initialized successfully.
   ✓ File 'data_kesehatan.csv' ready (Header written).

-> Initializing MAX30105 Sensor...
   ✓ MAX30105 Sensor found and configured (100Hz).

-> Initializing ESP-NOW Receiver...
   ✓ ESP-NOW Initialized (Channel 1).
   ✓ Receiver Callback Registered.

Waiting for ESP-NOW data from Transmitter (ESP32 #1)...
[ESP-NOW RECV] Temp: 31.42 °C | Hum: 62.15 %RH | Status: OK
[MAX30105] BPM: 74 | SpO2: 98 % | Status: VALID
[SD LOGGER] Logged row to SD: 12000, 74, 98, 31.42, 62.15
```

### 6.2 Kriteria Kelayakan Data Sebelum Tes Dimulai
- **ESP-NOW Link:** Data SHT31 (`Suhu` & `Kelembapan`) tidak bernilai `0.00` (menandakan sinyal ESP-NOW diterima dengan baik dari ESP32 #1).
- **MAX30105 Status:** BPM dan SpO2 sudah tidak bernilai `0` (kalkulasi window 100 sampel telah terpenuhi).
- **SD Card:** File CSV terbuka dan baris log bertambah setiap detik.

---

## 7. Prosedur & Protokol Gerakan Pasien (Panduan Pengambilan Data)

Untuk mendapatkan dataset yang komprehensif sebagai referensi analisis dinamika tekanan plantar, sirkulasi perifer (SpO2/BPM), dan mikroklimat (suhu/kelembapan), pasien diminta melakukan serangkaian gerakan terstruktur di bawah bimbingan operator.

```
       [ALUR PROTOKOL GERAKAN PASIEN]
  
  +-------------------------------------------------------+
  |  Fase 1: Baseline Istirahat (Duduk Rileks) - 3 Menit   |
  +---------------------------+---------------------------+
                              |
                              v
  +-------------------------------------------------------+
  |  Fase 2: Berdiri Statis (Weight-Bearing)   - 3 Menit   |
  +---------------------------+---------------------------+
                              |
                              v
  +-------------------------------------------------------+
  |  Fase 3: Alih Beban Kiri-Kanan (Shift)     - 2 Menit   |
  +---------------------------+---------------------------+
                              |
                              v
  +-------------------------------------------------------+
  |  Fase 4: Gerakan Jinjit & Angkat Jari     - 10-15x    |
  +---------------------------+---------------------------+
                              |
                              v
  +-------------------------------------------------------+
  |  Fase 5: Berjalan Normal (Normal Gait)     - 3 Menit   |
  +---------------------------+---------------------------+
                              |
                              v
  +-------------------------------------------------------+
  |  Fase 6: Pemulihan / Rest Recovery        - 3 Menit   |
  +-------------------------------------------------------+
```

---

### 7.1 Detail Pelaksanaan Setiap Gerakan

#### FASE 1 — Baseline Istirahat Duduk (Rest Baseline)
* **Durasi:** 3 Menit (Timestamp: 00:00 - 03:00)
* **Posisi Pasien:** Duduk tegak di kursi tanpa menyandarkan paha secara berlebih, kedua telapak kaki menapak ringan di lantai tanpa memberikan tekanan beban tubuh.
* **Instruksi Pasien:** "Duduk tenang, rilekskan kaki, jangan banyak bicara atau memindahkan posisi kaki."
* **Tujuan Pengambilan Data:** Mengambil data baseline kondisi suhu, kelembapan awal, SpO2 & BPM saat istirahat, serta offset dasar FSR.

---

#### FASE 2 — Berdiri Statis Tegak (Stance / Weight-Bearing)
* **Durasi:** 3 Menit (Timestamp: 03:00 - 06:00)
* **Posisi Pasien:** Berdiri tegak dengan kedua kaki dibuka selebar bahu. Pandangan lurus ke depan, beban tubuh terbagi seimbang (50:50) antara kaki kiri dan kanan.
* **Instruksi Pasien:** "Berdiri tegak secara alami, bagikan berat badan secara seimbang pada kedua kaki, usahakan tidak bergoyang."
* **Tujuan Pengambilan Data:** Mengukur distribusi tekanan plantar statis (tumit vs metatarsal vs ibu jari) saat menahan beban tubuh penuh.

---

#### FASE 3 — Pergeseran Beban Tubuh (Weight Shift / Lateral Balance)
* **Durasi:** 2 Menit (Timestamp: 06:00 - 08:00)
* **Posisi Pasien:** Berdiri tegak, lalu perlahan menggeser tumpuan berat badan ke kaki yang terpasang alat (selama 5 detik), lalu menggeser tumpuan ke kaki sebelahnya (selama 5 detik). Diulang sebanyak 10 kali.
* **Instruksi Pasien:** "Geser berat badan Anda perlahan ke kaki kanan (tahan 5 detik), lalu geser ke kaki kiri (tahan 5 detik). Lakukan secara perlahan."
* **Tujuan Pengambilan Data:** Memantau respon sensor FSR402 terhadap perubahan dinamika beban puncak (peak load) dan transisi lateral.

---

#### FASE 4 — Gerakan Jinjit (Plantarflexion) & Angkat Ujung Kaki (Dorsiflexion)
* **Durasi:** 10–15 kali Repetisi (~2 Menit, Timestamp: 08:00 - 10:00)
* **Posisi Pasien:** Berdiri di dekat pegangan/meja untuk menjaga keseimbangan (jika diperlukan).
* **Instruksi Gerakan:**
  1. **Jinjit (Plantarflexion):** Pasien mengangkat tumit tinggi-tinggi sehingga beban tertumpu pada bola kaki & ibu jari (tahan 2 detik), lalu turunkan.
  2. **Angkat Jari (Dorsiflexion):** Pasien mengangkat ujung depan kaki sehingga beban tertumpu penuh pada tumit (tahan 2 detik), lalu turunkan.
* **Tujuan Pengambilan Data:** Menguji puncak tekanan pada sensor FSR 1 (tumit) vs FSR 2 & 3 (metatarsal & ibu jari), serta mengamati perfusi SpO2 saat kontraksi otot betis.

---

#### FASE 5 — Berjalan Normal (Normal Gait Walking)
* **Durasi:** 3 Menit / ~30-50 Langkah (Timestamp: 10:00 - 13:00)
* **Posisi Pasien:** Berjalan lurus pada lintasan datar sepanjang 5–10 meter dengan kecepatan berjalan alami pasien.
* **Instruksi Pasien:** "Berjalanlah secara alami seperti biasa menyusuri lintasan ini. Putar balik perlahan di ujung lintasan."
* **Tujuan Pengambilan Data:** Mengambil siklus gait dinamis penuh (Heel Strike -> Midstance -> Push-off / Toe-off).

---

#### FASE 6 — Pemulihan Istirahat (Rest Recovery Phase)
* **Durasi:** 3 Menit (Timestamp: 13:00 - 16:00)
* **Posisi Pasien:** Duduk kembali di kursi pengujian dengan posisi rileks.
* **Instruksi Pasien:** "Silakan duduk kembali dan istirahat."
* **Tujuan Pengambilan Data:** Mengamati kecepatan pemulihan (recovery time) detak jantung (BPM), saturasi SpO2, serta penurunan suhu/kelembapan lokal pasca aktivitas fisik.

---

### 7.2 Lembar Catatan Gerakan Pasien (Log Time-Stamping)

Operator **WAJIB** mencatat timestamp waktu (atau milidetik pada log) saat perpindahan fase gerakan terjadi untuk memudahkan pelabelan data (data labeling) saat analisis:

| Fase Gerakan | Waktu Mulai (WIB / mm:ss) | Waktu Selesai (mm:ss) | Catatan Khusus / Kendala Pasien |
|--------------|---------------------------|-----------------------|--------------------------------|
| **Fase 1: Baseline Duduk** | ____ : ____ | ____ : ____ | |
| **Fase 2: Berdiri Statis** | ____ : ____ | ____ : ____ | |
| **Fase 3: Weight Shift** | ____ : ____ | ____ : ____ | |
| **Fase 4: Jinjit & Dorsi** | ____ : ____ | ____ : ____ | |
| **Fase 5: Berjalan Normal**| ____ : ____ | ____ : ____ | |
| **Fase 6: Rest Recovery**  | ____ : ____ | ____ : ____ | |

---

## 8. Monitoring & Manajemen Data (SD Card CSV)

### 8.1 Format File Log CSV (`data_kesehatan.csv`)
Data disimpan secara otomatis oleh ESP32 #2 pada SD Card dengan struktur kolom sebagai berikut:

```csv
Waktu_ms,BPM,SpO2,Suhu_SHT31,Kelembapan_SHT31
1000,75,98,22.81,64.07
2000,76,98,22.76,64.14
3000,75,98,22.80,64.10
```

* **`Waktu_ms`:** Waktu berjalan (timestamp `millis()`) sejak ESP32 Receiver dinyalakan (milidetik).
* **`BPM`:** Detak jantung hasil kalkulasi moving average sensor MAX30105 (bpm).
* **`SpO2`:** Saturasi oksigen darah perifer (% SpO2).
* **`Suhu_SHT31`:** Suhu permukaan kulit kaki dari ESP32 #1 via ESP-NOW (°C).
* **`Kelembapan_SHT31`:** Kelembapan mikro-lingkungan sol kaki (%RH).

### 8.2 Prosedur Pencabutan & Penyimpanan Data SD Card
1. Matikan daya ESP32 #2 sebelum mencabut Kartu SD (mencegah corrupt file).
2. Tekan dan keluarkan Kartu SD dari slotnya.
3. Masukkan Kartu SD ke Card Reader PC / Laptop operator.
4. Salin file `data_kesehatan.csv` dan rename sesuai format ID Pasien:  
   `DFU_[ID_PASIEN]_[TANGGAL]_[SESI].csv` (Contoh: `DFU_P001_20260929_S1.csv`).
5. Kosongkan / Arsipkan file di SD Card agar siap digunakan untuk sesi pengujian berikutnya.

---

## 9. Memahami Indikator Alert & Batas Kritis

Selama protokol gerakan berlangsung, operator harus memperhatikan Serial Monitor / Indikator sistem terhadap batas kriteria klinis berikut:

| Parameter | Rentang Normal | Batas Alert / Abnormal | Tindakan Operator |
|-----------|----------------|------------------------|-------------------|
| **SpO2** | 95% – 100% | **< 92%** | Hentikan aktivitas, minta pasien duduk & berikan oksigen/istirahat |
| **Detak Jantung (BPM)** | 60 – 100 bpm | **> 120 bpm** atau **< 50 bpm** | Istirahatkan pasien, cek nadi manual |
| **Suhu Kulit (SHT31)** | 30.0°C – 35.5°C | **> 37.5°C** (Indikasi Inflamasi/Infeksi) | Catat lokasi inflamasi, laporkan ke dokter |
| **Kelembapan (SHT31)** | 40% – 70% RH | **> 85% RH** (Risiko Maserasi Kulit) | Keringkan kaki pasien pasca tes |
| **Tekanan FSR** | 0 – 200 kPa | **> 450 kPa** (Tekanan Berlebih) | Hentikan jinjit berlebih jika pasien merasa nyeri |

---

## 10. Prosedur Pelepasan & Perawatan Alat

### LANGKAH 1 — Pelepasan dari Pasien
1. Matikan daya ESP32 #1 (Transmitter betis) dan ESP32 #2 (Receiver).
2. Lepaskan modul sensor MAX30105 dari kulit pasien secara perlahan.
3. Lepaskan konektor kabel dari casing betis (pegang housing konektor, jangan menarik kabelnya).
4. Buka strap velcro dan lepaskan casing betis.
5. Minta pasien melepas alas kaki, lalu keluarkan sol sensor secara perlahan dari alas kaki.

### LANGKAH 2 — Sanitasi & Pemeliharaan Alat
1. **Sol Sensor & Kabel:** Usap permukaan sol dan kabel menggunakan kain mikrofiber yang dibasahi sedikit alkohol 70% / cairan disinfektan medis (JANGAN MERENDAM SOL/SENSOR DALAM AIR).
2. **Casing Betis & Strap:** Bersihkan casing dengan lap kering. Cuci strap velcro secara berkala jika kotor.
3. **Penyimpanan:** Gulung kabel sensor secara melingkar longgar (diameter min 10 cm), masukkan seluruh komponen ke dalam tas simpan anti-statis / dry box.

---

## 11. Troubleshooting Cepat (ESP-NOW & SD Card)

### 11.1 ESP-NOW Tidak Terhubung (Data Suhu/FSR Bernilai 0.00 / Timeout)
- **Penyebab:** MAC Address receiver salah di program transmitter, atau channel WiFi tidak cocok.
- **Solusi:**
  1. Pastikan `broadcastAddress[]` pada `transmitSHT.ino` sesuai dengan MAC Address ESP32 #2.
  2. Pastikan kedua ESP32 berjalan pada **Channel WiFi 1**.
  3. Periksa daya baterai ESP32 #1 di betis.

### 11.2 SD Card Error ("SD Card Mount Failed" / "Cannot open data_kesehatan.csv")
- **Penyebab:** Kartu SD tidak terformat FAT32, longgar, atau corrupt.
- **Solusi:**
  1. Cabut dan pasang ulang Kartu SD.
  2. Format ulang Kartu SD ke file system **FAT32** (bukan exFAT/NTFS).
  3. Gunakan SD Card dengan kapasitas 32GB atau lebih kecil.

### 11.3 BPM / SpO2 Terbaca 0 atau Fluktuatif Ekstrem
- **Penyebab:** Sensor MAX30105 tergeser saat gerakan pasien, atau terkena cahaya lingkungan yang terlalu terang.
- **Solusi:**
  1. Kencangkan plester perekat sensor optik pada kulit.
  2. Tutup area sensor dengan kain gelap tipis untuk memblokir cahaya sekitar.
  3. Minta pasien tidak menggerakkan kaki berlebih saat pembacaan.

---

## 12. Checklist Harian (Pra & Pasca Sesi)

### Checklist Sebelum Sesi Pengujian
```
TANGGAL     : _______________________________
OPERATOR    : _______________________________
ID PASIEN   : _______________________________

[ ] Kartu SD (FAT32) terpasang di ESP32 #2
[ ] Baterai ESP32 #1 & #2 terisi penuh (> 80%)
[ ] Sol sensor bersih & tidak terlipat
[ ] Casing betis & strap velcro dalam kondisi baik
[ ] Pasien diinformasikan mengenai 6 fase gerakan
[ ] ESP-NOW terhubung (Data SHT31 & FSR terbaca di Serial)
[ ] MAX30105 terkalibrasi (BPM & SpO2 valid)
[ ] File CSV pada SD Card siap mencatat

Tanda Tangan Operator: _______________________
```

### Checklist Setelah Sesi Pengujian
```
[ ] Pengambilan data 6 fase gerakan selesai dilaksanakan
[ ] ESP32 #1 dan #2 dimatikan
[ ] Alat dilepas dari betis & kaki pasien dengan aman
[ ] File 'data_kesehatan.csv' berhasil disalin & di-rename ke PC
[ ] SD Card dikosongkan / diarsipkan untuk sesi berikutnya
[ ] Sol sensor & casing dibersihkan dengan alkohol swab
[ ] Kabel digulung rapi dan disimpan di dry box

Tanda Tangan Operator: _______________________
```

---

## Disclaimer

Sistem DIAFOS V2 dan SOP ini ditujukan **khusus untuk kegiatan penelitian akademik & uji akuisisi data**. Perangkat ini tidak dimaksudkan untuk menggantikan alat diagnosis medis tersertifikasi atau tindakan pertolongan darurat.

---
*SOP-DIAFOS-V2-002 | Diperbarui: September 2026 | Berdasarkan Dokumentasi Firmware ESP-NOW & SD Logger*
