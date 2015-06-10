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
	double       tr_span;//�֣�minute��Ϊ��λ
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
		 return tr_startTime;//.getDate();//��ʼʱ���������
	 };
	 cJulian getEndJulian()
	 {
		 return tr_endTime;//.getDate();
	 };
	 double getSpanTime()
	 {
		 return tr_span;
	 };

	 vector<Point2D> getTrack2D();//��γ��
	 vector<Point2D> getTrack2dGrid(double width,double height);//��������
	 vector<Point3D> getTrack3D();//ECI����
	 

};
#endif