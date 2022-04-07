package com.example.hot_or_cold;


/**
 * Class for a JSON formatted post request. This will be used when logging in, entering/exiting a session
 * or to sabotage a current session. Learned post request and Retrofit from this youtube video:
 * https://www.youtube.com/watch?v=GP5OyYDu_mU&ab_channel=CodinginFlow
 */
public class Post_Request {

    //The User ID
    private String UserID;

    //The current Session they are in (optional field)
    private String SessionID;

    //The sabotage they want to use (optional field, requires a sessionID)
    private String Sabotage;

    public Post_Request(String userID) {
        UserID = userID;
    }

    public Post_Request(String userID, String sessionID) {
        UserID = userID;
        SessionID = sessionID;
    }

    public Post_Request(String userID, String sessionID, String sabotage) {
        UserID = userID;
        SessionID = sessionID;
        Sabotage = sabotage;
    }
}


