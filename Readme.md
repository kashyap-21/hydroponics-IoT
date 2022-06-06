# Hydroponics

## Workflow

### Monitor
- Arduino takes data from sensors and actuators
- Arduino sends data to esp32
- Esp32 sends sensors & actuators data to firebase
- This data is monitored from flutter app

### Control
- Control parameters are controlled from flutter app and stored on firebase
- Esp32 takes data from firebase
- Esp32 controls the actuators
- Update the firebase database with new values

## Parameters

### Monitor
- Temperature : float
- Humidity  : float
- Moisture  : int
- pH  : float

### Control
- Light : number
- 4 Motors
  - 2 for acid base : number (time to keep it on)
  - 1 for continuous water flow : bool
  - 1 for ventilation : bool

## Resources 
- [Firebase Arduino Lib](https://github.com/mobizt/Firebase-ESP-Client) 
