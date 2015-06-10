#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include "math.h"
#include "set.h"

class cVector3d
{
public:
	Point3D vector3d;
public:
	cVector3d(const cVector3d &m_vector3d);
	cVector3d(double m_x,double m_y,double m_z);
	cVector3d(Point3D m_Point3D):vector3d(m_Point3D)
	{};
	~cVector3d()
	{
	};

	void setPoint3D(Point3D m_3d);
	Point3D getPoint3D();

	void MulScale(double m_scale);
	double getModVector();

	void getCirAngleVector3D(cVector3d r,
		                     cVector3d h,
							 double angle/*弧度*/);//向量r绕向量h旋转角度angle后的向量


};
#endif