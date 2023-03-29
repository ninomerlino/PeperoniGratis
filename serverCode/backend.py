from fastapi import FastAPI
from fastapi.responses import HTMLResponse, Response, FileResponse
from fastapi.staticfiles import StaticFiles
from datetime import datetime
import uvicorn

from store import Store, Record


app = FastAPI()
store = Store("./store.csv")
app.mount("/static", StaticFiles(directory="./client/static"), name="static")

@app.get("/", response_class=HTMLResponse)
async def read_root():
    with open("./client/index.html") as f:
        return HTMLResponse(content=f.read(), status_code=200)

@app.post("/api/write")
async def update(record: Record):
    store.write(record)
    store.save()
    return Response(status_code=200)

@app.get("/api/read")
async def read(from_time: str | None = None, to_time: str | None = None):
    try:
        from_time = datetime.fromisoformat(from_time.replace('Z','+00:00')) if from_time is not None else None
        to_time = datetime.fromisoformat(to_time.replace('Z','+00:00')) if to_time is not None else None
    except ValueError as e:
        return Response(content="Invalid date format", status_code=400)
    return store.read(from_time, to_time)

@app.get("/api/read_dump")
async def read_dump():
    return FileResponse("./store.csv", media_type="text/csv", filename="store.csv")


uvicorn.run(app, host="0.0.0.0", port=8000)