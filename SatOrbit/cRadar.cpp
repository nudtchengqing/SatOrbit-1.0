#include "cRadar.h"

double cRadar::getGeocentricAngle1()
{
	double h=getSatPosGEO().getPoint3D().z;
	double B=asin((RE+h)*sin(radar_I-radar_O)/RE)-(radar_I-radar_O);
	return B;
}

double cRadar::getGeocentricAngle2()
{
	double h=getSatPosGEO().getPoint3D().z;
	double B=asin((RE+h)*sin(radar_I+radar_O)/RE)-(radar_I+radar_O);
	return B;
}

vector<Point2D> cRadar::getAcessArea1()
{
	cVector3d r=getSatPosECI();
	//cVector3d v=getSatVelECI();
	cVector3d h=gethVector();
    
	vector<Point2D> result;
	double m_GMST=getGMST();

	double B=getGeocentricAngle1();
	

	cCover radar_cover(r,h,B,getn(),m_GMST);
	result=radar_cover.getCover();
	return result;
}

vector<Point2D> cRadar::getAcessArea2()
{
	cVector3d r=getSatPosECI();
	//cVector3d v=getSatVelECI();
	cVector3d h=gethVector();
    
	vector<Point2D> result;
	double m_GMST=getGMST();

	double B=getGeocentricAngle2();
	

	cCover radar_cover(r,h,B,getn(),m_GMST);
	result=radar_cover.getCover();
	return result;
}