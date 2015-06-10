#ifndef _SATPOINTCOVER_H_
#define _SATPOINTCOVER_H_
#include "cSatObject.h"
#include "cCover.h"


class cSatPointCover:public cSatObject
{
public:
	double caft_angle;//航天器高度角（度）
	int n;//采样点数
public:
	cSatPointCover(cSatOrbitKEY &satorbit,cJulian &m_j,double m_angle,int m_n):
	  cSatObject(satorbit,m_j)
	  {
		  caft_angle=m_angle;
		  n=m_n;
	  }
	~cSatPointCover()
	 {
	 };

	void setAngle(double angle){caft_angle=angle;};
	double getAngle(){return caft_angle;};
	double getSatPointAngle();//得到星下点角(弧度)
	void setn(int nn){n=nn;};
	int getn(){return n;};
	vector<Point2D> getValidCover();
};

#endif