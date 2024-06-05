import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:locoplanner/models/connections.dart';
import 'package:locoplanner/models/trip.dart';
import 'package:locoplanner/result.dart';
import 'package:loading_animation_widget/loading_animation_widget.dart';
import 'package:http/http.dart' as http;
import 'package:locoplanner/search_history.dart';

import 'utils.dart';

class NewSearch extends StatefulWidget {
  const NewSearch({super.key});

  @override
  State<NewSearch> createState() => _NewSearchState();
}

class _NewSearchState extends State<NewSearch> {

  final TextEditingController _startController = TextEditingController();
  final TextEditingController _endController = TextEditingController();
  final TextEditingController _citySizeController = TextEditingController();
  int _sleepTime = 8;
  DateTime? selectedDateTime;

  Future<void> fetchTrip() async {
    showDialog(
      context: context,
      builder: (context) {
        return Center(
          child: LoadingAnimationWidget.hexagonDots(
            color: Theme.of(context).colorScheme.primary,
            size: 55,
          ),
        );
      },
      barrierDismissible: false,
      useRootNavigator: false,
    );

    const uri = 'https://szajsjem.pl/route';

    selectedDateTime ??= DateTime.now();

    final body = jsonEncode(
        <String, dynamic>{
          'start': _startController.text,
          'end': _endController.text,
          'citysize': _citySizeController.text,
          'sleep': _sleepTime.toString(),
          'time': selectedDateTime!.subtract(const Duration(days: 40)).toString(),
        },
      );

    final response = await http.post(
      Uri.parse(uri,),
      headers: <String, String>{
        'Content-Type': 'application/json; charset=UTF-8',
      },
      body: body,
    );

    
    if (mounted) {
      Navigator.of(context).pop();
    }

    if (response.statusCode ~/ 100 != 2) {
      showDialog(
        context: context,
        builder: (context) {
          return AlertDialog(
            title: const Text('No Trip Found'),
            content: const Text('Sorry, no trip was found for the given search criteria.'),
            actions: [
              TextButton(
                onPressed: () {
                  Navigator.of(context).pop();
                },
                child: const Text('OK'),
              ),
            ],
          );
        },
      );
    } else{
      List<Trip> trips = tripFromJson(response.body);
      List<Connection> connections = Connection.connectionsFromTrips(trips);

      SearchHistory.addConnection(connections);
      
      Navigator.push(
        context,
        MaterialPageRoute(builder: (context) => ResultScreen(connections: connections)),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    double screenWidth = screenSize.width;
    double screenHeight = screenSize.height;
    
    const TextStyle localStyle = TextStyle(fontSize: 16, fontWeight: FontWeight.bold, fontFamily: 'Roboto', color: Colors.black);

    TextField fancyTextField(String hintText, TextEditingController controller, {TextInputType inputType = TextInputType.text}){
      return TextField(
        controller: controller,
        keyboardType: inputType,
        textAlign: TextAlign.center,
        style: localStyle,
        minLines: 1,
        maxLines: 2,
        decoration: InputDecoration(
          filled: true,
          fillColor: blueSecondary,
          hintText: hintText,
          hintStyle: localStyle,
          border: OutlineInputBorder(
            borderSide: BorderSide.none,
            borderRadius: BorderRadius.circular(25),
          ),
        ),
      );
    }

    DropdownMenu<String> selectableField(List<String> entries){

      List<DropdownMenuEntry<String>> dropdownMenuEntries = [];
      for (String entry in entries){
        dropdownMenuEntries.add(DropdownMenuEntry(value: entry, label: entry, labelWidget: textRoboto(entry, 16)));
      }

      void onSelected(String? value) {
        _sleepTime = int.parse(value!.split(' ')[0]);
      }

      return DropdownMenu<String>(
        expandedInsets: EdgeInsets.zero,
        dropdownMenuEntries: dropdownMenuEntries,
        textStyle: localStyle,
        initialSelection: '$_sleepTime hours',
        onSelected: onSelected,
        inputDecorationTheme: InputDecorationTheme(
          filled: true,
          fillColor: blueSecondary,
          hintStyle: localStyle,
          border: OutlineInputBorder(
            borderSide: BorderSide.none,
            borderRadius: BorderRadius.circular(25),
          ),
        ),
        menuStyle: MenuStyle(
          backgroundColor: MaterialStatePropertyAll<Color>(blueSecondary),
        ),
      );
    }

    Widget dateTimePickerWidget() {
      String placeholder = selectedDateTime != null
              ? '${selectedDateTime!.day}/${selectedDateTime!.month}/${selectedDateTime!.year} ${selectedDateTime!.hour}:${selectedDateTime!.minute}'
              : 'Select date';

      return GestureDetector(
      onTap: () {
        showDatePicker(
        context: context,
        initialDate: DateTime.now(),
        firstDate: DateTime.now(),
        lastDate: DateTime(2100),
        ).then((selectedDate) {
        if (selectedDate != null) {
          showTimePicker(
          context: context,
          initialTime: TimeOfDay.now(),
          ).then((selectedTime) {
          if (selectedTime != null) {
            setState(() {
              selectedDateTime = DateTime(
              selectedDate.year,
              selectedDate.month,
              selectedDate.day,
              selectedTime.hour,
              selectedTime.minute,
              );
            });
          }
          });
        }
        });
      },
      child: Container(
        width: double.infinity,
        padding: const EdgeInsets.symmetric(vertical: 12),
        decoration: BoxDecoration(
        color: blueSecondary,
        borderRadius: BorderRadius.circular(25),
        ),
        child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Text(
          placeholder,
          style: const TextStyle(
            fontSize: 16,
            fontWeight: FontWeight.bold,
            fontFamily: 'Roboto',
          ),
          ),
        ],
        ),
      ),
      );
    }

    Row queryRow(String upperText, String lowerText, Widget input){
      return Row(
        children: [
          SizedBox(
            width: screenWidth * 0.5,
            child: Padding(
            padding: const EdgeInsets.all(12),
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  textJS(upperText, 18),
                  textJS(lowerText, 12),
                ],
              ),
            ),
          ),
          Center(
            child: SizedBox(
              width: screenWidth * 0.5,
              child: Padding(
                padding: const EdgeInsets.all(12),
                child: input,
              ),
            ),
          ),
        ],
      );
    }

    return Scaffold(
      resizeToAvoidBottomInset: false,
      appBar: appBar(screenWidth, screenHeight, context: context),
      body: Center(
        child: Column(
          children: [
            queryRow('Starting station', 'Station you want to start your journey at', fancyTextField('Start', _startController),),
            queryRow('Ending station', 'Station you want to end your journey at', fancyTextField('End', _endController),),
            queryRow('City size', 'Minimum city population to justify a stop', fancyTextField('City size', _citySizeController, inputType: TextInputType.number),),
            queryRow('Sleep time', 'Minimum sleep time', selectableField(['4 hours', '5 hours', '6 hours', '7 hours', '8 hours', '9 hours', '10 hours'])),
            queryRow('Start date', 'The date and time your journey would start on', dateTimePickerWidget()),
            const Spacer(),
            yellowButton(() async {
                // Navigator.push(
                //   context,
                //   MaterialPageRoute(builder: (context) => ResultScreen(connections: randomConnections(5))),
                // );
                await fetchTrip();
              },
              screenWidth * 0.6,
              screenHeight * 0.05,
              'Search a trip'
            ),
            SizedBox(height: screenHeight * 0.05,)
          ],
        ),
      ),
    );
  }
}