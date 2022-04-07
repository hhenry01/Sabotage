package com.example.hot_or_cold;


/**
 * Contains the number of players in a session as a string.
 */
public class Metrics {
    private String NumPlayers;

    /**
     *
     * @return the number of players in a session as an int. or an exception if number of players wasn't a number in string form
     */
    public int getPlayerCount() {
        try{
            int PlayerCount_int = Integer.parseInt(NumPlayers);
            return PlayerCount_int;
        }
        catch (NumberFormatException ex){
            ex.printStackTrace();
        }
        return -1;
    }
}

