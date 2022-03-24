# WiFi Module
We are using this [microcontroller](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/overview) to provide WiFi to
our De1-SoC. The code in WiFi_Module.ino is based on 
[example code from Adafruit](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide). That link also includes setup instruction. We also
used [this](https://randomnerdtutorials.com/esp8266-nodemcu-http-get-post-arduino/#http-post) handy resource. To setup the module as a WiFi access point, we
referenced [this tutorial](https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/example-sketch-ap-web-server#:~:text=Not%20only%20can%20the%20ESP8266,pages%20to%20any%20connected%20client.).
We implemented the functionality to read data like GPS coordinates over serial and format the message into a JSON string to send as part of an HTTP request. When it send the request, it sends it to our 
[backend server](../../Server/) for further processing. It also contains more details on how the HTTP requests are formatted.

Note that the `host` field is currently set to an [ngrok](https://ngrok.com/) URL. This is for development purposes and
will need to be changed whenever the service is started. It will not be used for deployment. Currently, the server
will need to be run on port 80, which will require sudo permissons.

## Data Processing
The data which the WiFi module POSTs will be received from the De1-SoC via the serial interface. Details on what data
needs to be POSTed can be found on the [server's README](../../Server/README.md). The data which is received is
formatted as follows:
```
$<SessionID>, <latitude>, <N/S>, <longitude>, <E/W>, <NumPlayers>, <Win>*
```
In the current testing stage, use the Arduino IDE to enter into the COM port:
```
$777,49.123,N,123.456,W,33,111*
```
