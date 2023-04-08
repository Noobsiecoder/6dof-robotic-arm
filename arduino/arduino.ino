#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "config.h"

const float d1 = 0.8;   // distance from the base to the second joint
const float a2 = 11.0;  // length of the second link
const float a3 = 0.8;   // length of the third link
const float d4 = 6.0;   // distance from the third joint to the fourth joint
const float d5 = 0.8;   // distance from the fourth joint to the fifth joint
const float d6 = 0.8;   // distance from the fifth joint to the end effector

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

String ServoErrorResponse(String key, int statusCode, String message) {
  return String("{\"") + key + String("\": {\"statusCode\": \"") + statusCode + String("\",") + String("\"message\":") + String("\"") + message + ("\"}}");
}

String ServoSuccessResponse(String key, int statusCode, String message) {
  return String("{\"") + key + String("\": {\"statusCode\": \"") + statusCode + String("\",") + message + String("}}");
}

void HelloWorld() {
  server.send(200, "application/json", "{\"message\": \"Request handled successfully\"}");
}

std::array<float, 6> calculateInverseKinematics(float x, float y, float z) {
  // Calculate the inverse kinematics
  float r = sqrt(x * x + y * y);

  float theta1 = atan2(y, x);
  float theta2 = atan2(z - d1, r) - acos((a2 * a2 + (z - d1) * (z - d1) + r * r - a3 * a3) / (2 * a2 * sqrt((z - d1) * (z - d1) + r * r)));
  float theta3 = atan2((z - d1 - a2 * sin(theta2)), r - a2 * cos(theta2)) - acos((a2 * a2 + a3 * a3 - (z - d1 - a2 * sin(theta2)) * (z - d1 - a2 * sin(theta2)) - (r - a2 * cos(theta2)) * (r - a2 * cos(theta2))) / (2 * a2 * a3));
  float theta4 = atan2(d5, sqrt(d4 * d4 + d6 * d6)) - atan2(sqrt(d4 * d4 + d5 * d5 + d6 * d6 - d5 * d5), d5);
  float theta5 = atan2(sqrt(d4 * d4 + d5 * d5 + d6 * d6 - d5 * d5), d6) - atan2(sqrt(d4 * d4 + d5 * d5), d4);
  float theta6 = atan2(1, 0);  // or any other arbitrary value, since the orientation of the end effector can be freely defined

  std::array<float, 6> arr = { theta1, theta2, theta3, theta4, theta5, theta6 };
  return arr;
}

void moveEndEffector() {
  String key = "move";
  if (server.hasArg("x") == false || server.hasArg("y") == false || server.hasArg("z") == false) {
    int statusCode = 422;
    String message = "Missing args <x, y, z>";
    String jsonResponse = ServoErrorResponse(key, statusCode, message);
    server.send(statusCode, "application/json", jsonResponse);

    return;
  }

  int x = server.arg("x").toInt();
  int y = server.arg("y").toInt();
  int z = server.arg("z").toInt();

  std::array<float, 6> jointAngles = calculateInverseKinematics(x, y, z);

  servo1.servo.write(jointAngles[0]);
  servo2.servo.write(jointAngles[1]);
  servo3.servo.write(jointAngles[2]);
  servo4.servo.write(jointAngles[3]);
  servo5.servo.write(jointAngles[4]);
  servo6.servo.write(jointAngles[5]);

  int statusCode = 200;
  String message = String("\"x\": ") + x + String(",\"y\": ") + y + String(",\"z\": ") + z;
  String jsonResponse = ServoSuccessResponse(key, statusCode, message);
  server.send(statusCode, "application/json", jsonResponse);

  delay(1000);
}

void RotateServo() {
  String key = "rotate";
  if (server.hasArg("angle") == false || server.hasArg("arm") == false) {
    int statusCode = 422;
    String message = "Missing args <angle, value>";
    String jsonResponse = ServoErrorResponse(key, statusCode, message);
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
      if (angle <= 80)
        servo6.servo.write(180 - angle);
      break;
    default:
      int statusCode = 404;
      String message = "Invalid arm joint number, sequence: [1..6]";
      String jsonResponse = ServoErrorResponse(key, statusCode, message);
      server.send(statusCode, "application/json", jsonResponse);
      return;
  }

  int statusCode = 200;
  String message = String("\"angle\": ") + angle + String(",\"arm\": ") + arm;
  String jsonResponse = ServoSuccessResponse(key, statusCode, message);
  server.send(statusCode, "application/json", jsonResponse);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
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
  //The ESP8266 tries to reconnect automatically when the connection is lost
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  server.enableCORS(true);
  server.on("/", HelloWorld);
  server.on("/rotate", RotateServo);
  server.on("/move", moveEndEffector);
  server.begin();
}

void loop() {
  server.handleClient();
}
