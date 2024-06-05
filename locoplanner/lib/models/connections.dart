import 'package:locoplanner/models/station.dart';
import 'package:locoplanner/models/trip.dart';

class Connection{
  final String departure;
  final String arrival;
  final Station departureStation;
  final Station arrivalStation;

  Connection({required this.departure, required this.arrival, required this.departureStation, required this.arrivalStation});

  factory Connection.fromJson(Map<String, dynamic> json) {
    return Connection(
      departure: json['departure'],
      arrival: json['arrival'],
      departureStation: Station.fromJson(json['departureStation']),
      arrivalStation: Station.fromJson(json['arrivalStation']),
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'departure': departure,
      'arrival': arrival,
      'departureStation': departureStation.toJson(),
      'arrivalStation': arrivalStation.toJson(),
    };
  }

  static List<Connection> connectionsFromTrips(List<Trip> trips){
    List<Connection> connections = [];
    List<Station> stations = [];

    for (Trip trip in trips){
      Station newStation = Station(
        name: trip.station,
        country: 'Poland',
        city: trip.station,
        latitude: double.parse(trip.latitude),
        longitude: double.parse(trip.longitude)
      );
      stations.add(newStation);
    }

    for (int i=0; i < stations.length - 1; i++){
      connections.add(Connection(
        departure: trips[i].departureTime,
        arrival: trips[i+1].arrivalTime,
        departureStation: stations[i],
        arrivalStation: stations[i+1]
      ));
    }

    return connections;
  }
}