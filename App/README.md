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
Current build requires a server to run most features. For testing purposes, go to [Testing Server](../Testing/Server) and run the server using WSL on your computer. Make sure to specify port as 8000. If you don't use port 8000, you will have to go through all java files that use RetroBuild and update the URL.

Note: Current local host URL used in RetroBuild code only works for emulators (emulator local host url: http://10.0.2.2:8000). To test on an actual android phone, go to [Server](../Server) and start a ngrok server on your local host computer. Copy paste the URL and go to constants class [here](hot_or_cold/app/src/main/java/com/example/hot_or_cold/constants.java) and update the String 'URL' with the ngrok URL. Then download onto phone as you would normally. (Also requires MongoDB to be set up).


## Current Status
### Start Screen
- Opening App takes you to Start Screen. 
- Pressing Start Button will ask for GPS permission.
- If GPS permission is not given, display message saying GPS is required, otherwise, goes to Log in Screen.
- Saves GPS permission status.

### Log in Screen
- Log in screen asks for a user ID of 4 digits.
- New players input 0, and are given a random 4 digit user ID from the server.
- Players with an existing user ID can enter their 4 digit user ID into the text field
- User ID is put into a Post Request to the server.
- If user ID is invalid, or Post Request is invalid, error message is displayed
- If user ID is valid, takes you to the Maps Screen.

### Maps Screen
- Uses Google Maps API to create map.
- Map zooms in on current location with marker. (Street-relative zoom).
- Map sends Get Request that retrieves current data on all active sessions.
- Map displays each session with a circle of radius: 100, on map, with session ID and current player information listed in a marker in said circle.
- Map tracks location of player at all time.
- If player is within one of the circles on the map, they can click it to start the game. 
- Clicking the circle, while within range will bring you to the Hot or Cold Screen.
- Has tabs/button that can be pressed to go back to Start Screen.
### Hot or Cold Screen
- Displays current session ID, user ID, and current players in lobby. 
- Has tabs/buttons that can be pressed to go to Sabotage Screen, Maps Screen, or Start Screen.
- Tracks current location on GPS.
- Every time the location is changed (about 5m), app will send a Get Request to check current session data.
- Will update numbers of players in lobby if changed (not tested, but I think it works).
- Will check if current player is still within the range of the session. If not within range, it'll take you back to the Maps Screen.
- Main button can be pressed on a 3 second cooldown, and will alternate between hot (red) and cold (blue) on press.

### Sabotage Screen
- Choose a sabotage to sabotage the current session.

### Leaderboard Screen
- Sends get request to server
- Displays all users who have won in the session and their score.

