#include "pathfinding.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <string>
#include <queue>
#include <cmath>
#include <limits>
#include <corecrt_math_defines.h>

long long timeToSeconds(int seconds, int minute, int hour, int day, int month, int year, int year0) {
	int monthLengths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (year % 4 == 0) {
		monthLengths[1] = 29;
	}
	long long  totalSeconds = 0;
	for (int y = year0; y < year; y++) {
		totalSeconds += ((y % 4 == 0) ? 366 : 365) * 24 * 60 * 60;
	}
	for (int m = 0; m < month - 1; m++) {
		totalSeconds += monthLengths[m] * 24 * 60 * 60;
	}
	totalSeconds += (day - 1) * 24 * 60 * 60;
	totalSeconds += hour * 60 * 60;
	totalSeconds += minute * 60;
	return totalSeconds;
}

void pathfinder::loadStations(std::string filename){
	std::fstream file(filename.data(), std::ios::in);
	if (!file.is_open())return;
	std::string temp;
	bool ok = false;
	while (std::getline(file, temp)){
		std::istringstream ss(temp);
		std::string token;
		int i = 0;
		station s;
		if (!ok) {
			// Check the headers
			std::getline(ss, token, ',');
			if (token != "stop_id") return;
			std::getline(ss, token, ',');
			if (token != "stop_name") return;
			std::getline(ss, token, ',');
			if (token != "stop_lat") return;
			std::getline(ss, token, ',');
			if (token != "stop_lon") return;
			ok = true;
			continue;
		}
		while (std::getline(ss, token, ',')) {
			switch (i)
			{
			case(0):
				s.stationNum = atoi(token.c_str());
				break;
			case(1):
				s.stationName = token;
				break;
			case(2):
				s.lat = atof(token.c_str());
				break;
			case(3):
				s.lon = atof(token.c_str());
				break;
			default:
				i = 0;
				break;
			}
			i++;
		}
		this->stations.push_back(s);
	}
}

void pathfinder::loadConnections(std::string filename) {
	std::fstream file(filename, std::ios::in);
	if (!file.is_open()) return;

	std::string temp;
	bool ok = false;

	struct stop {
		int station;
		float departure, arrival;
		std::string platform;
		float official_dist_traveled;
	};

	std::unordered_map<std::string, std::vector<stop>> data;
	std::string trip_id;

	while (std::getline(file, temp)) {
		std::istringstream ss(temp);
		std::string token;
		int i = 0;
		stop s;

		if (!ok) {
			// Check the headers
			std::getline(ss, token, ',');
			if (token != "trip_id") return;
			std::getline(ss, token, ',');
			if (token != "stop_sequence") return;
			std::getline(ss, token, ',');
			if (token != "stop_id") return;
			std::getline(ss, token, ',');
			if (token != "arrival_time") return;
			std::getline(ss, token, ',');
			if (token != "departure_time") return;
			std::getline(ss, token, ',');
			if (token != "platform") return;
			std::getline(ss, token, ',');
			if (token != "official_dist_traveled") return;
			ok = true;
			continue;
		}

		std::getline(ss, token, ',');
		trip_id = token;  // Get trip_id

		int ye, mo, da;
		sscanf(trip_id.c_str(), "%d-%d-%d", &ye,&mo,&da);

		if (ye < year0)printf("date mismatch\n");

		while (std::getline(ss, token, ',')) {
			switch (i) {
			case 0:  // stop_sequence, we don't need to store this
				break;
			case 1:  // stop_id
				s.station = std::stoi(token);
				break;
			case 2:  // arrival_time
			{
				int h, m, se;
				sscanf(token.c_str(), "%d:%d:%d", &h, &m, &se);
				s.arrival = timeToSeconds(se,m,h,da,mo,ye,year0);
			}
			break;
			case 3:  // departure_time
			{
				int h, m, se;
				sscanf(token.c_str(), "%d:%d:%d", &h, &m, &se);
				s.departure = timeToSeconds(se, m, h, da, mo, ye, year0);
			}
			break;
			case 4:  // platform
				s.platform = token;
				break;
			case 5:  // official_dist_traveled
				s.official_dist_traveled = std::stof(token);
				break;
			default:
				break;
			}
			i++;
		}
		data[trip_id].push_back(s);
	}

	// Create graph connections
	for (auto& stops : data) {
		int start = -1;
		for (int i = 0; i < stops.second.size(); i++) {
			const auto& s = stops.second[i];
			if (start != -1) {
				// Create a connection from 'start' to 's.station'
				connection conn;
				conn.company = "";  // Fill with appropriate company data if available
				conn.line = stops.first;
				conn.departure = stops.second[i - 1].departure;
				conn.arrival = s.arrival;
				conn.nextStation = s.station;

				// Assuming stationNum corresponds to stop_id and is index in 'stations' vector
				// Find or create the station with 'start' id
				auto x = convertNumToPointer(start);
				if (x == NULL)
					continue;
				x->connections.push_back(conn);
			}
			start = s.station;
		}
	}
}

