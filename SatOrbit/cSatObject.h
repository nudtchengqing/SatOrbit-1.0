#ifndef _SATOBJECT_H_
#define _SATOBJECT_H_
#include "cSatOrbitKEY.h"
#include "cEci.h"
#include "cVector3d.h"

class cEci;
class cJulian;

class cSatObject//:public cSatOrbitKEY
{
public:
	cJulian julian;
    cSatOrbitKEY satorbitkey;
public:
	cSatObject(cSatOrbitKEY &m_satorbitkey,cJulian &m_julian);
	~cSatObject()
	{};

	void setJulian(cJulian m_j){julian=m_j;Initialize();};
	double getGMST(){return julian.toGMST();};
	cEci getEci()const{return eci;};
	cVector3d getSatPosECI();
    cVector3d getSatPosGEO();
	cVector3d getSatVelECI();
	cVector3d gethVector();//得到垂直于轨道面的向量 、
	double getSolarElevAngle();//返回弧度

public:
	//cVector3d sat_pos;
	//cVector3d sat_vel;
	cEci eci;

	void Initialize();

};
#endif