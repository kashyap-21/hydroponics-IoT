# Hydroponics

## Workflow

### Monitor
- Arduino takes data from sensors and actuators
- Arduino sends data to esp32
- Esp32 sends sensors & actuators data to firebase
- This data is monitored from flutter app

## Control
- Control parameters are controlled from flutter app and stored on firebase
- Esp32 takes data from firebase
- 
## Resources 
- [Firebase Arduino Lib](https://github.com/mobizt/Firebase-ESP-Client) 
