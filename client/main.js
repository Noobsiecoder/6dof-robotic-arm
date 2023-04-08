var slider = document.getElementById("servoSlider");
var servoP = document.getElementById("servoPos");
servoP.innerHTML = slider.value;
slider.oninput = function () {
    slider.value = this.value;
    servoP.innerHTML = this.value;
};


function servo(pos) {
    let arm = 5;
    fetch("http://192.168.1.16/rotate?" + new URLSearchParams({ angle: pos, arm }))
        .then((response) => response.json())
        .then((data) => console.log(data))
        .catch((err) => console.error(`Error serving request: ${err}`));
}