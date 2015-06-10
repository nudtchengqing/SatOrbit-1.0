#include "cSatPointCover.h"

double cSatPointCover::getSatPointAngle()
{
	cVector3d r=getSatPosECI();
	double h=sqrt(r.getPoint3D().x*r.getPoint3D().x
		          +r.getPoint3D().y*r.getPoint3D().y
				  +r.getPoint3D().z*r.getPoint3D().z);
	return asin(cos(deg2rad(getAngle()))*(RE/h));


}

vector<Point2D> cSatPointCover::getValidCover()
{
	cVector3d r=getSatPosECI();
	cVector3d v=getSatVelECI();
	cVector3d h=gethVector();
    
	vector<Point2D> result;
	double m_GMST=getGMST();

	double B;
	B=acos(RE*cos(deg2rad(getAngle()))/(RE+getSatPosGEO().getPoint3D().z))-deg2rad(getAngle());

	cCover sat_cover(r,h,B,getn(),m_GMST);
	result=sat_cover.getCover();
	return result;

}