package com.example.hot_or_cold;

import java.util.List;

/**
 * Locations contains the data for a DE1 on the server. This data includes the sessionID
 * , the coordinates of that DE1, the number of players in that current session in Metrics.
 */
public class Locations {

    //The Session ID of the DE1
    private String SessionID;

    //The coordinates of the DE1 in format [lat, long]
    private List<String> Coord;

    //Contains the player count of the session.
    private Metrics Metrics;

    public String getSessionID() {
        return SessionID;
    }

    public List<String> getCoords() {
        return Coord;
    }

    public com.example.hot_or_cold.Metrics getMetrics() {
        return Metrics;
    }
}
