from http.server import HTTPServer, BaseHTTPRequestHandler
import json
import argparse

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
      # TODO: Verify ID is and check with database

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
        print("Player " + str(winner) + " has just won a game!")

      self.send_response(200)
      self.end_headers()
    elif ("UserID" in message):
      print("TODO")
    else:
      self.send_response(400)
      self.wfile.write("Error, POST request missing session or user ID!")
      self.end_headers()

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