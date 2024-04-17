import 'package:flutter/material.dart';

Widget textRoboto(String text, double size,){
  return Text(text, style: TextStyle(fontSize: size, fontWeight: FontWeight.bold, fontFamily: 'Roboto', color: Colors.black));
}

Widget textJS(String text, double size){
  return Text(text, style: TextStyle(fontSize: size, fontWeight: FontWeight.w300, fontFamily: 'Josefin Sans', color: Colors.black));
}