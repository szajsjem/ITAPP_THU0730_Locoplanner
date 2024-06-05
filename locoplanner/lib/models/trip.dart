import 'dart:convert';

List<Trip> tripFromJson(String str) => List<Trip>.from(json.decode(str).map((x) => Trip.fromJson(x)));

String tripToJson(List<Trip> data) => json.encode(List<dynamic>.from(data.map((x) => x.toJson())));

class Trip {
    String arrivalTime;
    String departureTime;
    String latitude;
    String longitude;
    String station;
    String train;

    Trip({
        required this.arrivalTime,
        required this.departureTime,
        required this.latitude,
        required this.longitude,
        required this.station,
        required this.train,
    });

    factory Trip.fromJson(Map<String, dynamic> json) => Trip(
        arrivalTime: json["arrival_time"],
        departureTime: json["departure_time"],
        latitude: json["latitude"],
        longitude: json["longitude"],
        station: json["station"],
        train: json["train"],
    );

    Map<String, dynamic> toJson() => {
        "arrival_time": arrivalTime,
        "departure_time": departureTime,
        "latitude": latitude,
        "longitude": longitude,
        "station": station,
        "train": train,
    };
}
