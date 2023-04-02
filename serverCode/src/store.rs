use chrono::{DateTime, TimeZone, Utc};
use csv;
use derive_getters::Getters;
use serde::{Deserialize, Serialize};
use std::{error, path, result};

type Result<T> = result::Result<T, Box<dyn error::Error>>;

macro_rules! filter_time {
    ($option : ident, $op : tt) => {
        |r : &Record|{
            if let Some(time) = $option {
                if !(r.time $op *time) {
                    return false;
                }
            }
            true
        }
    };
}

#[derive(Debug, Serialize, Deserialize, Clone, Getters, PartialEq)]
pub struct Record {
    #[serde(default="Utc::now")]
    time: DateTime<Utc>,
    battery: Option<f32>,
    temperature: Option<f32>,
    humidity: Option<f32>,
    moisture: Option<f32>,
}

impl Record {
    pub fn new_without_time(
        battery: Option<f32>,
        temperature: Option<f32>,
        humidity: Option<f32>,
        moisture: Option<f32>,
    ) -> Self {
        Record {
            time: Utc::now(),
            battery,
            temperature,
            humidity,
            moisture,
        }
    }
    pub fn new<Tz: TimeZone>(
        time: DateTime<Tz>,
        battery: Option<f32>,
        temperature: Option<f32>,
        humidity: Option<f32>,
        moisture: Option<f32>,
    ) -> Self {
        Record {
            time: time.with_timezone(&Utc),
            battery,
            temperature,
            humidity,
            moisture,
        }
    }
}

#[derive(Debug)]
pub struct Store {
    path: String,
    records: Vec<Record>,
}

impl Store {
    pub fn open(path: &str) -> Result<Self> {
        let records = Self::load_from_file(path)?;
        Ok(Store {
            path: path.to_string(),
            records,
        })
    }

    fn load_from_file(path: &str) -> Result<Vec<Record>> {
        let mut records = Vec::new();
        let path = path::Path::new(path);
        if path.exists() {
            let mut file = csv::Reader::from_path(path)?;
            for record in file.deserialize() {
                let record = record?;
                records.push(record);
            }
        }
        Ok(records)
    }

    pub fn write(&mut self, record: Record) {
        self.records.push(record);
    }

    pub fn path(&self) -> &str {
        &self.path
    }

    pub fn save(&self) -> Result<()> {
        let mut writer = csv::Writer::from_path(&self.path)?;
        for record in &self.records {
            writer.serialize(record)?;
        }
        writer.flush()?;
        Ok(())
    }

    pub fn read(
        &self,
        start_time: &Option<DateTime<Utc>>,
        end_time: &Option<DateTime<Utc>>,
    ) -> Vec<Record> {
        self.records
            .iter()
            .filter(|r| filter_time!(start_time, >=)(r) && filter_time!(end_time, <=)(r))
            .map(|r| r.to_owned())
            .collect()
    }
    pub fn erase(&mut self, start_time: &Option<DateTime<Utc>>, end_time: &Option<DateTime<Utc>>) {
        self.records
            .retain(|r| !filter_time!(start_time, >=)(r) || !filter_time!(end_time, <=)(r))
    }
}
