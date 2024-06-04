from flask import Flask, request, jsonify
from datetime import datetime
from subprocess import call

app = Flask(__name__)

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
    time = json_data.get('time', datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    citysize = json_data.get('citysize', 0)

    # call(["pathfinding.cpp", start, end, time, sleep, citysize])

    
    # Process the data and return the result

    # [
    #   {
    #       "departure_city": "New York",
    #       "arrival_city": "Los Angeles",
    #       "departure_time": "2021-09-01 12:00:00",
    #       "arrival_time": "2021-09-01 18:00:00",
    #   },
    #   {
    #       "departure_city": "Los Angeles",
    #       "arrival_city": "New York",
    #       "departure_time": "2021-09-01 12:00:00",
    #       "arrival_time": "2021-09-01 18:00:00",
    #   }
    # ]

    return jsonify(
        {"start": start, "end": end, "time": time, "sleep": sleep, "citysize": citysize}
        ), 200

    # change, now its just to check

if __name__ == '__main__':
    app.run()