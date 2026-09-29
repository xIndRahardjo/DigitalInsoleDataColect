# SOP PENGGUNAAN ALAT DIAFOS V2
## Standar Operasional Prosedur — Sistem Monitoring Kaki Diabetik

**Versi Alat:** DIAFOS V2 (Desain PCB Terpisah + Casing Betis)  
**Dokumen:** SOP-DIAFOS-V2-001  
**Berlaku untuk:** Operator / Peneliti / Tenaga Kesehatan

---

## PERHATIAN PENTING

> Alat ini adalah perangkat monitoring **untuk keperluan penelitian**. Data yang dihasilkan **TIDAK** digunakan sebagai dasar diagnosis medis atau keputusan terapi klinis. Selalu konsultasikan hasil pengukuran dengan tenaga kesehatan profesional.

---

## Daftar Isi

1. [Deskripsi Alat](#1-deskripsi-alat)
2. [Komponen Fisik Alat](#2-komponen-fisik-alat)
3. [Persiapan Sebelum Pemasangan](#3-persiapan-sebelum-pemasangan)
4. [Prosedur Pemasangan Alat](#4-prosedur-pemasangan-alat)
5. [Prosedur Pengaktifan Sistem](#5-prosedur-pengaktifan-sistem)
6. [Prosedur Koneksi WiFi Pertama Kali](#6-prosedur-koneksi-wifi-pertama-kali)
7. [Monitoring Data Real-Time](#7-monitoring-data-real-time)
8. [Memahami Indikator Alert](#8-memahami-indikator-alert)
9. [Prosedur Pelepasan Alat](#9-prosedur-pelepasan-alat)
10. [Pemeliharaan Harian](#10-pemeliharaan-harian)
11. [Troubleshooting Cepat](#11-troubleshooting-cepat)
12. [Checklist Harian](#12-checklist-harian)

---

## 1. Deskripsi Alat

DIAFOS V2 adalah sistem monitoring kaki yang dirancang untuk mengukur parameter fisiologis pada pasien **Diabetes Foot Ulcer (DFU)**. Versi ini menggunakan desain **dua bagian terpisah** yang dihubungkan dengan kabel:

### Bagian 1 — Sol Sensor (Insole Unit)

Berisi seluruh sensor yang tertanam di dalam atau ditempelkan pada sol alas kaki. Sol ini dimasukkan ke dalam sepatu atau sandal pasien.

### Bagian 2 — Casing Betis (Calf Unit)

Berisi PCB utama (ESP32 + komponen elektronik) yang dikemas dalam casing. Casing ini dipasang di betis pasien menggunakan strap velcro.

```
      [CASING BETIS]
     +--------------+
     |  PCB + ESP32 |  <--- Dipasang di betis
     |  WiFi/Power  |      menggunakan strap velcro
     +------+-------+
            |
            |  Kabel panjang (bundle kabel sensor)
            |
     +------+-------+
     |  SOL SENSOR  |  <--- Dimasukkan ke dalam
     | FSR1  FSR2   |      sepatu / alas kaki
     | FSR3  FSR4   |
     | FSR5         |
     | MAX30102     |
     | HTU21D       |
     +--------------+
```

---

## 2. Komponen Fisik Alat

### 2.1 Sol Sensor (Insole Unit)

| No | Komponen | Lokasi pada Sol | Fungsi |
|----|----------|----------------|--------|
| 1 | FSR402 — Sensor 1 | Tumit (heel) | Mengukur tekanan tumit |
| 2 | FSR402 — Sensor 2 | Telapak depan (metatarsal) | Mengukur tekanan bola kaki |
| 3 | FSR402 — Sensor 3 | Ibu jari kaki | Mengukur tekanan ujung kaki |
| 4 | FSR402 — Sensor 4 | Sisi luar kaki | Mengukur tekanan tepi lateral |
| 5 | FSR402 — Sensor 5 | Tengah telapak kaki | Mengukur tekanan tengah |
| 6 | MAX30102 | Sisi kaki / pergelangan | Mengukur SpO2 & detak jantung |
| 7 | HTU21D/SHT21 | Bagian atas sol (dekat kulit) | Mengukur suhu & kelembapan |

### 2.2 Casing Betis (Calf Unit)

| No | Komponen | Keterangan |
|----|----------|-----------|
| 1 | Casing plastik | Melindungi PCB dari benturan & keringat |
| 2 | PCB utama (ESP32) | Modul komputasi dan WiFi |
| 3 | Konektor kabel | Tempat menyambungkan bundle kabel dari sol |
| 4 | Strap velcro | Untuk memasang casing di betis |
| 5 | Port pengisian daya / USB | Untuk mengisi baterai atau menghubungkan ke power bank |
| 6 | Tombol RESET | Untuk restart sistem jika diperlukan |
| 7 | Indikator LED | Menunjukkan status sistem (menyala = aktif) |

### 2.3 Aksesori Tambahan

| Item | Keterangan |
|------|-----------|
| Bundle kabel sensor | Kabel yang menghubungkan sol ke casing betis |
| Power bank / baterai | Sumber daya portabel |
| Kabel USB | Untuk pengisian daya atau upload program |
| Smartphone/tablet | Untuk monitoring via aplikasi Blynk |

---

## 3. Persiapan Sebelum Pemasangan

### 3.1 Persiapan Operator

- [ ] Cuci tangan dengan sabun sebelum menangani alat
- [ ] Gunakan sarung tangan sekali pakai jika diperlukan
- [ ] Pastikan area kerja bersih dan kering

### 3.2 Persiapan Alat

- [ ] Periksa kondisi fisik casing tidak ada retak atau kerusakan
- [ ] Periksa kondisi sol sensor tidak ada kerusakan pada sensor
- [ ] Periksa bundle kabel tidak ada kabel yang terputus atau terkelupas
- [ ] Periksa kondisi strap velcro pastikan masih menempel dengan baik
- [ ] Pastikan baterai / power bank sudah terisi penuh (minimal 80%)
- [ ] Pastikan koneksi WiFi tersedia di lokasi pengukuran

### 3.3 Persiapan Pasien

- [ ] Minta pasien untuk duduk dengan nyaman di kursi
- [ ] Minta pasien untuk melepas alas kaki (sepatu / sandal)
- [ ] Periksa kondisi kulit kaki dan betis pasien
- [ ] Pastikan area betis yang akan dipasang casing tidak ada luka terbuka
- [ ] Catat kondisi awal kulit pasien dalam lembar dokumentasi

> **Catatan:** Jika ada luka terbuka pada betis, konsultasikan dengan tenaga medis sebelum memasang casing di area tersebut.

---

## 4. Prosedur Pemasangan Alat

### LANGKAH 1 — Siapkan Sol Sensor

```
[1] Ambil sol sensor dari tempat penyimpanan
[2] Periksa seluruh permukaan sol — pastikan bersih dan kering
[3] Pastikan semua sensor menempel dengan baik pada sol
[4] Posisikan sol di atas lantai, sisi sensor menghadap ke atas
```

### LANGKAH 2 — Masukkan Sol ke Alas Kaki

```
[1] Ambil sepatu atau sandal yang akan digunakan pasien
[2] Keluarkan insole bawaan dari alas kaki jika ada
[3] Masukkan sol sensor ke dalam alas kaki
     - Pastikan tumit sol tepat di bagian tumit alas kaki
     - Pastikan permukaan sol rata dan tidak terlipat
     - Pastikan sensor FSR tidak tertekuk atau tertindih
[4] Keluarkan bundle kabel dari alas kaki melalui sisi samping/belakang
     - Kabel tidak boleh terjepit di bawah telapak kaki
```

**Penempatan sensor pada sol:**

```
  Pandangan atas sol sensor:

  +-----------------------------+
  |        [FSR 3]              |  <-- Ibu jari
  |                             |
  |  [FSR 2]           [FSR 4] |  <-- Bola kaki & sisi luar
  |                             |
  |       [FSR 5]               |  <-- Tengah
  |                             |
  |          [FSR 1]            |  <-- Tumit
  +-----------------------------+

  [MAX30102] dan [HTU21D] di sisi samping sol
  Bundle kabel keluar dari sisi belakang sol
```

### LANGKAH 3 — Pasang Alas Kaki ke Kaki Pasien

```
[1] Minta pasien untuk mengenakan alas kaki yang sudah dipasang sol sensor
[2] Pastikan pasien merasa nyaman — tidak ada tekanan yang tidak wajar
[3] Pastikan bundle kabel tidak menarik sol dari dalam alas kaki
[4] Rapikan posisi kabel di sisi kaki pasien
```

### LANGKAH 4 — Posisikan Bundle Kabel

```
[1] Arahkan bundle kabel dari alas kaki ke arah betis
[2] Jalurkan kabel di sepanjang sisi dalam kaki (sisi medial)
[3] Kabel tidak boleh mengganggu gerak atau menyebabkan ketidaknyamanan
[4] Sisa kabel yang berlebih digulung longgar dan diikat dengan karet
    kecil di dekat casing
```

> **Penting:** Pastikan bundle kabel memiliki sedikit kelonggaran (slack) agar tidak tegang saat pasien bergerak atau menekuk kaki.

### LANGKAH 5 — Pasang Casing pada Betis

```
[1] Ambil casing betis beserta strap velcro
[2] Tentukan posisi pemasangan:
     - Posisi ideal : Bagian bawah betis (sisi anterior / depan)
     - Ketinggian   : Sekitar 5-10 cm di atas mata kaki
     - Sisi         : Anterior (depan betis) untuk kenyamanan maksimal

[3] Lingkarkan strap velcro di sekeliling betis
[4] Posisikan casing di tengah strap (bagian depan betis)
[5] Rekatkan strap velcro dengan kekencangan yang tepat:
     TEPAT      : Casing tidak bergerak, bisa masuk 1-2 jari di bawah strap
     TERLALU KENCANG : Meninggalkan bekas, tidak bisa masukkan jari
     TERLALU LONGGAR : Casing mudah bergerak, bisa masuk 3+ jari

[6] Tanyakan kepada pasien apakah ada rasa tidak nyaman atau kesemutan
     Jika ya, kendurkan strap segera
```

### LANGKAH 6 — Sambungkan Konektor Kabel

```
[1] Ambil ujung bundle kabel dari alas kaki
[2] Hubungkan ke port konektor di casing betis
     - Pastikan konektor masuk dengan benar (tidak miring)
     - Rasakan sensasi "klik" jika ada pengaman konektor
[3] Pastikan kabel tidak dalam kondisi tertarik tegang
[4] Rapikan sisa kabel di sepanjang kaki
```

### LANGKAH 7 — Hubungkan Sumber Daya

```
[1] Sambungkan power bank ke port USB pada casing
    ATAU pastikan baterai internal sudah terpasang dan terisi
[2] Tempatkan power bank di saku atau kaitkan pada strap tambahan
[3] Pastikan kabel USB tidak menganggu gerak pasien
```

---

## 5. Prosedur Pengaktifan Sistem

### 5.1 Menghidupkan Alat

```
[1] Pastikan semua koneksi kabel sudah terpasang dengan benar
[2] Nyalakan alat melalui tombol power atau hubungkan ke power bank
[3] Amati indikator LED pada casing:
    - LED berkedip cepat : Sistem sedang booting
    - LED menyala stabil : Sistem siap digunakan
    - LED mati           : Periksa sumber daya
```

### 5.2 Memverifikasi Sistem Melalui Blynk

```
[1] Buka aplikasi Blynk di smartphone
[2] Login dengan akun yang terdaftar
[3] Buka dashboard "DIAFOS"
[4] Tunggu hingga status device berubah menjadi "Online"
    (biasanya 10 - 30 detik setelah alat dinyalakan)
[5] Verifikasi bahwa semua data sensor mulai tampil:
    - Pressure 1-5 : Tekanan kaki (5 titik)
    - SpO2         : Saturasi oksigen
    - Heart Rate   : Detak jantung
    - Temperature  : Suhu permukaan kaki
    - Humidity     : Kelembapan lokal
```

### 5.3 Urutan Inisialisasi Normal

Jika terhubung ke komputer via USB, Serial Monitor (115200 baud) menampilkan:

```
╔═══════════════════════════════════════╗
║   DIAFOS V2 - Monitoring System       ║
╚═══════════════════════════════════════╝

→ Initializing FSR402 Pressure Sensors...
  ✓ FSR402 pins initialized (5 sensors)

→ Connecting to WiFi...
  ✓ WiFi Connected!

→ Connecting to Blynk...
  ✓ Blynk Connected!

→ Initializing I2C Bus...
  ✓ I2C initialized
  ✓ Device at 0x40 - HTU21D/SHT21
  ✓ Device at 0x57 - MAX30102

╔═══════════════════════════════════════╗
║  SYSTEM STATUS SUMMARY                ║
║  FSR402 Sensors (5): ✓ Ready          ║
║  MAX30102 (SpO2)   : ✓ Connected      ║
║  HTU21D/SHT21      : ✓ Connected      ║
║  WiFi Connection   : ✓ Connected      ║
║  Blynk Cloud       : ✓ Connected      ║
╚═══════════════════════════════════════╝
```

---

## 6. Prosedur Koneksi WiFi Pertama Kali

Prosedur ini hanya dilakukan saat **pertama kali** menggunakan alat di jaringan WiFi baru.

### 6.1 Mode Konfigurasi WiFi

```
[1] Nyalakan alat
[2] Alat akan mencoba connect ke WiFi tersimpan (selama 10 detik)
[3] Jika tidak ada WiFi tersimpan, alat otomatis membuat hotspot:
    SSID     : DIAFOS_Setup
    Password : diafos123
```

### 6.2 Konfigurasi via Smartphone

```
[1] Buka pengaturan WiFi di smartphone
[2] Cari dan hubungkan ke "DIAFOS_Setup"
[3] Masukkan password: diafos123
[4] Browser akan otomatis membuka halaman konfigurasi
    Jika tidak otomatis, buka browser dan ketik: 192.168.4.1

[5] Di halaman konfigurasi:
    a. Tap "Configure WiFi"
    b. Pilih nama WiFi dari daftar yang tersedia
    c. Masukkan password WiFi
    d. Tap "Save"

[6] Alat akan restart dan otomatis terhubung ke WiFi yang dipilih
[7] Indikator LED akan stabil setelah berhasil terhubung
```

### 6.3 Verifikasi Koneksi

```
[1] Buka aplikasi Blynk
[2] Tunggu status device berubah menjadi "Online"
[3] Jika berhasil, dashboard akan menampilkan data sensor secara real-time
```

> **Catatan:** Kredensial WiFi tersimpan di dalam alat. Untuk sesi berikutnya, alat akan otomatis terhubung tanpa konfigurasi ulang, selama berada dalam jangkauan WiFi yang sama.

---

## 7. Monitoring Data Real-Time

### 7.1 Panel Data pada Aplikasi Blynk

| Virtual Pin | Parameter | Satuan | Nilai Normal |
|-------------|-----------|--------|--------------|
| V0 | Tekanan FSR 1 (Tumit) | kPa | 0 - 200 |
| V1 | Tekanan FSR 2 (Metatarsal) | kPa | 0 - 200 |
| V2 | Tekanan FSR 3 (Ibu Jari) | kPa | 0 - 200 |
| V3 | Tekanan FSR 4 (Sisi Luar) | kPa | 0 - 200 |
| V4 | Tekanan FSR 5 (Tengah) | kPa | 0 - 200 |
| V5 | Saturasi Oksigen (SpO2) | % | > 93% |
| V6 | Detak Jantung | BPM | 60 - 100 |
| V7 | Suhu Permukaan Kaki | derajat C | 30 - 35 |
| V8 | Kelembapan Lokal | %RH | < 74% |
| V9 | Level Peringatan | Teks | NORMAL |
| V10 | Status Sistem | Teks | Connected |

### 7.2 Prosedur Pengambilan Data

```
[1] Pastikan pasien sudah dalam posisi yang ditentukan oleh protokol penelitian

[2] Tunggu sistem stabil selama minimal 2 menit setelah pemasangan
    (sensor perlu waktu adaptasi terhadap suhu dan tekanan awal)

[3] Untuk pembacaan SpO2 yang akurat:
    - Sensor MAX30102 harus bersentuhan langsung dengan kulit
    - Pastikan sensor tidak bergerak saat pengukuran berlangsung
    - Nilai SpO2 memerlukan waktu 5-10 detik untuk stabil

[4] Selama sesi pengukuran:
    - Pantau dashboard Blynk secara berkala
    - Catat perubahan Level Peringatan
    - Dokumentasikan waktu dan kondisi pasien pada lembar catatan

[5] Data dikirim ke Blynk setiap 5 detik secara otomatis
```

### 7.3 Panduan Kondisi Aktivitas Pasien

| Kondisi | Durasi | Tujuan |
|---------|--------|--------|
| Duduk istirahat | 5 menit | Baseline data |
| Berdiri diam | 5 menit | Distribusi berat statis |
| Berjalan pelan | 5 menit | Pola tekanan dinamis |
| Duduk kembali | 5 menit | Data recovery |

> Sesuaikan protokol aktivitas dengan arahan peneliti utama.

---

## 8. Memahami Indikator Alert

Sistem DIAFOS V2 memiliki 4 level peringatan:

### Level NORMAL

```
Kondisi  : Semua parameter dalam rentang normal
Tindakan : Lanjutkan monitoring, tidak ada tindakan khusus
Blynk    : Label menampilkan "NORMAL"
```

### Level RINGAN

```
Kondisi  : 1 parameter berada di luar rentang normal
Parameter yang memicu:
  - Tekanan >= 448 kPa pada salah satu FSR
  - Suhu >= 38 derajat C
  - Kelembapan >= 74%
  - SpO2 <= 93%

Tindakan :
  [1] Catat waktu dan kondisi saat alert terjadi
  [2] Periksa apakah posisi kaki atau sensor bergeser
  [3] Observasi pasien dan tanyakan kondisi yang dirasakan
  [4] Lanjutkan monitoring dengan pengawasan lebih seksama
```

### Level SEDANG

```
Kondisi  : 2 parameter berada di luar rentang normal
Tindakan :
  [1] Hentikan aktivitas yang sedang dilakukan pasien
  [2] Minta pasien untuk duduk dan istirahat
  [3] Lakukan pemeriksaan fisik pada kaki (secara visual)
  [4] Catat semua data pada lembar dokumentasi
  [5] Konsultasikan dengan tenaga medis jika kondisi tidak membaik dalam 5 menit
```

### Level BAHAYA

```
Kondisi  : 3 atau lebih parameter abnormal ATAU parameter kritis terlampaui
Tindakan :
  [1] SEGERA hentikan sesi pengambilan data
  [2] Lepaskan alat dari pasien (ikuti Prosedur Pelepasan — Bagian 9)
  [3] Berikan pertolongan pertama sesuai kondisi pasien
  [4] Hubungi tenaga medis / dokter segera
  [5] Dokumentasikan semua kejadian secara detail
```

> **Penting:** Alert sistem ini adalah sistem peringatan dini berbasis data sensor. Penilaian klinis tetap harus dilakukan oleh tenaga kesehatan yang berkualifikasi.

---

## 9. Prosedur Pelepasan Alat

### LANGKAH 1 — Matikan Sistem

```
[1] Simpan atau catat data terakhir dari dashboard Blynk
[2] Matikan alat melalui tombol power atau cabut sumber daya
[3] Tunggu LED pada casing padam sepenuhnya
```

### LANGKAH 2 — Lepas Koneksi Kabel

```
[1] Lepaskan konektor bundle kabel dari casing betis
     - Pegang bagian konektor plastik (BUKAN kabelnya) saat melepas
     - Jangan menarik kabel secara paksa
[2] Gulung bundle kabel dengan longgar untuk mencegah kabel tertekuk
```

### LANGKAH 3 — Lepas Casing dari Betis

```
[1] Minta pasien untuk tetap duduk
[2] Buka rekat strap velcro perlahan dari satu sisi
[3] Angkat casing dari betis pasien dengan hati-hati
[4] Periksa kondisi kulit betis pasien:
     - Apakah ada bekas tekanan berlebih?
     - Apakah ada kemerahan atau bercak?
     - Tanyakan apakah ada rasa tidak nyaman atau nyeri
[5] Catat kondisi kulit pada lembar dokumentasi
```

### LANGKAH 4 — Lepas Sol dari Alas Kaki

```
[1] Minta pasien untuk melepas alas kaki terlebih dahulu
[2] Keluarkan sol sensor dari alas kaki dengan hati-hati
     - Pegang sol dari tepi, bukan dari area sensor
     - Tarik perlahan dan merata dari kedua sisi
     - Jangan menarik dari satu sisi saja untuk menghindari terlipat
[3] Periksa kondisi sol — pastikan tidak ada sensor yang terlepas
```

### LANGKAH 5 — Perawatan Pasca Pelepasan

```
[1] Bersihkan permukaan sol sensor dengan kain lembab (lap, jangan dicuci)
[2] Bersihkan permukaan casing dengan lap kering
[3] Periksa seluruh kabel untuk memastikan tidak ada kerusakan
[4] Simpan alat di tempat penyimpanan yang kering dan aman
[5] Lengkapi lembar dokumentasi pengukuran
```

---

## 10. Pemeliharaan Harian

### 10.1 Sebelum Setiap Sesi Penggunaan

| No | Pemeriksaan | Cara Memeriksa | Kondisi Normal |
|----|-------------|----------------|----------------|
| 1 | Status baterai / power bank | Cek indikator daya | Minimal 80% |
| 2 | Kondisi kabel | Inspeksi visual sepanjang kabel | Tidak ada kerusakan |
| 3 | Kondisi sol sensor | Inspeksi visual | Sensor menempel, tidak retak |
| 4 | Kondisi strap velcro | Rekatkan dan cek daya rekat | Menempel kuat, tidak aus |
| 5 | Kondisi konektor | Cek pin konektor | Bersih, tidak bengkok |
| 6 | Koneksi WiFi tersedia | Cek dari smartphone | Sinyal tersedia |

### 10.2 Setelah Setiap Sesi Penggunaan

```
[1] Bersihkan sol sensor dengan lap kering / kain lembab
[2] Bersihkan casing dari debu dan keringat dengan lap kering
[3] Gulung kabel dengan longgar dan ikat dengan pengikat kabel
[4] Isi ulang baterai / power bank
[5] Simpan semua komponen dalam tas / wadah penyimpanan
[6] Lengkapi rekap data sesi pada lembar dokumentasi
```

### 10.3 Pemeriksaan Mingguan

```
[1] Lakukan I2C Scan untuk verifikasi sensor MAX30102 & HTU21D terbaca
[2] Cek semua 5 FSR dengan memberikan tekanan manual pada masing-masing
[3] Verifikasi data di Blynk Dashboard masih update dengan benar
[4] Bersihkan konektor dengan cotton bud kering jika perlu
[5] Periksa kekuatan rekat strap velcro dan ganti jika sudah aus
```

---

## 11. Troubleshooting Cepat

### Alat Tidak Menyala

| Kemungkinan Penyebab | Solusi |
|---------------------|--------|
| Baterai / power bank habis | Isi ulang daya dan coba lagi |
| Kabel USB longgar | Pastikan kabel USB tersambung dengan kuat |
| Tombol power tidak berfungsi | Coba tekan lebih lama (2-3 detik) |

---

### Tidak Bisa Connect ke WiFi

| Kemungkinan Penyebab | Solusi |
|---------------------|--------|
| WiFi belum dikonfigurasi | Ikuti Prosedur Koneksi WiFi (Bagian 6) |
| Password WiFi salah | Reset konfigurasi WiFi, ulangi setup |
| Sinyal WiFi lemah | Dekatkan ke router WiFi |
| WiFi menggunakan frekuensi 5GHz | ESP32 hanya support WiFi 2.4GHz |

---

### Data Tidak Muncul di Blynk

| Kemungkinan Penyebab | Solusi |
|---------------------|--------|
| Alat tidak terhubung internet | Periksa koneksi WiFi terlebih dahulu |
| Auth Token salah di program | Cek kembali token di kode program |
| Aplikasi Blynk belum login | Login ke akun yang benar di aplikasi |

---

### SpO2 Bernilai 0 atau Tidak Stabil

| Kemungkinan Penyebab | Solusi |
|---------------------|--------|
| Sensor MAX30102 tidak menempel ke kulit | Pastikan sensor menyentuh kulit langsung |
| Sensor bergerak saat pengukuran | Stabilkan posisi sensor, minta pasien diam |
| Cahaya luar terlalu terang (sinar matahari) | Hindari sinar langsung pada sensor |
| Konektor kabel sensor longgar | Periksa dan kencangkan konektor |

> Tunggu minimal **10 detik** setelah sensor stabil untuk mendapatkan nilai SpO2 yang valid.

---

### Pembacaan FSR Semua Bernilai 0

| Kemungkinan Penyebab | Solusi |
|---------------------|--------|
| Konektor bundle kabel belum tersambung | Pasang kembali konektor dengan benar |
| Kabel FSR putus | Periksa setiap kabel secara visual |
| Posisi sol bergeser di dalam alas kaki | Keluarkan dan pasang kembali sol sensor |

---

### Casing Longgar di Betis

| Kemungkinan Penyebab | Solusi |
|---------------------|--------|
| Strap velcro terlalu longgar | Kencangkan kembali strap |
| Velcro sudah aus / tidak lengket | Ganti strap velcro baru |
| Diameter betis terlalu kecil | Gunakan bantalan foam tipis di bawah casing |

---

### Sistem Restart Sendiri (ESP32 Reboot)

| Kemungkinan Penyebab | Solusi |
|---------------------|--------|
| Daya dari power bank tidak stabil | Ganti power bank dengan output yang lebih stabil (min. 1A) |
| Kabel USB longgar | Pastikan kabel USB terhubung erat |
| Overheating | Pastikan casing tidak terhalang aliran udara |

---

## 12. Checklist Harian

### Checklist Sebelum Sesi

```
TANGGAL     : _______________________________
OPERATOR    : _______________________________
ID PASIEN   : _______________________________
SESI KE-    : _______________________________

PERSIAPAN ALAT:
[ ] Baterai / power bank terisi > 80%
[ ] Kondisi kabel tidak ada kerusakan
[ ] Sol sensor bersih dan semua sensor menempel
[ ] Casing tidak ada kerusakan fisik
[ ] Strap velcro masih lengket dengan baik
[ ] Konektor bersih dan pin tidak bengkok

PERSIAPAN PASIEN:
[ ] Kondisi kulit kaki diperiksa dan dicatat
[ ] Kondisi betis tempat pemasangan diperiksa
[ ] Tidak ada luka terbuka pada area pemasangan
[ ] Pasien memahami prosedur dan memberikan persetujuan

PEMASANGAN:
[ ] Sol terpasang dengan benar di dalam alas kaki
[ ] Bundle kabel tidak terjepit atau tegang
[ ] Casing terpasang di betis dengan kencangan yang tepat
[ ] Pasien tidak merasakan kesemutan atau ketidaknyamanan
[ ] Koneksi kabel tersambung dengan benar
[ ] Sumber daya tersambung

SISTEM:
[ ] Alat menyala (LED aktif)
[ ] WiFi terhubung
[ ] Status Blynk: Online
[ ] Semua data sensor tampil di dashboard (tidak ada nilai 0 yang janggal)

CATATAN AWAL:
__________________________________________________
__________________________________________________

Tanda tangan operator: ___________________________
```

---

### Checklist Setelah Sesi

```
WAKTU SELESAI  : _______________________________
DURASI SESI    : _______________________________

DATA:
[ ] Data telah tersimpan / dicatat dari Blynk
[ ] Lembar dokumentasi pasien dilengkapi
[ ] Alert yang terjadi selama sesi dicatat beserta waktu kejadian

PELEPASAN ALAT:
[ ] Alat dimatikan sebelum dilepas
[ ] Konektor dilepas dengan benar (tidak ditarik paksa)
[ ] Casing dilepas dari betis pasien
[ ] Kondisi kulit betis setelah pelepasan diperiksa dan dicatat
[ ] Sol dikeluarkan dari alas kaki dengan hati-hati
[ ] Kondisi sol dan sensor diperiksa setelah pelepasan

PASCA SESI:
[ ] Sol dibersihkan dengan lap lembab
[ ] Casing dibersihkan dengan lap kering
[ ] Kabel digulung dengan rapi
[ ] Baterai / power bank disambungkan untuk isi ulang
[ ] Alat disimpan di tempat penyimpanan yang benar

KONDISI ALAT SETELAH SESI:
[ ] Baik, tidak ada kerusakan
[ ] Ditemukan kerusakan pada: __________________

CATATAN TAMBAHAN:
__________________________________________________
__________________________________________________

Tanda tangan operator: ___________________________
```

---

## Kontak & Eskalasi

Jika ditemukan masalah yang tidak dapat diselesaikan dengan panduan ini:

| Kondisi | Tindakan |
|---------|----------|
| Kerusakan hardware (kabel putus, sensor rusak) | Hubungi tim teknis / peneliti utama |
| Kondisi medis pasien tidak normal | Hubungi tenaga kesehatan segera |
| Data tidak konsisten / mencurigakan | Dokumentasikan dan laporkan ke peneliti utama |
| Pertanyaan teknis program / firmware | Lihat README_Diafos.md atau hubungi pengembang |

---

## Disclaimer

Alat DIAFOS V2 adalah perangkat **penelitian** dan bersifat **supplementary**. Alat ini:
- **TIDAK** menggantikan pemeriksaan klinis oleh dokter atau tenaga kesehatan
- **TIDAK** digunakan sebagai dasar diagnosis medis
- **TIDAK** digunakan dalam kondisi darurat medis

**Untuk kondisi medis darurat, segera hubungi layanan kesehatan profesional (IGD / 119).**

---

*SOP ini dibuat berdasarkan README_Diafos.md — DIAFOS V2 Diabetic Foot Monitoring System*  
*Versi dokumen: SOP-DIAFOS-V2-001 | Terakhir diperbarui: September 2026*
