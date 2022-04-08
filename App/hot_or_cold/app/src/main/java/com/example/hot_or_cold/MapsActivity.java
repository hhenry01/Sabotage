package com.example.hot_or_cold;

import static com.example.hot_or_cold.constants.MIN_DIST;
import static com.example.hot_or_cold.constants.MIN_TIME;
import static com.example.hot_or_cold.constants.RADIUS;
import static com.example.hot_or_cold.constants.SMALL_RADIUS;
import static com.example.hot_or_cold.constants.URL;

import androidx.activity.result.contract.ActivityResultContracts;
import androidx.appcompat.widget.AppCompatButton;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.FragmentActivity;


import android.Manifest;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.Circle;
import com.google.android.gms.maps.model.CircleOptions;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;
import com.example.hot_or_cold.databinding.ActivityMapsBinding;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Stack;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MapsActivity extends FragmentActivity implements OnMapReadyCallback {

    //Map variables
    private GoogleMap mMap;
    private ActivityMapsBinding binding;

    //Current location variables
    private LocationListener locationListener;
    private LocationManager locationManager;

    //Marker variable
    private Marker marker;

    //Current location
    private LatLng latLng;

    //Constants for zoom degree
    final private float World = 1;
    final private float Landmass_continent = 5;
    final private float City = 10;
    final private float Streets = 15;
    final private float Buildings = 20;

    private List<Locations> locations;

    //User ID
    private String UserID;

    private JsonPlaceHolderApi jsonPlaceHolderApi;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();

        UserID = intent.getStringExtra("UserID");

        binding = ActivityMapsBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        //Ask for permissions just in case
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, PackageManager.PERMISSION_GRANTED);

        //Create retrofit to handle Get/Post requests and format class' into JSON objects
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        jsonPlaceHolderApi = retrofit.create(JsonPlaceHolderApi.class);

        //Display the UserID
        AppCompatButton right_side_button = findViewById(R.id.rlVar4);
        right_side_button.setText("User ID: " + UserID);
    }


    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     * This is where we can add markers or lines, add listeners or move the camera. In this case,
     * we just add a marker near Sydney, Australia.
     * If Google Play services is not installed on the device, the user will be prompted to install
     * it inside the SupportMapFragment. This method will only be triggered once the user has
     * installed Google Play services and returned to the app.
     */


    @SuppressLint("MissingPermission")
    @Override
    public void onMapReady(GoogleMap googleMap) {

        mMap = googleMap;

        //Default coordinates
        LatLng UBC = new LatLng(49.271723479330106, -123.2518942169877);

        List<String> current_location = new ArrayList<>();

        current_location.add("49.271723479330106");
        current_location.add("-123.2518942169877");

        Call<Get_Request> call = jsonPlaceHolderApi.GET_REQUEST_CALL(Integer.parseInt(UserID), current_location);

        call.enqueue(new Callback<Get_Request>() {
            @Override
            public void onResponse(Call<Get_Request> call, Response<Get_Request> response) {

                if(!response.isSuccessful()) {
                    return;
                }

                Get_Request posts = response.body();

                //Find and display every location on the map as a circle
                locations = posts.getLocations();
                if(locations != null && !locations.isEmpty()) {
                    for (Locations locs : locations) {
                        LatLng locs_coords = new LatLng(Double.parseDouble(locs.getCoords().get(0))
                                ,Double.parseDouble(locs.getCoords().get(1)));
                        Random r = new Random(Integer.parseInt(locs.getSessionID())); //Seeding with the session ID, so we don't get a different
                        double randomDistance = (RADIUS) * r.nextDouble();            //random value every time we open up maps.
                        Random s = new Random(Integer.parseInt(locs.getSessionID()));
                        double randomHeading = (360) * s.nextDouble();

                        LatLng random_coord = computeOffset(locs_coords, randomDistance,randomHeading);
                        //Display the circles which represent DE1 play ranges.
                        Circle circle = mMap.addCircle(new CircleOptions()
                                .center(random_coord)
                                .radius(RADIUS)
                                .strokeColor(Color.RED)
                                .fillColor(0x753EC8D0)  //default
                                .strokeWidth(5)
                                .clickable(true)
                        );

                        /* //Admin feature that displays the radius where WiFi RSSI will be used instead of GPS
                        mMap.addCircle(new CircleOptions()
                                .center(locs_coords)
                                .radius(SMALL_RADIUS)
                                .strokeColor(Color.BLACK)
                                .fillColor(0x753EC8D0)  //default
                                .strokeWidth(5)
                                .clickable(false)
                        );*/


                        //Let the circle hold information
                        circle.setTag(locs);

                        //Display details per session such as session ID and player count
                        mMap.addMarker(new MarkerOptions().position(random_coord).title(
                                locs.getSessionID() + ": " + locs.getMetrics().getPlayerCount() )).showInfoWindow();

                        /* //Admin feature that displays the actual location of the DE1 without randomness
                        mMap.addMarker(new MarkerOptions().position(locs_coords).title(
                                "Center")).showInfoWindow();
                        */


                    }
                }

            }

            @Override
            public void onFailure(Call<Get_Request> call, Throwable t) {
            }
        });


        //Move map to default location and create a marker, we need this before we find the current location
        marker = mMap.addMarker(new MarkerOptions().position(UBC).title("Current Location").visible(false)); //default location

        //Move map to location and zoom in to Streets level
        mMap.moveCamera(CameraUpdateFactory.newLatLngZoom(UBC, Streets));
        //Remove previous marker so we can add our new marker based on current location
        marker.remove();

        //Find current location and create marker at current location.
        locationListener = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {try {
                latLng = new LatLng(location.getLatitude(), location.getLongitude());
                marker.remove();
                marker = mMap.addMarker(new MarkerOptions().position(latLng).title("My Position"));
                mMap.moveCamera(CameraUpdateFactory.newLatLng(latLng));
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

        //When you click the circle, and you are within the radius of said circle, you'll start playing the game.
        mMap.setOnCircleClickListener(new GoogleMap.OnCircleClickListener() {
            @Override
            public void onCircleClick(Circle circle) {
                float[] distance = new float[1];
                Location.distanceBetween(latLng.latitude, latLng.longitude,
                        circle.getCenter().latitude, circle.getCenter().longitude, distance);
                if(distance[0] < RADIUS) {
                    Intent intent = new Intent(MapsActivity.this, hot_or_cold_Default.class);
                    Locations data = (Locations) circle.getTag();
                    intent.putExtra("SessionID", data.getSessionID());
                    intent.putExtra("Players", data.getMetrics().getPlayerCount());
                    intent.putExtra("UserID", UserID);
                    intent.putExtra("Actual_lat", Double.parseDouble(data.getCoords().get(0)));
                    intent.putExtra("Actual_long", Double.parseDouble(data.getCoords().get(1)));
                    intent.putExtra("Latitude", circle.getCenter().latitude);
                    intent.putExtra("Longitude", circle.getCenter().longitude);
                    //You'll send a post request when joining a session to let the server know to update player count
                    Post_Request post = new Post_Request(UserID, data.getSessionID());
                    Call<Void> call = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                    call.enqueue(new Callback<Void>() {
                        @Override
                        public void onResponse(Call<Void> call, Response<Void> response) {

                            if(!response.isSuccessful()) {
                                return;
                            } else {
                                startActivity(intent);
                            }
                        }

                        @Override
                        public void onFailure(Call<Void> call, Throwable t) {

                        }
                    });

                }
            }
        });


    }
    //Button/tab for returning to start screen
    public void return_home(View view) {
        Intent intent = new Intent(MapsActivity.this, MainActivity.class);
        startActivity(intent);
    }

    //Code adapted from https://stackoverflow.com/questions/639695/how-to-convert-latitude-or-longitude-to-meters
    public static LatLng computeOffset(LatLng from, double distance, double heading) {
        distance /= 6371009.0D;  //earth_radius = 6371009 # in meters
        heading = Math.toRadians(heading);
        double fromLat = Math.toRadians(from.latitude);
        double fromLng = Math.toRadians(from.longitude);
        double cosDistance = Math.cos(distance);
        double sinDistance = Math.sin(distance);
        double sinFromLat = Math.sin(fromLat);
        double cosFromLat = Math.cos(fromLat);
        double sinLat = cosDistance * sinFromLat + sinDistance * cosFromLat * Math.cos(heading);
        double dLng = Math.atan2(sinDistance * cosFromLat * Math.sin(heading), cosDistance - sinFromLat * sinLat);
        return new LatLng(Math.toDegrees(Math.asin(sinLat)), Math.toDegrees(fromLng + dLng));
    }

}