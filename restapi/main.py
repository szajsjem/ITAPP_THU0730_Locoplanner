from flask import Flask, request, jsonify
from datetime import datetime
from subprocess import call

app = Flask(__name__)


@app.route('/stations', methods=['GET'])
def stationsget():
    try:
        # Open the file and read lines
        with open('sta.txt', 'r') as file:
            station_lines = file.readlines()

        # Process the lines to create a list of dictionaries
        stations = [{"name": line.strip()} for line in station_lines if line.strip()]

        # Return the list as a JSON response
        return jsonify(stations), 200
    except FileNotFoundError:
        # Handle the case where the file does not exist
        return jsonify({"error": "File not found"}), 404
    except Exception as e:
        # Handle other potential exceptions
        return jsonify({"error": str(e)}), 500


@app.route('/route', methods=['POST'])
def generate_route():
    json_data = request.get_json()

    required_fields = ['start', 'end', 'sleep']

    for field in required_fields:
        if field not in json_data:
            return jsonify({"error": f"Missing required field: {field}"}), 400

    start = json_data['start']
    end = json_data['end']
    sleep = json_data['sleep']
    time = json_data.get('time', "2024-04-15,08:30:00")
    citysize = json_data.get('citysize', 0)

    output_file = "output.txt"

    command = [
        "locoplanner",
        "-start", start.encode('utf-8'),
        "-end", end.encode('utf-8'),
        "-o", output_file,
        "-stime", str(time),
        "-ci", str(citysize),
        "-sleep", str(sleep),
        "-etime", "240"
    ]
    call(command)

    try:
        with open(output_file, 'r') as file:
            lines = file.readlines()

        result = []
        for line in lines:
            station, lat, lon, arrival, departure, train = line.strip().split(',')
            result.append({
                "station": station,
                "latitude": lat,
                "longitude": lon,
                "arrival_time": arrival,
                "departure_time": departure,
                "train": train
            })
        if result.__sizeof__() > 1:
            return jsonify(result)
        else:
            return jsonify({"error": "no route found"}), 500

    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == '__main__':
    app.run(debug=True, port=5000)