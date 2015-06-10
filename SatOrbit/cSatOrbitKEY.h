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
	double getInclination(){return m_Inclination;};//�õ��������
	double getEccentricity(){return m_Eccentricity;};//�õ����ƫ����
	double getRAAN(){return m_RAAN;};//�õ�������ྭ
	double getArgPerigee(){return m_ArgPerigee;};//�õ����ص�Ǿ�
	double getmnAnomaly(){return m_mnAnomaly;};//�õ�ƽ�����
	double getSemiMajor(){return m_SemiMajor;};//�õ����뾶
	//cOrbit *getOrbit(){return m_orbit;};
	int    getLaunchYear();
	//double getCyc(){return m_Cyc;};//�õ������˶�������

public:
	double m_Inclination;
	double m_Eccentricity;
	double m_RAAN;
	double m_ArgPerigee;
	double m_mnAnomaly;
	double m_SemiMajor;
	//double m_Cyc;//���� ��/Ȧ
   // cOrbit m_orbit;
	void Initialial();
};
#endif