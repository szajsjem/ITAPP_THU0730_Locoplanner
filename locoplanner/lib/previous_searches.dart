import 'dart:math';

import 'package:flutter/material.dart';
import 'package:locoplanner/result.dart';

import 'models/connections.dart';
import 'utils.dart';

class PreviousSearches extends StatefulWidget {
  const PreviousSearches({super.key});

  @override
  State<PreviousSearches> createState() => _PreviousSearchesState();
}

class _PreviousSearchesState extends State<PreviousSearches> {
  
  List<List<Connection>> previousSearches = [];

  @override
  void initState() {
    super.initState();

    for (int i = 0; i < 5; i++) {
      previousSearches.add(randomConnections(5));
    }
  }

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    double screenWidth = screenSize.width;
    double screenHeight = screenSize.height;

    return Scaffold(
      appBar: appBar(screenWidth, screenHeight),
      body: Center(
        child: Container(
          width: screenWidth,
          color: blueMain,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Column(
                children: previousSearches.map((search) {
                  String start = search.first.departureStation.name;
                  String destination = search.last.arrivalStation.name;
                  String totalTripTime = "${Random().nextInt(10)} hours ${Random().nextInt(60)} minutes";

                  return InkWell(
                    onTap: () {
                      Navigator.push(
                        context,
                        MaterialPageRoute(builder: (context) => ResultScreen(connections: randomConnections(5))),
                      );
                    },
                    child: Card(
                      child: ListTile(
                        title: Text('Start: $start'),
                        subtitle: Text('Destination: $destination'),
                        trailing: Text('Total Trip Time:\n $totalTripTime'),
                      ),
                    ),
                  );
                }).toList(),
              )
            ],
          ),
        ),
      ),
    );
  }
}