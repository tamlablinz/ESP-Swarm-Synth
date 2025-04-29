#include <esp_now.h>
#include <WiFi.h>

// Replace with your receivers MAC addresses
uint8_t broadcastAddresses[][6] = {
  {0x84, 0xfc, 0xe6, 0xc5, 0x99, 0x70},  // Receiver 1
  {0x70, 0x04, 0x1d, 0xfe, 0x71, 0x1a},  // Receiver 2
  {0x48, 0x27, 0xe2, 0x59, 0x55, 0xc8}   // Receiver 3 
};

const int numReceivers = sizeof(broadcastAddresses) / sizeof(broadcastAddresses[0]);

typedef struct struct_message {
  char q[32];
  int a, b, c; // frequencies Receiver 1
  int d, e, f; // volumes Receiver 1
  int g; // waveform Receiver 1
  int h; // noise volume Receiver 1
  int i, j, k; // frequencies Receiver 2
  int l, m, n; // volumes Receiver 2
  int o; // waveform Receiver 2
  int p; // noise volume Receiver 2
  int r, s, t; // frequencies Receiver 2
  int u, v, w; // volumes Receiver 2
  int x; // waveform Receiver 2
  int y; // noise volume Receiver 2
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  for (int i = 0; i < numReceivers; i++) {
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, broadcastAddresses[i], 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (!esp_now_is_peer_exist(broadcastAddresses[i])) {
      if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
        return;
      }
    }
  }

  strcpy(myData.q, "OSC1");
}

void loop() {
  updateControl();
}

void updateControl() {
  static int temp_val = 0;
  static bool receivingNumber = false;
  static unsigned long lastSendTime = 0;

  while (Serial.available()) {
    char in_byte = Serial.read();

    if (in_byte >= '0' && in_byte <= '9') {
      temp_val = temp_val * 10 + (in_byte - '0');
      receivingNumber = true;
    } else {
      if (in_byte == 'i') myData.i = temp_val;
      else if (in_byte == 'b') myData.b = temp_val;
      else if (in_byte == 'c') myData.c = temp_val;
      else if (in_byte == 'd') myData.d = temp_val;
      else if (in_byte == 'e') myData.e = temp_val;
      else if (in_byte == 'f') myData.f = temp_val;
      else if (in_byte == 'g') myData.g = temp_val;
      else if (in_byte == 'h') myData.h = temp_val;
      else if (in_byte == 'a') myData.a = temp_val;
      else if (in_byte == 'j') myData.j = temp_val;
      else if (in_byte == 'k') myData.k = temp_val;
      else if (in_byte == 'l') myData.l = temp_val;
      else if (in_byte == 'm') myData.m = temp_val;
      else if (in_byte == 'n') myData.n = temp_val;
      else if (in_byte == 'o') myData.o = temp_val;
      else if (in_byte == 'p') myData.p = temp_val;
      else if (in_byte == 'r') myData.r = temp_val;
      else if (in_byte == 's') myData.s = temp_val;
      else if (in_byte == 't') myData.t = temp_val;
      else if (in_byte == 'u') myData.u = temp_val;
      else if (in_byte == 'v') myData.v = temp_val;
      else if (in_byte == 'w') myData.w = temp_val;
      else if (in_byte == 'x') myData.x = temp_val;
      else if (in_byte == 'y') myData.y = temp_val;

      temp_val = 0;
      receivingNumber = false;

      unsigned long currentTime = millis();
      if (currentTime - lastSendTime > 50) { 
        for (int i = 0; i < numReceivers; i++) {
          esp_err_t result = esp_now_send(broadcastAddresses[i], (uint8_t *)&myData, sizeof(myData));
          if (result != ESP_OK) {
            Serial.println("Error sending data");
          }
        }
        lastSendTime = currentTime;
      }
    }
  }
  yield(); // Yield CPU time
}
