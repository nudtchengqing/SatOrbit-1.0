#include "CoordCovert.h"
#include "math.h"



void CartesianToGeodetic (PCRDGEODETIC pcg, PCRDCARTESIAN pcc,
double dSemiMajorAxis, double dFlattening)
{
	double e2;//第一偏心率的平方
	e2=2*dFlattening-dFlattening*dFlattening;

	pcg->longitude=atan2(pcc->y,pcc->x);
	double W,N,N1=0,B,B1;
	B1=atan2(pcc->z,sqrt(pcc->x*pcc->x+pcc->y*pcc->y));
	while(1)
	{   
		W=sqrt(1-e2*sin(B1)*sin(B1));
		N1=dSemiMajorAxis/W;
		B=atan2((pcc->z+N1*e2*sin(B1)),sqrt(pcc->x*pcc->x+pcc->y*pcc->y));

			if(fabs(B-B1)<delta)
				break;
			else
				B1=B;
	}

	pcg->latitude=B;
	N=dSemiMajorAxis/sqrt(1-e2*sin(pcg->latitude)*sin(pcg->latitude));
	pcg->height=sqrt(pcc->x*pcc->x+pcc->y*pcc->y)/cos(B)-N;
    

}
/*
void CartesianToGeodetic (PCRDGEODETIC pcg, PCRDCARTESIAN pcc, 
						  double dSemiMajorAxis, double dFlattening) 
{
	double N;
	double a;
	double b;
	double e2;
	double FAI;
	double B;
	double W;
	double SIN_B;
	double R;
    
	a = dSemiMajorAxis;
	b = a - a*dFlattening;
	e2 = (a*a - b*b) / (a*a);
	FAI = atan (pcc->z / sqrt(pcc->x*pcc->x + pcc->y*pcc->y));
	R = sqrt (pcc->x*pcc->x + pcc->y*pcc->y + pcc->z*pcc->z); 
	pcg->longitude = atan2 (pcc->y, pcc->x);
	//pcg->latitude = asin (pcc->z / a);
	pcg->latitude = FAI;

	do 
	{
		B	 = pcg->latitude;
		SIN_B = sin (B);
		W  = sqrt (1 - e2*SIN_B*SIN_B);
		N  = a / W;
		pcg->latitude 
		= atan (tan (FAI) * (1.0 + a * e2 / pcc->z * SIN_B / W));
	} while (fabs(B - pcg->latitude) > 1.0e-15);

    pcg->height = R * cos (FAI) / cos (pcg->latitude) - N;
}*/

//由大地坐标转换为笛卡尔坐标
void GeodeticToCartesian (PCRDCARTESIAN pcc, PCRDGEODETIC pcg,
double dSemiMajorAxis, double dFlattening)
{   
	double e2;//第一偏心率的平方
	double N;//卯酉圈半径
	e2=2*dFlattening-dFlattening*dFlattening;
	N=dSemiMajorAxis/sqrt(1-e2*sin(pcg->latitude)*sin(pcg->latitude));

	pcc->x=(N+pcg->height)*cos(pcg->latitude)*cos(pcg->longitude);
	pcc->y=(N+pcg->height)*cos(pcg->latitude)*sin(pcg->longitude);
	pcc->z=(N*(1-e2)+pcg->height)*sin(pcg->latitude);

}

//由笛卡尔坐标转换为站心地平坐标
void CartesianToTopocentric (PCRDTOPOCENTRIC pct,
PCRDCARTESIAN pcc,
PCRDCARTESIAN pccCenter,
double dSemiMajorAxis,
double dFlattening)
{
	double dx,dy,dz;
	dx=pcc->x-pccCenter->x;
	dy=pcc->y-pccCenter->y;
	dz=pcc->z-pccCenter->z;

	PCRDGEODETIC pd;
	pd=(PCRDGEODETIC)malloc(sizeof(CRDGEODETIC));

    CartesianToGeodetic (pd,pccCenter,dSemiMajorAxis,dFlattening);

	pct->northing=-sin(pd->latitude)*cos(pd->longitude)*dx
		-sin(pd->latitude)*sin(pd->longitude)*dy
		+cos(pd->latitude)*dz;
	pct->easting=-sin(pd->longitude)*dx
		+cos(pd->longitude)*dy;
	pct->upping=cos(pd->latitude)*cos(pd->longitude)*dx
		+cos(pd->latitude)*sin(pd->longitude)*dy
		+sin(pd->latitude)*dz;
	free(pd);

}

//由站心地平直角坐标转换为站心地平极坐标
void TopocentricToTopocentricPolar (PCRDTOPOCENTRICPOLAR pctp,
PCRDTOPOCENTRIC pct)
{   

	pctp->range=sqrt(pct->northing*pct->northing+pct->easting*pct->easting+pct->upping*pct->upping);
	pctp->azimuth=atan(pct->easting/pct->northing);
	pctp->elevation=asin(pct->upping/pctp->range);


}

//由站心地平极坐标转换为站心地平直角坐标
void TopocentricPolarToTopocentric (PCRDTOPOCENTRIC pct,
PCRDTOPOCENTRICPOLAR pctp)
{
	pct->northing=pctp->range*cos(pctp->elevation)*cos(pctp->azimuth);
	pct->easting=pctp->range*cos(pctp->elevation)*sin(pctp->azimuth);
	pct->upping=pctp->range*sin(pctp->elevation);

}
