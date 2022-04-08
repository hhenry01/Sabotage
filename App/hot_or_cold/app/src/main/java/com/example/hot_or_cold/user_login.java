package com.example.hot_or_cold;


import static com.example.hot_or_cold.constants.URL;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;


import java.util.ArrayList;
import java.util.List;


import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class user_login extends AppCompatActivity {

    //Used for Get and Post requests
    private JsonPlaceHolderApi jsonPlaceHolderApi;

    //Used for allowing players to enter an ID
    private EditText userID;

    //The final UserID
    private String UserID;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_login);
        //Set up edit text as well as retrofit
        userID = (EditText)findViewById(R.id.editText1);
        //Create retrofit object that handles converting class' into JSON objects and post/get them to URL
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl(URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build();

        jsonPlaceHolderApi = retrofit.create(JsonPlaceHolderApi.class);
    }


    /**
     * Checks login input. Makes sure it is a 4 digit number that has already been registered, or 0.
     * @param view
     * @throws InterruptedException
     */
    public void login(View view) throws InterruptedException {

        //Check that our text is only numbers
        if(isNumeric(userID.getText().toString())) {
            if(userID.getText().toString().equals("0")) { //If text is 0, get a new userID.
                List<String> current_location = new ArrayList<>();


                current_location.add("49.271723479330106");
                current_location.add("-123.2518942169877");

                //Get a new userID by sending a Get Request with UserID 0,
                Call<Get_Request> call = jsonPlaceHolderApi.GET_REQUEST_CALL(Integer.parseInt("0"), current_location);

                call.enqueue(new Callback<Get_Request>() {
                    @Override
                    public void onResponse(Call<Get_Request> call, Response<Get_Request> response) {

                        if(!response.isSuccessful()) {
                            return;
                        }

                        //Receive new UserID and display
                        Get_Request posts = response.body();
                        UserID = Integer.toString(posts.getUserID());
                        TextView invalid = findViewById(R.id.invalid);
                        invalid.setText("New User: " + UserID);
                        Post_Request post = new Post_Request(UserID);//, "0", "0");
                        //Then post that UserID that we've just now received and go to Maps Page
                        Call<Void> called = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                        called.enqueue(new Callback<Void>() {
                            @Override
                            public void onResponse(Call<Void> called, Response<Void> response) {

                                if(!response.isSuccessful()) {
                                    TextView invalid = findViewById(R.id.invalid);
                                    invalid.setText("Code: " + response.code());
                                    return;
                                } else {
                                    Intent intent = new Intent(user_login.this, MapsActivity.class);
                                    intent.putExtra("UserID", UserID);
                                    startActivity(intent);
                                }

                            }

                            @Override
                            public void onFailure(Call<Void> called, Throwable t) {
                                TextView invalid = findViewById(R.id.invalid);
                                invalid.setText(t.getMessage());
                            }
                        });

                    }

                    @Override
                    public void onFailure(Call<Get_Request> called, Throwable t) {
                    }
                });
            } else {
                //Post the userID inputted and check if it is in the database, if not we'll get an error code
                UserID = userID.getText().toString();
                Post_Request post = new Post_Request(UserID);//, "0", "0");
                Call<Void> call = jsonPlaceHolderApi.POST_REQUEST_CALL(post);
                call.enqueue(new Callback<Void>() {
                    @Override
                    public void onResponse(Call<Void> call, Response<Void> response) {

                        if(!response.isSuccessful()) {
                            TextView invalid = findViewById(R.id.invalid);
                            invalid.setText("Code: " + response.code());
                            return;
                        } else {
                            Intent intent = new Intent(user_login.this, MapsActivity.class);
                            intent.putExtra("UserID", UserID);
                            startActivity(intent);
                        }

                    }

                    @Override
                    public void onFailure(Call<Void> call, Throwable t) {
                        TextView invalid = findViewById(R.id.invalid);
                        invalid.setText(t.getMessage());
                    }
                });
            }

        } else {
            TextView invalid = findViewById(R.id.invalid);
            invalid.setText("Invalid UserID"); //Display error message for invalid ID
        }
    }

    //Check if string is only numbers
    public static boolean isNumeric(String str) {
        try {
            Double.parseDouble(str);
            return true;
        } catch(NumberFormatException e){
            return false;
        }
    }

}