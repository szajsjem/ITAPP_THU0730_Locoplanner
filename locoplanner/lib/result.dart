import 'package:flutter/material.dart';
import 'package:flutter_polyline_points/flutter_polyline_points.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';

import 'models/station.dart';
import 'models/connections.dart';
import 'utils.dart';

class ResultScreen extends StatefulWidget {

  final List<Connection> connections;
  const ResultScreen({super.key, required this.connections});

  @override
  State<ResultScreen> createState() => _ResultScreenState();
}

class _ResultScreenState extends State<ResultScreen> {
  late GoogleMapController _controller;
  late PolylinePoints polylinePoints;
  Map<PolylineId, Polyline> polylines = {};
  List<LatLng> polylineCoordinates = [];

  LatLng _middle(){
    double lat = 0;
    double long = 0;
    for (Connection connection in widget.connections){
      lat += connection.departureStation.latitude;
      long += connection.departureStation.longitude;
    }

    return LatLng(lat / widget.connections.length, long / widget.connections.length);
  }

  @override
  void initState() {
    super.initState();
    polylinePoints = PolylinePoints();

    for (int i = 0; i < widget.connections.length - 1; i++){
      polylineCoordinates.add(LatLng(widget.connections[i].departureStation.latitude, widget.connections[i].departureStation.longitude));
      polylines[PolylineId(i.toString())] = Polyline(
        polylineId: PolylineId(i.toString()),
        color: blueMain,
        points: polylineCoordinates,
        width: 5,
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    double screenWidth = screenSize.width;
    double screenHeight = screenSize.height;

    SizedBox buffer = SizedBox(height: screenHeight * 0.02);

    String mainText = "${widget.connections[0].departureStation.name} -> ${widget.connections[widget.connections.length - 1].arrivalStation.name}";

    return Scaffold(
      appBar: appBar(screenWidth, screenHeight),
      body: Column(
        children: [
          SizedBox(
            width: screenWidth * 0.8,
            child: Column(
              children: [
                buffer,
                Card(
                  color: blueSecondary,
                  child: Padding(
                    padding: const EdgeInsets.all(8.0),
                    child: Column(
                      children: [
                        textRoboto(mainText, 20),
                        buffer,
                        for (int i = 0; i < widget.connections.length + 1; i++)
                          Padding(
                            padding: const EdgeInsets.symmetric(vertical: 4.0),
                            child: Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              
                                // if(i > 0) textJS(widget.connections[i - 1].departure, 16),
                                // if(i > 0) const Icon(Icons.arrow_forward),
                                // const SizedBox(width: 4),
                                // if(i == widget.connections.length - 1) textJS(widget.connections[i].departureStation.name, 16) else textJS(widget.connections[i].arrivalStation.name, 16),
                                // const SizedBox(width: 4),
                                // if(i < widget.connections.length) const Icon(Icons.arrow_forward),
                                // if(i < widget.connections.length) textJS(widget.connections[i].arrival, 16),
                              children: i==0 ? [
                                textJS(widget.connections[i].departureStation.name, 16),
                                const Icon(Icons.arrow_forward),
                                textJS(widget.connections[i].departure, 16),
                              ] : i != widget.connections.length ? [
                                textJS(widget.connections[i - 1].departure, 16),
                                const Icon(Icons.arrow_forward),
                                textJS(widget.connections[i - 1].arrivalStation.name, 16),
                                const Icon(Icons.arrow_forward),
                                textJS(widget.connections[i - 1].arrival, 16)
                              ] : [
                                textJS(widget.connections[i - 1].arrival, 16),
                                const Icon(Icons.arrow_forward),
                                textJS(widget.connections[i - 1].arrivalStation.name, 16),
                              ],
                            ),
                          ),
                      ],
                    ),
                  ),
                ),
              ],
            ),
          ),
          const Spacer(),
          Center(
            child: SizedBox(
              width: screenWidth,
              height: screenHeight * 0.5,
              child: GoogleMap(
                initialCameraPosition: CameraPosition(
                  target: _middle(),
                  zoom: 8,
                ),
                onMapCreated: (GoogleMapController controller) {
                  _controller = controller;
                },
                polylines: Set<Polyline>.of(polylines.values),
              ),
            ),
          ),
        ],
      ),
    );
  }
}