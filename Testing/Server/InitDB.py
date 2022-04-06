"""
Fills the mongoDB database with the necessary data for Test_Client to run
properly.
"""
import pymongo
import Test_Data
import time
import copy as cp


def clearDB():
    client = pymongo.MongoClient("mongodb+srv://De1-SoC:l2b-11_sabotage@"
                                 "sabotage.q6bsa.mongodb.net/Sabotage?"
                                 "retryWrites=true&w=majority")
    db = client["Sabotage"]
    userIDCluster = db["IDs"]
    usersCluster = db["Users"]
    sessionsCluster = db["Sessions"]

    userIDCluster.delete_many({})
    usersCluster.delete_many({})
    sessionsCluster.delete_many({})


def initDB():
    client = pymongo.MongoClient("mongodb+srv://De1-SoC:l2b-11_sabotage@"
                                 "sabotage.q6bsa.mongodb.net/Sabotage?"
                                 "retryWrites=true&w=majority")
    db = client["Sabotage"]
    userIDCluster = db["IDs"]
    usersCluster = db["Users"]
    sessionsCluster = db["Sessions"]
    test_POST_De1 = cp.deepcopy(Test_Data.test_POST_De1)
    test_POST_app = cp.deepcopy(Test_Data.test_POST_app)

    # Initialize for De1-SoC POST test
    if sessionsCluster.find_one({
            "SessionID": test_POST_De1["SessionID"]}) is None:
        sessionsCluster.insert_one({
            "SessionID": test_POST_De1["SessionID"],
            "Coord": [test_POST_De1["Coord"]["lat"],
                      test_POST_De1["Coord"]["lon"]],
            "Metrics": {
                "NumPlayers": int(test_POST_De1["NumPlayers"])
            }
        })
    user = usersCluster.find_one({"UserID": test_POST_De1["Win"]})
    if user is None:
        usersCluster.insert_one({"UserID": test_POST_De1["Win"]})
    usersCluster.update_one({"UserID": test_POST_De1["Win"]},
                            [{"$set": {
                                "CurrSession": {
                                    "ID": test_POST_De1["SessionID"],
                                    "Start": time.time(),
                                    "Win": False
                                },
                                "NearbySessions": []
                            }}])
    usersCluster.update_one({"UserID": test_POST_De1["Win"]},
                            {"$set": {"SabTokens": 1}})
    # Initialize for Mobile App POST test
    user = usersCluster.find_one({"UserID": test_POST_app["UserID"]})
    if user is None:
        usersCluster.insert_one({"UserID": test_POST_app["UserID"]})
    nearby = [{"SessionID": loc["SessionID"], "Coord": loc["Coord"]}
              for loc in Test_Data.locations]
    usersCluster.update_one({"UserID": test_POST_app["UserID"]},
                            [{"$set": {
                                "CurrSession": {
                                    "ID": Test_Data.loc0["SessionID"],
                                    "Start": time.time(),
                                    "Win": False
                                },
                                "NearbySessions": nearby,
                                "SabTokens": 1
                            }}])
    session = sessionsCluster.find_one(
        {"SessionID": test_POST_app["SessionID"]})
    if session is None:
        sessionsCluster.insert_one(
            {"SessionID": test_POST_app["SessionID"],
             "Coord": ["49.49", "-123.11"],
             "Metrics": {"NumPlayers": 66}})
    # Initialize user ID records for GET /lobby
    IDs = [int(test_POST_De1["Win"]), int(test_POST_app["UserID"])]
    for id in IDs:
        if userIDCluster.find_one({"ID": id}) is None:
            userIDCluster.insert_one({"ID": id})
    # Locations
    for loc in Test_Data.locations:
        if sessionsCluster.find_one({"SessionID": loc["SessionID"]}) is None:
            x = cp.deepcopy(loc)
            x["Metrics"]["NumPlayers"] = int(loc["Metrics"]["NumPlayers"])
            sessionsCluster.insert_one(x)
    # Test leave with -1 flag
    sessionsCluster.update_one({
        "SessionID": Test_Data.testLeaveSession["SessionID"],
    }, {"$set": Test_Data.testLeaveSession}, upsert=True)  # Always overwrite
    if (userIDCluster.find_one(
            {"ID": Test_Data.testLeaveUser["UserID"]}) is None):
        userIDCluster.insert_one(
            {"ID": int(Test_Data.testLeaveUser["UserID"])})
    usersCluster.update_one({
        "UserID": Test_Data.testLeaveUser["UserID"]
    }, {"$set": Test_Data.testLeaveUser}, upsert=True)  # Always overwrite


if __name__ == "__main__":
    initDB()
