#define BLYNK_TEMPLATE_ID "TMPL6nkryoVaR"
#define BLYNK_TEMPLATE_NAME "2RELAY"
#define BLYNK_AUTH_TOKEN "vLiK7xK3_HNTwB60Ros9tE5CJ3uidq3X"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = ":))";     // Tên wifi
char pass[] = "99999999a";       // Mật khẩu wifi

const int btn[2] = {13, 12};     // Chỉ 2 nút nhấn
const int out[2] = {14, 27};     // Chỉ 2 relay
unsigned long timeDelay = millis();
unsigned long timeUpdate = millis();
bool update = 0;

ICACHE_RAM_ATTR void handleBtn() {
  if (millis() - timeDelay > 500) {
    for (int i = 0; i < 2; i++) {
      bool btnState = digitalRead(btn[i]);
      if (btnState == 0) {
        digitalWrite(out[i], !digitalRead(out[i]));
      }
    }
    update = 1;
    timeDelay = millis();
  }
}

BLYNK_CONNECTED() {
  Blynk.syncAll();
}

BLYNK_WRITE(V1) {
  int p = param.asInt();
  digitalWrite(out[0], p);
}

BLYNK_WRITE(V2) {
  int p = param.asInt();
  digitalWrite(out[1], p);
}

void setup() {
  Serial.begin(115200); // UART giao tiếp PC
  for (int i = 0; i < 2; i++) {
    pinMode(btn[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(btn[i]), handleBtn, FALLING);
    pinMode(out[i], OUTPUT);
    digitalWrite(out[i], LOW);
  }
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  if (millis() - timeUpdate > 1000) {
    Blynk.virtualWrite(V0, millis() / 1000); // gửi thời gian thực
    timeUpdate = millis();
  }

  if (update == 1) {
    Blynk.virtualWrite(V1, digitalRead(out[0]));
    Blynk.virtualWrite(V2, digitalRead(out[1]));
    update = 0;
  }
}
