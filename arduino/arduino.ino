#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "config.h"


struct servo1 {
  Servo servo;
  const uint8_t pin = 5;
};

struct servo2 {
  Servo servo;
  const uint8_t pin = 4;
};

struct servo3 {
  Servo servo;
  const uint8_t pin = 12;
};

struct servo4 {
  Servo servo;
  const uint8_t pin = 0;
};

struct servo5 {
  Servo servo;
  const uint8_t pin = 2;
};

struct servo6 {
  Servo servo;
  const uint8_t pin = 16;
};

ESP8266WebServer server;
struct servo1 servo1;
struct servo2 servo2;
struct servo3 servo3;
struct servo4 servo4;
struct servo5 servo5;
struct servo6 servo6;

String ServoErrorResponse(int statusCode, String message) {
  return String("{\"rotate\": {\"statusCode\": \"") + statusCode + String("\",") + String("\"message\":") + String("\"") + message + ("\"}}");
}

String ServoSuccessResponse(int statusCode, int angle, int arm) {
  return String("{\"rotate\": {\"statusCode\": \"") + statusCode + String("\",") + String("\"angle\": \"") + angle + String("\",") + String("\"arm\": \"") + arm + String("\"}}");
}

void HelloWorld() {
  server.send(200, "application/json", "{\"message\": \"Request handled successfully\"}");
}

void RotateServo() {
  if (server.hasArg("angle") == false || server.hasArg("arm") == false) {
    int statusCode = 422;
    String message = "Missing args <angle, value>";
    String jsonResponse = ServoErrorResponse(statusCode, message);
    server.send(statusCode, "application/json", jsonResponse);

    return;
  }

  int angle = server.arg("angle").toInt();
  int arm = server.arg("arm").toInt();
  switch (arm) {
    case 1:
      servo1.servo.write(angle);
      break;
    case 2:
      servo2.servo.write(angle);
      break;
    case 3:
      servo3.servo.write(angle);
      break;
    case 4:
      servo4.servo.write(angle);
      break;
    case 5:
      servo5.servo.write(angle);
      break;
    case 6:
      servo6.servo.write(angle);
      break;
    default:
      int statusCode = 404;
      String message = "Invalid arm joint number, sequence: [1..6]";
      String jsonResponse = ServoErrorResponse(statusCode, message);
      server.send(statusCode, "application/json", jsonResponse);
      return;
  }

  int statusCode = 200;
  String jsonResponse = ServoSuccessResponse(statusCode, angle, arm);
  server.send(statusCode, "application/json", jsonResponse);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  servo1.servo.attach(servo1.pin);
  servo2.servo.attach(servo2.pin);
  servo3.servo.attach(servo3.pin);
  servo4.servo.attach(servo4.pin);
  servo5.servo.attach(servo5.pin);
  servo6.servo.attach(servo6.pin);

  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.enableCORS(true);
  server.on("/", HelloWorld);
  server.on("/rotate", RotateServo);
  server.begin();
}

void loop() {
  server.handleClient();
}
