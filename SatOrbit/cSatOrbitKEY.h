#ifndef _SATORBITKEY_H_
#define _SATORBITKEY_H_
#include "PlatProjection.h"
#include "cOrbit.h"

class cOrbit;

class cSatOrbitKEY
{
public:
	TLE tle;
public:
	cSatOrbitKEY(const TLE &m_tle);
	cSatOrbitKEY(string &strname,string &str1,string &str2);
	~cSatOrbitKEY()
	{
		//delete m_orbit;
	};
	TLE getTLE(){return tle;};
	double getInclination(){return m_Inclination;};//得到轨道交角
	double getEccentricity(){return m_Eccentricity;};//得到轨道偏心率
	double getRAAN(){return m_RAAN;};//得到升交点赤经
	double getArgPerigee(){return m_ArgPerigee;};//得到近地点角距
	double getmnAnomaly(){return m_mnAnomaly;};//得到平近点角
	double getSemiMajor(){return m_SemiMajor;};//得到长半径
	//cOrbit *getOrbit(){return m_orbit;};
	int    getLaunchYear();
	//double getCyc(){return m_Cyc;};//得到卫星运动的周期

public:
	double m_Inclination;
	double m_Eccentricity;
	double m_RAAN;
	double m_ArgPerigee;
	double m_mnAnomaly;
	double m_SemiMajor;
	//double m_Cyc;//周期 天/圈
   // cOrbit m_orbit;
	void Initialial();
};
#endif