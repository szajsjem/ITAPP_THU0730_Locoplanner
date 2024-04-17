import 'package:flutter/material.dart';

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
      appBar: AppBar(
        title: Image.asset('lib/assets/Locoplanner.png', width: screenWidth * 0.6, height: screenHeight *.05,),
        backgroundColor: blueMain,
      ),
      body: Center(
        child: Column(
          children: [
            queryRow('Starting station', 'Station you want to start your journey at', fancyTextField('Start', _startController),),
            queryRow('Ending station', 'Station you want to end your journey at', fancyTextField('End', _endController),),
            queryRow('City size', 'Minimum city population to justify a stop', fancyTextField('200000', _citySizeController, inputType: TextInputType.number),),
            queryRow('Sleep time', 'Minimum sleep time', selectableField(['4 hours', '5 hours', '6 hours', '7 hours', '8 hours', '9 hours', '10 hours'])),
            const Spacer(),
            yellowButton(() => null, screenWidth * 0.6, screenHeight * 0.05, 'Search a trip'),
            SizedBox(height: screenHeight * 0.05,)
          ],
        ),
      ),
    );
  }
}