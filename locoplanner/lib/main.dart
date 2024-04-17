
import 'package:flutter/material.dart';
import 'package:locoplanner/new_search.dart';

import 'utils.dart';

void main() {
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

    ButtonStyle style = ElevatedButton.styleFrom(
      backgroundColor: const Color.fromARGB(255, 255, 199, 0),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
      fixedSize: Size(screenWidth * 0.6, screenHeight * 0.05),
    );
     
    return Scaffold(
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Image.asset('lib/assets/Locoplanner.png', width: screenWidth * 0.8,),
            ElevatedButton(onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => NewSearch()),
              );
            }, style: style, child: textRoboto('Create a trip', 16),),
            ElevatedButton(onPressed: () {}, style: style, child: textRoboto('Previous searches', 16),),
          ],
        ),
      ),
    );
  }
}
