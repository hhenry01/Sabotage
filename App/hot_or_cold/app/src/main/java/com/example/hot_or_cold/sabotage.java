package com.example.hot_or_cold;

import static com.example.hot_or_cold.constants.MIN_DIST;
import static com.example.hot_or_cold.constants.MIN_TIME;
import static com.example.hot_or_cold.constants.RADIUS;
import static com.example.hot_or_cold.constants.URL;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.AppCompatButton;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.se.omapi.Session;
import android.view.View;
import android.widget.TextView;

import com.google.android.gms.maps.model.LatLng;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class sabotage extends AppCompatActivity {


    //Text variables
    String Session_ID;
    String Players_in_lobby;
    String User_ID;
    int players_in_lobby;

    //Location variables
    private LocationListener locationListener;
    private LocationManager locationManager;
    private LatLng latLng; //Current Location
    private LatLng circle_center;
    private LatLng actual_center;
    private List<Locations> locations;
    private int sabotage_tokens = 4;

    private AppCompatButton sabotage_button;

    private JsonPlaceHolderApi jsonPlaceHolderApi;


    @SuppressLint("MissingPermission")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sabotage);

        Intent intent = getIntent();

        sabotage_button = findViewById(R.id.Sabotage_button);
        circle_center = new LatLng(intent.getDoubleExtra("Latitude", -1)
                ,intent.getDoubleExtra("Longitude", -1));

        actual_center = new LatLng(intent.getDoubleExtra("Actual_lat", -1),
                intent.getDoubleExtra("Actual_long", -1));

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

        TextView sabotage_tokens_text;
        sabotage_tokens_text = findViewById(R.id.SABOTAGE_TOKENS);

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
                float[] curr_distance = new float[1];
                Location.distanceBetween(circle_center.latitude, circle_center.longitude,
                        latLng.latitude, latLng.longitude, curr_distance);
                if(curr_distance[0] > RADIUS) {
                    Intent intent = new Intent(sabotage.this, MapsActivity.class);
                    intent.putExtra("UserID", User_ID);
                    Post_Request post = new Post_Request(User_ID, "-1");
                    Call<Void> called = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                    called.enqueue(new Callback<Void>() {
                        @Override
                        public void onResponse(Call<Void> called, Response<Void> response) {

                            if(!response.isSuccessful()) {
                                return;
                            } else {
                                startActivity(intent);
                            }

                        }

                        @Override
                        public void onFailure(Call<Void> called, Throwable t) {
                        }
                    });
                } else {
                    Call<Get_Sabotage> call = jsonPlaceHolderApi.GET_SABOTAGE_CALL(Integer.parseInt(User_ID), Integer.parseInt(Session_ID));
                    call.enqueue(new Callback<Get_Sabotage>() {
                        @Override
                        public void onResponse(Call<Get_Sabotage> call, Response<Get_Sabotage> response) {

                            if(!response.isSuccessful()) {
                                players_text.setText("Code: " + response.code());
                                return;
                            }

                            Get_Sabotage posts = response.body();
                            players_in_lobby = posts.getNumPlayers();
                            Players_in_lobby = Integer.toString(players_in_lobby);
                            players_text.setText("Players: " + players_in_lobby);
                            sabotage_tokens = posts.getSabTokens();
                            sabotage_tokens_text.setText("Sabotage Tokens: " + sabotage_tokens);
                        }

                        @Override
                        public void onFailure(Call<Get_Sabotage> call, Throwable t) {
                            players_text.setText(t.getMessage());
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

    //Button/tab for returning to start screen
    public void Open_maps(View view) {
        Intent intent = new Intent(sabotage.this, MapsActivity.class);
        intent.putExtra("UserID", User_ID);
        Post_Request post = new Post_Request(User_ID, "-1");
        Call<Void> called = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
        called.enqueue(new Callback<Void>() {
            @Override
            public void onResponse(Call<Void> called, Response<Void> response) {

                if(!response.isSuccessful()) {
                    return;
                } else {
                    startActivity(intent);
                }

            }

            @Override
            public void onFailure(Call<Void> called, Throwable t) {
            }
        });
    }

    //Button/tab for returning to maps
    public void hot_or_cold(View view) {
        Intent intent = new Intent(sabotage.this, hot_or_cold_Default.class);
        intent.putExtra("UserID", User_ID);
        intent.putExtra("SessionID", Session_ID);
        intent.putExtra("Players", players_in_lobby);
        intent.putExtra("Actual_lat", actual_center.latitude);
        intent.putExtra("Actual_long", actual_center.longitude);
        intent.putExtra("Latitude", circle_center.latitude);
        intent.putExtra("Longitude", circle_center.longitude);
        startActivity(intent);
    }

    public void leaderboards(View view) {
        Intent intent = new Intent(sabotage.this, Leaderboard.class);
        intent.putExtra("UserID", User_ID);
        intent.putExtra("SessionID", Session_ID);
        intent.putExtra("Players", players_in_lobby);
        intent.putExtra("Actual_lat", actual_center.latitude);
        intent.putExtra("Actual_long", actual_center.longitude);
        intent.putExtra("Latitude", circle_center.latitude);
        intent.putExtra("Longitude", circle_center.longitude);
        startActivity(intent);
    }

    public void swap_sabotage(View view) {
        List<String> current_location = new ArrayList<>();

        current_location.add(Double.toString(latLng.latitude));
        current_location.add(Double.toString(latLng.longitude));

        Call<Get_Sabotage> call = jsonPlaceHolderApi.GET_SABOTAGE_CALL(Integer.parseInt(User_ID), Integer.parseInt(Session_ID));
        call.enqueue(new Callback<Get_Sabotage>() {
            @Override
            public void onResponse(Call<Get_Sabotage> call, Response<Get_Sabotage> response) {

                if(!response.isSuccessful()) {
                    sabotage_button.setText("Code: " + response.code());
                    return;
                }

                Get_Sabotage posts = response.body();
                sabotage_tokens = posts.getSabTokens();
                if(posts.getSabotage() != 0) { //Check if lobby is already being sabotaged
                    sabotage_button.setText("Session Currently being sabotaged");
                } else {
                    if(sabotage_tokens <= 0) { //Check sabotage tokens count
                        sabotage_button.setText("Not enough sabotage tokens");
                    } else {
                        if(players_in_lobby < 2) { //Check if there are players in lobby to sabotage
                            sabotage_button.setText("No players to sabotage");
                        } else {
                            Post_Request post = new Post_Request(User_ID, Session_ID, "1");
                            Call<Void> call_post = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                            call_post.enqueue(new Callback<Void>() {
                                @Override
                                public void onResponse(Call<Void> call_post, Response<Void> response) {

                                    if(!response.isSuccessful()) {
                                        System.out.println(response.body());
                                        sabotage_button.setText("Code: " + response.code());
                                        return;
                                    } else {
                                        sabotage_button.setText("Swap Sabotage Initiated");
                                        sabotage_tokens--;
                                        TextView sabotage_tokens_text;
                                        sabotage_tokens_text = findViewById(R.id.SABOTAGE_TOKENS);
                                        sabotage_tokens_text.setText("Sabotage Tokens: " + sabotage_tokens);
                                    }

                                }

                                @Override
                                public void onFailure(Call<Void> call_post, Throwable t) {
                                    sabotage_button.setText(t.getMessage());
                                }
                            });

                        }
                    }
                }

            }

            @Override
            public void onFailure(Call<Get_Sabotage> call, Throwable t) {
                sabotage_button.setText(t.getMessage());
            }
        });


        sabotage_button.postDelayed(new Runnable() {
            @Override
            public void run() {
                sabotage_button.setText("Sabotage");
            }
        }, 3 * 1000);
    }

    public void cooldown_sabotage(View view) {
        List<String> current_location = new ArrayList<>();

        current_location.add(Double.toString(latLng.latitude));
        current_location.add(Double.toString(latLng.longitude));

        Call<Get_Sabotage> call = jsonPlaceHolderApi.GET_SABOTAGE_CALL(Integer.parseInt(User_ID), Integer.parseInt(Session_ID));
        call.enqueue(new Callback<Get_Sabotage>() {
            @Override
            public void onResponse(Call<Get_Sabotage> call, Response<Get_Sabotage> response) {

                if(!response.isSuccessful()) {
                    sabotage_button.setText("Code: " + response.code());
                    return;
                }

                Get_Sabotage posts = response.body();
                sabotage_tokens = posts.getSabTokens();
                if(posts.getSabotage() != 0) { //Check if lobby is already being sabotaged
                    sabotage_button.setText("Session Currently being sabotaged");
                } else {
                    if(sabotage_tokens <= 0) { //Check sabotage tokens count
                        sabotage_button.setText("Not enough sabotage tokens");
                    } else {
                        if(players_in_lobby < 2) { //Check if there are players in lobby to sabotage
                            sabotage_button.setText("No players to sabotage");
                        } else {
                            Post_Request post = new Post_Request(User_ID, Session_ID, "2");
                            Call<Void> call_post = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                            call_post.enqueue(new Callback<Void>() {
                                @Override
                                public void onResponse(Call<Void> call_post, Response<Void> response) {

                                    if(!response.isSuccessful()) {
                                        System.out.println(response.body());
                                        sabotage_button.setText("Code: " + response.code());
                                        return;
                                    } else {
                                        sabotage_button.setText("Cooldown Sabotage Initiated");
                                        sabotage_tokens--;
                                        TextView sabotage_tokens_text;
                                        sabotage_tokens_text = findViewById(R.id.SABOTAGE_TOKENS);
                                        sabotage_tokens_text.setText("Sabotage Tokens: " + sabotage_tokens);
                                    }

                                }

                                @Override
                                public void onFailure(Call<Void> call_post, Throwable t) {
                                    sabotage_button.setText(t.getMessage());
                                }
                            });

                        }
                    }
                }

            }

            @Override
            public void onFailure(Call<Get_Sabotage> call, Throwable t) {
                sabotage_button.setText(t.getMessage());
            }
        });


        sabotage_button.postDelayed(new Runnable() {
            @Override
            public void run() {
                sabotage_button.setText("Sabotage");
            }
        }, 3 * 1000);
    }

    public void random_sabotage(View view) {
        List<String> current_location = new ArrayList<>();

        current_location.add(Double.toString(latLng.latitude));
        current_location.add(Double.toString(latLng.longitude));

        Call<Get_Sabotage> call = jsonPlaceHolderApi.GET_SABOTAGE_CALL(Integer.parseInt(User_ID), Integer.parseInt(Session_ID));
        call.enqueue(new Callback<Get_Sabotage>() {
            @Override
            public void onResponse(Call<Get_Sabotage> call, Response<Get_Sabotage> response) {

                if(!response.isSuccessful()) {
                    sabotage_button.setText("Code: " + response.code());
                    return;
                }

                Get_Sabotage posts = response.body();
                sabotage_tokens = posts.getSabTokens();
                if(posts.getSabotage() != 0) { //Check if lobby is already being sabotaged
                    sabotage_button.setText("Session Currently being sabotaged");
                } else {
                    if(sabotage_tokens <= 0) { //Check sabotage tokens count
                        sabotage_button.setText("Not enough sabotage tokens");
                    } else {
                        if(players_in_lobby < 2) { //Check if there are players in lobby to sabotage
                            sabotage_button.setText("No players to sabotage");
                        } else {
                            Post_Request post = new Post_Request(User_ID, Session_ID, "3");
                            Call<Void> call_post = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                            call_post.enqueue(new Callback<Void>() {
                                @Override
                                public void onResponse(Call<Void> call_post, Response<Void> response) {

                                    if(!response.isSuccessful()) {
                                        System.out.println(response.body());
                                        sabotage_button.setText("Code: " + response.code());
                                        return;
                                    } else {
                                        sabotage_button.setText("Random Sabotage Initiated");
                                        sabotage_tokens--;
                                        TextView sabotage_tokens_text;
                                        sabotage_tokens_text = findViewById(R.id.SABOTAGE_TOKENS);
                                        sabotage_tokens_text.setText("Sabotage Tokens: " + sabotage_tokens);
                                    }

                                }

                                @Override
                                public void onFailure(Call<Void> call_post, Throwable t) {
                                    sabotage_button.setText(t.getMessage());
                                }
                            });

                        }
                    }
                }

            }

            @Override
            public void onFailure(Call<Get_Sabotage> call, Throwable t) {
                sabotage_button.setText(t.getMessage());
            }
        });


        sabotage_button.postDelayed(new Runnable() {
            @Override
            public void run() {
                sabotage_button.setText("Sabotage");
            }
        }, 3 * 1000);

    }

}

