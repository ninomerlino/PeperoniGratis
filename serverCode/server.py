from typing import Optional
from fastapi import FastAPI
from fastapi.responses import HTMLResponse, Response, FileResponse
from fastapi.staticfiles import StaticFiles
import uvicorn
from store import Store, Record
from multiprocessing import Process, Lock


webServer = FastAPI()
arduinoServer = FastAPI()
store_lock = Lock()
store = Store("./store.csv")
webServer.mount("/static", StaticFiles(directory="./client/static"), name="static")

@webServer.get("/", response_class=HTMLResponse)
async def read_root():
    with open("./client/index.html") as f:
        return HTMLResponse(content=f.read(), status_code=200)

@webServer.get("/api/read")
async def read(last_n: Optional[int] = None):
    store_lock.acquire()
    records = store.read(last_n)
    store_lock.release()
    return records

@webServer.get("/api/read_dump")
async def read_dump():
    return FileResponse("./store.csv", media_type="text/csv", filename="store.csv")

#------------------Arduino server------------------

@arduinoServer.post("/api/write")
async def update(record: Record):
    store_lock.acquire()
    store.write(record)
    store.save()
    store_lock.release()
    return Response(content="OK", status_code=200)

#--------------------------------------------------

if __name__ == "__main__":
    webProcess = Process(target=uvicorn.run,kwargs={"app":webServer,"port": 80,"host":"0.0.0.0"})
    arduinoProcess = Process(target=uvicorn.run,kwargs={"app":arduinoServer,"port": 8000,"host":"0.0.0.0"})
    webProcess.start()
    arduinoProcess.start()
    webProcess.join()
    arduinoProcess.join()
