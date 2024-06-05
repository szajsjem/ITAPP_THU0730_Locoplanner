import 'package:flutter/material.dart';
import 'package:flutter_polyline_points/flutter_polyline_points.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';

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
  late List<bool> expandedConnections;
  Map<PolylineId, Polyline> polylines = {};
  List<LatLng> polylineCoordinates = [];
  Map<MarkerId, Marker> markers = <MarkerId, Marker>{};

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
    expandedConnections = List<bool>.filled(widget.connections.length, false);
    super.initState();
    polylinePoints = PolylinePoints();


    for (int i = 0; i < widget.connections.length; i++){
      final coordinates = LatLng(widget.connections[i].departureStation.latitude, widget.connections[i].departureStation.longitude);
      final Marker marker = Marker(
        markerId: MarkerId(i.toString()),
        position: coordinates,
        infoWindow: InfoWindow(
          title: widget.connections[i].departureStation.name,
          snippet: widget.connections[i].departure,
        ),
      );
      
      markers[MarkerId(i.toString())] = marker;

      polylineCoordinates.add(coordinates);
    }

    final coordinates = LatLng(widget.connections.last.arrivalStation.latitude, widget.connections.last.arrivalStation.longitude);
    int i = widget.connections.length;
    final Marker marker = Marker(
      markerId: MarkerId(i.toString()),
      position: coordinates,
      infoWindow: InfoWindow(
        title: widget.connections.last.arrivalStation.name,
        snippet: widget.connections.last.arrival,
      ),
    );
    
    markers[MarkerId(i.toString())] = marker;

    polylineCoordinates.add(coordinates);
    polylines[PolylineId(i.toString())] = Polyline(
      polylineId: PolylineId(i.toString()),
      color: blueMain,
      points: polylineCoordinates,
      width: 5,
    );
  }

  Future showMap(double screenWidth, double screenHeight) async {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          content: SizedBox(
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
              markers: Set<Marker>.of(markers.values),
            ),
          ),
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    double screenWidth = screenSize.width;
    double screenHeight = screenSize.height;

    SizedBox buffer = SizedBox(height: screenHeight * 0.02);

    String mainText = "${widget.connections[0].departureStation.name} -> ${widget.connections[widget.connections.length - 1].arrivalStation.name}";

    const textPadding = EdgeInsets.all(4.0);

    return Scaffold(
      appBar: appBar(screenWidth, screenHeight, context: context),
      body: Column(
        children: [
          Center(
            child: SizedBox(
              width: screenWidth * 0.9,
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
                          SizedBox(
                            height: screenHeight * 0.35,
                            child: ListView.builder(
                              shrinkWrap: true,
                              itemCount: widget.connections.length,
                              itemBuilder: (context, index) {
                                Connection connection = widget.connections[index];
                                return ExpansionPanelList(
                                  expansionCallback: (panelIndex, isExpanded) {
                                    setState(() => expandedConnections[index] = isExpanded);
                                  },
                                  elevation: 1,
                                  expandedHeaderPadding: EdgeInsets.zero,
                                  children: [
                                    ExpansionPanel(
                                      headerBuilder: (BuildContext context, bool isExpanded) {
                                        return ListTile(
                                          title: Text(
                                            "${connection.departureStation.name} to ${connection.arrivalStation.name}",
                                            style: TextStyle(fontSize: 16),
                                          ),
                                        );
                                      },
                                      body: Padding(
                                        padding: const EdgeInsets.all(8.0),
                                        child: Column(
                                          crossAxisAlignment: CrossAxisAlignment.start,
                                          children: [
                                            Text("Departure: ${connection.departure}"),
                                            SizedBox(height: 8),
                                            Text("Arrival: ${connection.arrival}"),
                                          ],
                                        ),
                                      ),
                                      isExpanded: expandedConnections[index],
                                    ),
                                  ],
                                );
                              },
                            ),
                          )
                        ]
                      )
                    )
                  ),
                  buffer,
                  buffer,
                  yellowButton(() => showMap(screenWidth, screenHeight), screenWidth * 0.5, screenHeight * 0.05, "Show on the map"),
                ]
              )
            ),
          ),
        ],
      ),
    );
  }
}

