use ::PeperoniServer::store::Record;
use ::PeperoniServer::store::Store;
use rand::thread_rng;
use rand::Rng;

fn main() {
    let mut rng = thread_rng();
    let mut store = Store::open("store.csv").unwrap();
    for record in store.read(None, None) {
        println!("{:?}", *record);
    }
    for _ in 0..2 {
        let record = Record::new_without_time(
            Some(rng.gen_range(0..100) as f32),
            Some(rng.gen_range(0..50) as f32),
            Some(rng.gen_range(0..50) as f32),
            Some(rng.gen_range(0..50) as f32),
        );
        store.write(record);
    }
    store.save().unwrap();
}
