from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
import json
import argparse
import time
import random
import pymongo
from functools import partial
from Testing import Test_Data

MAX_USER_ID = 9999
WIN_COOLDOWN = 3600  # 1 hour
IDLE_TIMER = 600  # Kick an idle user after 10 minutes
SABOTAGE_DURATION = 30

client = pymongo.MongoClient("mongodb+srv://De1-SoC:l2b-11_sabotage@sabotage."
                             "q6bsa.mongodb.net/Sabotage?retryWrites="
                             "true&w=majority")
db = client["Sabotage"]
userIDCluster = db["IDs"]
usersCluster = db["Users"]
sessionsCluster = db["Sessions"]


# Given a player location [lat, lon], return a list of nearby sessions.
def getNearby(playerLoc):
    return [{"SessionID": loc["SessionID"], "Coord": loc["Coord"]}
            for loc in Test_Data.locations]


# Generate a unique user ID
def generateUserID():
    ids = random.sample(range(1, MAX_USER_ID+1), MAX_USER_ID)
    for id in ids:
        yield id


class HTTPRequestHandler(BaseHTTPRequestHandler):
    def __init__(self, idGen, *args, **kwargs):
        self.ids = idGen
        super().__init__(*args, **kwargs)

    """
    Given a user ID, check if the user exists.
    If yes, then return the given ID and False (not a new user).
    If no, then return a newly generated ID and true (is a new user).
    """
    def checkUser(self, messageID):
        if (not messageID):
            userID = next(self.ids)  # TODO check for max ID error
            while userIDCluster.find_one({"ID": userID}) is not None:
                print(userIDCluster.find({"ID": userID}))
                userID = next(self.ids)
            print(f"New User, ID: {userID}")
            userIDCluster.insert_one({"ID": userID})
            return str(userID), True
        else:
            userID = messageID
            print("User ID: " + str(userID))
            return str(userID), False

    def do_POST_DE1(self, message):
        session = message["SessionID"]
        print("Received POST request from De1-SoC: " + session)
        if sessionsCluster.find_one({"SessionID": session}) is None:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error finding session".encode())
            return
        # Session IDs should only be added to the database manually
        if ("Coord" in message):
            lat = float(message["Coord"]["lat"])
            lon = float(message["Coord"]["lon"])
            print("Received new GPS coordinates:\n" +
                  "Latitude: " + str(lat) + '\n' +
                  "Longitude: " + str(lon))
            sessionsCluster.update_one({"SessionID": session},
                                       {"$set": {"Coord": [lat, lon]}})

        if ("NumPlayers" in message):
            numPlayers = int(message["NumPlayers"])
            print("Number of players updated to: " + str(numPlayers))
            sessionsCluster.update_one({"SessionID": session},
                                       [{"$set": {"Metrics": {
                                        "NumPlayers": numPlayers
                                        }}}])

        if ("Win" in message):
            winner = message["Win"]
            winningUser = usersCluster.find_one({"UserID": winner})
            if winningUser is None:
                print("Winner not in database, reject \"win\" condition")
            else:
                if winningUser["CurrSession"]["ID"] == session:
                    # the session ID sent along with the POST request.
                    print(f"Player {winner} has just won a game!")
                    usersCluster.update_one({"UserID": winner}, [
                        {"$set": {"CurrSession":
                                  {"Timeout": time.time() + WIN_COOLDOWN,
                                   "Win": True}}}
                    ])
                else:
                    self.send_response(200)
                    self.end_headers()
                    self.wfile.write(f"Warning, {winner} tried to win"
                                     " a session he's not in".encode())
                    return

        self.send_response(200)
        self.end_headers()

    def do_POST_App(self, message):
        print("Received POST request from mobile app")
        userID = message["UserID"]
        user = usersCluster.find_one({"UserID": userID})
        if user is None:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("User not found in database".encode())
            return

        if ("SessionID" in message):
            session = message["SessionID"]
            if ("CurrSession" not in user
                    or user["CurrSession"]["ID"] != session):
                currSession = filter(
                    lambda loc: loc["SessionID"] == session,
                    user["NearbySessions"]
                )
                if currSession is None:
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write("No nearby sessions".encode())
                    return
                startTime = time.time()
                print(f"User {userID} starting at: {startTime}")
                usersCluster.update_one({"UserID": userID}, [
                    {"$set": {"CurrSession": {
                        "ID": session,
                        "Start": time.time(),
                        "Timeout": time.time() + IDLE_TIMER,
                        "Win": False
                    }}}
                ])

            if ("Sabotage" in message):
                sabNum = int(message["Sabotage"])
                print("Attempting sabotage: " + str(sabNum))
                # TODO Verify the user is allowed to do the sabotage.
                sessionsCluster.update_one(
                    {"SessionID": session},
                    {"$set": {
                        "Sabotage": {
                            "SaboteurID": userID,
                            "SabotageID": sabNum,
                            "Duration": time.time() + SABOTAGE_DURATION
                        }
                    }})

        self.send_response(200)
        self.end_headers()

    def do_POST(self):
        print("Handling POST request.")

        # Reject requests that are not JSON
        if (self.headers["Content-Type"] != "application/json"):
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, POST request is not a JSON string"
                             .encode())
            return

        length = int(self.headers["Content-Length"])
        message = json.loads(self.rfile.read(length))

        # SessionID is required for De1-SoC POST requests.
        # All others are optional.
        # if check for UserID must come before check for SessionID
        # as POST App contains SessionID as well
        if ("UserID" in message):
            self.do_POST_App(message)
        elif ("SessionID" in message):
            self.do_POST_DE1(message)
        else:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, POST request missing session or user ID!"
                             .encode())

    def parse_GET(self):
        params = parse_qs(urlparse(self.path).query)

        if (not params):
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, GET request missing parameters!".encode())
            return
        # Parse URL for ID parameter
        userID, newUser = self.checkUser(int(params["UserID"][0]))
        if (newUser):
            usersCluster.insert_one({
                "UserID": userID,
                "Timeout": time.time() + IDLE_TIMER
            })
        return userID, params

    def do_GET_sessions(self):
        userID, params = self.parse_GET()
        playerLoc = params["PlayerLoc"]
        nearbySessions = getNearby(playerLoc)
        usersCluster.update_one({"UserID": userID}, {
            "$set": {"NearbySessions": nearbySessions}
        }, upsert=True)  # Always update nearby sessions

        responseMSG = {
            "UserID": userID,
            "Sessions": nearbySessions
        }
        responseMSG = json.dumps(responseMSG)

        self.send_response(200)
        self.send_header("Content-type", "application/json")
        self.end_headers()
        self.wfile.write(responseMSG.encode())

    def do_GET_Lobby_Info(self):
        userID, params = self.parse_GET()
        sessionID = params["SessionID"][0]
        print(f"Getting session info for {sessionID}")
        session = sessionsCluster.find_one({"SessionID": sessionID})
        if session is None:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, session not found".encode())
            return

        if "Sabotage" in session:
            sabotage = session["Sabotage"]
        else:
            sabotage = None
        numPlayers = session["NumPlayers"]
        winner = usersCluster.find_one({"UserID": userID})
        if "CurrSession" in winner:
            win = winner["CurrSession"]["Win"]
        else:
            win = False
        responseMSG = {} if sabotage is None else sabotage
        responseMSG["NumPlayers"] = numPlayers
        responseMSG["Win"] = win
        responseMSG = json.dumps(responseMSG)

        self.send_response(200)
        self.send_header("Content-type", "application/json")
        self.end_headers()
        self.wfile.write(responseMSG.encode())

    def do_GET(self):
        print("Handling GET request")
        route = self.path.split('?')[0]
        if route == "/sessions":
            self.do_GET_sessions()
        elif route == "/lobby":
            self.do_GET_Lobby_Info()
        else:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, GET request has invalid route!".encode())


def main():
    parser = argparse.ArgumentParser(
        description="Remote server that processes HTTP requests"
                    "from the De1-SoC and mobile app")
    parser.add_argument("-i",
                        "--ip",
                        nargs="?",
                        metavar="IP_ADDRESS",
                        type=str,
                        help="IP address to run the server on",
                        default=""
                        )  # Default is localhost
    parser.add_argument("-p",
                        "--port",
                        metavar="SERVER_PORT",
                        type=int,
                        help="Port to run the server on",
                        required=True
                        )
    args = parser.parse_args()
    print("Serving at port: " + str(args.port))
    print("With IP bound to: " + ("localhost" if args.ip == "" else args.ip))

    userIDGenerator = generateUserID()
    handler = partial(HTTPRequestHandler, userIDGenerator)
    httpd = HTTPServer((args.ip, args.port), handler)
    httpd.serve_forever()


if __name__ == "__main__":
    main()
