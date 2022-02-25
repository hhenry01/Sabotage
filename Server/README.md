# Server
Handles HTTP requests from the De1-SoC and the mobile app. See [`Testing/Server/`](../Testing/Server/) for how to test.
# Format
The messages contained in the HTTP requests are formatted in JSON strings.
## De1-SoC POST
POST requests from the De1-SoC will be a subset of the following fields. The only mandatory field is the SessionID.
```
{
  SessionID: <Integer ID of the game session/lobby associated with the De1-SoC>,
  Coord: 
  {
    lat: <Floating point latitude>,
    lon: <Floating point longitude>
  },
  NumPlayers: <Integer number of players in the lobby>,
  Win: <Integer user ID of player that found the De1-SoC>
}
```