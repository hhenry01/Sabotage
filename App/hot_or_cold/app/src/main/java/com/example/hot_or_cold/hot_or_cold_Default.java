package com.example.hot_or_cold;

import static com.example.hot_or_cold.constants.MAC_ADDRESS_0;
import static com.example.hot_or_cold.constants.MIN_DIST;
import static com.example.hot_or_cold.constants.MIN_STRENGTH;
import static com.example.hot_or_cold.constants.MIN_TIME;
import static com.example.hot_or_cold.constants.RADIUS;
import static com.example.hot_or_cold.constants.SMALL_RADIUS;
import static com.example.hot_or_cold.constants.URL;


import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatButton;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.os.Build;
import android.os.Bundle;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.view.View;
import android.widget.TextView;


import com.google.android.gms.maps.model.LatLng;

import java.util.List;
import java.util.Random;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;


/**
 * This is the main hot or cold page. It will need to receive a Session ID (String), players in lobby (int), User ID (String).
 * The Session ID, and UserID should remain constant while your on this screen. players in lobby will be updated by get requests
 * every time the player moves.
 *
 * We will also receive the location of the DE1, as well as the center of the session circle, to check if we've exited the range
 * of the play session.
 */
public class hot_or_cold_Default extends AppCompatActivity {

    int COOLDOWN = 3; //Default button cooldown value
    boolean reverse_or_not = false; //When false, our game will work normally, when true, we'll reverse the button colour

    //Text variables
    String Session_ID;
    String Players_in_lobby;
    String User_ID;
    int players_in_lobby;

    //Location variables
    private LocationListener locationListener;
    private LocationManager locationManager;
    private LatLng latLng; //Current Location
    private LatLng previous_latLng; //Previous Location before button press
    private LatLng circle_center; //Center of the session circle
    private LatLng actual_center; //actual location of the De1
    private boolean check = true;

    //Wifi Strength variables
    private int prev_strength = MIN_STRENGTH;
    private int curr_strength = 0;

    //For making Get and Post requests
    private JsonPlaceHolderApi jsonPlaceHolderApi;


