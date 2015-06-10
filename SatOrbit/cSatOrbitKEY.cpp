#include "cSatOrbitKEY.h"
#include "cTle.h"
#include "stdafx.h"

cSatOrbitKEY::cSatOrbitKEY(const TLE &m_tle)
{
	tle=m_tle;
    Initialial();

}
cSatOrbitKEY::cSatOrbitKEY(string &strname,string &str1,string &str2)
{
	tle.name=strname;
	tle.line1=str1;
	tle.line2=str2;
	Initialial();
}


void cSatOrbitKEY::Initialial()
{
	assert(!tle.name.empty());
    assert(!tle.line1.empty());
    assert(!tle.line2.empty());

	cTle c_tle(tle.name,tle.line1,tle.line2);
	cOrbit m_orbit(c_tle);
    
	m_Inclination=m_orbit.Inclination();
	m_Eccentricity=m_orbit.Eccentricity();
	m_RAAN=m_orbit.RAAN();
	m_ArgPerigee=m_orbit.ArgPerigee();
	m_mnAnomaly=m_orbit.mnAnomaly();
	m_SemiMajor=m_orbit.SemiMajor();
	//m_Cyc=1.0/m_orbit.mnMotion();
	
	
}

int cSatOrbitKEY::getLaunchYear()
{
	string syear=tle.line1.substr(9,2);
       char *cyear = (char *)syear.c_str();  
       int year=atoi(cyear);
       if(year>57)    //判断具体年份
	      year+=1900;
	   else
		   year+=2000;
    return year;
}