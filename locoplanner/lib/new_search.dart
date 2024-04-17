import 'package:flutter/material.dart';

import 'utils.dart';

class NewSearch extends StatelessWidget {
  const NewSearch({super.key});

  @override
  Widget build(BuildContext context) {
    Size screenSize = MediaQuery.of(context).size;
    double screenWidth = screenSize.width;
    double screenHeight = screenSize.height;
    
    const TextStyle localStyle = TextStyle(fontSize: 16, fontWeight: FontWeight.bold, fontFamily: 'Roboto', color: Colors.black);
    const Color fieldColor = Color.fromARGB(255, 155, 215, 234);

    TextField fancyTextField(String hintText, TextEditingController controller, {TextInputType inputType = TextInputType.text}){
      return TextField(
        // controller: controller,
        keyboardType: inputType,
        textAlign: TextAlign.center,
        style: localStyle,
        minLines: 1,
        maxLines: 2,
        decoration: InputDecoration(
          filled: true,
          fillColor: fieldColor,
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

      return DropdownMenu<String>(
        expandedInsets: EdgeInsets.zero,
        dropdownMenuEntries: dropdownMenuEntries,
        textStyle: localStyle,
        initialSelection: '8 hours',
        inputDecorationTheme: InputDecorationTheme(
          filled: true,
          fillColor: fieldColor,
          hintStyle: localStyle,
          border: OutlineInputBorder(
            borderSide: BorderSide.none,
            borderRadius: BorderRadius.circular(25),
          ),
        ),
        menuStyle: const MenuStyle(
          backgroundColor: MaterialStatePropertyAll<Color>(fieldColor),
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
        backgroundColor: const Color.fromARGB(255, 25, 179, 245),
      ),
      body: Center(
        child: Column(
          children: [
            queryRow('Starting station', 'Station you want to start your journey at', fancyTextField('Start', TextEditingController()),),
            queryRow('Ending station', 'Station you want to end your journey at', fancyTextField('End', TextEditingController()),),
            queryRow('City size', 'Minimum city population to justify a stop', fancyTextField('200000', TextEditingController(), inputType: TextInputType.number),),
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