// SatOrbitView.h : CSatOrbitView 类的接口
//


#pragma once
#include "MainFrm.h"
#include "SatOrbitDoc.h"
#include "OpenGLInit.h"
#include "OpenGLFontInit.h"

#include "cSatPointCover.h"
#include "cSatTrack.h"
#include "cSun.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
class CSatOrbitView : public CView
{
protected: // 仅从序列化创建
	CSatOrbitView();
	DECLARE_DYNCREATE(CSatOrbitView)

// 属性
public:
	CSatOrbitDoc* GetDocument() const;
public:
	unsigned int listBase;//创建一个字体显示列表的基准ID
public:
	TLE m_tle;
	int sat_flag;
	ControlParameter m_Control;
	CommonTime  m_realOrbitstart;//打开程序时的时间,用于实时时刻的轨道显示
	CommonTime  m_realStart;//实时轨道的开始和结束时间
	CommonTime  m_realEnd;
public:
	CommonTime m_realTime;//实时系统时间
	CommonTime m_simuTime;//模拟时间
	int        m_timeFlag;//实时定时器和模拟定时器设置的控制变量
	int        m_realFlag;
	int        m_play;//1播放 2停止 0重置 3前进  4后退
	double     m_playTimeAdd; //模拟时控制速度
public:
	vector<Point2D> m_orbitData;
// 操作
public:
    CClientDC *m_pDC;//opengl环境配置
	HGLRC m_hglrc;

	int m_iViewWidth; //视口的宽度
	int m_iViewHeight;//视口的高度

	AUX_RGBImageRec *m_pImageBack; //背景位图结构指针
	BYTE *m_pImage;      //位图RGB数据数组

	int m_iImgWidth;//背景位图的宽度
	int m_iImgHeight;//背景位图的高度
public:
	ControlParameter InitializeOption(ControlParameter m_Control);//初始化参数
	void DrawBackground();//地图背景绘制
	void DrawBackgroundGrid();//绘制经纬线网格
	void DrawBackgroundText();//显示经纬度数
public:
    Point2D Geo2Grid(double width,double height,Point2D geo);//地理坐标转换成网格坐标
	CString CommonTime2CString(CommonTime m_commonTime);//通用时间转换成字符串形式
	Point2D GetViewRec();//得到视图的大小
	void GetDocData(CSatOrbitDoc* pDoc);
public:
    vector<Point2D> SatOrbitTranslate(vector<Point2D> m_data);
    vector<Point2D> SatOrbitCompute(cJulian m_start,cJulian m_end,double spanTime);
	void SatOrbitDisplay(CommonTime m_start,CommonTime m_end);//轨道的绘制
	Point2D SatObjectCompute(cJulian m_realTime);//卫星的网格坐标计算
	void SatObjectDisplay(CommonTime m_realTime);//卫星的绘制
    vector<Point2D> SatCoverCompute(cJulian m_realTime ,double minAngle,int n);//卫星覆盖计算
	void SatCoverDisplay(CommonTime m_realTime);//卫星覆盖的绘制
	void SatSunDisplay(CommonTime m_realTime);//太阳的绘制
	void SetEditdata(CString str);//模拟时间的显示
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CSatOrbitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg LRESULT UpdateOrbit2D(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT DocUpdate(WPARAM wParam,LPARAM lParam);
public:
//	afx_msg void OnPaint();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // SatOrbitView.cpp 中的调试版本
inline CSatOrbitDoc* CSatOrbitView::GetDocument() const
   { return reinterpret_cast<CSatOrbitDoc*>(m_pDocument); }
#endif

