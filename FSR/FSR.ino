// Definisikan pin ADC yang digunakan
const int FSR_PIN = 32; // Menggunakan GPIO 34 (ADC1)

// Konfigurasi ADC ESP32
const float VCC = 3.3;             // Tegangan referensi ESP32 (Volt)
const int ADC_RESOLUTION = 4095;   // Resolusi ADC 12-bit (0 - 4095)

// Nilai Resistor Tetap
const float R_SERI = 10000.0;      // 10k Ohm
const float R_GND  = 100000.0;     // 100k Ohm

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Konfigurasi resolusi dan atenuasi ADC ESP32
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db); // Mengukur rentang penuh hingga ~3.3V

  Serial.println("Pengujian FSR402 Dimulai...");
}

void loop() {
  // 1. Membaca nilai mentah ADC
  int adcRaw = analogRead(FSR_PIN);

  // 2. Menghitung tegangan output pada pin ADC (Vout)
  float vOut = (adcRaw / (float)ADC_RESOLUTION) * VCC;

  // 3. Menghitung resistansi FSR402
  // Rumus: Vout = VCC * (R_GND / (R_fsr + R_SERI + R_GND))
  // Maka: R_fsr = (VCC * R_GND / Vout) - R_SERI - R_GND
  float rFSR = 0;
  if (vOut > 0.05) { // Mencegah pembagian nol saat belum ditekan
    rFSR = ((VCC * R_GND) / vOut) - R_SERI - R_GND;
    if (rFSR < 0) rFSR = 0;
  } else {
    rFSR = 1000000.0; // Anggap > 1M Ohm saat tidak ada tekanan
  }

  // Tampilkan data ke Serial Monitor / Serial Plotter
  Serial.print("ADC_Raw:");
  Serial.print(adcRaw);
  Serial.print(", Tegangan_V:");
  Serial.print(vOut, 2);
  Serial.print(", R_FSR_Ohm:");
  Serial.println(rFSR, 0);

  delay(50); // Sampling interval 20Hz
}