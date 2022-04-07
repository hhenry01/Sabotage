package com.example.hot_or_cold;

import static com.example.hot_or_cold.constants.MIN_DIST;
import static com.example.hot_or_cold.constants.MIN_TIME;
import static com.example.hot_or_cold.constants.RADIUS;
import static com.example.hot_or_cold.constants.URL;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatButton;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.google.android.gms.maps.model.LatLng;

import java.util.HashMap;
import java.util.List;
import java.util.Set;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 *  This is where our leaderboard will be displayed. The leaderboards are per session
 *  so each session will have it's own leaderboard of players and their scores.
 */
public class Leaderboard extends AppCompatActivity {
    //Text variables
    private String Session_ID;
    private String Players_in_lobby;
    private String User_ID;
    private int players_in_lobby;

    //Location variables
    private LocationListener locationListener;
    private LocationManager locationManager;
    private LatLng latLng; //Current Location
    private LatLng circle_center; //Center of the session circle
    private LatLng actual_center; //actual location of the De1

    //Used for Get and Post requests
    private JsonPlaceHolderApi jsonPlaceHolderApi;

    @SuppressLint("MissingPermission")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_leaderboard);
        //Get information from the previous page: Either MapsActivity or Sabotage, maybe Leaderboard
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
        players_in_lobby = intent.getIntExtra("Players", -1);
        Players_in_lobby = Integer.toString(players_in_lobby);
        players_text.setText("Players: " + players_in_lobby);

        TextView user_id_text;
        user_id_text = findViewById(R.id.USER_ID);
        User_ID = intent.getStringExtra("UserID");
        user_id_text.setText("User ID: " + User_ID);

        TextView Leaderboard_text;
        Leaderboard_text = findViewById(R.id.LEADERBOARD);

        //Creating retrofit to convert classes to JSON and send them to URL
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        jsonPlaceHolderApi = retrofit.create(JsonPlaceHolderApi.class);

        //Check current location is within range
        locationListener = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {try {
                latLng = new LatLng(location.getLatitude(), location.getLongitude());

                //Checks if we're within the radius of the play session circle
                float[] curr_distance = new float[1];
                Location.distanceBetween(circle_center.latitude, circle_center.longitude,
                        latLng.latitude, latLng.longitude, curr_distance);

                //If we exit the radius of the session circle, we'll go back to the maps page
                if(curr_distance[0] > RADIUS) {
                    Intent intent = new Intent(Leaderboard.this, MapsActivity.class);
                    intent.putExtra("UserID", User_ID);
                    //When we leave the session, we let the server know by sending an special ID of -1
                    Post_Request post = new Post_Request(User_ID, "-1");
                    Call<Void> called = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                    called.enqueue(new Callback<Void>() {
                        @Override
                        public void onResponse(Call<Void> called, Response<Void> response) {

                            //Error checking code. Checks if we get a bad return code, e.g. 400
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
                                startActivity(intent);
                            }

                        }

                        //Checks if we failed to post, server issue, syntax issue, etc...
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

                            //Error checking code. Checks if we get a bad return code, e.g. 400
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

                            //Update players in lobby
                            players_in_lobby = posts.getNumPlayers();
                            Players_in_lobby = Integer.toString(players_in_lobby);
                            players_text.setText("Players: " + players_in_lobby);
                            //Iterate through leaderboard field and display all of them
                            List<HashMap<String, Integer>> Leaderboard = posts.getLeaderboard();
                            String content = "";
                            for(HashMap<String, Integer> Users : Leaderboard) {
                                Set<String> User_ID = Users.keySet();
                                String User_String = User_ID.iterator().next(); //returns only key
                                content += "UserID: " + User_String + "         Score: " + Users.get(User_String) + "\n";
                            }
                            Leaderboard_text.setText(content);
                        }

                        //Checks if we failed to post, server issue, syntax issue, etc...
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

    //Button/tab for returning to maps
    public void open_maps(View view) {
        Intent intent = new Intent(Leaderboard.this, MapsActivity.class);
        intent.putExtra("UserID", User_ID);
        //Send a post request to let the server know we're exiting the session
        Post_Request post = new Post_Request(User_ID, "-1");
        Call<Void> called = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
        called.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> called, Response<Void> response) {
                TextView players_text;
                players_text = findViewById(R.id.CURRENT_PLAYERS_IN_LOBBY);

                //If post is unsuccessful, display error code before resetting
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

            //If post completely failed, display message as well as cause before resetting
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

    //Button/tab for sabotage
    public void sabotage(View view) {
        Intent intent = new Intent(Leaderboard.this, sabotage.class);
        intent.putExtra("SessionID", Session_ID);
        intent.putExtra("Players", players_in_lobby);
        intent.putExtra("UserID", User_ID);
        intent.putExtra("Actual_lat", actual_center.latitude);
        intent.putExtra("Actual_long", actual_center.longitude);
        intent.putExtra("Latitude", circle_center.latitude);
        intent.putExtra("Longitude", circle_center.longitude);
        startActivity(intent);
    }

    //Button/tab for returning to maps
    public void hot_or_cold(View view) {
        Intent intent = new Intent(Leaderboard.this, hot_or_cold_Default.class);
        intent.putExtra("UserID", User_ID);
        intent.putExtra("SessionID", Session_ID);
        intent.putExtra("Players", players_in_lobby);
        intent.putExtra("Actual_lat", actual_center.latitude);
        intent.putExtra("Actual_long", actual_center.longitude);
        intent.putExtra("Latitude", circle_center.latitude);
        intent.putExtra("Longitude", circle_center.longitude);
        startActivity(intent);
    }
}