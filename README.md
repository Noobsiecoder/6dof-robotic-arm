# IoT-based 6DOF Robotic Arm with Inverse Kinematics Control for Remote Operation via Web App

- A research study on an IoT- based robotic arm that utilises an ATmega2560 micro-controller and an ESP8266 WiFi module for network connectivity.
- The robotic arm is designed to follow inverse kinematic principles and is controlled through REST API communication between a client and a server.

## API Reference

#### Rotate link with servo angle

```http
  POST /rotate?arm=1&angle=180
```

| Parameter | Type     | Description         | Required |
| :-------- | :------- | :------------------ | :------: |
| `arm`     | `number` | Link/Arm [1..6]     |    ✅    |
| `angle`   | `number` | Angle(deg) [0..180] |    ✅    |

#### Move end effector position to specified coordinate spatial system

```http
  POST /move?x=10&y=5&z=12
```

| Parameter | Type     | Description  | Required |
| :-------- | :------- | :----------- | :------: |
| `x`       | `number` | X coordinate |    ✅    |
| `y`       | `number` | Y coordinate |    ✅    |
| `z`       | `number` | Z coordinate |    ✅    |

## Variables

- To run this project, you will need to add the following variables to your:

| File        | Directory    | Variables          |
| :---------- | :----------- | :----------------- |
| `config.h`  | `/arduino`   | `SSID` = WiFi_NAME |
|             |              | `PASS` = WiFi_PASS |
| `config.js` | `/client/js` | `HOST` = LOCAL_IP  |

## Authors

- [@noobsiecoder](https://www.github.com/noobsiecoder)
- [@Incogniter](https://www.github.com/Incogniter)

## Demo

| Robot                                                                                               | Components                                                                                                    |
| :-------------------------------------------------------------------------------------------------- | :------------------------------------------------------------------------------------------------------------ |
| ![side view](https://github.com/Noobsiecoder/6dof-robotic-arm/blob/main/demo/robot_1.png?raw=true)  | ![ATmega2560 + ESP8266 WiFi](https://github.com/Noobsiecoder/6dof-robotic-arm/blob/main/demo/mc.png?raw=true) |
| ![front view](https://github.com/Noobsiecoder/6dof-robotic-arm/blob/main/demo/robot_2.png?raw=true) | ![claw gripper](https://github.com/Noobsiecoder/6dof-robotic-arm/blob/main/demo/gripper.png?raw=true)         |
