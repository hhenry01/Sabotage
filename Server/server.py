from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs
import json
import argparse
import time
from Testing import Test_Data

"""
Given a player location [lat, lon], return a list of nearby
sessions.
"""
def getNearby(playerLoc):
  return Test_Data.locations

"""
Generate a unique user ID
"""
def generateUserID():
  return 1

"""
Given a user ID, check if the user exists.
If yes, then return the given ID and False (not a new user).
If no, then return a newly generated ID and true (is a new user).
"""
def checkUser(messageID):
  if (not messageID):
    userID = generateUserID()
    print("New User, ID: " + str(userID))
    return userID, True
  else:
    userID = messageID
    print("User ID: " + str(userID))
    return userID, False

class HTTPRequestHandler(BaseHTTPRequestHandler):
  def __init__(self, *args, **kwargs):
    super().__init__(*args, **kwargs)

  def do_POST(self):
    print("Handling POST request.")

    # Reject requests that are not JSON
    if (self.headers["Content-Type"] != "application/json"):
      self.send_response(400)
      self.wfile.write("Error, POST request is not a JSON string")
      self.end_headers()
      return

    length = int(self.headers["Content-Length"])
    message = json.loads(self.rfile.read(length))

    # SessionID is required for De1-SoC POST requests. All others are optional.
    if ("SessionID" in message):
      print("Received POST request from De1-SoC: " + message["SessionID"])
      # TODO: Verify ID and check ID exists database
      # IDs should only be added to the database manually by admins

      if ("Coord" in message):
        lat = float(message["Coord"]["lat"])
        lon = float(message["Coord"]["lon"])
        # TODO: Update database with coordinates
        print("Received new GPS coordinates:\n" +
              "Latitude: " + str(lat) + '\n' +
              "Longitude: " + str(lon))

      if ("NumPlayers" in message):
        numPlayers = int(message["NumPlayers"])
        # TODO: Update database with num players.
        print("Number of players updated to: " + str(numPlayers))

      if ("Win" in message):
        winner = int(message["Win"])
        # TODO: Verify that the winner is in the database
        # Check that the session ID in the user's database entry matches
        # the session ID sent along withh the POST request.
        print("Player " + str(winner) + " has just won a game!")

      self.send_response(200)
      self.end_headers()

    elif ("UserID" in message):
      print("Received POST request from mobile app")
      userID = int(message["UserID"])
      # TODO Verify userID with database
      if ("Start" in message):
        location = int(message["Start"])
        # TODO Get actual location by referencing user's database entry
        # then write the session ID to the user's database entry.
        startTime = time.time()
        # TODO write the start time to the user's database entry
        print("User " + str(userID) + " starting at: " + str(startTime))
      if ("Sabotage" in message):
        sabNum = int(message["Sabotage"])
        print("Attempting sabotage: " + str(sabNum))
        # TODO Verify the user is allowed to do the sabotage.

      self.send_response(200)
      self.end_headers()

    else:
      self.send_response(400)
      self.wfile.write("Error, POST request missing session or user ID!")
      self.end_headers()

  def do_GET(self):
    print("Handling GET request")
    params = parse_qs(urlparse(self.path).query)

    if (not params):
      self.send_response(400)
      self.wfile.write("Error, GET request missing parameters!".encode())
      self.end_headers()
      return

    userID, newUser = checkUser(int(params["UserID"][0])) # Parse URL for ID parameter
    if (newUser):
      #TODO Add new user to database
      x = 0 # Prevent errors, remove when above TODO is implemented
    playerLoc = params["PlayerLoc"]
    nearbySessions = getNearby(playerLoc)
    metrics = {}
    for session in nearbySessions:
      for metric in session["Metrics"]:
        if (metric not in metrics):
          metrics[metric] = []
        metrics[metric].append(session["Metrics"][metric])

    responseMSG = {
      "UserID": userID,
      "Locations": nearbySessions,
      "Metrics": metrics
    }
    responseMSG = json.dumps(responseMSG)

    self.send_response(200)
    self.send_header("Content-type", "application/json")
    self.end_headers()
    self.wfile.write(responseMSG.encode())

def main():
  parser = argparse.ArgumentParser(description= "Remote server that processes HTTP requests"
                                                "from the De1-SoC and mobile app")
  parser.add_argument("-i",
                      "--ip",
                      nargs="?",
                      metavar="IP_ADDRESS",
                      type=str,
                      help="IP address to run the server on",
                      default=""
                      ) # Default is localhost
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

  httpd = HTTPServer((args.ip, args.port), HTTPRequestHandler)
  httpd.serve_forever()

if __name__ == "__main__":
  main()