#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// MAC Address ESP32 Receiver Anda
uint8_t broadcastAddress[] = {0xEC, 0xE3, 0x34, 0x14, 0xA1, 0x50};

typedef struct struct_message {
  int id;
  int counter;
  char pesan[32];
} struct_message;

struct_message sendData;
esp_now_peer_info_t peerInfo;

// Callback pengiriman (ESP32 Core v3.x)
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Status Pengiriman Paket #");
  Serial.print(sendData.counter);
  Serial.print(": ");
  
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("SUCCESS (Berhasil terhubung ke Receiver!)");
  } else {
    Serial.println("FAIL (Gagal - Receiver mati/berbeda channel)");
  }
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

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1; 
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Gagal menambahkan Peer ESP-NOW!");
    return;
  }

  sendData.id = 1;
  strcpy(sendData.pesan, "Tes Komunikasi OK");
}

void loop() {
  static int count = 0;
  count++;
  sendData.counter = count;

  Serial.print("Mengirim paket #");
  Serial.print(count);
  Serial.println("...");

  esp_now_send(broadcastAddress, (uint8_t *)&sendData, sizeof(sendData));
  delay(2000);
}