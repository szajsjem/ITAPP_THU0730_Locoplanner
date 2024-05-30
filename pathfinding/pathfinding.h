#pragma once
#include <string>
#include <vector>

struct station;

struct connection {
	std::string company, line;
	long long departure;
	long long arrival;
	int nextStation;
	station* next;//calc
};

struct station {
	std::string stationName;
	int stationNum;
	float lat, lon;
	std::vector<connection> connections;
};

struct visitpoint {
	int station;
	bool avoid;
	bool need;
	float visitTimeSeconds;
};

struct trainride{
	station* start;
	connection* ride;
};

class pathfinder {
	std::vector<station> stations;
	int year0 = 2024;
	long long startingSecond = 0;
	long long maxTripSeconds = 1;
	long long desiredTripSeconds = 1;
	std::vector<visitpoint> anywhere;
	std::vector<std::vector<visitpoint>> ordered;
	visitpoint start;
	visitpoint end;
	std::vector<trainride> calculatedOrder;

	void fillLookupConnections();
	station* convertNumToPointer(int stationNum);
	station* convertNameToPointer(std::string stationName);

public:
	void loadStations(std::string filename);
	void loadConnections(std::string filename);

	/*
		tutaj mogê dodaæ lokalizacje gps poszczególnych stacji jak chcemy to na mapie wyœwietlaæ
	*/
	std::vector<std::string> getStations();

	/*
		station name /\
		needToVisit -> set to false if Cool to visit
		order -> order of the cool cities to visit,
		if they have the same number thier order within is set based on the heuristic
		if -1 the position is anywhere
	*/
	void setTravelPoint(std::string station,int order=-1, bool needToVisit=true, float minimumVisitLengthMinutes =15, bool avoid=false, bool start=false, bool end=false);
	void setStartTime(int minute, int hour, int day, int month = 4, int year = 2024);
	void setMaxTripTime(float hours);
	void setDesiredTripTime(float hours);
	void setTravelType(std::string type);
	void setMinimumSleepTime(float hours);
	void setMinimumCityPopulationToStop(int population);
	
	void calculate();

	std::vector<std::string> getTrainStops();
	std::vector<connection> getconnections();
};