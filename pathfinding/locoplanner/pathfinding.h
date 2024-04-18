#pragma once
#include <string>
#include <vector>

struct connection {
	std::string company,line;
	float departure;
	float arrival;
	union {
		int nextStation;
		std::string nextStationName;
	};
};

struct station {
	std::string stationName;
	float lat, lon;
	std::vector<connection> connections;
};

struct tripStats {
	//todo
};

class pathfinder {
	std::vector<station> stations;
public:
	void loadStations(std::string filename);
	void loadConnections(std::string filename);

	/*
		station name or maybe id?
		needToVisit -> set to false if Cool to visit
		order -> order of the cool cities to visit,
		if they have the same number thier order within is set based on the heuristic
		if -1 the position is anywhere
	*/
	void setTravelPoint(std::string station,int order=-1, bool needToVisit=true, float minimumVisitLength=0, bool avoid=false, bool start=false, bool end=false);
	void setStartTime(float hour, int day, int month = 4, int year = 2024);
	void setMaxTripTime(float hours);
	void setDesiredTripTime(float hours);
	void setTravelType(std::string type);
	void setMinimumSleepTime(float hours);
	void setMinimumCityPopulationToStop(int population);
	
	void calculate();

	std::vector<std::string> getTrainStops();
	std::vector<connection> getconnections();
};