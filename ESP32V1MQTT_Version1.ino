#include <WiFi.h>
#include <PubSubClient.h>

// 1. Cấu hình Wi-Fi nhà bạn
const char* ssid     = "PHAN HOME 5c1";
const char* password = "31trinhdinhthao";

// 2. Thông số Broker Nexora của bạn
const char* mqtt_server = "mqtt.nexora.id.vn";
const int   mqtt_port   = 1884; // Cổng dịch vụ cấp
const char* mqtt_user   = "nguyentrongphuc2_e4419e";
const char* mqtt_pass   = "pfGDrGQWj2fyZ6CD";

// 3. Topic điều khiển (Bắt buộc dùng đúng tiền tố được cấp)
const char* topic_sub = "u/nguyentrongphuc2_e4419e/led";

// Chân Onboard LED ESP32 DevKit V1
const int ledPin = 2; 

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Nhan lenh tu topic [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  if (message == "ON") {
    digitalWrite(ledPin, HIGH);
    Serial.println("-> Da BAT den LED");
  } else if (message == "OFF") {
    digitalWrite(ledPin, LOW);
    Serial.println("-> Da TAT den LED");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Dang ket noi den Nexora MQTT...");
    String clientId = "ESP32DevKit-" + String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println(" Thanh cong!");
      client.subscribe(topic_sub);
      Serial.print("Da dang ky lang nghe topic: ");
      Serial.println(topic_sub);
    } else {
      Serial.print(" That bai, ma loi rc=");
      Serial.print(client.state());
      Serial.println(" -> Thu lai sau 3 giay...");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Kết nối Wi-Fi
  Serial.print("Dang ket noi Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi da san sang!");

  // Cấu hình MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}