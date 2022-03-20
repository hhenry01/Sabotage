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
## Mobile App POST
POST requests from the mobile app will be a subset of the following fields. The only mandatory field
is the user ID.
```
{
  UserID: <Integer ID of user>,
  Start: <Starting location number (integer)>,
  Sabotage: <Integer number associated with an action>
}
```
Sabotage actions should not be executed until after verifying the user is allowed to use that action.

## Mobile App GET
GET requests from the mobile app will return the following fields. The user ID should be sent along with the GET
request as a parameter. If it's a new user without an ID, it will have an ID of 0 and will get assigned an ID.
The user's GPS location must also be included as a parameter to get nearby sessions.
The user ID (new or already existing) is returned with the rest of the HTTP response.
```
{
  UserID: <Integer ID of user>,
  Locations: [
    [lat, lon], # GPS coordinates of location 0
    [lat, lon],
    ...
  ],
  Metrics: {
    PlayerCounts: [
      <# players at location 0>,
      <# players at location 1>,
      ...
      <# players at location n - 1>
    ],
    TBD: [
      <TBD>
    ]
  }
}
```