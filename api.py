from fastapi import FastAPI
from pydantic import BaseModel
from datetime import datetime

app = FastAPI()

class Sensor(BaseModel):
    node_id: str
    zone_id: str
    sensors: dict
    status: str

@app.get("/")
async def home():
    return {"message": "health okay"}


@app.post("/api/sensors")
async def read_sensors(data: Sensor):
    timestamp_date = datetime.now().strftime(r"%Y-%m-%d %H:%M:%S")
    print(f"Direct read {data}")
    print(f"Timestamp: {timestamp_date}")
    print(f"Temp: {data.sensors["temp"]}")
    print(f"Humid: {data.sensors["humidity"]}")
    return {"message": "status ok"}