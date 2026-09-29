#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "heartRate.h" // KITA KEMBALIKAN LIBRARY INI!

MAX30105 particleSensor;

// === Variabel BPM ===
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
int beatAvg = 0;

unsigned long sampleCounter = 0;
unsigned long lastBeatSample = 0;
unsigned long lastValidBpmSample = 0;

// === Variabel SpO2 ===
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

// === Variabel Visual Plotter ===
float dcFilter = 0;

void setup() {
  Serial.begin(115200);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("Sensor MAX30105 tidak terdeteksi.");
    while (1);
  }

  // Konfigurasi wajib SpO2 (100Hz = 0.01 detik per sampel)
  byte ledBrightness = 0x1F;
  byte sampleAverage = 4;
  byte ledMode = 3;
  int sampleRate = 100;
  int pulseWidth = 411;
  int adcRange = 4096;

  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
}

void loop() {
  while (!particleSensor.available()) {
    particleSensor.check();
  }

  long currentRed = particleSensor.getRed();
  long currentIR = particleSensor.getIR();
  particleSensor.nextSample();

  // SETIAP ADA DATA MASUK, TAMBAH COUNTER (1 Sampel = 0.01 Detik)
  sampleCounter++;

  if (currentIR > 20000) {

    // 1. === DETEKSI DETAK JANTUNG SUPER AKURAT ===
    // Kita menggunakan library bawaan untuk melacak puncak gelombang, 
    // namun menghitung WAKTUNYA menggunakan sampleCounter agar tidak terkena bug sensor.
    if (checkForBeat(currentIR) == true) {
      long samplesPassed = sampleCounter - lastBeatSample;
      lastBeatSample = sampleCounter;

      // Filter anti double-click (Jeda minimum 30 sampel = 0.3 detik)
      if (samplesPassed > 30) {
        float bpm = 60.0 / (samplesPassed * 0.01);

        // Hanya masukkan angka normal
        if (bpm > 40 && bpm < 160) {
          // Instant fill: Langsung isi angka pada detak pertama agar tidak naik perlahan
          if (beatAvg == 0) {
            for (byte x = 0; x < RATE_SIZE; x++) rates[x] = (byte)bpm;
          } else {
            rates[rateSpot++] = (byte)bpm;
            rateSpot %= RATE_SIZE;
          }

          beatAvg = 0;
          for (byte x = 0 ; x < RATE_SIZE ; x++) beatAvg += rates[x];
          beatAvg /= RATE_SIZE;

          lastValidBpmSample = sampleCounter;
        }
      }
    }

    // 2. === KUMPULKAN DATA SPO2 ===
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

    // --- TIMEOUT MEMORY HOLD ---
    if (sampleCounter - lastValidBpmSample > 400) beatAvg = 0;
    if (sampleCounter - lastValidSpO2Sample > 400) lastValidSpO2 = 0;

    // 3. === VISUALISASI GELOMBANG PLOTTER ===
    // Filter ini murni hanya untuk merapikan gambar di layar (tidak dipakai berhitung)
    if (dcFilter == 0) dcFilter = currentIR; 
    dcFilter = (0.95 * dcFilter) + (0.05 * currentIR);
    float wavePlot = currentIR - dcFilter;

    // TAMPILKAN KE SERIAL PLOTTER
    Serial.print("Gelombang:");
    Serial.print(wavePlot);
    Serial.print(", BPM:");
    Serial.print(beatAvg);
    Serial.print(", SpO2:");
    Serial.println(lastValidSpO2);

  } else {
    // Reset parameter total saat jari dilepas
    bufferIndex = 0;
    lastValidSpO2 = 0;
    beatAvg = 0;
    dcFilter = 0; 
    
    Serial.print("Gelombang:0, BPM:0, SpO2:0\n");
  }
}