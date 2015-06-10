#include "cVector3d.h"

cVector3d::cVector3d(const cVector3d &m_vector3d)
{
	vector3d=m_vector3d.vector3d;
}

cVector3d::cVector3d(double m_x, double m_y, double m_z)
{
	vector3d.x=m_x;
	vector3d.y=m_y;
	vector3d.z=m_z;
}

void cVector3d::setPoint3D(Point3D m_3d)
{
	vector3d=m_3d;
}

Point3D cVector3d::getPoint3D()
{
	return vector3d;
}

void cVector3d::MulScale(double m_scale)
{
	vector3d.x*=m_scale;
	vector3d.y*=m_scale;
	vector3d.z*=m_scale;
}

double cVector3d::getModVector()
{
	return sqrt(vector3d.x*vector3d.x+
		vector3d.y*vector3d.y+
		vector3d.z*vector3d.z);
}

void cVector3d::getCirAngleVector3D(cVector3d r,
									cVector3d h,
									double angle)
{
	double rmod=sqrt(h.getPoint3D().x*h.getPoint3D().x
		            +h.getPoint3D().y*h.getPoint3D().y
					+h.getPoint3D().z*h.getPoint3D().z);
	h.MulScale(1/rmod);
	//h=getScaleVector3d(h,1/rmod);
	vector3d.x=r.getPoint3D().x*(h.getPoint3D().x*h.getPoint3D().x*(1-cos(angle))+cos(angle))+
		r.getPoint3D().y*(h.getPoint3D().x*h.getPoint3D().y*(1-cos(angle))-h.getPoint3D().z*sin(angle))+
		r.getPoint3D().z*(h.getPoint3D().x*h.getPoint3D().z*(1-cos(angle))+h.getPoint3D().y*sin(angle));
	vector3d.y=r.getPoint3D().x*(h.getPoint3D().x*h.getPoint3D().y*(1-cos(angle))+h.getPoint3D().z*sin(angle))+
		r.getPoint3D().y*(h.getPoint3D().y*h.getPoint3D().y*(1-cos(angle))+cos(angle))+
		r.getPoint3D().z*(h.getPoint3D().y*h.getPoint3D().z*(1-cos(angle))-h.getPoint3D().x*sin(angle));
	vector3d.z=r.getPoint3D().x*(h.getPoint3D().x*h.getPoint3D().z*(1-cos(angle))-h.getPoint3D().y*sin(angle))+
		r.getPoint3D().y*(h.getPoint3D().y*h.getPoint3D().z*(1-cos(angle))+h.getPoint3D().x*sin(angle))+
		r.getPoint3D().z*(h.getPoint3D().z*h.getPoint3D().z*(1-cos(angle))+cos(angle));

}
