#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "config.h"

// Define arm geometry and dimensions
const float L1 = 52.0;  // Length of the first arm segment
const float L2 = 130.0; // Length of the second arm segment
const float L3 = 65.0;  // Length of the third arm segment
const float L4 = 75.0;  // Length of the fourth arm segment
const float L5 = 75.0;  // Length of the fifth arm segment
const float L6 = 100.0; // Length of the sixth arm segment

struct servo1
{
  Servo servo;
  const uint8_t pin = 5;
};

struct servo2
{
  Servo servo;
  const uint8_t pin = 4;
};

struct servo3
{
  Servo servo;
  const uint8_t pin = 12;
};

struct servo4
{
  Servo servo;
  const uint8_t pin = 0;
};

struct servo5
{
  Servo servo;
  const uint8_t pin = 2;
};

struct servo6
{
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

String ServoErrorResponse(String key, int statusCode, String message)
{
  return String("{\"") + key + String("\": {\"statusCode\": \"") + statusCode + String("\",") + String("\"message\":") + String("\"") + message + ("\"}}");
}

String ServoSuccessResponse(String key, int statusCode, String message)
{
  return String("{\"") + key + String("\": {\"statusCode\": \"") + statusCode + String("\",") + message + String("}}");
}

void HelloWorld()
{
  server.send(200, "application/json", "{\"message\": \"Request handled successfully\"}");
}

void handleNotFound()
{
  server.send(404, "application/json", "{\"message\": \"Page not found\"}");
}

void toDegree(std::array<float, 6> &arr)
{
  for (auto &angle : arr)
  {
    angle = angle < 0 ? 180 + (angle * 180 / M_PI) : angle * 180 / M_PI;
    angle = floor(angle * 100.0) / 100.0; // Rounding to two decimal places
  }
}

std::array<float, 6> calculateInverseKinematics(float x_target, float y_target, float z_target)
{
  // Define maximum angle constraints
  const float theta2_max = 120.0 * M_PI / 180.0; // Maximum angle for joint 2 in radians
  const float theta4_max = 120.0 * M_PI / 180.0; // Maximum angle for joint 4 in radians
  const float theta5_max = 120.0 * M_PI / 180.0; // Maximum angle for joint 5 in radians
  const float theta6_max = 120.0 * M_PI / 180.0; // Maximum angle for joint 6 in radians

  // Calculate the joint angles
  float theta1 = atan2(y_target, x_target);

  float r1 = sqrt(x_target * x_target + y_target * y_target) - L1;
  float r2 = sqrt(r1 * r1 + z_target * z_target);

  float alpha1 = acos(r1 / r2);
  float alpha2 = acos((L4 * L4 - L3 * L3 - r2 * r2) / (-2.0 * L3 * r2));

  float beta1 = acos((r2 * r2 - L3 * L3 - L4 * L4) / (-2.0 * L3 * L4));
  float beta2 = acos((L6 * L6 - L5 * L5 - r2 * r2) / (-2.0 * L5 * r2));

  float gamma1 = acos((r2 * r2 - L5 * L5 - L6 * L6) / (-2.0 * L5 * L6));
  float gamma2 = atan2(z_target, r1);

  float theta2 = alpha1 + alpha2;
  if (theta2 > theta2_max)
    theta2 = theta2_max;

  float theta3 = beta1 - M_PI;
  float theta4 = beta2 - M_PI;
  if (theta4 > theta4_max)
    theta4 = theta4_max;

  float theta5 = gamma1;
  if (theta5 > theta5_max)
    theta5 = theta5_max;

  float theta6 = gamma2;
  if (theta6 > theta6_max)
    theta6 = theta6_max;

  std::array<float, 6> arr = {theta1, theta2, theta3, theta4, theta5, theta6};
  toDegree(arr);
  return arr;
}

void moveEndEffector()
{
  String key = "move";
  if (server.hasArg("x") == false || server.hasArg("y") == false || server.hasArg("z") == false)
  {
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

void RotateServo()
{
  String key = "rotate";
  if (server.hasArg("angle") == false || server.hasArg("arm") == false)
  {
    int statusCode = 422;
    String message = "Missing args <angle, value>";
    String jsonResponse = ServoErrorResponse(key, statusCode, message);
    server.send(statusCode, "application/json", jsonResponse);

    return;
  }

  int angle = server.arg("angle").toInt();
  int arm = server.arg("arm").toInt();
  switch (arm)
  {
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

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
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
  // The ESP8266 tries to reconnect automatically when the connection is lost
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  // Cross origin resource sharing policy enabled
  server.enableCORS(true);
  server.on("/", HTTP_GET, HelloWorld);
  server.on("/rotate", HTTP_POST, RotateServo);   // rotate a link using angle
  server.on("/move", HTTP_POST, moveEndEffector); // move end effector to a location
  server.onNotFound(handleNotFound);              // When a client requests an unknown URI
  server.begin();
}

void loop()
{
  server.handleClient();
}
