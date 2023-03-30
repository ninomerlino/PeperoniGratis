use chrono::{DateTime, Utc};
use serde::{Deserialize, Serialize};
use std::result::Result;

#[derive(Debug,Serialize,Deserialize)]
struct Record{
    #[serde(with = "Datetime::rfc3339")]
    time : DateTime<Utc>,
    battery : Option<f32>,
    temperature : Option<f32>,
    humidity : Option<f32>,
    moisture : Option<f32>
}


