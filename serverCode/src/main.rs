use ::peperoni_server::store::{Record, Store};
use chrono::{DateTime, Utc};
use lazy_static::lazy_static;
use ::peperoni_server::internal_error;
use rocket::{fs::NamedFile};
use std::path::{Path, PathBuf};
use std::sync::{Mutex, Arc};
use rocket::{routes, get};
use rocket::{fs, serde::json::Json};

lazy_static! {
    static ref STORE: Arc<Mutex<Store>> = Arc::new(Mutex::new(Store::open("store.csv").unwrap()));
}

#[get("/")]
async fn index() -> std::io::Result<NamedFile> {
    fs::NamedFile::open("./client/index.html").await
}

#[get("/static/<path..>")]
async fn static_files(path: PathBuf) -> std::io::Result<NamedFile> {
    fs::NamedFile::open(Path::new("./client/static").join(path)).await
}

#[get("/api/read?<start_time>&<end_time>")]
async fn read(start_time : Option<String>, end_time : Option<String>) -> internal_error::Result<Json<Vec<Record>>>{
    let start_time = start_time.map(|s| DateTime::parse_from_rfc3339(&s).unwrap().with_timezone(&Utc));
    let end_time = end_time.map(|s| DateTime::parse_from_rfc3339(&s).unwrap().with_timezone(&Utc));
    let store = STORE.lock()?;
    let records = store.read(&start_time, &end_time);
    Ok(Json(records))
}

#[rocket::main]
async fn main() -> Result<(), rocket::Error> {
    let _rocket = rocket::build()
        .mount("/", routes![index, static_files,read])
        .launch()
        .await?;

    Ok(())
}


