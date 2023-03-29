import store
import random
import datetime

db = store.Store("./store.csv")
start = datetime.datetime.utcnow().astimezone(datetime.timezone.utc) - datetime.timedelta(days=1)
print(start.tzinfo)
for i in range(1400):
    r = store.Record(
        time=start.isoformat(),
        battery=float(random.randint(0, 100)),
        temperature=float(random.randint(0, 50)),
        humidity=float(random.randint(0, 100)),
        moisture=float(random.randint(0, 300)),
    )
    db.write(r)
    start += datetime.timedelta(minutes=1)
db.save()

