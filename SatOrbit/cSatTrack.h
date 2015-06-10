#ifndef _SATTRACK_H_
#define _SATTRACK_H_
#include "cSatOrbitKEY.h"
#include "cSatObject.h"

class cSatTrack
{
public:
	cSatOrbitKEY tr_satkey;
	cJulian      tr_startTime;
	cJulian      tr_endTime;
	double       tr_span;//分（minute）为单位
public:
	cSatTrack(cSatOrbitKEY &m_satkey,cJulian &m_startTime,cJulian &m_endTime,double m_span):
	  tr_satkey(m_satkey),tr_startTime(m_startTime),tr_endTime(m_endTime),tr_span(m_span)
	  {
	  };
     ~cSatTrack()
	 {};

	 void setStartTime(cJulian set_startTime)
	 {
		 tr_startTime=set_startTime;
	 };
	 void setEndTime(cJulian set_endTime)
	 {
		 tr_endTime=set_endTime;
	 };
	 void setSpanTime(double set_span)
	 {
		 tr_span=set_span;
	 };
	 cJulian getStartJulian()
	 {
		 return tr_startTime;//.getDate();//开始时间的儒略日
	 };
	 cJulian getEndJulian()
	 {
		 return tr_endTime;//.getDate();
	 };
	 double getSpanTime()
	 {
		 return tr_span;
	 };

	 vector<Point2D> getTrack2D();//经纬度
	 vector<Point2D> getTrack2dGrid(double width,double height);//网格坐标
	 vector<Point3D> getTrack3D();//ECI坐标
	 

};
#endif