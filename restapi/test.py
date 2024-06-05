import pytest
import requests

url = "https://szajsjem.pl/"#"http://127.0.0.1:5000/"
def test_request_stations():
    response = requests.get(url+"stations")
    data = response.json()
    assert data
    assert response.status_code == 200


def test_route_wrong():
    req = {
        #"start": "Katowice",
        "end": "Wrocław Główny",
        "sleep": 8
    }
    response = requests.post(url+"route", json=req)
    assert response.status_code != 200

def test_route_ok():
    req = {
        "start": "Katowice",
        "end": "Wrocław Główny",
        "sleep": 8
    }
    response = requests.post(url+"route", json=req)
    assert response.status_code == 200
    stations = response.json()
    #smth like this
    """
    [{'arrival_time': '0000-00-00 00:00', 'departure_time': '2024-04-17 04:49', 'latitude': '50.257610', 'longitude': '19.017408', 'station': 'Katowice', 'train': '40001'}, {'arrival_time': '2024-04-17 05:05', 'departure_time': '2024-04-17 05:06', 'latitude': '50.305317', 'longitude': '18.787264', 'station': 'Zabrze', 'train': '40001'}, {'arrival_time': '2024-04-17 05:12', 'departure_time': '2024-04-17 23:55', 'latitude': '50.301098', 'longitude': '18.677227', 'station': 'Gliwice', 'train': '38172-3'}, {'arrival_time': '2024-04-18 02:42', 'departure_time': '0000-00-00 00:00', 'latitude': '51.098057', 'longitude': '17.036860', 'station': 'Wrocław Główny', 'train': 'end'}]
    """
    assert stations
    assert stations[0]
    assert "latitude" in stations[0]
    assert "longitude" in stations[0]
    assert "station" in stations[0]
    assert "departure_time" in stations[0]