
#include "pathfinding.h"

using namespace std;

int main(){
	pathfinder p;
	p.loadStations("stations");
	p.loadConnections("connections");
	p.setTravelPoint("Warszawa", -1, true, 0, false, true, false);
	p.setTravelPoint("Wrocław", -1, true, 0, false, false, true);
	//p.setTravelPoint()//other stations to visit, nice to visit and to avoid
	p.setStartTime(9.50, 15);
	p.setDesiredTripTime(12);
	p.setMaxTripTime(18);
	
	p.calculate();

	auto road = p.getconnections();



	return 0;
}
