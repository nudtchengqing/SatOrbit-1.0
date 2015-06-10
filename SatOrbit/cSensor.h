#ifndef _SENSOR_H_
#define _SENSOR_H_
#include "cSatObject.h"
#include "cCover.h"

class cSensor:public cSatObject
{
public:
	double sen_Q;//侧视角  弧度
	double sen_O;//视场角  弧度
	int    sen_n;// 采样数 
public:
	cSensor(cSatOrbitKEY &m_sat,cJulian &m_j,
		double m_Q,double m_O,int m_n):
	   cSatObject(m_sat,m_j)
	   {
		   sen_Q=m_Q;
		   sen_O=m_O;
		   sen_n=m_n;
	    };
	~cSensor()
	{};
	void setQ(double Q){sen_Q=Q;};
	void setO(double O){sen_O=O;};
	void setn(int n){sen_n=n;};
	int  getn(){return sen_n;};

	double getGeocentricAngle();//得到覆盖地心角
	vector<Point2D> getAcessArea();//得到寻访区

};

#endif