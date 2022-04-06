from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
from functools import partial
import json
import argparse
import time
import random
import pymongo
import math
import signal

MAX_USER_ID = 9999
USER_ID_LENGTH = 4  # 4 digits
WIN_COOLDOWN = 3600  # 1 hour
IDLE_TIMER = 600  # Kick an idle user after 10 minutes
SABOTAGE_DURATION = 30  # 30 seconds
COUNTS_AS_CLOSE = 4  # Totem is within range of Gage and vice versa
DEFAULT_SABTOKENS = 3  # New playes get 3 tokens starting off
WIN_SABTOKENS = 2  # Earn 2 tokens for winning a game
SCORE_BASE = 5000  # 5000 seconds (1.39 h) until the score goes to 0
SCORE_BONUS_MULT = 20
SCORE_BONUS_MAX = 1000

client = pymongo.MongoClient("mongodb+srv://De1-SoC:l2b-11_sabotage@sabotage."
                             "q6bsa.mongodb.net/Sabotage?retryWrites="
                             "true&w=majority")
db = client["Sabotage"]
userIDCluster = db["IDs"]
usersCluster = db["Users"]
sessionsCluster = db["Sessions"]


# Given a player location [lat, lon], return a list of nearby sessions.
def getNearby(playerLoc):  # BROKEN
    def howClose(lat0, lon0, lat1, lon1):
        return abs((float(lat1) - float(lat0)) / (float(lon1) - float(lon0)))

    locations = sessionsCluster.find()
    closeLocs = filter(
        lambda loc:
        howClose(loc["Coord"][0], loc["Coord"][1],
                 playerLoc[0], playerLoc[1]) < COUNTS_AS_CLOSE,
        locations
    )
    return [{"SessionID": loc["SessionID"],
             "Coord": loc["Coord"],
             "Metrics": loc["Metrics"]}
            for loc in closeLocs]


# Generate a unique user ID
def generateUserID():
    ids = random.sample(range(1, MAX_USER_ID+1), MAX_USER_ID)
    for id in ids:
        yield id


def calcScore(start, end, mod):
    score = max(SCORE_BASE - (end - start), 0)
    if score > 0:
        score += min(mod * SCORE_BONUS_MULT, SCORE_BONUS_MAX)
    return int(score)


# Callback to remove the Sabotage from a session
def sabotageTimeoutHandler(sessionID, saboteurID, sabNum, signum, frame, ):
    session = sessionsCluster.find_one({"SessionID": sessionID})
    if session is not None:
        if (math.isclose(session["Sabotage"]["Duration"],
                         time.time(), abs_tol=1)
                and saboteurID == session["Sabotage"]["SaboteurID"]
                and sabNum == session["Sabotage"]["SabotageID"]):
            sessionsCluster.update_one({
                "SessionID": sessionID
            }, {"$unset": {"Sabotage": ""}})


