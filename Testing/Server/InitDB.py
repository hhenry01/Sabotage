"""
Fills the mongoDB database with the necessary data for Test_Client to run
properly.
"""
import pymongo
import Test_Data
import copy as cp


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
        sessionsCluster.insert_one(test_POST_De1)
    user = usersCluster.find_one({"UserID": test_POST_De1["Win"]})
    if user is None:
        usersCluster.insert_one({"UserID": test_POST_De1["Win"]})
    usersCluster.update_one({"UserID": test_POST_De1["Win"]},
                            [{"$set": {"CurrSession": {
                                "ID": test_POST_De1["SessionID"]
                            }}}])
    # Initialize for Mobile App POST test
    user = usersCluster.find_one({"UserID": test_POST_app["UserID"]})
    if user is None:
        usersCluster.insert_one({"UserID": test_POST_app["UserID"]})
    nearby = [{"SessionID": loc["SessionID"], "Coord": loc["Coord"]}
              for loc in Test_Data.locations]
    usersCluster.update_one({"UserID": test_POST_app["UserID"]},
                            {"$set": {"NearbySessions": nearby}})
    session = sessionsCluster.find_one(
        {"SessionID": test_POST_app["SessionID"]})
    if session is None:
        sessionsCluster.insert_one(
            {"SessionID": test_POST_app["SessionID"],
             "NumPlayers": "66"})


if __name__ == "__main__":
    initDB()
