import requests
import argparse
import json
import Test_Data
import InitDB
import copy as cp
import pymongo
import math

client = pymongo.MongoClient("mongodb+srv://De1-SoC:l2b-11_sabotage@"
                             "sabotage.q6bsa.mongodb.net/Sabotage?"
                             "retryWrites=true&w=majority")
db = client["Sabotage"]
userIDCluster = db["IDs"]
usersCluster = db["Users"]
sessionsCluster = db["Sessions"]


def main():
    parser = argparse.ArgumentParser(
        description="Test client that simulates POST requests from a De1-SoC")

    parser.add_argument("-u",
                        "--url",
                        nargs="?",
                        metavar="URL",
                        type=str,
                        help="URL of the server being run",
                        default="http://127.0.0.1:8000"
                        # Can use http://www.example.com
                        )
    parser.add_argument("-c",
                        "--clear",
                        action="store_true")

    args = parser.parse_args()
    if args.clear:
        InitDB.clearDB()
    InitDB.initDB()  # Fill database with necessary fields for testing
    test_POST_De1 = cp.deepcopy(Test_Data.test_POST_De1)
    test_POST_app = cp.deepcopy(Test_Data.test_POST_app)

    # Test De1 POST
    print("========================================")
    print("Test De1-SoC POST")
    print("========================================")
    data = json.dumps(test_POST_De1)
    headers = {"Content-Type": "application/json"}
    url = args.url + "/de1"
    prevWinner = usersCluster.find_one({"UserID": test_POST_De1["Win"]})
    prevTokens = prevWinner["SabTokens"]
    prevWin = prevWinner["CurrSession"]["Win"]
    print("Sending: " + data + " to: " + url)
    r = requests.post(url, data=data, headers=headers)

    if (r.status_code == 200):
        print("POST request was successful!")
        session = sessionsCluster.find_one({
            "SessionID": test_POST_De1["SessionID"]
        })
        assert session is not None
        assert session["Coord"][0] == test_POST_De1["Coord"]["lat"]
        assert session["Coord"][1] == test_POST_De1["Coord"]["lon"]
        winner = usersCluster.find_one({"UserID": test_POST_De1["Win"]})
        assert winner["CurrSession"]["Win"]
        if not prevWin:
            assert winner["SabTokens"] == prevTokens + 2
        else:
            assert winner["SabTokens"] == prevTokens
    elif (r.status_code == 400):
        print("Error, POST /de1 returned with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    print("========================================")
    print("Test POST /de1 with move")
    print("========================================")
    moveLat = Test_Data.moveLat
    moveLon = Test_Data.moveLon
    test_POST_De1["Coord"] = {
        "lat": moveLat,
        "lon": moveLon
    }
    data = json.dumps(test_POST_De1)
    prevWinner = usersCluster.find_one({"UserID": test_POST_De1["Win"]})
    prevTokens = prevWinner["SabTokens"]
    prevWin = prevWinner["CurrSession"]["Win"]
    # Reuse from previous test
    r = requests.post(url, data=data, headers=headers)
    if (r.status_code == 200):
        print("POST request was succesful!")
        session = sessionsCluster.find_one({
            "SessionID": test_POST_De1["SessionID"]
        })
        assert session["Moved"], "De1 not marked as moved!"
        print(session)
        assert session["Coord"][0] == moveLat
        assert session["Coord"][1] == moveLon
        winner = usersCluster.find_one({"UserID": test_POST_De1["Win"]})
        assert winner["CurrSession"]["Win"]
        if not prevWin:
            assert winner["SabTokens"] == prevTokens + 2
        else:
            assert winner["SabTokens"] == prevTokens
    elif (r.status_code == 400):
        print("Error, POST /de1 move failed with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    print("========================================")
    print("Test POST /de1 with last known location")
    print("========================================")
    test_POST_De1["Coord"] = {
        "lat": "0",
        "lon": "0"
    }
    data = json.dumps(test_POST_De1)
    r = requests.post(url, data=data, headers=headers)
    if (r.status_code == 200):
        print("POST request was successful!")
        session = sessionsCluster.find_one({
            "SessionID": test_POST_De1["SessionID"]
        })
        assert session["Coord"][0] == moveLat
        assert session["Coord"][1] == moveLon
    elif (r.status_code == 400):
        print("Error, POST /de1 last known failed with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    # Test app POST
    print("========================================")
    print("Test POST /app")
    print("========================================")
    print(test_POST_app)
    data = json.dumps(test_POST_app)
    url = args.url + "/app"
    oldUser = usersCluster.find_one({"UserID": test_POST_app["UserID"]})
    oldSessionID = oldUser["CurrSession"]["ID"]
    prevOldSession =\
        sessionsCluster.find_one({"SessionID": oldSessionID})
    prevPlayers = sessionsCluster.find_one({
        "SessionID": test_POST_app["SessionID"]
    })["Metrics"]["NumPlayers"]
    prevTokens = oldUser["SabTokens"]
    print("Sending: " + data + " to: " + url)
    r = requests.post(url, data=data, headers=headers)
    if (r.status_code == 200):
        print("POST request was successful!")
        user = usersCluster.find_one({"UserID": test_POST_app["UserID"]})
        session =\
            sessionsCluster.find_one({"SessionID": test_POST_app["SessionID"]})
        oldSession = sessionsCluster.find_one({"SessionID": oldSessionID})
        assert user["CurrSession"]["ID"] == test_POST_app["SessionID"]
        assert (oldSession["Metrics"]["NumPlayers"]
                == prevOldSession["Metrics"]["NumPlayers"] - 1)
        assert session["Metrics"]["NumPlayers"] == prevPlayers + 1
        assert user["SabTokens"] == prevTokens - 1
        assert (session["Sabotage"]["SaboteurID"]
                == test_POST_app["UserID"]
                and session["Sabotage"]["SabotageID"]
                == test_POST_app["Sabotage"])
    elif (r.status_code == 400):
        print("Error, POST /app returned with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    print("========================================")
    print("Test POST /app leave current session")
    print("========================================")
    data = {
        "UserID": Test_Data.testLeaveUser["UserID"],
        "SessionID": "-1"
    }
    data = json.dumps(data)
    print("Sending: " + data + " to: " + url)
    # Reuse headers
    r = requests.post(url, data=data, headers=headers)
    if (r.status_code == 200):
        print("POST request was successful")
        session = sessionsCluster.find_one({
            "SessionID": Test_Data.testLeaveSession["SessionID"]
        })
        user = usersCluster.find_one({
            "UserID": Test_Data.testLeaveUser["UserID"]
        })
        assert session["Metrics"]["NumPlayers"] == 0
        assert "CurrSession" not in user
    elif (r.status_code == 400):
        print("Error, POST /app returned with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    # Test app GET /sessions
    print("========================================")
    print("Test App GET /sessions")
    print("========================================")
    params = {"UserID": "0", "PlayerLoc": Test_Data.playerLoc}
    r = requests.get(args.url + "/sessions", params=params)
    if (r.status_code == 200):
        print("GET /sessions request was successful!")
        result = r.json()
        print(r.json())
        assert result["UserID"] != "0"
        assert\
            userIDCluster.find_one({"ID": int(result["UserID"])}) is not None
        assert len(result["Sessions"]) != 0
        user = usersCluster.find_one({"UserID": result["UserID"]})
        assert user is not None
        # Default 3 tokens for new user defined in server.py
        assert user["SabTokens"] == 3
        assert (len(result["Sessions"])
                == len(usersCluster.find_one(
                    {"UserID": result["UserID"]}
                )["NearbySessions"]))
    elif (r.status_code == 400):
        print("Error, GET /sessions returned with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    # Test app GET /lobbies
    print("========================================")
    print("Test App GET /lobby for winner")
    print("========================================")
    params = {"UserID": test_POST_De1["Win"],
              "SessionID": test_POST_De1["SessionID"]}
    print(params["UserID"])
    r = requests.get(args.url + "/lobby", params=params)
    if (r.status_code == 200):
        print("GET /lobby request was succesful!")
        result = r.json()
        print(result)
        session = sessionsCluster.find_one({
            "SessionID": test_POST_De1["SessionID"]
        })
        if "SaboteurID" in result:
            assert result["SaboteurID"] == session["SaboteurID"]
            assert result["Sabotage"] == session["Sabotage"]
            assert math.isclose(result["Duration"], session["Duration"],
                                abs_tol=5)  # Tolerate 5 second difference
        assert (session["Metrics"]["NumPlayers"]
                == int(result["NumPlayers"]))
        if result["Win"]:
            assert usersCluster.find_one({
                "UserID": test_POST_De1["Win"]
            })["CurrSession"]["Win"]
            assert result["Leaderboard"] is not None
            present = False
            for pair in result["Leaderboard"]:
                if test_POST_De1["Win"] in pair:
                    present = True
                    break
            assert present, "Winner not found in leaderboard"
    elif (r.status_code == 400):
        print("Error, GET /lobby returned with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    print("========================================")
    print("Test App GET /lobby for sabotage")
    print("========================================")
    params = {"UserID": test_POST_app["UserID"],
              "SessionID": test_POST_app["SessionID"]}
    r = requests.get(args.url + "/lobby", params=params)
    if (r.status_code == 200):
        print("GET /lobby request was succesful!")
        result = r.json()
        print(result)
        session = sessionsCluster.find_one({
            "SessionID": test_POST_app["SessionID"]
        })
        if "SaboteurID" in result:
            assert (result["SaboteurID"]
                    == session["Sabotage"]["SaboteurID"])
            assert result["Sabotage"] == session["Sabotage"]["SabotageID"]
            assert math.isclose(  # Tolerate 5 second difference
                result["Duration"], session["Sabotage"]["Duration"], abs_tol=5)
            assert (session["Metrics"]["NumPlayers"]
                    == int(result["NumPlayers"]))
        if result["Win"]:
            assert usersCluster.find_one({
                "UserID": test_POST_app["UserID"]
            })["CurrSession"]["Win"]
            assert result["Leaderboard"] is not None
            present = False
            for pair in result["Leaderboard"]:
                if test_POST_app["UserID"] in pair:
                    present = True
                    break
            assert present, "Winner not found in leaderboard"
    elif (r.status_code == 400):
        print("Error, Get /lobby returned with code 400")
        assert False, r.text
    else:
        print("Error, unexpected status code...")
        assert False

    print("**********************************")
    print("All tests pass!")
    print("**********************************")


if __name__ == "__main__":
    main()
