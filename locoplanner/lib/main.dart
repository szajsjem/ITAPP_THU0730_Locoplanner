
import 'package:flutter/material.dart';
import 'package:locoplanner/new_search.dart';
import 'package:locoplanner/previous_searches.dart';
import 'package:locoplanner/search_history.dart';

import 'utils.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await SearchHistory.loadData();

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Locoplanner',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    double screenWidth = screenSize.width;
    double screenHeight = screenSize.height;

    double buttonWidth = screenWidth * 0.6;
    double buttonHeight = screenHeight * 0.05;
     
    return Scaffold(
      body: Center(
        child: Container(
          width: screenWidth,
          color: blueMain,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Image.asset('lib/assets/Locoplanner.png', width: screenWidth * 0.8,),
              SizedBox(height: screenHeight * 0.2,),
              yellowButton(
                () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => const NewSearch()),
                  );
                },
                buttonWidth,
                buttonHeight,
                'Create a trip'
              ),
              SizedBox(height: screenHeight * 0.03,),
              yellowButton(
                () {
                  Navigator.push(
                    context,
                    MaterialPageRoute(builder: (context) => const PreviousSearches()),
                  );
                },
                buttonWidth,
                buttonHeight,
                'Previous searches'
              ),
            ],
          ),
        ),
      ),
    );
  }
}