std::vector<std::string> pathfinder::getStations(){
	std::vector<std::string> r;
	for (auto& s : this->stations) {
		if(s.connections.size()>0)
			r.push_back(s.stationName);
	}
	return r;
}

void pathfinder::setTravelPoint(std::string mStation, int order, bool needToVisit, float minimumVisitLengthMinutes, bool avoid, bool start, bool end){
	if (start) {
		auto& v = this->start;
		v.avoid = false;
		v.need = true;
		v.station = convertNameToPointer(mStation)->stationNum;
		v.visitTimeSeconds = 0;
		return;
	}
	if (end) {
		auto& v = this->end;
		v.avoid = false;
		v.need = true;
		v.station = convertNameToPointer(mStation)->stationNum;
		v.visitTimeSeconds = 0;
		return;
	}

	visitpoint k;
	k.avoid = avoid;
	k.need = needToVisit;
	k.station = convertNameToPointer(mStation)->stationNum;
	k.visitTimeSeconds = minimumVisitLengthMinutes*60; if (k.visitTimeSeconds < 0)k.visitTimeSeconds = 0;

	if (order < 0) {
		this->anywhere.push_back(k);
		return;
	}

	if (order >= this->ordered.size()) {
		ordered.resize(order + 1);
	}

	this->ordered[order].push_back(k);
}

void pathfinder::setStartTime(int minute, int hour, int day, int month, int year){
	startingSecond = timeToSeconds(0, minute, hour, day, month, year, year0);
}

void pathfinder::setMaxTripTime(float hours){
	maxTripSeconds = hours * 60 * 60;
}

void pathfinder::setDesiredTripTime(float hours){
	maxTripSeconds = hours * 60 * 60;
}

void pathfinder::setTravelType(std::string type){
	//todo
}

void pathfinder::setMinimumSleepTime(float hours){
	//todo
}

void pathfinder::setMinimumCityPopulationToStop(int population){
	//todo
}

double calculateDistance(float lat1, float lon1, float lat2, float lon2) {
	const double R = 6371e3; // Earth radius in meters
	double phi1 = lat1 * M_PI / 180;
	double phi2 = lat2 * M_PI / 180;
	double deltaPhi = (lat2 - lat1) * M_PI / 180;
	double deltaLambda = (lon2 - lon1) * M_PI / 180;

	double a = sin(deltaPhi / 2) * sin(deltaPhi / 2) +
		cos(phi1) * cos(phi2) *
		sin(deltaLambda / 2) * sin(deltaLambda / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));

	return R * c; // Distance in meters
}