    @SuppressLint("MissingPermission")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_hot_or_cold_default);

        //Get information from the previous page: Either MapsActivity or Sabotage, or Leaderboard
        Intent intent = getIntent();

        //This is the center of the circle shown on the maps page of the current session
        circle_center = new LatLng(intent.getDoubleExtra("Latitude", -1)
                                   ,intent.getDoubleExtra("Longitude", -1));

        //This is the actual coordinates of the DE1 within the session
        actual_center = new LatLng(intent.getDoubleExtra("Actual_lat", -1),
                                   intent.getDoubleExtra("Actual_long", -1));

        //Session details text, this should remain constant as long as we don't exit back into Maps or the Start screen
        TextView session_id_text;
        session_id_text = findViewById(R.id.SESSION_ID);
        Session_ID = intent.getStringExtra("SessionID");
        session_id_text.setText("Session ID: " + Session_ID);

        TextView players_text;
        players_text = findViewById(R.id.CURRENT_PLAYERS_IN_LOBBY);
        players_in_lobby = intent.getIntExtra("Players", -1) + 1;
        Players_in_lobby = Integer.toString(players_in_lobby);
        players_text.setText("Players: " + players_in_lobby);

        TextView user_id_text;
        user_id_text = findViewById(R.id.USER_ID);
        User_ID = intent.getStringExtra("UserID");
        user_id_text.setText("User ID: " + User_ID);

        //Set button colour
        AppCompatButton main_button;
        main_button = findViewById(R.id.rlVar1);
        Drawable green_button = getResources().getDrawable(R.drawable.roud_button);
        main_button.setBackgroundDrawable(green_button);

        //Using Retrofit to create a json formatter that sends/receives data to/from our URL
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        jsonPlaceHolderApi = retrofit.create(JsonPlaceHolderApi.class);

        //Check current location is within range
        //Code adapted from: https://javapapers.com/android/get-current-location-in-android/
        locationListener = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {try {
                //Our current location
                latLng = new LatLng(location.getLatitude(), location.getLongitude());

                //If this is the first time we've gone to this page, then our previous location will be our first location.
                if(check) {
                    previous_latLng = latLng;
                    check = false;
                }

                //Checks if we're within the radius of the play session circle
                float[] curr_distance = new float[1];
                Location.distanceBetween(circle_center.latitude, circle_center.longitude,
                        latLng.latitude, latLng.longitude, curr_distance);

                //If we exit the radius of the session circle, we'll go back to the maps page
                if(curr_distance[0] > RADIUS) {
                    Intent intent = new Intent(hot_or_cold_Default.this, MapsActivity.class);
                    intent.putExtra("UserID", User_ID);
                    //We'll also send a post request that will let the server know we left the session
                    Post_Request post = new Post_Request(User_ID, "-1"); //-1 sessionID means we left the session
                    Call<Void> called = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                    called.enqueue(new Callback<Void>() {
                        @Override
                        public void onResponse(Call<Void> called, Response<Void> response) {

                            //Error checking code, if the post did not work, we can see the code
                            if(!response.isSuccessful()) {
                                players_text.setText("Exiting Session Post did not work, code: " + response.code());
                                players_text.postDelayed(new Runnable() {
                                    @Override
                                    public void run() {
                                        players_text.setText("Players" + players_in_lobby);
                                    }
                                }, 3 * 1000);
                                return;
                            } else {
                                startActivity(intent); //Go to maps page
                            }
                        }

                        //Error checking why our post did not work, server issue? syntax issue? etc...
                        @Override
                        public void onFailure(Call<Void> called, Throwable t) {
                            players_text.setText(t.getMessage());
                            System.out.println(t.getCause());
                            players_text.postDelayed(new Runnable() {
                                @Override
                                public void run() {
                                    players_text.setText("Players" + players_in_lobby);
                                }
                            }, 3 * 1000);
                        }

                    });
                } else {
                    //Get Request to check for the player count in current lobby. If location changes we'll send a get request and update player count if necessary
                    Call<Get_Sabotage> call = jsonPlaceHolderApi.GET_SABOTAGE_CALL(Integer.parseInt(User_ID), Integer.parseInt(Session_ID));
                    call.enqueue(new Callback<Get_Sabotage>() {
                        @Override
                        public void onResponse(Call<Get_Sabotage> call, Response<Get_Sabotage> response) {

                            //Error checking code, if we get a bad return code, e.g. 400.
                            if(!response.isSuccessful()) {
                                players_text.setText("Code: " + response.code());
                                players_text.postDelayed(new Runnable() {
                                    @Override
                                    public void run() {
                                        players_text.setText("Players" + players_in_lobby);
                                    }
                                }, 3 * 1000);
                                return;
                            }

                            Get_Sabotage posts = response.body();

                            //Update lobby player count, SessionID and UserID should remain the same.
                            players_in_lobby = posts.getNumPlayers();
                            Players_in_lobby = Integer.toString(players_in_lobby);
                            players_text.setText("Players: " + players_in_lobby);
                        }

                        //Error checking code, if we could not connect to server, or json request wasn't formatted correctly
                        @Override
                        public void onFailure(Call<Get_Sabotage> call, Throwable t) {
                            players_text.setText(t.getMessage());
                            System.out.println(t.getCause());
                            players_text.postDelayed(new Runnable() {
                                @Override
                                public void run() {
                                    players_text.setText("Players" + players_in_lobby);
                                }
                            }, 3 * 1000);
                        }

                    });
                }
            }
            catch (SecurityException e){
                e.printStackTrace();
            }

            }

            @Override
            public void onStatusChanged(String s, int i, Bundle bundle) {

            }

            @Override
            public void onProviderEnabled(String s) {

            }

            @Override
            public void onProviderDisabled(String s) {

            }
        };

        locationManager = (LocationManager) getSystemService(LOCATION_SERVICE);
        try {
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, MIN_TIME, MIN_DIST, locationListener);
        }
        catch (SecurityException e){
            e.printStackTrace();
        }

    }


    /**
     * When the button is pressed, it'll use your previous GPS location and your current GPS location to check if you are closer (hotter) or further (colder)
     * by displaying a red (hot) or blue (cold) button. The button will remain this colour for 3 seconds, in that time, it'll be unable to be pressed,
     * as a cooldown feature. When within the Small Radius, we will use the wifi feature, which will use the Wifi RSSI strength as
     * a substitute for GPS location. Or we will just display a very close button (dark red), if wifi is not working. These features may be changed
     * based on whatever sabotage is currently going on. The phone will also vibrate based on whether you are closer or not,
     * however this feature may not work depending on your Android API level.
     * @param view Necessary for button functionality
     */
    @SuppressLint("MissingPermission")
    public void hot_or_cold(View view) {

        AppCompatButton main_button;
        // set relative layout with its id
        Drawable blue_button = getResources().getDrawable(R.drawable.roud_button_blue);
        Drawable red_button = getResources().getDrawable(R.drawable.roud_button_red);
        Drawable very_close_button = getResources().getDrawable(R.drawable.very_close_button);
        Drawable green_button = getResources().getDrawable(R.drawable.roud_button);
        Drawable yellow_button = getResources().getDrawable(R.drawable.yellow_button);
        main_button = findViewById(R.id.rlVar1);

        //Vibration code adapted from: https://stackoverflow.com/questions/13950338/how-to-make-an-android-device-vibrate-with-different-frequency
        Vibrator v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        int vibration_time;
        TextView very_close_text;
        very_close_text = findViewById(R.id.close);
        /*
          Prev location will be the location of the last button press, or the location where you entered the session
          if you just entered the session.
         */

        float[] prev_dist = new float[1];
        float[] curr_dist = new float[1];


        //Check our distance from the DE1 with our previous location and our current location to check which is closer.
        Location.distanceBetween(previous_latLng.latitude, previous_latLng.longitude, actual_center.latitude, actual_center.longitude, prev_dist);
        Location.distanceBetween(latLng.latitude, latLng.longitude, actual_center.latitude, actual_center.longitude, curr_dist);

        //Check whether our lobby is being sabotaged, and by which sabotage
        Call<Get_Sabotage> call = jsonPlaceHolderApi.GET_SABOTAGE_CALL(Integer.parseInt(User_ID), Integer.parseInt(Session_ID));
        call.enqueue(new Callback<Get_Sabotage>() {
            @Override
            public void onResponse(Call<Get_Sabotage> call, Response<Get_Sabotage> response) {

                //Error checking code, for if we get a bad response, e.g. 400
                if(!response.isSuccessful()) {
                    very_close_text.setText("Code: " + response.code());
                    return;
                }

                //Checking if we're being sabotaged and updating player count
                Get_Sabotage posts = response.body();
                TextView players_text;
                players_text = findViewById(R.id.CURRENT_PLAYERS_IN_LOBBY);
                players_in_lobby = posts.getNumPlayers();
                Players_in_lobby = Integer.toString(players_in_lobby);
                players_text.setText("Players: " + players_in_lobby);

                if(posts.getSabotage() == 0) { //Not being sabotaged, set default values
                    COOLDOWN = 3;
                    reverse_or_not = false;
                } else if(posts.getSaboteurID() == Integer.parseInt(User_ID)) {
                    //Do nothing
                } else if(posts.getSabotage() == 1) { //Reverse sabotage, reverse the signal, i.e. blue when actually red and vice versa
                    COOLDOWN = 3;
                    reverse_or_not = true;
                    very_close_text.setText("You are being sabotaged!");
                } else if(posts.getSabotage() == 2) { //Cooldown sabotage, increase the button cooldown to 10, or the duration, whichever is smaller
                    COOLDOWN = posts.getDuration() < 10 ? (int) posts.getDuration() : 10;
                    reverse_or_not = false;
                    very_close_text.setText("You are being sabotaged!");
                } else if(posts.getSabotage() == 3) { //Random sabotage, could leave everything the same, or everything could be reversed. MIND GAMES with sabotage 1
                    COOLDOWN = 3;
                    Random random_int = new Random(posts.getSaboteurID());
                    reverse_or_not = random_int.nextBoolean();
                    very_close_text.setText("You are being sabotaged!");
                } else {
                    very_close_text.setText("Sabotage not recognized"); //We shouldn't get this, but just to make sure we only received valid sabotages
                }

            }

            //Error checking code, for if the server has issues, or if our JSON request was badly formatted, syntax errors, etc...
            @Override
            public void onFailure(Call<Get_Sabotage> call, Throwable t) {
                very_close_text.setText(t.getMessage());
                System.out.println(t.getCause());
            }
        });

        boolean update_location = true;
        //If you are within SMALL_RADIUS, you will use the wifi rssi to measure closeness instead of GPS now
        //if wifi of the DE1 could not be found we'll just display a darker red button and let them know they're within the SMALL_RADIUS
        int wifi_strength = wifi_strength_test();
        if(wifi_strength != 0) {
            if((wifi_strength < 0) ^ reverse_or_not) {
                if(reverse_or_not) {
                    wifi_strength = wifi_strength * (-1);
                }
                main_button.setBackgroundDrawable(red_button);
                vibration_time = 1000;
                very_close_text.setText("Wifi level: " + wifi_strength);
            } else {
                if(reverse_or_not) {
                    wifi_strength = wifi_strength * (-1);
                }
                main_button.setBackgroundDrawable(blue_button);
                vibration_time = 500;
                very_close_text.setText("Wifi level: " + wifi_strength * (-1));
            }
            //If we aren't within SMALL_RADIUS we'll just check if our previous location is closer or further than our current
        } else if(curr_dist[0] <= SMALL_RADIUS) {
            main_button.setBackgroundDrawable(very_close_button);
            vibration_time = 1000;
            very_close_text.setText("You are within " + SMALL_RADIUS + "m");
        } else if(Math.abs(prev_dist[0] - curr_dist[0]) <= 1) {
            main_button.setBackgroundDrawable(yellow_button);
            vibration_time = 500;
            very_close_text.setText("Move at least 10m before pressing the button");
            prev_strength = MIN_STRENGTH;
            update_location = false;
        } else if((prev_dist[0] <= curr_dist[0]) ^ reverse_or_not) {
            main_button.setBackgroundDrawable(blue_button);
            vibration_time = 500;
            very_close_text.setText("You are not within " + SMALL_RADIUS + "m" );
            prev_strength = MIN_STRENGTH;
        } else {
            main_button.setBackgroundDrawable(red_button);
            vibration_time = 1000;
            very_close_text.setText("You are not within " + SMALL_RADIUS + "m" );
            prev_strength = MIN_STRENGTH;
        }

        //After checking, we'll set the previous location to be our current location on button press
        if(update_location) {
            previous_latLng = latLng;
        }

        //Vibrate phone with amplitude based on whether we were hot or cold. May not work for certain API levels
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            v.vibrate(VibrationEffect.createOneShot(vibration_time, VibrationEffect.DEFAULT_AMPLITUDE));
        } else {
            //deprecated in API 26
            v.vibrate(vibration_time);
        }

        //Don't let button be pressed for COOLDOWN seconds.
        //Cooldown code adapted from: https://stackoverflow.com/questions/13398104/android-how-to-put-a-delay-after-a-button-is-pushed
        main_button.setEnabled(false);
        main_button.postDelayed(new Runnable() {
            @Override
            public void run() {
                main_button.setEnabled(true);
                main_button.setBackgroundDrawable(green_button);
                very_close_text.setText("");

            }
        }, COOLDOWN * 1000);

    }

    //Button/tab for returning to start screen
    public void return_home(View view) {
        Intent intent = new Intent(hot_or_cold_Default.this, MainActivity.class);
        startActivity(intent);
    }

    //Button/tab for returning to maps
    public void open_maps(View view) {
        Intent intent = new Intent(hot_or_cold_Default.this, MapsActivity.class);
        intent.putExtra("UserID", User_ID);
        //Send a post request to let the server know we're exiting the session
        Post_Request post = new Post_Request(User_ID, "-1");
        Call<Void> called = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
        called.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> called, Response<Void> response) {

                TextView players_text;
                players_text = findViewById(R.id.CURRENT_PLAYERS_IN_LOBBY);
                //Error checking code, text will display error code, e.g. 400 then reset
                if(!response.isSuccessful()) {
                    players_text.setText("Code: " + response.code());
                    players_text.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            players_text.setText("Players" + players_in_lobby);
                        }
                    }, 3 * 1000);
                    return;
                } else {
                    startActivity(intent);
                }

            }

            //Check for failure, i.e. syntax error, server error, other JSON related errors.
            @Override
            public void onFailure(Call<Void> called, Throwable t) {
                TextView players_text;
                players_text = findViewById(R.id.CURRENT_PLAYERS_IN_LOBBY);
                players_text.setText(t.getMessage());
                System.out.println(t.getCause());
                players_text.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        players_text.setText("Players" + players_in_lobby);
                    }
                }, 3 * 1000);
            }
        });
    }

    //Button/tab for sabotage screen
    public void sabotage(View view) {
        Intent intent = new Intent(hot_or_cold_Default.this, sabotage.class);
        intent.putExtra("SessionID", Session_ID);
        intent.putExtra("Players", players_in_lobby);
        intent.putExtra("UserID", User_ID);
        intent.putExtra("Actual_lat", actual_center.latitude);
        intent.putExtra("Actual_long", actual_center.longitude);
        intent.putExtra("Latitude", circle_center.latitude);
        intent.putExtra("Longitude", circle_center.longitude);
        startActivity(intent);
    }

    //Button/tab for leaderboards screen
    public void leaderboards(View view) {
        Intent intent = new Intent(hot_or_cold_Default.this, Leaderboard.class);
        intent.putExtra("SessionID", Session_ID);
        intent.putExtra("Players", players_in_lobby);
        intent.putExtra("UserID", User_ID);
        intent.putExtra("Actual_lat", actual_center.latitude);
        intent.putExtra("Actual_long", actual_center.longitude);
        intent.putExtra("Latitude", circle_center.latitude);
        intent.putExtra("Longitude", circle_center.longitude);
        startActivity(intent);
    }

    /**
     * Button for checking whether we won or not. It will call a lobby get request which
     * will check the server to see if we have won the game or not and display a message accordingly.
     * The button also has a cooldown so that someone can't spam the server with get requests
     * @param view necessary parameter for button
     */
    public void win(View view) {
        Call<Get_Sabotage> call = jsonPlaceHolderApi.GET_SABOTAGE_CALL(Integer.parseInt(User_ID), Integer.parseInt(Session_ID));
        call.enqueue(new Callback<Get_Sabotage>() {
            @Override
            public void onResponse(Call<Get_Sabotage> call, Response<Get_Sabotage> response) {

                //Error checking code, displays error code, e.g. 400
                if(!response.isSuccessful()) {
                    AppCompatButton win_button = findViewById(R.id.rlVar6);
                    win_button.setText("Code: " + response.code());
                    win_button.setEnabled(false);
                    win_button.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            win_button.setEnabled(true);
                            win_button.setText("DID I WIN?");
                        }
                    }, 3 * 1000);
                    return;
                }

                //Check if we won, display win message if we did, then put button on cooldown
                Get_Sabotage posts = response.body();
                AppCompatButton win_button = findViewById(R.id.rlVar6);

                if(posts.isWin()) {
                    win_button.setText("You've Won!!!");
                } else {
                    win_button.setText("Did not won :(");
                }

                win_button.setEnabled(false);
                win_button.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        win_button.setEnabled(true);
                        win_button.setText("DID I WIN?");
                    }
                }, 3 * 1000);

            }

            //More error checking code, for if our server failed, or get request formatting was off, syntax error, etc...
            @Override
            public void onFailure(Call<Get_Sabotage> call, Throwable t) {
                AppCompatButton win_button = findViewById(R.id.rlVar6);
                win_button.setText(t.getMessage());
                win_button.setEnabled(false);
                win_button.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        win_button.setEnabled(true);
                        win_button.setText("DID I WIN?");
                    }
                }, 3 * 1000);
            }
        });
    }


    /**Returns 0 if the wifi could not be found, returns the wifi strength level (a negative number)
      curr_strength is greater than prev_strength, returns a positive number which is the
      wifi strength level * -1 if the prev_strength is greater than curr_strength. Then sets
      prev_strength to be equal to the curr_strength. Code created using docs:
     https://developer.android.com/reference/android/bluetooth/le/ScanResult**/
    private int wifi_strength_test() {
        WifiManager wifi = (WifiManager) getApplicationContext().getSystemService(WIFI_SERVICE);
        wifi.startScan();
        List<ScanResult> scan = wifi.getScanResults();
        curr_strength = 0;//Set our default to 0, since 0 is impossible for RSSI values
        //Check all available wifis
        for(ScanResult wifis : scan) {
            //If wifi matches our DE1 Mac address, record the rssi strength level
            if (wifis.BSSID.equals(MAC_ADDRESS_0)) {
                curr_strength = wifis.level;
                break;
            }
        }
        //We didn't find the wifi module.
        if(curr_strength == 0) {
            return 0;
        }
        //We found the wifi module
        if(curr_strength > prev_strength) { //Our curr_strength is greater
            prev_strength = curr_strength;
            return curr_strength;
        } else { //Our curr_strength is smaller or hasn't changed
            prev_strength = curr_strength;
            return curr_strength * (-1);
        }
    }
}