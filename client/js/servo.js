import { HOST } from "./config";

var slider1 = document.getElementById("servoSlider1");
var servoP1 = document.getElementById("servoPos1");
servoP1.innerHTML = slider1.value;
slider1.oninput = function () {
  slider1.value = this.value;
  servoP1.innerHTML = this.value;
};

var slider2 = document.getElementById("servoSlider2");
var servoP2 = document.getElementById("servoPos2");
servoP2.innerHTML = slider2.value;
slider2.oninput = function () {
  slider2.value = this.value;
  servoP2.innerHTML = this.value;
};

var slider3 = document.getElementById("servoSlider3");
var servoP3 = document.getElementById("servoPos3");
servoP3.innerHTML = slider3.value;
slider3.oninput = function () {
  slider3.value = this.value;
  servoP3.innerHTML = this.value;
};
var slider4 = document.getElementById("servoSlider4");
var servoP4 = document.getElementById("servoPos4");
servoP4.innerHTML = slider4.value;
slider4.oninput = function () {
  slider4.value = this.value;
  servoP4.innerHTML = this.value;
};
var slider5 = document.getElementById("servoSlider5");
var servoP5 = document.getElementById("servoPos5");
servoP5.innerHTML = slider5.value;
slider5.oninput = function () {
  slider5.value = this.value;
  servoP5.innerHTML = this.value;
};

var slider6 = document.getElementById("servoSlider6");
var servoP6 = document.getElementById("servoPos6");
servoP6.innerHTML = slider6.value;
slider6.oninput = function () {
  slider6.value = this.value;
  servoP6.innerHTML = this.value;
};

HOST = ""; // Insert IP

function rotateServo(angle, arm) {
  fetch(`${HOST}/rotate?` + new URLSearchParams({ angle, arm }))
    .then((response) => response.json())
    .then((data) => console.log(data))
    .catch((err) => console.error(`Error serving request: ${err}`));
}
