#include "cCover.h"


vector<Point2D> cCover::getCover()
{
	vector<Point2D> sat_cover;
	Point2D satpoint;

	cVector3d r1(0,0,0);
	r1.getCirAngleVector3D(r,h,B);
		
	cVector3d r2(r1);
	//Vector3d r1;
	CRDCARTESIAN rECEF;
	CRDGEODETIC rGEO;
	rGEO.height=0;rGEO.latitude=0;rGEO.longitude=0;
	Point2D startPoint;
for(int i=0;i<n;i++)
{
	r2.getCirAngleVector3D(r1,r,2*PI*i/n);
	//r2=getCirAngleVector3d(r1,r,2*PI*i/n);
	//6.将矢量转换到地球固连坐标系（ECEF）中，得到rECEF;
	
	rECEF.x=(cos(GMST)*r2.getPoint3D().x+sin(GMST)*r2.getPoint3D().y)*1000;
	rECEF.y=(-sin(GMST)*r2.getPoint3D().x+cos(GMST)*r2.getPoint3D().y)*1000;
	rECEF.z=r2.getPoint3D().z*1000;
    //7.得到覆盖区边界上一点P1的经纬度satpoint

	CartesianToGeodetic(&rGEO,&rECEF,a,flattening);

	satpoint.x=rGEO.longitude*180/PI;//经度
	satpoint.y=rGEO.latitude*180/PI;//纬度
	if(i==0)
		startPoint=satpoint;
	
	//r2.x=0;r2.y=0;r2.z=0;
	sat_cover.push_back(satpoint);
}
sat_cover.push_back(startPoint);
   return sat_cover;
}