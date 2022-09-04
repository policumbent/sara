
function main() {
    let temp = document.getElementById("temperature");
    let pressure = document.getElementById("pressure");
    let hum = document.getElementById("humidity");
    let win_speed = document.getElementById("wind_speed");
    let win_dir = document.getElementById("wind_direction");

    let form = document.getElementById("get_data");

    form.addEventListener("submit", function (event) {

        event.preventDefault();
        const xhr = new XMLHttpRequest();
        xhr.open("GET", "/api/refresh");

        xhr.onload = () => {
            const data = JSON.parse(xhr.response);
            console.log(data);
            temp.textContent = data["temperature"];
            pressure.textContent = data["pressure"];
            hum.textContent = data["humidity"];
            win_speed.textContent = data["wind_speed"];
            win_dir.textContent = data["wind_direction"];
        }

        xhr.send();
    });
}