void pathfinder::calculate(){//todo keep ordered in order

	fillLookupConnections();

	//cities to visit
	std::vector<std::pair<station*,visitpoint*>> stops;//translate global stationNum to local num:i stops[i]
	stops.push_back({
		convertNumToPointer(this->start.station),
		&this->start
		});
	for (auto& x : anywhere)stops.push_back({
		convertNumToPointer(x.station),
		&x});
	for (auto& y : ordered)for (auto& x : y)stops.push_back({ 
		convertNumToPointer(x.station),
		&x
		});

	//greedy+a*, better would be: tsp+a*
	std::vector<std::vector<connection*>> suborder;//ordered in global stationNum
	std::vector<bool> used(stops.size(),false);
	int curr = 0;
	long long currSecond = startingSecond;
	for (int i = 0; i < stops.size(); i++) {
		used[i] = true;
		if (i == stops.size() - 1)break;
		double minimum = std::numeric_limits<double>::max();
		int num = -1;
		std::vector<connection*> bestPath;

		for (int ii = 0; ii < stops.size(); ii++) {
			if (used[ii]) continue;

			// A* search to find the shortest path from stops[curr] to stops[ii]
			std::priority_queue<std::pair<double, std::vector<connection*>>> pq;
			pq.push({ 0, {} });

			while (!pq.empty()) {
				auto [cost, path] = pq.top();
				pq.pop();
				station* currentStation = path.empty() ? stops[curr].first : path.back()->next;

				if (currentStation->stationNum == stops[ii].first->stationNum) {
					if (cost < minimum) {
						minimum = cost;
						bestPath = path;
						num = ii;
					}
					break;
				}

				for (auto& conn : currentStation->connections) {
					if (conn.departure > currSecond + stops[curr].second->visitTimeSeconds) {
						double newCost = conn.arrival - conn.departure + calculateDistance(conn.next->lat, conn.next->lon, stops[ii].first->lat, stops[ii].first->lon);
						std::vector<connection*> newPath = path;
						newPath.push_back(&conn);
						pq.push({ newCost, newPath });
					}
				}
			}
		}

		if (num != -1) {
			suborder.push_back(bestPath);
			curr = num;
		}
		
	}
	{
		std::priority_queue<std::pair<double, std::vector<connection*>>> pq;
		pq.push({ 0, {} });
		station* ends = convertNumToPointer(end.station);
		while (!pq.empty()) {
			auto [cost, path] = pq.top();
			pq.pop();
			station* currentStation = path.empty() ? stops[curr].first : path.back()->next;

			if (currentStation->stationNum == ends->stationNum) {
				suborder.push_back(path);
				break;
			}

			for (auto& conn : currentStation->connections) {
				if (conn.departure > currSecond + stops[curr].second->visitTimeSeconds) {
					double newCost = conn.arrival-conn.departure + calculateDistance(ends->lat, ends->lon, conn.next->lat, conn.next->lon);
					std::vector<connection*> newPath = path;
					newPath.push_back(&conn);
					pq.push({ newCost, newPath });
				}
			}
		}
	}

	calculatedOrder.clear();
	station* s = stops[0].first;
	for(auto& i: suborder)
		for (auto& k : i) {
			calculatedOrder.push_back({ s,k });
			s = k->next;
		}
}

std::vector<std::string> pathfinder::getTrainStops(){
	std::vector<std::string> w;
	for (auto& x : calculatedOrder) {
		w.push_back(x.start->stationName);
	}
	return w;
}

std::vector<connection> pathfinder::getconnections(){
	std::vector<connection> w;
	for (auto& x : calculatedOrder) {
		w.push_back(*x.ride);
	}
	return w;
}


void pathfinder::fillLookupConnections() {
	for (auto& sta : stations)
		for (auto& conn : sta.connections) {
			conn.next = convertNumToPointer(conn.nextStation);
		}
}

station* pathfinder::convertNumToPointer(int stationNum)
{
	for (auto& s : stations) {
		if (s.stationNum == stationNum) {
			return &s;
		}
	}
	printf("not found station\n");
	return NULL;
}
station* pathfinder::convertNameToPointer(std::string stationName)
{
	for (auto& s : stations) {
		if (s.stationName == stationName) {
			return &s;
		}
	}
	printf("not found station\n");
	return NULL;
}
