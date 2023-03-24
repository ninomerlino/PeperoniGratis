// Global variables
var batteryChart, temperatureChart, humidityChart, soilMoistureChart;

// Function to retrieve data from the server
function getData(url, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function () {
    if (xhr.readyState === 4 && xhr.status === 200) {
      callback(JSON.parse(xhr.responseText));
    }
  };
  xhr.open("GET", url, true);
  xhr.send();
}

// Function to create charts
function createCharts(data) {
  // Create chart for battery level
  var batteryCtx = document.getElementById("batteryChart").getContext("2d");
  batteryChart = new Chart(batteryCtx, {
    type: "line",
    data: {
      labels: data.dates,
      datasets: [
        {
          label: "Battery Charge Level",
          data: data.battery,
          fill: false,
          borderColor: "rgb(75, 192, 192)",
          lineTension: 0.1,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
    },
  });

  // Create chart for temperature
  var temperatureCtx = document
    .getElementById("temperatureChart")
    .getContext("2d");
  temperatureChart = new Chart(temperatureCtx, {
    type: "line",
    data: {
      labels: data.dates,
      datasets: [
        {
          label: "Temperature",
          data: data.temperature,
          fill: false,
          borderColor: "rgb(255, 99, 132)",
          lineTension: 0.1,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
    },
  });

  // Create chart for humidity
  var humidityCtx = document.getElementById("humidityChart").getContext("2d");
  humidityChart = new Chart(humidityCtx, {
    type: "line",
    data: {
      labels: data.dates,
      datasets: [
        {
          label: "Humidity",
          data: data.humidity,
          fill: false,
          borderColor: "rgb(54, 162, 235)",
          lineTension: 0.1,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
    },
  });

  // Create chart for soil moisture
  var soilMoistureCtx = document
    .getElementById("soilMoistureChart")
    .getContext("2d");
  soilMoistureChart = new Chart(soilMoistureCtx, {
    type: "line",
    data: {
      labels: data.dates,
      datasets: [
        {
          label: "Soil Moisture",
          data: data.soil_moisture,
          fill: false,
          borderColor: "rgb(255, 206, 86)",
          lineTension: 0.1,
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
    },
  });
}

// Call getData every 30 seconds to update the chart data
setInterval(function() {
  getData("data.txt", function (data) {
    batteryChart.data.datasets[0].data = data.battery;
    batteryChart.update();
    temperatureChart.data.datasets[0].data = data.temperature;
    temperatureChart.update();
    humidityChart.data.datasets[0].data = data.humidity;
    humidityChart.update();
    soilMoistureChart.data.datasets[0].data = data
    soil_moisture;
    soilMoistureChart.update();
  });
}, 30000); // 30 seconds in milliseconds

// Call getData to initialize the chart data on page load
getData("data.txt", createCharts);
