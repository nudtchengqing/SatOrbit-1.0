#ifndef _COORDCOVERT_H
#define _COORDCOVERT_H

#include "stdlib.h"
#include "globals.h"
//WGS-84椭球体参数
const double a=6378137.0;//长半轴
const double flattening=1/298.257223563;//扁率
const double delta=0.0000001;
typedef struct tagCRDCARTESIAN{
	double x;
	double y;
	double z;
}CRDCARTESIAN;

typedef CRDCARTESIAN *PCRDCARTESIAN;
//笛卡尔坐标系

typedef struct tagCRDGEODETIC{
	double longitude;
	double latitude;
	double height;
}CRDGEODETIC;

typedef CRDGEODETIC *PCRDGEODETIC;
//大地坐标系

typedef struct tagCRDTOPOCENTRIC{
	double northing;
	double easting;
	double upping;
}CRDTOPOCENTRIC;

typedef CRDTOPOCENTRIC *PCRDTOPOCENTRIC;
//站心地平坐标系（线坐标形式）

typedef struct tagCRDTOPOCENTRICPOLAR{
	double range;
	double azimuth;
	double elevation;
}CRDTOPOCENTRICPOLAR;

typedef CRDTOPOCENTRICPOLAR *PCRDTOPOCENTRICPOLAR;
//站心地平坐标系（极坐标形式）

//由笛卡尔坐标转换为大地坐标
void CartesianToGeodetic (PCRDGEODETIC pcg, PCRDCARTESIAN pcc,
double dSemiMajorAxis, double dFlattening);
//pcg：指向所转换出的大地坐标的指针；
//pcc：指向待转换的笛卡尔坐标的指针；
//dSemiMajorAxis：参考椭球的长半轴；
//dFlattening：参考椭球的扁率。

//由大地坐标转换为笛卡尔坐标
void GeodeticToCartesian (PCRDCARTESIAN pcc, PCRDGEODETIC pcg,
double dSemiMajorAxis, double dFlattening);
//pcc：指向所转换出的笛卡尔坐标的指针；
//pcg：指向待转换的大地坐标的指针；
//dSemiMajorAxis：参考椭球的长半轴；
//dFlattening：参考椭球的扁率。

//由笛卡尔坐标转换为站心地平坐标
void CartesianToTopocentric (PCRDTOPOCENTRIC pct,
PCRDCARTESIAN pcc,
PCRDCARTESIAN pccCenter,
double dSemiMajorAxis,
double dFlattening);
//pct：指向所转换出的站心地平坐标的指针；
//pcc：指向待转换的笛卡尔坐标的指针；
//pccCenter：指向站心的笛卡尔坐标的指针；
//dSemiMajorAxis：参考椭球的长半轴；
//dFlattening：参考椭球的扁率。

//由站心地平直角坐标转换为站心地平极坐标
void TopocentricToTopocentricPolar (PCRDTOPOCENTRICPOLAR pctp,
PCRDTOPOCENTRIC pct);
//pctp：指向所转换出的站心地平极坐标的指针；
//pct：指向待转换的站心地平坐标的指针；


//由站心地平极坐标转换为站心地平直角坐标
void TopocentricPolarToTopocentric (PCRDTOPOCENTRIC pct,PCRDTOPOCENTRICPOLAR pctp);
//pct：指向所转换的站心地平坐标的指针；
//pctp：指向待转换的站心地平极坐标的指针；



#endif
