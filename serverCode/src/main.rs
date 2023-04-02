use ::peperoni_server::store::Record;
use ::peperoni_server::store::Store;
use actix_files::{NamedFile, Files};
use actix_web::{http::StatusCode, HttpResponse};
use actix_web::{Responder, get, post, web, App, HttpResponseBuilder, HttpServer};
use chrono::{DateTime, Utc};
use lazy_static::lazy_static;
use std::sync::{Mutex, Arc};

lazy_static! {
    static ref STORE: Arc<Mutex<Store>> = Arc::new(Mutex::new(Store::open("old.csv").unwrap()));
}

async fn error(err: impl std::error::Error) -> impl Responder {
    HttpResponseBuilder::new(StatusCode::BAD_REQUEST).body(err.to_string())
}

#[get("/")]
async fn home() -> std::io::Result<NamedFile> {
    NamedFile::open("./client/index.html")
}


#[get("/api/read")]
async fn read(
    from_time: web::Query<Option<String>>,
    to_time: web::Query<Option<String>>,
) -> std::result::Result<web::Json<Vec<Record>>, Box<dyn std::error::Error>> {
    let from_time = match from_time.into_inner() {
        Some(time) => Some(DateTime::parse_from_rfc3339(&time)?.with_timezone(&Utc)),
        None => None,
    };
    let to_time = match to_time.into_inner() {
        Some(time) => Some(DateTime::parse_from_rfc3339(&time)?.with_timezone(&Utc)),
        None => None,
    };
    let lock = STORE.lock()?;
    let records = lock.read(&from_time, &to_time);
    Ok(web::Json(records))
}

#[post("/api/write")]
async fn write(
    record: web::Json<Record>,
) -> std::result::Result<HttpResponse, Box<dyn std::error::Error>> {
    let mut lock = STORE.lock()?;
    lock.write(record.into_inner());
    Ok(HttpResponseBuilder::new(StatusCode::OK).body("OK"))
}

#[actix_web::main] // or #[tokio::main]
async fn main() -> std::io::Result<()> {
    let web_server =
        HttpServer::new(|| App::new().service(home).service(Files::new("/static", "./client/static")).service(read))
            .bind(("127.0.0.1", 8080))?
            .workers(1)
            .run();
    web_server.await?;
    Ok(())
}
