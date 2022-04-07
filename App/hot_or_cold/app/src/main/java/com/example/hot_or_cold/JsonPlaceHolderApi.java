package com.example.hot_or_cold;

import java.util.List;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.Headers;
import retrofit2.http.POST;
import retrofit2.http.Query;

/**
 * Learned how to make this from this youtube series: https://www.youtube.com/watch?v=4JGvDUlfk7Y&ab_channel=CodinginFlow
 */
public interface JsonPlaceHolderApi {

    //Get request for sessions data
    @GET("/sessions")
    Call<Get_Request> GET_REQUEST_CALL(
            @Query("UserID") Integer UserID, //Takes a UserID
            @Query("PlayerLoc") List<String> PlayerLoc); //Takes players location [Lat, Long]

    //Get request for lobby data
    @GET("/lobby")
    Call<Get_Sabotage> GET_SABOTAGE_CALL(
            @Query("UserID") Integer UserID, //Takes a UserID
            @Query("SessionID") Integer SessionID); //Takes current SessionID of the User

    //Post request
    @Headers("Content-Type: application/json")
    @POST("/app")
    Call<Void> POST_REQUEST_CALL(@Body Post_Request Post);
}
