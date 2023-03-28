from pydantic import BaseModel, Field
from typing import Optional
from datetime import datetime

class Record(BaseModel):
    datetime: float = Field(default_factory=lambda: datetime.now().isoformat())
    battery: float | None = None
    temperature:  float | None = None
    humidity: float | None = None
    moisture: float | None = None

    def to_csv(self) -> str:
        return ",".join(
            [
                str(self.datetime) if self.datetime is not None else "",
                str(self.battery) if self.battery is not None else "",
                str(self.temperature) if self.temperature is not None else "",
                str(self.humidity) if self.humidity is not None else "",
                str(self.moisture) if self.moisture is not None else "",
            ]
        )

    @staticmethod
    def from_csv(csv_entry : str):
        timestamp, battery, temperature, humidity, moisture = csv_entry.replace('\n', '').split(",")
        return Record(
            timestamp= datetime.fromisoformat(timestamp) if timestamp != "" else None,
            battery= float(battery) if battery != "" else None,
            temperature= float(temperature) if temperature != "" else None,
            humidity= float(humidity) if humidity != "" else None,
            moisture= float(moisture) if moisture != "" else None,
        )

class Store:
    def __init__(self, path):
        self.path = path
        try:
            with open(self.path, "r") as f:
                self.records = [Record.from_csv(line) for line in f.readlines()]
        except FileNotFoundError:
            self.records = []

    def write(self, record: Record):
        self.records.append(record)

    def save(self):
        with open(self.path, "w") as f:
            for record in self.records:
                f.write(record.to_csv() + "\n")

    def read(self, from_time : datetime | None = None, to_time : datetime | None = None):
        local_records = self.records
        if not from_time is None:
            local_records = [record for record in local_records if record.timestamp >= from_time]
        if not to_time is None:
            local_records = [record for record in local_records if record.timestamp <= to_time]
        return local_records
        

