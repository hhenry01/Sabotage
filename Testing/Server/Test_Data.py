test_POST_De1 = {
  "SessionID": "123",
  "Coord": {
    "lat": "49.1234",
    "lon": "-123.5678"
  },
  "NumPlayers": "2",
  "Win": "0317"
}

test_POST_app = {
  "UserID": "0444",
  "SessionID": "987",
  "Sabotage": "1"
}

loc0 = {
  "SessionID": "988",
  "Coord": ["49.123", "-123.456"],
  "Metrics": {
    "NumPlayers": "4"
  }
}

loc1 = {
  "SessionID": "987",
  "Coord": ["49.0", "-123.0"],
  "Metrics": {
    "NumPlayers": "0"
  }
}

loc2 = {
  "SessionID": "654",
  "Coord": ["49.5", "-123.5"],
  "Metrics": {
    "NumPlayers": "19"
  }
}

loc3 = {  # Life building
    "SessionID": "111",
    "Coord": ["49.26744", "-123.25066"],
    "Metrics": {
        "NumPlayers": "11"
    }
}

loc4 = {  # Totem
    "SessionID": "450",
    "Coord": ["49.25938", "-123.25166"],
    "Metrics": {
        "NumPlayers": "25"
    }
}

loc5 = {
    "SessionID": "1918",
    "Coord": ["49.26236", "-123.25013"],
    "Metrics": {
        "NumPlayers": "3"
    }
}

testLeaveSession = {
    "SessionID": "a5e9",
    "Coord": ["49.8814", "-119.47856"],
    "Metrics": {
        "NumPlayers": 1  # Leave the others as strings
    }
}
testLeaveUser = {
    "UserID": "9001",
    "NearbySessions": [
        {
            "SessionID": testLeaveSession["SessionID"],
            "Coord": [
                testLeaveSession["Coord"][0],
                testLeaveSession["Coord"][1]
            ]

        }
    ],
    "CurrSession": {
        "ID": testLeaveSession["SessionID"]
    },
    "SabTokens": 0
}

moveLat = "49.26753"  # Life building
moveLon = "-123.24978"
locations = [loc0, loc1, loc2, loc3, loc4, loc5]
playerLoc = ["49.542", "-123.222"]
