import 'package:flutter/material.dart';

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
      backgroundColor: const Color.fromARGB(255, 255, 199, 0),
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(10)),
      fixedSize: Size(width, height),
    ),
    child: textRoboto(text, 16),
  );
}