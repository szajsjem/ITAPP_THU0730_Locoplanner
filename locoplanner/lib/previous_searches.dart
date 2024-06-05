import 'dart:math';

import 'package:flutter/material.dart';
import 'package:locoplanner/result.dart';
import 'package:locoplanner/search_history.dart';

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

    previousSearches = SearchHistory.connections;
  }

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    double screenWidth = screenSize.width;
    double screenHeight = screenSize.height;

    return Scaffold(
      appBar: appBar(screenWidth, screenHeight, isResultsPanel: true, context: context),
      body: Center(
        child: SingleChildScrollView(
          child: Container(
            width: screenWidth,
            color: blueMain,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                Column(
                  children: previousSearches.map((search) {
                    DateTime startDate = DateTime.parse(search.first.departure);
                    DateTime endDate = DateTime.parse(search.last.arrival);
                    int totalTime = endDate.difference(startDate).inMinutes;

                    Duration ert = Duration.zero;
                    for (int i=0; i<search.length; i++){
                      ert += DateTime.parse(search[i].arrival).difference(DateTime.parse(search[i].departure));
                    }

                    int ertInt = ert.inMinutes;
                    int astInt = (totalTime - ertInt) ~/ search.length;

                    String start = search.first.departureStation.name;
                    String destination = search.last.arrivalStation.name;
                    String totalEnRouteTime = "${ertInt~/60} h ${ertInt % 60} min";
                    String totalTravelTime = "${totalTime ~/ (60 * 24)} d ${totalTime ~/ 60} h ${totalTime % 60} min";
                    String averageSightseeingTime = "${astInt~/24} h ${astInt % 60} min";
                    int citiesVisited = search.length;
                    List<Connection> route = search;

                    return InkWell(
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(builder: (context) => ResultScreen(connections: route)),
                        );
                      },
                      child: Padding(
                        padding: const EdgeInsets.fromLTRB(8.0, 2.0, 8.0, 2.0),
                        child: Card(
                          child: Padding(
                            padding: const EdgeInsets.all(8.0),
                            child: Column(
                              children: [
                                  ClipRRect(
                                  borderRadius: BorderRadius.circular(8.0),
                                  child: Image.asset(
                                      'lib/assets/cities/${
                                        ['dresden.jpg', 'prague.jpg', 'warsaw.jpg', 'berlin.jpg', 'helsinki.jpg', 'vienna.jpg', 'vilnius.jpg','wroclaw.jpg']
                                        [Random().nextInt(8)]}',
                                      height: 0.1 * screenHeight,
                                      width: double.infinity,
                                      fit: BoxFit.cover,
                                    ),
                                  ),
                                ExpansionTile(
                                  title: Text('$start → $destination'),
                                  children: <Widget>[
                                    Row(
                                      mainAxisAlignment: MainAxisAlignment.start,
                                      children: [
                                        Column(
                                          crossAxisAlignment: CrossAxisAlignment.start,
                                          children: [
                                            Row(
                                              mainAxisAlignment: MainAxisAlignment.start,
                                              children: [
                                                const Icon(Icons.train,),
                                                const SizedBox(width: 10,),
                                                Text(totalEnRouteTime),
                                              ],
                                            ),
                                            
                                            Row(
                                              mainAxisAlignment: MainAxisAlignment.start,
                                              children: [
                                                const Icon(Icons.timeline_outlined),
                                                const SizedBox(width: 10,),
                                                Text(totalTravelTime),
                                              ],
                                            ),
                                            
                                            Row(
                                              mainAxisAlignment: MainAxisAlignment.start,
                                              children: [
                                                const Icon(Icons.timer_rounded),
                                                const SizedBox(width: 10,),
                                                Text(averageSightseeingTime),
                                              ],
                                            ),
                                          ],
                                        ),
                                        const Spacer(),
                                        
                                        Column(
                                          crossAxisAlignment: CrossAxisAlignment.end,
                                            children: [
                                              Row(
                                                mainAxisAlignment: MainAxisAlignment.start,
                                                children: [
                                                  Text(citiesVisited.toString()),
                                                  const SizedBox(width: 10,),
                                                  const Icon(Icons.location_city),
                                                ],
                                              ),
                                              
                                              Row(
                                                mainAxisAlignment: MainAxisAlignment.start,
                                                children: [
                                                  Text(route[Random().nextInt(max(route.length, 1))].arrivalStation.name),
                                                  const SizedBox(width: 10,),
                                                  const Icon(Icons.stars_rounded),
                                                ],
                                              ),
                                                                                            
                                              const Row(
                                                mainAxisAlignment: MainAxisAlignment.start,
                                                children: [
                                                  Text('1'),
                                                  SizedBox(width: 10,),
                                                  Icon(Icons.flag),
                                                ],
                                              ),
                                            ]
                                        ),
                                      ],
                                    ),
                                  ],
                                ),
                              ],
                            ),
                          ),
                        ),
                      ),
                    );
                  }).toList(),
                )
              ],
            ),
          ),
        ),
      )
    );
  }
}