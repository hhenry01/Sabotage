package com.example.hot_or_cold;

public class constants {

    //The current URL used for posting and getting
    public static final String URL = "https://46b7-128-189-93-114.ngrok.io/";

    //URL for emulator posts and get
    public static final String EMULATOR_URL = "http://10.0.2.2:8000/";

    //URL for wifi host
    public static final String NGROK_URL = "http://dc8a-185-153-179-48.ngrok.io";

    //MAC address of wifi module for DE1-SoC #0, currently we only have one, since we only have one wifi module
    public static final String MAC_ADDRESS_0 = "de:4f:22:0a:8f:ec";

    //Radius of play range using GPS
    public static final int RADIUS = 100;

    //Radius that uses wifi instead of GPS
    public static final int SMALL_RADIUS = 20;

    //Constants for when location is updated
    public static final long MIN_TIME = 1000; // 1 second
    public static final long MIN_DIST = 5; // 5 Meters

    //The minimum wifi connection strength
    public static final int MIN_STRENGTH = -100; // in dBm, using -100, since that basically means you are not connected.

}
