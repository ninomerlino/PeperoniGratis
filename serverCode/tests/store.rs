use ::peperoni_server::store::*;
use chrono::{DateTime, Utc};
use std::thread::sleep;
#[test]
fn test_record_new_without_time() {
    let record = Record::new_without_time(Some(1.0), Some(2.0), Some(3.0), Some(4.0));
    assert_eq!(*record.battery(), Some(1.0));
    assert_eq!(*record.temperature(), Some(2.0));
    assert_eq!(*record.humidity(), Some(3.0));
    assert_eq!(*record.moisture(), Some(4.0));
}

#[test]
fn test_store_open() {
    let store = Store::open("notexist.csv").unwrap();
    assert_eq!(store.path(), "notexist.csv");
    assert_eq!(store.read(&None, &None).len(), 0);
}

#[test]
fn test_store_load_from_file() {
    let correct_records = vec![
        Record::new(
            //2023-03-31T17:58:05.023500211Z,78.0,25.0,35.0,2.0
            DateTime::parse_from_rfc3339("2023-03-31T17:58:05.023500211Z").unwrap(),
            Some(78.0),
            Some(25.0),
            Some(35.0),
            Some(2.0),
        ),
        Record::new(
            //2023-03-31T17:58:05.023512783Z,7.0,16.0,0.0,27.0
            DateTime::parse_from_rfc3339("2023-03-31T17:58:05.023512783Z").unwrap(),
            Some(7.0),
            Some(16.0),
            Some(0.0),
            Some(27.0),
        ),
        Record::new(
            //2023-03-31T17:58:05.023515087Z,62.0,42.0,24.0,9.0
            DateTime::parse_from_rfc3339("2023-03-31T17:58:05.023515087Z").unwrap(),
            Some(62.0),
            Some(42.0),
            Some(24.0),
            Some(9.0),
        ),
    ];
    let store = Store::open("./tests/test.csv").unwrap();
    let records = store.read(&None, &None);
    assert_eq!(records.len(), 3);
    for (cr, sr) in correct_records.iter().zip(records.iter()) {
        assert_eq!(cr, sr);
    }
}

#[test]
fn test_store_write() {
    let mut store = Store::open("write.csv").unwrap();
    store.erase(&None, &None);
    assert_eq!(store.read(None, None).len(), 0);
    let record = Record::new_without_time(Some(1.0), Some(2.0), Some(3.0), Some(4.0));
    store.write(record);
    let records = store.read(None, None);
    assert_eq!(records.len(), 1);
    assert_eq!(*records[0].battery(), Some(1.0));
    assert_eq!(*records[0].temperature(), Some(2.0));
    assert_eq!(*records[0].humidity(), Some(3.0));
    assert_eq!(*records[0].moisture(), Some(4.0));
    store.save().unwrap();
    let store = Store::open("write.csv").unwrap();
    let records = store.read(None, None);
    assert_eq!(records.len(), 1);
    assert_eq!(*records[0].battery(), Some(1.0));
    assert_eq!(*records[0].temperature(), Some(2.0));
    assert_eq!(*records[0].humidity(), Some(3.0));
    assert_eq!(*records[0].moisture(), Some(4.0));
}

#[test]
fn test_store_erase() {
    let mut store = Store::open("erase.csv").unwrap();
    store.erase(None, None);
    assert_eq!(store.read(None, None).len(), 0);
    let record = Record::new_without_time(Some(1.0), Some(2.0), Some(3.0), Some(4.0));
    store.write(record);
    let records = store.read(None, None);
    assert_eq!(records.len(), 1);
    assert_eq!(*records[0].battery(), Some(1.0));
    assert_eq!(*records[0].temperature(), Some(2.0));
    assert_eq!(*records[0].humidity(), Some(3.0));
    assert_eq!(*records[0].moisture(), Some(4.0));
    store.erase(None, None);
    assert_eq!(store.read(None, None).len(), 0);
    store.save().unwrap();
    let store = Store::open("erase.csv").unwrap();
    assert_eq!(store.read(None, None).len(), 0);
}

#[test]
fn test_read_filter() {
    let mut store = Store::open("filter.csv").unwrap();
    store.write(Record::new_without_time(
        Some(78.0),
        Some(25.0),
        Some(35.0),
        Some(2.0),
    ));
    sleep(std::time::Duration::from_millis(1));
    store.write(Record::new_without_time(
        Some(7.0),
        Some(16.0),
        Some(0.0),
        Some(27.0),
    ));
    sleep(std::time::Duration::from_millis(1));
    store.write(Record::new_without_time(
        Some(62.0),
        Some(42.0),
        Some(24.0),
        Some(9.0),
    ));
    sleep(std::time::Duration::from_millis(1));
    store.write(Record::new_without_time(
        Some(78.0),
        Some(25.0),
        Some(35.0),
        Some(2.0),
    ));
    sleep(std::time::Duration::from_millis(1));
    store.write(Record::new_without_time(
        Some(7.0),
        Some(16.0),
        Some(0.0),
        Some(27.0),
    ));
    let records = store.read(None, None);
    assert_eq!(records.len(), 5);
    let start_time = records[1].time().clone();
    let end_time = records[3].time().clone();
    drop(records);
    let three_records = store.read(Some(&start_time), Some(&end_time));
    assert_eq!(three_records.len(), 3);
    assert_eq!(*three_records[1].battery(), Some(62.0));
    store.erase(Some(&start_time), Some(&end_time));
    let records = store.read(None, None);
    assert_eq!(records.len(), 2);
    assert_eq!(*records[0].battery(), Some(78.0));
    assert_eq!(*records[1].battery(), Some(7.0));
}
