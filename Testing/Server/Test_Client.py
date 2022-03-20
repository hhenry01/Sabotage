import requests
import argparse
import json
import Test_Data

test_POST_De1 = Test_Data.test_POST_De1
test_POST_app = Test_Data.test_POST_app

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

  # Test De1 POST
  print("====================")
  print("Test De1-SoC POST")
  print("====================")
  data = json.dumps(test_POST_De1)
  headers = {"Content-Type": "application/json"}
  print("Sending: " + data + " to: " + args.url)
  r = requests.post(args.url, data=data, headers=headers)

  if (r.status_code == 200):
    print("POST request was successful!")
  elif (r.status_code == 400):
    print(r.text)
  else:
    print("Error, unexpected status code...")

  # Test app POST
  print("====================")
  print("Test App POST")
  print("====================")
  data = json.dumps(test_POST_app)
  print("Sending: " + data + " to: " + args.url)
  r = requests.post(args.url, data=data, headers=headers)
  if (r.status_code == 200):
    print("POST request was successful!")
  elif (r.status_code == 400):
    print(r.text)
  else:
    print("Error, unexpected status code...")

  # Test app GET
  print("====================")
  print("Test App GET")
  print("====================")
  params = {"UserID": "0", "PlayerLoc": Test_Data.playerLoc}
  r = requests.get(args.url, params=params)
  if (r.status_code == 200):
    print("GET request returned successfully")
    print(r.json())
  elif (r.status_code == 400):
    print(r.text)
  else:
    print("Error, unexpected status code...")

if __name__ == "__main__":
  main()