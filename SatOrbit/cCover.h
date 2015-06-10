#ifndef _COVER_H_
#define _COVER_H_

#include "cVector3d.h"
#include "CoordCovert.h"
#include <vector>
using namespace std;

class cCover
{
public:
	cVector3d r,h;//r为卫星位置星下点和地心方向的矢量
	              //h为卫星轨道面的法向量
	double B;    //B为覆盖地心角
	int n;//n为采样分片
	double GMST;///*格林尼治恒星时*/
public:
	//cCover(const cCover &m_cover);
	cCover(cVector3d m_r,cVector3d m_h,double m_B,int m_n,double m_GMST):
	r(m_r),h(m_h),B(m_B),n(m_n),GMST(m_GMST)  {};
	~cCover()
	{
	};
	void setr(cVector3d rr){r=rr;};
	void seth(cVector3d hh){h=hh;};
	void setB(double BB){B=BB;};
	void setn(int nn){n=nn;};
	void setGMST(double GMSTset){GMST=GMSTset;};

	vector<Point2D> getCover();
};
#endif