# Hot or Cold App 

App created on Android Studios, written in Java. All xml templates, Java classes and other relevant files located [here](hot_or_cold/app/src/main)

## Figma/UI concepts
Anyone with this link can view the Figma UI draft designs. These are the main templates that our app will be following, although they may be subject to changes as certain features come and go. 

[click here for Figma link](https://www.figma.com/file/olH5i8Xk7tbdFAkaufjcTU/Untitled?node-id=0%3A1)

![ScreenShot](Figma_UI_design.png?raw=true) 

## Installation

Click [here](https://developer.android.com/studio) and download android studio.
Then go to File->Open and find the Hot or Cold app in whatever folder you downloaded it into. It should have a little android logo next to it.

## Emulator
To use Android Studio built in emulators, on the top, next to app, find Device Manager. Then Create Device by picking a android phone model (This creates the dimensions), and then pick an API level. You'll have to download an API level if you haven't already. Once that is done you can create the emulator. 

For purposes of testing, I am using a pixel 3a XL Resolution: (1080 x 2160), DP: (432 x 864), with an API level: 25 (Nougat). This older API level allows for a faster Emulator, but if your computer can handle it, a more modern API level (such as 30 or 31) is recommended for more accuracy.

## Testing
For testing on a physical android phone, click [here](https://developer.android.com/studio/run/device) for more details. You just plug in a USB charging cable from your computer to your android phone. Wait for the device manager to recognize your device, and press the play button the same you would on an emulator.

For testing on virtual (i.e. emulator) phone, open device manager, press the play button next to the device you want to emulate (this could take a while), and run app (play button at the top of screen) to run the app. If you want to see error messages, run it on debug mode (debug symbol next to run app button). If you make any modifications while the app is running, and you want to see the changes, rerun the app by pressing the run app or debug button again. 


### Networking
Current build requires a server to run most features. For testing purposes, go to [Testing Server](../Testing/Server) and run the server using WSL on your computer.

Note: Current local host URL used in RetroBuild code only works for emulators (emulator local host url: http://10.0.2.2:8000 - with port 8000). To test on an actual android phone, go to [Server](../Server) and start a ngrok server on your local host computer. Copy paste the URL and go to constants class [here](hot_or_cold/app/src/main/java/com/example/hot_or_cold/constants.java) and update the String 'URL' with the ngrok URL. Then download onto phone as you would normally. (Also requires MongoDB to be set up).


## Current Status
### Start Screen
- Opening App takes you to Start Screen. 
- Pressing Start Button will ask for GPS permission.
- If GPS permission is not given, display message saying GPS is required, otherwise, goes to Log in Screen.
- Saves GPS permission status.

### Log in Screen
- Log in screen asks for a user ID of 4 digits.
- New players input 0, and are given a random 4 digit user ID from the server.
- Players with an existing user ID can enter their 4 digit user ID into the text field allowing them to use their previous account
- user ID is put into a Post Request to the server.
- If user ID is invalid, or Post Request is invalid, error message is displayed
- If user ID is valid, takes you to the Maps Screen.

### Maps Screen
- Uses Google Maps API to create map.
- Map zooms in on current location with marker. (Street-relative zoom).
- Map sends Get Request that retrieves current data on all active sessions.
- Map displays each session with a circle of radius: 100, on map, with session ID and current player information listed in a marker in said circle.
- Map tracks location of player at all time and displays their location on map.
- If player is within one of the circles on the map, they can click it to start the game. 
- Clicking the circle, while within range will bring you to the Hot or Cold Screen.
- Has tabs/button that can be pressed to go back to Start Screen.
### Hot or Cold Screen
- Displays current session ID, user ID, and current players in lobby. 
- Has tabs/buttons that can be pressed to go to Sabotage Screen, Maps Screen, or Start Screen.
- Tracks current location on GPS.
- Every time the location is changed (about 5m), app will send a Get Request to check current session data.
- Will update numbers of players in lobby if changed.
- Will check if current player is still within the range of the session. If not within range, it'll take you back to the Maps Screen.
- Main button can be pressed on a 3 second cooldown.
- Outside of a 20m radius of the DE1, when button is pressed, it will check your current location and compare it with the location of your last button press to determine if you are hot (red/closer) or cold (blue/further).
- Within 20m radius of the DE1, the button uses the same tatic as above, except it uses WiFi RSSI strength instead of location.


### Sabotage Screen
- Choose a sabotage to sabotage the current session.
- Each sabotage will affect every player in the lobby except for the saboteur.
- Sabotage only works if there is at least one other person in the lobby to sabotage.
- Sabotage requires sabotage tokens. New players start with 3. Each sabotage uses one token. You can earn new tokens by winning a game.
- Sabotages include: 
- Cooldown sabotage - Increase button cooldown from 3 to 10 seconds
- Reverse sabotage - Display blue when hot and red when cold
- Random sabotage - Might be the same as reverse sabotage, or might do nothing. This creates some mind-games because players can't simply assume their buttons are reversed and instead have to use logic, or take a guess.


### Leaderboard Screen
- Sends get request to server
- Displays all users who have won in the session and their score.

### Test Server
- Hidden admin page mode that can be used to test individual features before adding it to main app. (not accessible in current build)

## Outside Sources
- All HTTP get and post request code was created using the Retrofit API, which allows classes to be converted to JSON strings which can then be sent to a URL.
- I learned how to use the Retrofit API from this YouTuber's Retrofit series - [Coding in Flow](https://www.youtube.com/watch?v=4JGvDUlfk7Y&list=PLrnPJCHvNZuCbuD3xpfKzQWOj3AXybSaM&ab_channel=CodinginFlow) 
- The maps screen was created using the Google Maps API. After creating an account and an API Key, I was able to follow the instructions at [Maps SDK for Android](https://developers.google.com/maps/documentation/android-sdk/start) and build on the code.
- Location tracking code was built on [this](https://javapapers.com/android/get-current-location-in-android/) example, using the LocationListener Class.
- Vibrating code used from StackOverflow example [here](https://stackoverflow.com/questions/13950338/how-to-make-an-android-device-vibrate-with-different-frequency)
- Button Cooldown code adapted from StackOverflow example [here](https://stackoverflow.com/questions/13398104/android-how-to-put-a-delay-after-a-button-is-pushed)
- I used the android studio docs on ScanResult/WifiManger [here](https://developer.android.com/reference/android/bluetooth/le/ScanResult) to create the WiFi RSSI hot or cold feature.
- GPS Permissions code adapted from StackOverflow example [here](https://stackoverflow.com/questions/43039149/how-to-ask-gps-permission-and-after-finish-show-map)
- Location circles created using [Maps Circle SDK for Android](https://developers.google.com/maps/documentation/android-sdk/reference/com/google/android/libraries/maps/model/Circle)
- Code for converting distance (in meters) offset to new longitude, latitude coordinates based on [example](https://stackoverflow.com/questions/639695/how-to-convert-latitude-or-longitude-to-meters)



## Error Checking
- Try/Catch statements on all location based features, since GPS location is not guaranteed.
- Post/Get requests are always checked to see if they had a failure or returned an error code, and will display the error for a brief moment on the app to let user know their command didn't go through.
- If statements stop user from inputting invalid options and displays an error message e.g. login screen - only takes integers, sabotage - only works with enough tokens and players in lobby.

## Possibility for Extension
- Sabotages can be easily added as the code is the same for all sabotages, except posting a different Sabotage ID. This sabotage ID is then read in the hot or cold page, where you can affect the hot or cold feature however you want, based on the sabotage ID.
- Leaderboard features such as a global leaderboard can be easily implemented as the code on user end should remain the same, just have server send all leaderboard data, instead of just the session leaderboard data.
- Radius of sessions can be easily changed by going into constants and updating the 100m to anything else. This can increase/decrease the play range depending on possible factors.
- Radius of WiFi RSSI feature can be similarly changed, from 20m to a larger radius, if one WiFi module has a stronger signal strength than another or vice versa.
- Multiple DE1's can be added, if each has the proper equipment. To update app, simply add the MAC address of the WiFi modules into the constants class, and check for it in the wifi_strength_test function (at the bottom of hot_or_cold_default_class).



