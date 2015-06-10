#include "cSatTrack.h"

vector<Point2D> cSatTrack::getTrack2D()
{ /*
	vector<Point2D> result;
	Point2D  tr_point;
	cTle m_tle(tr_satkey.getTLE().name,
		tr_satkey.getTLE().line1,
		tr_satkey.getTLE().line2);
	cOrbit m_orbit(m_tle);
	cEci tr_eci;
	double starttime=getStartJulian();
	double endtime=getEndJulian();
	double realtime=starttime;
	double epochtime=m_orbit.Epoch().getDate();

	while (realtime<=endtime)
	{
		m_orbit.getPosition((realtime-epochtime)*24*60,&tr_eci);
		tr_point.x=tr_eci.toGeo().m_Lon*180/PI;
		tr_point.y=tr_eci.toGeo().m_Lat*180/PI;
		result.push_back(tr_point);
		realtime+=(getSpanTime()/(24*60));
	}
	return result;
	*/
	cSatObject tr_satObject(tr_satkey,tr_startTime);
	cJulian m_start=getStartJulian();
	cJulian m_end=getEndJulian();
	vector<Point2D> result;
	Point2D  tr_point;
	while(m_start.getDate()<m_end.getDate())
	{
		tr_point.x=tr_satObject.getSatPosGEO().getPoint3D().x;
		tr_point.y=tr_satObject.getSatPosGEO().getPoint3D().y;

		result.push_back(tr_point);

		m_start.addMin(getSpanTime());
		tr_satObject.setJulian(m_start);
	}

	return result;


}

vector<Point2D> cSatTrack::getTrack2dGrid(double width,double height)
{
	vector<Point2D> geoTrack=getTrack2D();
	vector<Point2D> gridTrack;
	int geoSize=int(geoTrack.size());
	Point2D front,back;
	Point2D mid;
	Point2D a1;
	for(int i=0;i<geoSize-1;i++)
	{
		front=GeoToGrid(width,height,geoTrack[i]);
		back=GeoToGrid(width,height,geoTrack[i+1]);
		//gridTrack.push_back(GeoToGrid(width,height,geoTrack[i]));
		gridTrack.push_back(front);
		if(sqrt(pow(front.x-back.x,2)+pow(front.y-back.y,2))>3*width/4)
		{
			if(front.x<width/2&&back.x>width/2)
			{
				if(i>0)
				{   mid=GeoToGrid(width,height,geoTrack[i-1]);
					a1.x=0;
				    a1.y=-front.x*(front.y-mid.y)/(front.x-mid.x)+front.y;
					gridTrack.push_back(a1);
                }
				if(i<geoSize-2)
                {   mid=GeoToGrid(width,height,geoTrack[i+2]);
					a1.x=width;
				    a1.y= (width-back.x)*(back.y-mid.y)/(back.x-mid.x)+back.y;
					gridTrack.push_back(a1);
				}
			}
			if(front.x>width/2&&back.x<width/2)
			{
				if(i>0)
				{
					mid=GeoToGrid(width,height,geoTrack[i-1]);
					a1.x=width;
					a1.y=(width-front.x)*(front.y-mid.y)/(front.x-mid.x)+front.y;
					gridTrack.push_back(a1);
                }
				if(i<geoSize-2)
				{
					mid=GeoToGrid(width,height,geoTrack[i+2]);
					a1.x=0;
					a1.y=-back.x*(back.y-mid.y)/(back.x-mid.x)+back.y;
					gridTrack.push_back(a1);
				}

			}
		}
	}
	return gridTrack;

}

vector<Point3D> cSatTrack::getTrack3D()
{
	cSatObject tr_satObject(tr_satkey,tr_startTime);
	cJulian m_start=getStartJulian();
	cJulian m_end=getEndJulian();
	vector<Point3D> result;
	Point3D  tr_point;
	while(m_start.getDate()<m_end.getDate())
	{
		tr_point.x=tr_satObject.getSatPosECI().getPoint3D().x;
		tr_point.y=tr_satObject.getSatPosECI().getPoint3D().y;
		tr_point.z=tr_satObject.getSatPosECI().getPoint3D().z;

		result.push_back(tr_point);

		m_start.addMin(getSpanTime());
		tr_satObject.setJulian(m_start);
	}

	return result;
  
}