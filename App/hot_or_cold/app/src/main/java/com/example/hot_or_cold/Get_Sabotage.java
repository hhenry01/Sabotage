package com.example.hot_or_cold;


import java.util.HashMap;
import java.util.List;

/**
 * Class for /lobby get requests. This get request returns the data of the current session the player
 * is in. This data is specific to the current play session, and is only used for when viewing the
 * Leaderboard of the current session (Leaderboard class), the hot or cold play screen (hot_or_cold_Default),
 * and the sabotage page (sabotage_class). I learned all about get requests and retrofit from this youtube
 * video: https://www.youtube.com/watch?v=4JGvDUlfk7Y&ab_channel=CodinginFlow
 */
public class Get_Sabotage {

    /*
      Current player sabotaging the lobby. If no one is sabotaging, then the ID will be
      0, which is an invalid UserID.
     */
    private int SaboteurID;

    /*
      The current sabotage on the lobby. 1 for Swap Sabotage, 2 for Cooldown Sabotage,
      3 for Random Sabotage. Default will be 0, which means no one is sabotaging the lobby.
     */
    private int Sabotage;

    /*
      The remaining duration of the current sabotage, mainly useful for the Cooldown sabotage.
     */
    private float Duration;

    /*
      The number of players in the current lobby.
     */
    private int NumPlayers;

    /*
      The number of sabotage tokens the user has. Without sabotage tokens, you cannot sabotage a
      lobby. You use one sabotage token per sabotage and gain 2 when you win a game. New users
      start with 3.
     */
    private int SabTokens;

    /*
      True if the current user has won the game by entering their UserID into the VGA on the DE1
      of their session.
     */
    private boolean Win;

    /*
      Contains the score and UserID of all players in the lobby.
     */
    private List<HashMap<String,Integer>> Leaderboard;

    public int getSaboteurID() {
        return SaboteurID;
    }

    public int getNumPlayers() {
        return NumPlayers;
    }

    public boolean isWin() {
        return Win;
    }

    public int getSabTokens() {
        return SabTokens;
    }

    public int getSabotage() {
        return Sabotage;
    }

    public float getDuration() {
        return Duration;
    }

    public List<HashMap<String, Integer>> getLeaderboard() {
        return Leaderboard;
    }
}
