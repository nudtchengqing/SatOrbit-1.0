#include "set.h"

CommonTime cJulian2CommonTime(cJulian m_julian)
{   
	CommonTime  m_commonTime;
	int sec;
    double pm;
	m_julian.getComponent(&m_commonTime.year,&m_commonTime.month,&pm);
	m_commonTime.day=int(pm);
	sec=(int)((pm-m_commonTime.day)*24*60*60+0.5);
	m_commonTime.hour=sec/3600;
	m_commonTime.minute=(sec%3600)/60;
	m_commonTime.second=(sec%3600)%60;
	return m_commonTime;
}

CommonTime CommonTimeAddMinute(CommonTime m_time,double min)
{
	cJulian m_(m_time.year,m_time.month,m_time.day,
			   m_time.hour,m_time.minute,m_time.second);
	m_.addMin(min);
    return cJulian2CommonTime(m_);
}
//如果m_time1>m_time2 返回0
//如果m_time1<m_time2 返回1
int CommonTimeCompare(CommonTime m_time1,CommonTime m_time2)
{
	cJulian m1(m_time1.year,m_time1.month,m_time1.day,
			   m_time1.hour,m_time1.minute,m_time1.second);
	cJulian m2(m_time2.year,m_time2.month,m_time2.day,
			   m_time2.hour,m_time2.minute,m_time2.second);
	if(m1.getDate()>=m2.getDate())
		return 0;
	else
		return 1;
}
//时间的标准化,把通用时标准到UTC时间的儒略时,作为标准时间接口
//m_satState=0实时状态 m_satState=1模拟状态
//m_timeState=0北京时间 m_timeState=1UTC
cJulian cJulianNormal(CommonTime m_time,int m_satState/*卫星状态*/,int m_timeState/*时间状态*/)
{
	   cJulian m_real(m_time.year,m_time.month,m_time.day,
		           m_time.hour,m_time.minute,m_time.second);
	   if(m_satState==0)//实时
	      m_real.addHour(-8);//由于实时状态下系统是北京时间
	   else//模拟
	   {
			if(m_timeState==0)
			{
			  m_real.addHour(-8);
			}
	   }
  return m_real;
}