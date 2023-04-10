# IoT-based 6DOF Robotic Arm with Inverse Kinematics Control for Remote Operation via Web App

- This project presents an IoT-based 6 Degree of Freedom (6DOF) robotic arm that can be controlled remotely through a web application in a local network. 
- The arm is designed to be controlled via a user-friendly web interface and can be manipulated to a specific spatial point using the inverse kinematics (DH) algorithm. 
- The system is developed using open-source hardware and software, making it affordable and accessible to a wide range of users.

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
