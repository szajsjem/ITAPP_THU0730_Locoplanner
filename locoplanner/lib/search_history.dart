import 'dart:convert';

import 'package:locoplanner/models/connections.dart';
import 'package:shared_preferences/shared_preferences.dart';

class SearchHistory{
  static SharedPreferences? sharedPreferences;
  static const String connectionsSP = 'cons';
  static final List<List<Connection>> _connections = [];

  static Future<void> loadData() async {
    if (sharedPreferences == null)
    {
      sharedPreferences = await SharedPreferences.getInstance();
      
      final json = sharedPreferences!.getString(connectionsSP);
      if (json != null){
        final Map<String, dynamic> data = jsonDecode(json) as Map<String, dynamic>;

        if (data['connections'] != null){
          for (final List<Connection> connection in data['connections']){
            _connections.add(connection);
          }
        }
      }
    }
  }

  static Future<void> saveData() async {
    sharedPreferences ??= await SharedPreferences.getInstance();

    List<List<Map<String, dynamic>>> data = [];

    for (final List<Connection> connection in _connections){
      data.add(connection.map((x) => x.toJson()).toList());
    }

    sharedPreferences!.setString(connectionsSP, jsonEncode(data));
  }

  static List<List<Connection>> get connections => _connections;

  static void addConnection(List<Connection> connection){
    _connections.add(connection);
    saveData();
  }
}