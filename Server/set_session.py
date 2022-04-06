import argparse
import pymongo


client = pymongo.MongoClient("mongodb+srv://De1-SoC:l2b-11_sabotage@sabotage."
                             "q6bsa.mongodb.net/Sabotage?retryWrites="
                             "true&w=majority")
db = client["Sabotage"]
sessionsCluster = db["Sessions"]


def main():
    parser = argparse.ArgumentParser(
        description="Set session parameters for a given session"
    )
    parser.add_argument(
        "-i",
        "--id",
        metavar="SESSION_ID",
        type=str,
        help="Session ID of the session to set",
        required=True
    )
    parser.add_argument(
        "-c",
        "--coord",
        nargs=2,
        metavar=("LAT", "LON"),
        help="Coordinates of the session",
        default=("49.262093", "-123.248329")
    )
    parser.add_argument(
        "-n",
        "--numPlayers",
        metavar="NUMPLAYERS",
        type=int,
        help="Number of players in the session",
        default=0
    )
    args = parser.parse_args()

    lat, lon = args.coord
    sessionsCluster.update_one({
        "Session": args.id
    }, {"$set": {
        "Coord": [lat, lon],
        "Metrics": {
            "NumPlayers": args.numPlayers
        }}},
        upsert=True)


if __name__ == "__main__":
    main()
