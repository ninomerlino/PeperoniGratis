from fastapi import FastAPI
from fastapi.responses import HTMLResponse, Response, FileResponse
from fastapi.staticfiles import StaticFiles
from datetime import datetime
import uvicorn
from store import Store, Record
from multiprocessing import Process


webServer = FastAPI()
arduinoServer = FastAPI()
store = Store("./store.csv")
webServer.mount("/static", StaticFiles(directory="./client/static"), name="static")

@webServer.get("/", response_class=HTMLResponse)
async def read_root():
    with open("./client/index.html") as f:
        return HTMLResponse(content=f.read(), status_code=200)

@webServer.get("/api/read")
async def read(from_time: str | None = None, to_time: str | None = None):
    try:
        from_time = datetime.fromisoformat(from_time.replace('Z','+00:00')) if from_time is not None else None # type: ignore
        to_time = datetime.fromisoformat(to_time.replace('Z','+00:00')) if to_time is not None else None # type: ignore
    except ValueError as e:
        return Response(content="Invalid date format", status_code=400)
    return store.read(from_time, to_time) # type: ignore

@webServer.get("/api/read_dump")
async def read_dump():
    return FileResponse("./store.csv", media_type="text/csv", filename="store.csv")

#------------------Arduino server------------------

@arduinoServer.post("/api/write")
async def update(record: Record):
    store.write(record)
    store.save()
    return Response(content="OK", status_code=200)

#--------------------------------------------------

if __name__ == "__main__":
    webProcess = Process(target=uvicorn.run,kwargs={"app":webServer,"port": 8001,"host":"0.0.0.0"})
    arduinoProcess = Process(target=uvicorn.run,kwargs={"app":arduinoServer,"port": 8000,"host":"0.0.0.0"})
    webProcess.start()
    arduinoProcess.start()
    webProcess.join()
    arduinoProcess.join()
