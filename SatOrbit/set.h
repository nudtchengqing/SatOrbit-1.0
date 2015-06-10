#ifndef _SET_H
#define _SET_H
#include "cJulian.h"
#include <string>
using namespace std;
//#include "stdafx.h"

const double RE=6378.135;//����뾶(km)

typedef struct tagTLE{
	string name;
	string line1;
	string line2;
}TLE;//������������

typedef TLE *PTLE;
typedef struct tagPoint2D{
	double x;
	double y;
}Point2D;// 2ά��

typedef Point2D *PPoint2D;

typedef struct tagPoint3D{
	double x;
	double y;
	double z;
}Point3D;//��ά��

typedef Point3D *PPoint3D;

typedef struct tagCommonTime{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	double second;
}CommonTime;//ʱ��

typedef CommonTime *PCommonTime;

typedef struct tagControlParameter{

	CommonTime m_startCommonTime;//ģ��Ŀ�ʼʱ��
	CommonTime m_endCommonTime;//ģ��Ľ���ʱ��
	double m_spanTime;//ģ��Ĳ���		
	int   m_satState; //��ʾ���ǵ�״̬ 0ʵʱ  1ģ��
	int   m_timeState;//ʱ����ʽ   0����ʱ��  1UTC
	int   m_simuPlayVIndex;
	double m_realTimeLong;
	int    m_realSatPos;

	int m_gridShow;//�����ľ�γ�ȸ������Ƿ���ʾ 0����ʾ 1��ʾ
	int m_satShow; //�������Ƿ���ʾ  0����ʾ  1��ʾ
	int m_sunShow;//̫���Ƿ���ʾ   0����ʾ  1��ʾ
	int m_satPointOrbitShow;//���ǵ����µ�켣�ڶ�άͶӰͼ���Ƿ���ʾ 0����ʾ 1��ʾ
	double m_minAngle;//��С����ֵ ���������ǵĿɸ�������
	int m_satCoverShow;//���ǵĸ����Ƿ���ʾ 0����ʾ  1��ʾ

	//int m_mapShowIndex;//����ͼ������
	int m_satCoverColorIndex;//���Ǹ�����ɫ������
///////////////////////////////////////////////////////
	//3D
	int m_map3dIndex;//����ͼ����
	int m_coord3dShow;//����ϵ��ʾ
	int m_orbitColorIndex;//�����ɫ����
	int m_coverColorIndex;//������ɫ
	int m_orbit3dShow;//��ά�����ʾ
	int m_cover3dShow;//������ʾ
}ControlParameter;



CommonTime cJulian2CommonTime(cJulian m_julian);
CommonTime CommonTimeAddMinute(CommonTime m_time,double min);
int CommonTimeCompare(CommonTime m_time1,CommonTime m_time2);//����0m_time1>2,����1m_time1<2
cJulian cJulianNormal(CommonTime m_time,int m_satState/*����״̬*/,int m_timeState/*ʱ��״̬*/);//ʱ��ı�׼��

#endif