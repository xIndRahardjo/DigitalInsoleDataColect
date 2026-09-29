#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

typedef struct struct_message {
  int id;
  int counter;
  char pesan[32];
} struct_message;

struct_message incomingData;

// Callback penerimaan (ESP32 Core v3.x)
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int len) {
  memcpy(&incomingData, data, sizeof(incomingData));
  
  Serial.print("-> [DATA MASUK] Dari ESP32 ID: ");
  Serial.print(incomingData.id);
  Serial.print(" | Paket #");
  Serial.print(incomingData.counter);
  Serial.print(" | Pesan: ");
  Serial.println(incomingData.pesan);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  // Kunci ke Wi-Fi Channel 1
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Gagal Inisialisasi ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP32 Receiver Siap Menunggu Data (Channel 1)...");
}

void loop() {
  // Loop kosong
}