#ifndef _RADAR_H_
#define _RADAR_H_

#include "cSatObject.h"
#include "cCover.h"

class cRadar:public cSatObject
{
public:
	double radar_I;//入射角 弧度
	double radar_O;//视场角
	int radar_n;//采样数
public:
	cRadar(cSatOrbitKEY &r_sat,cJulian &r_j,
		double r_I,double r_O,int r_n):
	   cSatObject(r_sat,r_j)
	   {
		   radar_I=r_I;
		   radar_O=r_O;
		   radar_n=r_n;
	   };
   ~cRadar()
   {
   };

   void setI(double r_i){radar_I=r_i;};
   void setO(double r_o){radar_O=r_o;};
   void setn(int r_n){radar_n=r_n;};
   int getn(){return radar_n;};

   double getGeocentricAngle1();//得到覆盖地心角B1
   double getGeocentricAngle2();//得到覆盖地心角B2

   vector<Point2D> getAcessArea1();//得到寻访边界1
   vector<Point2D> getAcessArea2();//得到寻访边界2
};
#endif