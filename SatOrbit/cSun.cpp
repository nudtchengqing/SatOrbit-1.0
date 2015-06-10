#include "cSun.h"
#include "globals.h"

//#include "cJulian.h"

void cSun::getComTime(cJulian m_j,/*in*/int *year, int *month, int *day, int *hour, int *min, double *sec)
{
	int nyear,nmonth,nday,nhour,nmin,nsecond;
	//double nsec;
	double pdm;
	m_j.getComponent(&nyear,&nmonth,&pdm);
    nday=int(pdm);
    nsecond=(int)((pdm-nday)*24*60*60+0.5);
    nhour=nsecond/3600;
    nmin=(nsecond%3600)/60;
    nsecond=(nsecond%3600)%60;
	*year=nyear;
	*month=nmonth;
	*day=nday;
	*hour=nhour;
	*min=nmin;
	*sec=(double)nsecond;
}
double cSun::getJulianDate()
{
	return sun_j.getDate();
}

cVector3d cSun::GetSunPos()
{
	// int year,month,day,hour,min,sec; 
     double d,w,a,e,M,oblecl,L,E,xe,ye,r,v,lon,x,y,z,xequt,yequt,zequt,dist,RA,Decl; 

////////////////////////////////////////////�������////////////////////////////////////// 
	d=getJulianDate()-2451543.5;//��������գ� 
	w=282.9404+4.70935*0.00001*d;//�����㾭�� 
	a=1; 
	e=0.016709-1.151*0.000000001*d;//ƫ���� 
	M = 356.0470 + 0.9856002585 * d;//ƽ����� 
	oblecl = 23.4393-3.563*0.0000001 * d;//�Ƴཻ�� 
//////////////////////////////////////////////////////////////////////////////////////////// 
	L=w+M;//̫����ƽ������ 
	L=fmod(L,360); 
	E=M + (180/PI) * e * sin(M*RADS_PER_DEG) * (1 + e * cos(M*RADS_PER_DEG));//�������շ��̵Ľ��ƽ⣩ 
	E=fmod(E,360); 
	xe=cos(E*RADS_PER_DEG) - e;//��Բ����ϵ�ֱ������x 
	ye=sin(E*RADS_PER_DEG) * sqrt(1 - e*e);//��Բ����ϵ�ֱ������y 
	r=sqrt(xe*xe+ye*ye);//���� 
	v=atan2(ye,xe)*180/PI;//������ 
//lon=fmod((v+w),360);//̫���ľ��� 
//̫���ĻƵ�ֱ������ 
	lon=v+w; 
	x=r*cos(lon*PI/180); 
	y=r*sin(lon*PI/180); 
	z=0; 
//̫���ĳ��ֱ������ 
	xequt=x; 
	yequt=y*cos(oblecl*PI/180); 
	zequt = y* sin(oblecl*PI/180); 
//�յؾ��롢�ྭ��γ 
	dist=sqrt(xequt*xequt+yequt*yequt); 
	RA=atan2(yequt,xequt)*180/PI; 
	RA=fmod(RA,360); 
	Decl=asin(zequt/r)*180/PI; 
	Decl=fmod(Decl,360); 

//cJulian m_j(year,month,day,hour,min,sec);
//double m_gmst=m_j.toGMST();
	double m_gmst=sun_j.toGMST();
	RA-=m_gmst*180/PI;
		if(RA<-180)
		   RA+=360;
		if(RA>180)
		 RA-=360;

	cVector3d result(RA,Decl,dist);
return result;

//////////////////////////////////////////////////////////////////// 
}


double cSun::getRiJiao(double lon)
{
	cJulian r_j=getJulian();
	r_j.addHour(8);
    int NF,Y,R,  S,  F;
	double sec;
    getComTime(getJulian(),&NF,&Y,&R,&S,&F,&sec);

	double A = (double)NF/4;
	double N0 = 79.6764+0.2422*(NF-1985)-floor((double)(NF-1985)/4);
	double B = A-floor(A);
	double C = 32.8;
	if(Y<=2)
		C = 30.6;
	if(B==0&&Y>2)
		C = 31.8;
	double G = floor(30.6*Y-C+0.5)+R;
	//double L = (JD+JF/60)/15;                //��������
	double L=(lon*180/PI)/15;
	double H = S-8+F/60;                     //ʱ������
	double N = G+(H-L)/24;                   //�������
	double rijiao = 2*PI*(N-N0)/365.2422;    //�����ս�
	return rijiao;                           //�����սǣ�����ֵ��


}

double cSun::getChiWeiJiao(double lon)
{
	double rijiao =getRiJiao(lon);
	double chiweijiao = 0.3723+23.2567*sin(rijiao)+0.1149*sin(2*rijiao)-0.1712*sin(3*rijiao)-0.758*cos(rijiao)
		                +0.3656*cos(2*rijiao)+0.0201*cos(3*rijiao);
	return chiweijiao;               //���س�γ�ǣ��Ƕ�ֵ��

}

double cSun::getShiCha(double lon)
{
   double rijiao = getRiJiao(lon);
	double shicha = 0.0028-1.9857*sin(rijiao)+9.9059*sin(2*rijiao)-7.0924*cos(rijiao)-0.6882*cos(2*rijiao);
	return shicha;                             //����ʱ��Է���Ϊ��λ��
}

double cSun::getShiJiao(double lon)
{
	cJulian m_j=getJulian();
	m_j.addHour(8);
    int year,month,day,hour,min;
	double sec;
	getComTime(m_j,/*in*/&year,&month,&day,&hour,&min,&sec);
	double sd = hour+(min-(120-(lon*180/PI))*4)/60;
    //double sd = S+(F-(120-(JD+JF/60))*4)/60;     //���ݱ���ʱ������ط�ʱ
	double sr = sd+getShiCha(lon)/60;  //ʱ������
	double shijiao = (sr-12)*15;
	return shijiao;                              //����ʱ�ǣ��Ƕ�ֵ��
}

double cSun::getHighAngle(double lon,double lat)
{
	double Chiwei=getChiWeiJiao(lon)*PI/180;
	double Shijiao=getShiJiao(lon)*PI/180;
	return asin(sin(lat)*sin(Chiwei)+cos(lat)*cos(Chiwei)*cos(Shijiao))*180/PI;


}

