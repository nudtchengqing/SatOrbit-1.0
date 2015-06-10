#ifndef _SENSOR_H_
#define _SENSOR_H_
#include "cSatObject.h"
#include "cCover.h"

class cSensor:public cSatObject
{
public:
	double sen_Q;//���ӽ�  ����
	double sen_O;//�ӳ���  ����
	int    sen_n;// ������ 
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

	double getGeocentricAngle();//�õ����ǵ��Ľ�
	vector<Point2D> getAcessArea();//�õ�Ѱ����

};

#endif