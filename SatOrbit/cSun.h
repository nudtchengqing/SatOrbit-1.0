#ifndef _SUN_H_
#define _SUN_H_

#include "cVector3d.h"
#include "cJulian.h"
class cSun
{
public:
    cJulian sun_j;
public:
	cSun(const cJulian &m_j):sun_j(m_j)
	{};
	/*cSun(int year,int month,int day,int hour,int min,double sec)
	{
		sun_j(year,month,day,hour,min,sec);
	};*/
	~cSun()
	{};
	void setJulian(cJulian set_j){sun_j=set_j;};
	void getComTime(cJulian m_j,/*in*/int *year,int *month,int *day,int *hour,int *min,double *sec);
	double getJulianDate();
	cJulian getJulian(){return sun_j;};
    cVector3d GetSunPos();//����γ�����յؾ���

	double getRiJiao(double lon);
	//lonΪ���� latΪά�ȣ�����ֵ��
	   double getChiWeiJiao(double lon);
	
    double getShiCha(double lon);
	
    double getShiJiao(double lon);

	double getHighAngle(double lon,double lat);//�õ���ʱ������̫���߶Ƚ� ���ؽǶ�ֵ
};
#endif