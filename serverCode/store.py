from pydantic import BaseModel, Field
from typing import Optional
from datetime import datetime, timezone


class Record(BaseModel):
    '''
        A record of the sensor data
        datetime: The time the record was created (UTC)
        battery: The battery voltage
        temperature: The temperature in °C
        humidity: The humidity in %
        moisture: The moisture in %
    '''
    time: str = Field(default_factory=lambda: datetime.utcnow().astimezone(timezone.utc).isoformat())
    battery: float | None = None
    temperature:  float | None = None
    humidity: float | None = None
    moisture: float | None = None

    def to_csv(self) -> str:
        '''
            Returns a csv string of the record
        '''
        return ",".join(
            [
                str(self.time) if self.time is not None else "",
                str(self.battery) if self.battery is not None else "",
                str(self.temperature) if self.temperature is not None else "",
                str(self.humidity) if self.humidity is not None else "",
                str(self.moisture) if self.moisture is not None else "",
            ]
        )

    @staticmethod
    def from_csv(csv_entry : str):
        '''
            Creates a record from a csv string
            cvc_entry: The csv string
        '''
        dt, battery, temperature, humidity, moisture = csv_entry.replace('\n', '').split(",")
        if dt == "":
            raise ValueError(f"Missing datetime in record {csv_entry}")
        return Record(
            time= str(datetime.fromisoformat(dt)),
            battery= float(battery) if battery != "" else None,
            temperature= float(temperature) if temperature != "" else None,
            humidity= float(humidity) if humidity != "" else None,
            moisture= float(moisture) if moisture != "" else None,
        )
    def __ge__(self, other : datetime):
        return datetime.fromisoformat(self.time) >= other
    
    def __le__(self, other : datetime):
        return datetime.fromisoformat(self.time) <= other

class Store:
    '''
        A store for the records
        path: The path to the csv file
    '''
    def __init__(self, path):
        self.path = path
        try:
            with open(self.path, "r") as f:
                self.records = [Record.from_csv(line) for line in f.readlines()]
        except FileNotFoundError:
            self.records = []

    def write(self, record: Record):
        '''
            Writes a record to the store
            record: The record to write
        '''
        self.records.append(record)

    def save(self):
        '''
            Saves the store to the csv file
        '''
        with open(self.path, "w") as f:
            for record in self.records:
                f.write(record.to_csv() + "\n")

    def read(self, last_n : Optional[int] = None):
        '''
            Reads the records from the store
            from_time: The time from which to read the records
            to_time: The time to which to read the records
        '''
        return self.records[-last_n:] if last_n is not None else self.records
        

