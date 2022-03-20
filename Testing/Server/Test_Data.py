test_POST_De1 = {
  "SessionID": "123",
  "Coord": {
    "lat": "49.1234", 
    "lon": "-123.5678"
  },
  "NumPlayers": "2",
  "Win": "987"
}

test_POST_app = {
  "UserID": "1212",
  "Start": "1",
  "Sabotage": "1"
}

loc0 = {
  "SessionID": "987",
  "Coords": ["49.123", "-123.456"],
  "Metrics": {
    "PlayerCount": "4"
  }
}

loc1 = {
  "SessionID": "444",
  "Coords": ["49.0", "-123.0"],
  "Metrics": {
    "PlayerCount": "0"
  }
}

loc2 = {
  "SessionID": "654",
  "Coords": ["49.5", "-123.5"],
  "Metrics": {
    "PlayerCount": "19"
  }
}

locations = [loc0, loc1, loc2]
playerLoc = ["49.542", "-123.222"]