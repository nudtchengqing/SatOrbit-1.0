#ifndef _COVER_H_
#define _COVER_H_

#include "cVector3d.h"
#include "CoordCovert.h"
#include <vector>
using namespace std;

class cCover
{
public:
	cVector3d r,h;//rΪ����λ�����µ�͵��ķ����ʸ��
	              //hΪ���ǹ����ķ�����
	double B;    //BΪ���ǵ��Ľ�
	int n;//nΪ������Ƭ
	double GMST;///*�������κ���ʱ*/
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