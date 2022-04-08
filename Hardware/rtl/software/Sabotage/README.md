## Hardware Component

![image](https://user-images.githubusercontent.com/71032077/162308862-72bdb28e-76e9-42ef-8d85-78d60eaf4439.png)

# GPS
- Gets location data of the De1 and parses these NMEA messages to receive longitude and latitude
- Converts the gps data to degrees decimal
- Programmed on Nios and uses serial RX and TX  to communicate with the GPS module

# Touchscreen
- Uses a lookup table to determine characters touched on the screen 

# VGA
- Displays numberpad and user input on the touchscreen display

# WiFi
- Transmits data from the De1 to the server
- Get proximity to De1 via WiFi RSSI
- Sends HTTP POST requests to the server with GPS and UserID inputted though the touch screen to the server

