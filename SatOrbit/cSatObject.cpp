#include "cSatObject.h"
#include "cSun.h"
cSatObject::cSatObject(cSatOrbitKEY &m_satorbitkey, cJulian &m_julian):
   satorbitkey(m_satorbitkey),julian(m_julian)
   {
	   Initialize();
   }

cVector3d cSatObject::getSatPosECI()
   {
	   cVector3d sat_pos(getEci().getPos().m_x,
		                 getEci().getPos().m_y,
						 getEci().getPos().m_z);
	   return sat_pos;
     }

cVector3d cSatObject::getSatPosGEO()
{   
	double x,y;
	x=rad2deg(getEci().toGeo().m_Lon);
	y=rad2deg(getEci().toGeo().m_Lat);
	if(x>180)
		x-=360;

	cVector3d sat_geo(x,
					   y,
					   getEci().toGeo().m_Alt);
 return sat_geo;

}

cVector3d cSatObject::getSatVelECI()
{
	cVector3d sat_vel(getEci().getVel().m_x,
					  getEci().getVel().m_y,
					  getEci().getVel().m_z);
	return sat_vel;

}

cVector3d cSatObject::gethVector()
{
	double x=getSatPosECI().getPoint3D().y*getSatVelECI().getPoint3D().z-
		     getSatPosECI().getPoint3D().z*getSatVelECI().getPoint3D().y;
	double y=getSatPosECI().getPoint3D().z*getSatVelECI().getPoint3D().x-
		     getSatPosECI().getPoint3D().x*getSatVelECI().getPoint3D().z;
	double z=getSatPosECI().getPoint3D().x*getSatVelECI().getPoint3D().y-
		     getSatPosECI().getPoint3D().y*getSatVelECI().getPoint3D().x;
	cVector3d h(x,y,z);
	return h;

}
void cSatObject::Initialize()
   {
	   cTle c_tle(satorbitkey.getTLE().name,
		          satorbitkey.getTLE().line1,
		          satorbitkey.getTLE().line2);
	   cOrbit m_orbit(c_tle);
	   double m_spantime=julian.getDate()-m_orbit.Epoch().getDate();
	   m_orbit.getPosition(m_spantime*24*60,&eci);
	   

/*
	   sat_pos.vector3d.x=getEci().getPos().m_x;
	   sat_pos.vector3d.y=getEci().getPos().m_y;
	   sat_pos.vector3d.z=getEci().getPos().m_z;

	   sat_vel.vector3d.x=getEci().getVel().m_x;
	   sat_vel.vector3d.y=getEci().getVel().m_y;
	   sat_vel.vector3d.z=getEci().getVel().m_z;*/

   }

double cSatObject::getSolarElevAngle()
{
    cSun sat_sun(julian);
	double sat_chiwei=sat_sun.getChiWeiJiao(getSatPosGEO().getPoint3D().x);
	double sat_weidu=getSatPosGEO().getPoint3D().y;

	if(sat_weidu>0)
		return PI/2-(sat_weidu-sat_chiwei*PI/180);
	else
		return PI/2-(sat_chiwei*PI/180-sat_weidu);

}