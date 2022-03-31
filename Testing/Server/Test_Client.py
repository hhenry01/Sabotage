import requests
import argparse
import json
import Test_Data
import InitDB
import copy as cp


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

    args = parser.parse_args()
    InitDB.initDB()  # Fill database with necessary fields for testing
    test_POST_De1 = cp.deepcopy(Test_Data.test_POST_De1)
    test_POST_app = cp.deepcopy(Test_Data.test_POST_app)

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
    print(test_POST_app)
    data = json.dumps(test_POST_app)
    print("Sending: " + data + " to: " + args.url)
    r = requests.post(args.url, data=data, headers=headers)
    if (r.status_code == 200):
        print("POST request was successful!")
    elif (r.status_code == 400):
        print(r.text)
    else:
        print("Error, unexpected status code...")

    # Test app GET /sessions
    print("====================")
    print("Test App GET /sessions")
    print("====================")
    params = {"UserID": "0", "PlayerLoc": Test_Data.playerLoc}
    r = requests.get(args.url + "/sessions", params=params)
    if (r.status_code == 200):
        print("GET /sessions request was successful!")
        print(r.json())
    elif (r.status_code == 400):
        print(r.text)
    else:
        print("Error, unexpected status code...")

    # Test app GET /lobbies
    print("====================")
    print("Test App GET /lobby for winner")
    print("====================")
    params = {"UserID": test_POST_De1["Win"],
              "SessionID": test_POST_De1["SessionID"]}
    r = requests.get(args.url + "/lobby", params=params)
    if (r.status_code == 200):
        print("GET /lobby request was succesful!")
        print(r.json())
    elif (r.status_code == 400):
        print(r.text)
    else:
        print("Error, unexpected status code...")

    print("====================")
    print("Test App GET /lobby for sabotage")
    print("====================")
    params = {"UserID": test_POST_app["UserID"],
              "SessionID": test_POST_app["SessionID"]}
    r = requests.get(args.url + "/lobby", params=params)
    if (r.status_code == 200):
        print("GET /lobby request was succesful!")
        print(r.json())
    elif (r.status_code == 400):
        print(r.text)
    else:
        print("Error, unexpected status code...")


if __name__ == "__main__":
    main()
