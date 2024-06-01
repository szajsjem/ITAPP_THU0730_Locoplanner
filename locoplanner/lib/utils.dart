import 'dart:math';

import 'package:flutter/material.dart';
import 'package:locoplanner/models/station.dart';

import 'models/connections.dart';

Color blueMain = const Color.fromARGB(255, 25, 179, 245);
Color yellowMain = const Color.fromARGB(255, 255, 199, 0);
Color blueSecondary = const Color.fromARGB(255, 155, 215, 234);

Text textRoboto(String text, double size,){
  return Text(text, style: TextStyle(fontSize: size, fontWeight: FontWeight.bold, fontFamily: 'Roboto', color: Colors.black));
}

Text textJS(String text, double size){
  return Text(text, style: TextStyle(fontSize: size, fontWeight: FontWeight.w300, fontFamily: 'Josefin Sans', color: Colors.black));
}

ElevatedButton yellowButton(Function()? onPressed, double width, double height, String text){
  return ElevatedButton(onPressed: onPressed,
    style: ElevatedButton.styleFrom(
      elevation: 5,
      backgroundColor: yellowMain,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
      fixedSize: Size(width, height),
    ),
    child: textRoboto(text, 16),
  );
}

AppBar appBar(double screenWidth, double screenHeight, {bool isResultsPanel = false, required BuildContext context}){
  return AppBar(
    title: Image.asset('lib/assets/Locoplanner.png', width: screenWidth * 0.6, height: screenHeight *.05,),
    backgroundColor: blueMain,
    actions: isResultsPanel ? [
      IconButton(
        icon: const Icon(Icons.info),
        onPressed: () {
          showDialog(
            context: context,
            builder: (BuildContext context) {
              return const AlertDialog(
                title: Text('Icon caption', textAlign: TextAlign.center,),
                content: Column(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    Row(
                      children: [
                        Icon(Icons.train),
                        Text(' - '),
                        Text('Total time en route'),
                      ],
                    ),
                    SizedBox(height: 10,),
                    Row(
                      children: [
                        Icon(Icons.timeline_outlined),
                        Text(' - '),
                        Text('Total trip time'),
                      ],
                    ),
                    SizedBox(height: 10,),
                    Row(
                      children: [
                        Icon(Icons.timer_rounded),
                        Text(' - '),
                        Flexible(
                          child: Text(
                            'Average sightseeing time per city',
                            maxLines: 2,
                          ),
                        ),
                      ],
                    ),
                    SizedBox(height: 10,),
                    Row(
                      children: [
                        Icon(Icons.location_city),
                        Text(' - '),
                        Text(
                          'Count of visited cities',
                          maxLines: 2,
                        ),
                      ],
                    ),
                    SizedBox(height: 10,),
                    Row(
                      children: [
                        Icon(Icons.stars_rounded),
                        Text(' - '),
                        Text('Highlighted city'),
                      ],
                    ),
                    SizedBox(height: 10,),
                    Row(
                      children: [
                        Icon(Icons.flag),
                        Text(' - '),
                        Text('Count of visited countries'),
                      ],
                    ),
                  ]
                ),
              );
            },
          );
        },
      )
    ] : null,
  );
}

List<Station> randomPoints(int amount){
  List<Station> points = [];
  Random random = Random();
  for (int i = 0; i <= amount; i++){
    points.add(Station(
      name: 'Station $i',
      city: 'City $i',
      country: 'Country $i',
      longitude: 16.562243 + random.nextDouble(),
      latitude: 50.603557 + random.nextDouble(),
    ));
  }

  return points;
}

List<Connection> randomConnections(int amount){
  List<Connection> connections = [];
  List<Station> points = randomPoints(amount);
  Random random = Random();
  int j = 0;
  for (int i = j; i < points.length - 1; i++){
    connections.add(Connection(
      departure: "1${j++}:${random.nextInt(60).toString().padLeft(2, '0')}",
      arrival: "1${j++}:${random.nextInt(60).toString().padLeft(2, '0')}",
      departureStation: points[i],
      arrivalStation: points[i + 1],
    ));
  }

  return connections;
}
