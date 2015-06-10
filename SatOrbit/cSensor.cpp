#include "cSensor.h"


double cSensor::getGeocentricAngle()
{
	double B;//»¡¶È
	double h=getSatPosGEO().getPoint3D().z;
	B=asin((RE+h)*sin(sen_Q+sen_O)/RE)-(sen_Q+sen_O);
	return B;
}

vector<Point2D> cSensor::getAcessArea()
{
	cVector3d r=getSatPosECI();
	cVector3d v=getSatVelECI();
	cVector3d h=gethVector();
    
	vector<Point2D> result;
	double m_GMST=getGMST();

	double B=getGeocentricAngle();
	

	cCover sat_cover(r,h,B,getn(),m_GMST);
	result=sat_cover.getCover();
	return result;
}