#include "wpt.h"
#include <string>

using namespace std;

wpt::wpt() {}

wpt::wpt(double lat_, double lon_, double alt_) :
	lat(lat_), lon(lon_), alt(alt_) {}


void wpt::rename(string name_) {
	wptName = name_;
	return;
}


double wpt::Lat() {
	return lat;
}


double wpt::Lon() {
	return lon;
}


double wpt::Alt() {
	return alt;
}


std::ostream& operator<< (std::ostream& stream, const wpt& waypoint) {
	//wpt with no name
	if (waypoint.wptName.compare("<un-named>") == 0) {
		stream << "Waypoint: " << waypoint.lat << ", " << waypoint.lon 
			<< " @" << waypoint.alt << endl;
	}
	//wpt with name defined
	else {
		stream << "Waypoint: " << waypoint.wptName << waypoint.lat << ", " 
			<< waypoint.lon << " @" << waypoint.alt << endl;
	}
	return stream;
}