# Wrapper function for setting up the timeout
def sabotageTimeout(sessionID, saboteurID, sabNum):
    handler = partial(sabotageTimeoutHandler, sessionID, saboteurID, sabNum)
    signal.signal(signal.SIGALRM, handler)
    signal.alarm(SABOTAGE_DURATION)


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
            try:
                userID = next(self.ids)
                while userIDCluster.find_one({"ID": userID}) is not None:
                    print(userIDCluster.find({"ID": userID}))
                    userID = next(self.ids)
                print(f"New User, ID: {userID}")
                userIDCluster.insert_one({"ID": userID})
                return str(userID).zfill(USER_ID_LENGTH), True
            except StopIteration:
                self.send_response(400)
                self.end_headers()
                self.wfile.write("Error, no availble user IDs".encode())
                return "", False
        else:
            userID = messageID
            if userIDCluster.find_one({"ID": int(userID)}) is None:
                self.send_response(400)
                self.end_headers()
                self.wfile.write("Error, user not in database".encode())
                return "", False
            print(f"User ID: {userID}")
            return str(userID).zfill(USER_ID_LENGTH), False

    def do_POST_DE1(self, message):
        sessionID = message["SessionID"]
        print("Received POST request from De1-SoC: " + sessionID)
        session = sessionsCluster.find_one({"SessionID": sessionID})
        # Session IDs should only be added to the database manually
        if session is None:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error finding session".encode())
            return
        if ("Coord" in message):
            lat = float(message["Coord"]["lat"])
            lon = float(message["Coord"]["lon"])
            print("Received new GPS coordinates:\n" +
                  "Latitude: " + str(lat) + '\n' +
                  "Longitude: " + str(lon))
            storedCoord = session["Coord"]
            if (not math.isclose(float(storedCoord[0]), lat, abs_tol=0.00005)
                    or not
                    math.isclose(float(storedCoord[1]), lon, abs_tol=0.00005)):
                print(f"De1-SoC {sessionID} has moved!")
                if lat != 0.0 and lon != 0.0:
                    sessionsCluster.update_one(
                        {"SessionID": sessionID},
                        {"$set": {
                               "Coord": [str(lat), str(lon)]
                        }})
                sessionsCluster.update_one(
                    {"SessionID": sessionID},
                    {"$set": {"Moved": True}})
            if lat == 0.0 and lon == 0.0:
                print("Received lat lon (0, 0). Using last known location")

        # Update numPlayers when updating a user's
        # CurrSession instead
        if ("NumPlayers" in message):
            print("NumPlayers from POST /de1 deprecated! Do nothing")

        if ("Win" in message):
            winner = message["Win"]
            winningUser = usersCluster.find_one({"UserID": winner})
            if winningUser is None:
                print("Winner not in database, reject \"win\" condition")
            else:
                if winningUser["CurrSession"]["ID"] == sessionID:
                    if winningUser["CurrSession"]["Win"]:
                        print(f"Player {winner} re-entered their win,"
                              " do nothing")
                    else:
                        print(f"Player {winner} has just won a game!")
                        usersCluster.update_one({"UserID": winner}, [
                            {"$set": {
                                "CurrSession": {
                                 "Timeout": time.time() + WIN_COOLDOWN,
                                 "Win": True
                                }}
                             }
                        ])
                        usersCluster.update_one(
                            {"UserID": winner},
                            {"$inc": {"SabTokens": WIN_SABTOKENS}}
                        )
                        score = calcScore(
                            winningUser["CurrSession"]["Start"],
                            time.time(),
                            session["Metrics"]["NumPlayers"]
                        )
                        sessionsCluster.update_one({
                            "SessionID": sessionID
                        }, {"$push": {"Leaderboard": {winner: score}}})
                else:
                    self.send_response(200)
                    self.end_headers()
                    self.wfile.write(f"Warning, {winner} tried to win"
                                     " a session he's not in".encode())
                    return

        self.send_response(200)
        self.end_headers()

    def leaveSession(self, user, clear=False):
        oldSession = sessionsCluster.find_one({
            "SessionID": user["CurrSession"]["ID"]
        })
        if oldSession is not None:
            sessionsCluster.update_one({
                "SessionID": user["CurrSession"]["ID"]
            }, {"$inc": {"Metrics.NumPlayers": -1}})
        if clear:
            usersCluster.update_one({
                "UserID": user["UserID"]
            }, {"$unset": {"CurrSession": ""}})

    def do_POST_App(self, message):
        print("Received POST request from mobile app")
        userID = message["UserID"]
        user = usersCluster.find_one({"UserID": userID})
        if user is None:
            print("User not found int database")
            self.send_response(400)
            self.end_headers()
            self.wfile.write("User not found in database".encode())
            return

        if ("SessionID" in message):
            sessionID = message["SessionID"]
            # Need to check != session second in boolean
            if ("CurrSession" not in user or
                    user["CurrSession"]["ID"] != sessionID):
                if "CurrSession" in user:
                    # "-1" is a flag for leave current session
                    if sessionID == "-1":
                        print("User leaving session")
                        self.leaveSession(user, clear=True)
                        self.send_response(200)
                        self.end_headers()
                    elif user["CurrSession"]["ID"] != sessionID:
                        print("User leaving previous session for new one")
                        oldSession = sessionsCluster.find_one({
                            "SessionID": user["CurrSession"]["ID"]
                        })
                        if oldSession is not None:
                            sessionsCluster.update_one({
                                "SessionID": user["CurrSession"]["ID"]
                            }, {"$inc": {"Metrics.NumPlayers": -1}})
                # Make sure someone's not starting a game from across the city
                try:
                    next(filter(
                        lambda loc: loc["SessionID"] == sessionID,
                        user["NearbySessions"]
                    ))
                except StopIteration:
                    print("Not a nearby session")
                    self.send_response(400)
                    self.end_headers()
                    self.wfile.write("Not a nearby session".encode())
                    return

                startTime = time.time()
                print(f"User {userID} starting at: {startTime}")
                usersCluster.update_one({"UserID": userID}, [
                    {"$set": {"CurrSession": {
                        "ID": sessionID,
                        "Start": time.time(),
                        "Timeout": time.time() + IDLE_TIMER,
                        "Win": False
                    }}}
                ])
                sessionsCluster.update_one({
                    "SessionID": sessionID
                }, {"$inc": {"Metrics.NumPlayers": 1}})

            if ("Sabotage" in message):
                sabNum = message["Sabotage"]
                print("Attempting sabotage: " + sabNum)
                tokens = user["SabTokens"]
                if tokens > 0:
                    usersCluster.update_one(
                        {"UserID": userID},
                        {"$inc": {"SabTokens": -1}}
                    )
                    sessionsCluster.update_one(
                        {"SessionID": sessionID},
                        [{"$set": {
                            "Sabotage": {
                                "SaboteurID": userID,
                                "SabotageID": sabNum,
                                "Duration": time.time() + SABOTAGE_DURATION
                            }
                        }}])
                    sabotageTimeout(sessionID, userID, sabNum)
                else:
                    print("Attempted sabotage with no tokens, do nothing")

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
        route = self.path.split('?')[0]
        if route == "/de1":
            self.do_POST_DE1(message)
        elif route == "/app":
            self.do_POST_App(message)
        else:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, POST request with invalid route!"
                             .encode())

    def parse_GET(self):
        params = parse_qs(urlparse(self.path).query)

        if (not params):
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, GET request missing parameters!".encode())
            return "", ""
        # Parse URL for ID parameter
        userID, newUser = self.checkUser(int(params["UserID"][0]))
        if (not userID):
            return "", ""
        if (newUser):
            usersCluster.insert_one({
                "UserID": userID,
                "Timeout": time.time() + IDLE_TIMER,
                "SabTokens": DEFAULT_SABTOKENS
            })
        return userID, params

    def do_GET_sessions(self):
        userID, params = self.parse_GET()
        if userID == "" and params == "":
            return  # Headers handled in parse_GET
        playerLoc = params["PlayerLoc"]
        nearbySessions = getNearby(playerLoc)
        usersCluster.update_one({"UserID": userID}, {
            "$set": {"NearbySessions": nearbySessions}
        })

        responseMSG = {
            "UserID": userID,
            "Sessions": nearbySessions
        }
        print(responseMSG)
        responseMSG = json.dumps(responseMSG)

        self.send_response(200)
        self.send_header("Content-type", "application/json")
        self.end_headers()
        self.wfile.write(responseMSG.encode())

    def do_GET_Lobby_Info(self):
        userID, params = self.parse_GET()
        if userID == "" and params == "":
            return  # Header handled in parse_GET
        sessionID = params["SessionID"][0]
        print(f"Getting session info for {sessionID}")
        session = sessionsCluster.find_one({"SessionID": sessionID})
        if session is None:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, session not found".encode())
            return

        if "Sabotage" in session:
            # sabotage = session["Sabotage"]
            sabotage = {
                "SaboteurID": session["Sabotage"]["SaboteurID"],
                "Sabotage": session["Sabotage"]["SabotageID"],
                "Duration": session["Sabotage"]["Duration"]
            }
        else:
            sabotage = None
        if "NumPlayers" in session["Metrics"]:
            numPlayers = str(session["Metrics"]["NumPlayers"])
        else:
            numPlayers = "0"
        winner = usersCluster.find_one({"UserID": userID})
        if winner is None:
            self.send_response(400)
            self.end_headers()
            self.wfile.write("Error, winner not found in database".encode())
            return
        sabTokens = winner["SabTokens"]
        if "CurrSession" in winner:
            win = winner["CurrSession"]["Win"]
        else:
            win = False
        if "Leaderboard" in session:
            leaderboard = session["Leaderboard"]
            # It's a list of dicts with one key-value pair, which
            # we sort by value
            leaderboard.sort(
                key=lambda x: list(x.values())[0],
                reverse=True
            )
        else:
            leaderboard = []
        responseMSG = {} if sabotage is None else sabotage
        responseMSG["NumPlayers"] = numPlayers
        responseMSG["SabTokens"] = sabTokens
        responseMSG["Win"] = win
        responseMSG["Leaderboard"] = leaderboard
        print(responseMSG)
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
                        default=""  # Default is localhost
                        )
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
