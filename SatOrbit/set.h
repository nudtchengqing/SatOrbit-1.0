#ifndef _SET_H
#define _SET_H
#include "cJulian.h"
#include <string>
using namespace std;
//#include "stdafx.h"

const double RE=6378.135;//地球半径(km)

typedef struct tagTLE{
	string name;
	string line1;
	string line2;
}TLE;//卫星两行星历

typedef TLE *PTLE;
typedef struct tagPoint2D{
	double x;
	double y;
}Point2D;// 2维点

typedef Point2D *PPoint2D;

typedef struct tagPoint3D{
	double x;
	double y;
	double z;
}Point3D;//三维点

typedef Point3D *PPoint3D;

typedef struct tagCommonTime{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	double second;
}CommonTime;//时间

typedef CommonTime *PCommonTime;

typedef struct tagControlParameter{

	CommonTime m_startCommonTime;//模拟的开始时间
	CommonTime m_endCommonTime;//模拟的结束时间
	double m_spanTime;//模拟的步长		
	int   m_satState; //显示卫星的状态 0实时  1模拟
	int   m_timeState;//时间形式   0北京时间  1UTC
	int   m_simuPlayVIndex;
	double m_realTimeLong;
	int    m_realSatPos;

	int m_gridShow;//背景的经纬度格网线是否显示 0不显示 1显示
	int m_satShow; //卫星名是否显示  0不显示  1显示
	int m_sunShow;//太阳是否显示   0不显示  1显示
	int m_satPointOrbitShow;//卫星的星下点轨迹在二维投影图上是否显示 0不显示 1显示
	double m_minAngle;//最小仰角值 （控制卫星的可覆盖区域）
	int m_satCoverShow;//卫星的覆盖是否显示 0不显示  1显示

	//int m_mapShowIndex;//背景图的索引
	int m_satCoverColorIndex;//卫星覆盖颜色的索引
///////////////////////////////////////////////////////
	//3D
	int m_map3dIndex;//纹理图索引
	int m_coord3dShow;//坐标系显示
	int m_orbitColorIndex;//轨道颜色索引
	int m_coverColorIndex;//覆盖颜色
	int m_orbit3dShow;//三维轨道显示
	int m_cover3dShow;//覆盖显示
}ControlParameter;



CommonTime cJulian2CommonTime(cJulian m_julian);
CommonTime CommonTimeAddMinute(CommonTime m_time,double min);
int CommonTimeCompare(CommonTime m_time1,CommonTime m_time2);//返回0m_time1>2,返回1m_time1<2
cJulian cJulianNormal(CommonTime m_time,int m_satState/*卫星状态*/,int m_timeState/*时间状态*/);//时间的标准化

#endif