#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>
#include <filesystem>
#include "pathfinding.h"
#include <locale.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>

using namespace std;

int mainttst(){
	setlocale(LC_CTYPE, ".UTF8");
	srand(time(NULL));
	pathfinder p;
#ifdef WIN32
    p.loadStations("../../../../dataset/polish/stops.txt");
    p.loadConnections("../../../../dataset/polish/stop_times.txt");
#else
	p.loadStations("/home/pi/dataset/polish/stops.txt");
    printf("stops loaded\n");
	p.loadConnections("/home/pi/dataset/polish/stop_times.txt");
    printf("connections loaded\n");
#endif
    auto stations = p.getStations();
	int size = stations.size();
    if (size == 0) {
        printf("no data\n");
        return 0;
    }
	p.setStartTime(50, 9, 15);
	p.setDesiredTripTime(12);
	p.setMinimumTrainHopTime(5);
	p.setMaxTripTime(240);
    while (true) {
        p.setTravelPoint(stations[rand() % size], -1, true, 0, false, true, false);
        //p.setTravelPoint(stations[rand() % size], -1, true, 200, false, false, false);
        //p.setTravelPoint(stations[rand() % size], -1, true, 200, false, false, false);
        //p.setTravelPoint(stations[rand() % size], -1, true, 200, false, false, false);
        //p.setTravelPoint(stations[rand() % size], -1, true, 200, false, false, false);
        p.setTravelPoint(stations[rand() % size], -1, true, 0, false, false, true);
        //p.setTravelPoint()//other stations to visit, nice to visit and to avoid

        p.calculate();
        auto sta = p.getTrainStops();
        printf("----Road----\nstart:%s\n", sta[0].c_str());
        auto road = p.getconnections();
        for (auto& a : road) {
            printf("%s->%s ,%d,%d\n", a.line.c_str(), a.next->stationName.c_str(), (int)a.departure, (int)a.arrival);
        }
    }
	p.saveToFile("out.csv");

	return 0;
}


void printHelp() {
    std::cout << "Usage: main -start startStation,YYYY-MM-DD,HH:MM -end endStation -point station,order,needToVisit,minVisitLength -stations stationsFile -connections connectionsFile [-o outputFile] [-os availableStationsFile] [-pointFile TravelPoints.txt]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -start       Start station, date and time in the format station,YYYY-MM-DD,HH:MM" << std::endl;
    std::cout << "  -end         End station" << std::endl;
    std::cout << "  -point       Travel point in the format station,order,needToVisit,minVisitLength" << std::endl;
    std::cout << "  -stations    Path to the stations file" << std::endl;
    std::cout << "  -connections Path to the connections file" << std::endl;
    std::cout << "  -o           Path to the output file" << std::endl;
    std::cout << "  -os          Path to the file to save available stations" << std::endl;
    std::cout << "  -help        Display this help message" << std::endl;
}

void loadTravelPointsFromFile(const std::string& filename, std::vector<std::string>& points) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open travel points file: " << filename << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        points.push_back(line);
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_CTYPE, ".UTF8");

    std::string startStation = "Hel";
    std::string endStation = "Wrocław Główny";
    std::string outputFile = "out.txt";
    int startYear = 2024, startMonth = 4, startDay = 15, startHour = 8, startMinute = 0;
    float desiredTripTime = 12.0, maxTripTime = 240.0, minimumTrainHopTime = 5.0;
    int citysize = 0;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-start" && i + 1 < argc) {
            startStation = argv[++i];
        }
        else if (arg == "-end" && i + 1 < argc) {
            endStation = argv[++i];
        }
        else if (arg == "-stime" && i + 1 < argc) {
            std::sscanf(argv[++i], "%d-%d-%d,%d:%d", &startYear, &startMonth, &startDay, &startHour, &startMinute);
        }
        else if (arg == "-etime" && i + 1 < argc) {
            maxTripTime = atoi(argv[++i]);
        }
        else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        }
        else if (arg == "-ci" && i + 1 < argc) {
            citysize = atoi(argv[++i]);
        }
        else if (arg == "-sleep" && i + 1 < argc) {
            minimumTrainHopTime = atoi(argv[++i]);
        }
        else if (arg == "-help") {
            printHelp();
            return 0;
        }
        else {
            printHelp();
            return 1;
        }
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "start " << startStation << std::endl;
    std::cout << "end " << endStation << std::endl;
    std::cout << "out file " << outputFile << std::endl;
    std::cout << "start time " << startYear << startMonth << startDay << startHour<<startMinute<< std::endl;
    std::cout << "desiredTripTime " << desiredTripTime <<  std::endl;
    std::cout << "maxTripTime " << maxTripTime << std::endl;
    std::cout << "minimumTrainHopTime " << minimumTrainHopTime << std::endl;
    std::cout << "citysize " << citysize << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    if (startStation.empty() || endStation.empty()) {
        printHelp();
        return 1;
    }

    pathfinder p;
    p.loadStations("stops.txt");
    p.loadConnections("stop_times.txt");
    p.loadCitysizes("cities.txt");

    auto stations = p.getStations();
    std::ofstream ofs("sta.txt");
    for (const auto& station : stations) {
        ofs << station << std::endl;
    }
    ofs.close();

    p.setTravelPoint(startStation, -1, true, 0, false, true, false);

    p.setTravelPoint(endStation, -1, true, 0, false, false, true);

    p.setStartTime(startMinute, startHour, startDay, startMonth, startYear);
    p.setDesiredTripTime(desiredTripTime);
    p.setMinimumTrainHopTime(minimumTrainHopTime);
    p.setMaxTripTime(maxTripTime);

    p.calculate();


    if (!outputFile.empty()) {
        p.saveToFile(outputFile);
    }

    return 0;
}