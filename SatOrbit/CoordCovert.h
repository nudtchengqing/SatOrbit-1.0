#ifndef _COORDCOVERT_H
#define _COORDCOVERT_H

#include "stdlib.h"
#include "globals.h"
//WGS-84���������
const double a=6378137.0;//������
const double flattening=1/298.257223563;//����
const double delta=0.0000001;
typedef struct tagCRDCARTESIAN{
	double x;
	double y;
	double z;
}CRDCARTESIAN;

typedef CRDCARTESIAN *PCRDCARTESIAN;
//�ѿ�������ϵ

typedef struct tagCRDGEODETIC{
	double longitude;
	double latitude;
	double height;
}CRDGEODETIC;

typedef CRDGEODETIC *PCRDGEODETIC;
//�������ϵ

typedef struct tagCRDTOPOCENTRIC{
	double northing;
	double easting;
	double upping;
}CRDTOPOCENTRIC;

typedef CRDTOPOCENTRIC *PCRDTOPOCENTRIC;
//վ�ĵ�ƽ����ϵ����������ʽ��

typedef struct tagCRDTOPOCENTRICPOLAR{
	double range;
	double azimuth;
	double elevation;
}CRDTOPOCENTRICPOLAR;

typedef CRDTOPOCENTRICPOLAR *PCRDTOPOCENTRICPOLAR;
//վ�ĵ�ƽ����ϵ����������ʽ��

//�ɵѿ�������ת��Ϊ�������
void CartesianToGeodetic (PCRDGEODETIC pcg, PCRDCARTESIAN pcc,
double dSemiMajorAxis, double dFlattening);
//pcg��ָ����ת�����Ĵ�������ָ�룻
//pcc��ָ���ת���ĵѿ��������ָ�룻
//dSemiMajorAxis���ο�����ĳ����᣻
//dFlattening���ο�����ı��ʡ�

//�ɴ������ת��Ϊ�ѿ�������
void GeodeticToCartesian (PCRDCARTESIAN pcc, PCRDGEODETIC pcg,
double dSemiMajorAxis, double dFlattening);
//pcc��ָ����ת�����ĵѿ��������ָ�룻
//pcg��ָ���ת���Ĵ�������ָ�룻
//dSemiMajorAxis���ο�����ĳ����᣻
//dFlattening���ο�����ı��ʡ�

//�ɵѿ�������ת��Ϊվ�ĵ�ƽ����
void CartesianToTopocentric (PCRDTOPOCENTRIC pct,
PCRDCARTESIAN pcc,
PCRDCARTESIAN pccCenter,
double dSemiMajorAxis,
double dFlattening);
//pct��ָ����ת������վ�ĵ�ƽ�����ָ�룻
//pcc��ָ���ת���ĵѿ��������ָ�룻
//pccCenter��ָ��վ�ĵĵѿ��������ָ�룻
//dSemiMajorAxis���ο�����ĳ����᣻
//dFlattening���ο�����ı��ʡ�

//��վ�ĵ�ƽֱ������ת��Ϊվ�ĵ�ƽ������
void TopocentricToTopocentricPolar (PCRDTOPOCENTRICPOLAR pctp,
PCRDTOPOCENTRIC pct);
//pctp��ָ����ת������վ�ĵ�ƽ�������ָ�룻
//pct��ָ���ת����վ�ĵ�ƽ�����ָ�룻


//��վ�ĵ�ƽ������ת��Ϊվ�ĵ�ƽֱ������
void TopocentricPolarToTopocentric (PCRDTOPOCENTRIC pct,PCRDTOPOCENTRICPOLAR pctp);
//pct��ָ����ת����վ�ĵ�ƽ�����ָ�룻
//pctp��ָ���ת����վ�ĵ�ƽ�������ָ�룻



#endif
