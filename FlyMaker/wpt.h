#pragma once
#include <string>

// *************************************************************
// This is the definition of wpt class witch defined a way point 
// in .fly file
// *************************************************************

class wpt
{
private:
	std::string wptName = "<un-named>"; //define only needed
	double lat;                         //latitude (m)
	double lon;                         //lontitude (m)
	double alt;                         //altitude (m)
public:
	wpt();
	wpt(double lat_, double lon_, double alt_);

	//this will give or change the name of the wpt
	void rename(std::string name_);

	double Lat();
	double Lon();
	double Alt();

	friend std::ostream& operator<< (std::ostream& stream, const wpt& waypoint);
};
