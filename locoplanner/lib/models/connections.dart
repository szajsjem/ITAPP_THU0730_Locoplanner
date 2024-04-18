import 'package:locoplanner/models/station.dart';

class Connection{
  final String departure;
  final String arrival;
  final Station departureStation;
  final Station arrivalStation;

  Connection({required this.departure, required this.arrival, required this.departureStation, required this.arrivalStation});
}