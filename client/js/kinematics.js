import { HOST } from "./config";

function inverseKinematics(event) {
  event.preventDefault();
  let x = parseInt(document.getElementById("x_coord").value);
  let y = parseInt(document.getElementById("y_coord").value);
  let z = parseInt(document.getElementById("z_coord").value);

  fetch(`${HOST}/move?x=${x}&y=${y}&z=${z}`)
    .then((response) => response.json())
    .then((data) => console.log(data))
    .catch((err) => console.error(`Error serving request: ${err}`));
}
