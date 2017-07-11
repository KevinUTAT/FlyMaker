// FlyMaker.cpp : Defines the entry point for the console application.
// Author: Kevin Xu
// Revision: V0.23
// Copyright 2017, Drone Delivery Canada Inc. All rights reserved.


// *************************************************************************
// This program's main purpose is to generate .fly file automatically
// Program take in an .kml file from usually Google Earth, this program
// will generate a .fly file that can be loaded into MicroPilot Autopilot
// *************************************************************************

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <fstream>
#include "wpt.h"
#include "tinyxml2/tinyxml2.h"


using namespace std;


int main(int argc, char **argv)
{
	tinyxml2::XMLDocument doc;
	char* fileDir;
	if (argc == 2) {
		cout << "loading kml file from: " << argv[1] << endl;
		//loading the kml file
		if (doc.LoadFile(argv[1])) {
			cout << "kml file loading error" << endl;
			getchar();
			return 0;
		}
	}
	else {
		cout << "FlyMaker V0.23 instruction:" << endl;
		cout << "    1. Make sure that \"footer.txt\" is in the same folder as this .exe"
			<< endl;
		cout << "    2. Close this window and drag and drop your .kml file onto the FlyMaker.exe"
			<< endl;
		cout << "    3. A .fly file will be created in the same folder as your .kml file"
			<< endl;
		getchar();
		return 0;
	}

	// this is a list of way point that define a path
	vector <wpt> path;

	// bring the iterator to our level of interest
	tinyxml2::XMLElement *current = doc.FirstChildElement("kml")
		->FirstChildElement("Document")
		->FirstChildElement("Placemark");

	while (current != nullptr)
	{
		tinyxml2::XMLElement *newPt = current->FirstChildElement("Point");

		//error handling plus exit condition
		if (newPt == nullptr) {
			//cout << "No Data found" << endl;
			break;
		}

		newPt = newPt->FirstChildElement("coordinates");

		//error handling plus exit condition
		if (newPt == nullptr) {
			//cout << "No Data found" << endl;
			break;
		}

		const char* coorStr = newPt->GetText();   //extract the text of coordinate
		cout << "Reading Way point: " << coorStr << endl;

		//parcel the coordinate
		string newLat;
		string newLon;
		string newAlt;

		stringstream coorSS(coorStr);
		getline(coorSS, newLat, ',');
		getline(coorSS, newLon, ',');
		getline(coorSS, newAlt, ',');

		// write into the path
		wpt newWpt(stod(newLat), stod(newLon), stod(newAlt));
		path.push_back(newWpt);

		current = current->NextSiblingElement("Placemark");
	}

	// Now we write a .fly file
	string flyName;
	flyName.append(argv[1]);
	flyName.erase(flyName.length() - 3);
	flyName.append("fly");
	ofstream outfly(flyName, std::ofstream::out);

	// here is the header
	outfly << "// This .fly file is generated automatically by FlyMaker V0.23"
		<< endl;
	outfly << "// Copyright 2017, Drone Delivery Canada Inc. All rights reserved."
		<< endl;

	outfly << endl;
	outfly << endl;

	// take off procedural 
	outfly << "metric" << endl;
	outfly << "[recordHistory] = 50" << endl;
	outfly << "[disableGcsCmds] = 0xFFFFFFFF" << endl;
	outfly << "takeoff" << endl;
	outfly << "climb 30" << endl;
	outfly << "[disableGcsCmds] = 0" << endl;
	outfly << "waitClimb 30" << endl;
	outfly << "hoverAt (0, 0)" << endl;

	// start way pints
	for (unsigned i = 0; i < path.size(); ++i) {
		outfly << "climb " << path[i].Alt() << endl;

		char EW = 'E';
		char NS = 'N';
		if (path[i].Lat() < 0) EW = 'W';
		if (path[i].Lon() < 0) NS = 'S';
		outfly << "flyTo " << '(' << abs(path[i].Lat()) << EW
			<< ", "
			<< abs(path[i].Lon()) << NS << ')' << endl;
	}

	// land
	char EW = 'E';
	char NS = 'N';
	if ((path.end()-1)->Lat() < 0) EW = 'W';
	if ((path.end()-1)->Lon() < 0) NS = 'S';
	outfly << "circuit " << '(' << abs((path.end()-1)->Lat()) << EW
		<< ", "
		<< abs((path.end()-1)->Lon()) << NS << ')'
		<< ", 0, 0" << endl;
	outfly << "repeat -1" << '\n' << '\n' << endl;


	//Footer, all default platens
	string footerPath;
	footerPath.append(argv[0]);
	footerPath.erase(footerPath.length() - 12);
	footerPath.append("footer.txt");
	ifstream footerFS(footerPath);
	if (!footerFS) {
		cout << "Error: footer.txt file missing" << endl;
		outfly << "Error: footer.txt file missing" << endl;
		getchar();
		return 0;
	}

	string footerStr((std::istreambuf_iterator<char>(footerFS)),
		std::istreambuf_iterator<char>());

	outfly << footerStr << endl;

	cout << "Fly file generating successfully" << endl;
	cout << "ENTER to exit" << endl;

	getchar();
	return 0;
}