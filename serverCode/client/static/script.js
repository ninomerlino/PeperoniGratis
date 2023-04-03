class DataStore {
    constructor() {
        this.data = [];
        this.last_update = Date.now();
        this.exipre_time = 60 * 1000;
        this.time_window = 60 * 60 * 1000;// last hour
    }
    async update() {
        let from_time = new Date(Date.now() - this.time_window).toISOString();
        this.data = await this.read_data(from_time);
        this.last_update = Date.now();
    }
    async read_data(from_time) {
        const response = await fetch("/api/read?start_time=" + from_time);
        if (response.ok) {
            const data = await response.json();
            return data;
        } else {
            throw new Error("Error reading data");
        }
    }
    set_time_window(time) {
        this.time_window = time * 1000;
    }
    async get_data() {
        if (this.last_update + this.expire_time < Date.now()) {
            await this.update();
        }
        return this.data;
    }
}

function format_date(date) {
    let year = date.getFullYear();
    let month = date.getMonth();
    let day = date.getDate();
    let hour = date.getHours();
    let minute = date.getMinutes();
    let second = date.getSeconds();
    return `${year}-${month}-${day} ${hour}:${minute}:${second}`;
}

function set_label(id, value) {
    let label = document.getElementById(id);
    label.value = value;
}

var batteryChart = undefined;
var temperatureChart = undefined;
var humidityChart = undefined;
var moistureChart = undefined;
var dataStore = new DataStore();

function create_battery_chart(data) {
    batteryChart = new Chart(
        document.getElementById("battery_chart"),
        {
            type: "bar",
            data: {
                labels: [""],
                datasets: [
                    {
                        label: "Battery Charge",
                        data: data[data.length - 1]["battery"],
                    }
                ]
            },
            options: {
                indexAxis: 'y',
                scales: {
                    x: {
                        min: 0,
                        max: 100,
                        ticks: {
                            stepSize: 10
                        }
                    }
                }
            }
        }
    );
}

function create_temperature_chart(data) {
    temperatureChart = new Chart(
        document.getElementById("temperature_chart"),
        {
            type: "bar",
            data: {
                labels: data.map((record) => format_date(new Date(record["time"]))),
                datasets: [
                    {
                        label: "Temperature",
                        data: data.map((record) => record["temperature"]),
                    }
                ],
            },
            options: {
                fill: true,
                cubicInterpolationMode: 'monotone',
            }
        }
    );
}

function create_humidity_chart(data) {
    humidityChart = new Chart(
        document.getElementById("humidity_chart"),
        {
            type: "line",
            data: {
                labels: data.map((record) => format_date(new Date(record["time"]))),
                datasets: [
                    {
                        label: "Humidity",
                        data: data.map((record) => record["humidity"]),
                    }
                ],
            },
            options: {
                fill: true,
                cubicInterpolationMode: 'monotone',
            }
        }
    );
}

function create_moisture_chart(data) {
    moistureChart = new Chart(
        document.getElementById("moisture_chart"),
        {
            type: "line",
            data: {
                labels: data.map((record) => format_date(new Date(record["time"]))),
                datasets: [
                    {
                        label: "Moisture",
                        data: data.map((record) => record["moisture"]),
                    }
                ],
            },
            options: {
                fill: true,
                cubicInterpolationMode: 'monotone',
            }
        }
    );
}

async function setup() {
    await dataStore.update();
    let data = await dataStore.get_data();
    if (data.length > 0) {
        create_battery_chart();
        create_temperature_chart();
        create_humidity_chart();
        create_moisture_chart();
        set_label("update_text", format_date(new Date(data[data.length - 1]["time"])));
        set_label("temperature_text", data[data.length - 1]["temperature"]);
        set_label("humidity_text", data[data.length - 1]["humidity"]);
        set_label("moisture_text", data[data.length - 1]["moisture"]);
    } else {
        show_msg("No data available");
    }
}


async function update() {
    let data = await dataStore.get_data();
    if (data.length > 0) {
        if (batteryChart == undefined) {
            create_battery_chart(data);
        } else {
            batteryChart.data.labels = [""];
            batteryChart.data.datasets[0].data = [data[data.length - 1]["battery"]];
            batteryChart.update();
        }
        if (temperatureChart == undefined) {
            create_temperature_chart(data);
        } else {
            temperatureChart.data.labels = data.map((record) => format_date(new Date(record["time"])));
            temperatureChart.data.datasets[0].data = data.map((record) => record["temperature"]);
            temperatureChart.update();
        }
        if (humidityChart == undefined) {
            create_humidity_chart(data);
        } else {
            humidityChart.data.labels = data.map((record) => format_date(new Date(record["time"])));
            humidityChart.data.datasets[0].data = data.map((record) => record["humidity"]);
            humidityChart.update();
        }
        if (moistureChart == undefined) {
            create_moisture_chart(data);
        } else {
            moistureChart.data.labels = data.map((record) => format_date(new Date(record["time"])));
            moistureChart.data.datasets[0].data = data.map((record) => record["moisture"]);
            moistureChart.update();
        }
        set_label("update_text", format_date(new Date(data[data.length - 1]["time"])));
        set_label("temperature_text", data[data.length - 1]["temperature"]);
        set_label("humidity_text", data[data.length - 1]["humidity"]);
        set_label("moisture_text", data[data.length - 1]["moisture"]);
    } else {
        show_msg("No data available");
    }
}

async function change_time_window() {
    let hours = document.getElementById("time_window").value;
    dataStore.set_time_window(hours * 60 * 60);
    await dataStore.update();
    update();
}

function hide_msg() {
    document.getElementById("msg_section").style.display = "none";
}

function show_msg(text) {
    document.getElementById("msg_text").innerHTML = text;
    document.getElementById("msg_section").style.display = "block";
}