#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>
#include <filesystem>
#include "pathfinding.h"

using namespace std;

int main(){
	setlocale(LC_ALL, "pl_PL");
	pathfinder p;
	p.loadStations("../../../../dataset/polish/stops.txt");
	p.loadConnections("../../../../dataset/polish/stop_times.txt");
	auto stations = p.getStations();
	int size = stations.size();
	p.setTravelPoint(stations[1], -1, true, 0, false, true, false);
	p.setTravelPoint(stations[3], -1, true, 0, false, false , false);
	p.setTravelPoint(stations[5], -1, true, 0, false, false, true);
	//p.setTravelPoint()//other stations to visit, nice to visit and to avoid
	p.setStartTime(50, 9, 15);
	p.setDesiredTripTime(12);
	p.setMaxTripTime(18);
	
	p.calculate();
	auto sta = p.getTrainStops();
	printf("----Road----\nstart:%s\n", sta[0].c_str());
	auto road = p.getconnections();
	for (auto& a : road) {
		printf("%s->%s ,%d,%d\n", a.line.c_str(), a.next->stationName.c_str(), (int)a.departure, (int)a.arrival);
	}


	return 0;
}
