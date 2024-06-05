class Station{
  final String name;
  final String city;
  final String country;
  final double latitude;
  final double longitude;

  Station({
    required this.name,
    required this.city,
    required this.country,
    required this.latitude,
    required this.longitude
  });

  factory Station.fromJson(Map<String, dynamic> json) {
    return Station(
      name: json['name'],
      city: json['city'],
      country: json['country'],
      latitude: json['latitude'],
      longitude: json['longitude'],
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'name': name,
      'city': city,
      'country': country,
      'latitude': latitude,
      'longitude': longitude,
    };
  }
}