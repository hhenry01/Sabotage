import requests
import argparse
import json

test_POST_input = {
  "SessionID": "123",
  "Coord": {
    "lat": "49.1234", 
    "lon": "-123.5678"
  },
  "NumPlayers": "2",
  "Win": "987"
}

def main():
  parser = argparse.ArgumentParser(description="Test client that simulates POST requests from a De1-SoC")
 
  parser.add_argument("-u",
                      "--url",
                      nargs="?",
                      metavar="URL",
                      type=str,
                      help="URL of the server being run",
                      default="http://127.0.0.1:8000" # Can use http://www.example.com
                      )

  args = parser.parse_args()
  data = json.dumps(test_POST_input)
  headers = {"Content-Type": "application/json"}
  print("Sending: " + data + " to: " + args.url)
  r = requests.post(args.url, data=data, headers=headers)

  if (r.status_code == 200):
    print("POST request was successful!")
  elif (r.status_code == 400):
    print(r.text)
  else:
    print("Error, unexpected status code...")

if __name__ == "__main__":
  main()