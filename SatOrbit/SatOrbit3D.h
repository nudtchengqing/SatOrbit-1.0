#pragma once

#include <gl\gl.h>		//add OpenGL Function Library
#include <gl\glu.h>	
#include <gl\glaux.h>
#include "cSatTrack.h"
#include "OpenGLFontInit.h"

// CSatOrbit3D 对话框

class CSatOrbit3D : public CDialog
{
	DECLARE_DYNAMIC(CSatOrbit3D)

public:
	CSatOrbit3D(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSatOrbit3D();

// 对话框数据
	enum { IDD = IDD_SAT_3D };
public:
	unsigned int listBaseFont;//创建一个字体显示列表的基准ID
	HGLRC m_hglrc;	
	CClientDC* m_pDC;
	GLuint		texture[1];		// 存储一个地球纹理
public:
	AUX_RGBImageRec *LoadBMP(CHAR *Filename);// 载入位图图象
	int LoadGLTextures(CHAR *Filename);// 载入位图，并转换成纹理
	void InitGL();
public:
	double scale;
	int    sat_flag;
	CommonTime m_startCommonTime;//模拟的开始时间
	CommonTime m_endCommonTime;//模拟的结束时间
	double  m_spanTime;//模拟的步长		
	int     m_satState; //显示卫星的状态 0实时  1模拟
	int     m_timeState;//时间形式   0北京时间  1UTC
	TLE     m_tle;
	double  m_minAngle;//航天器最小仰角
	/////////////////
	CommonTime m_realTime;//实时系统时间
	CommonTime m_simuTime;//模拟时间
	////////////////////
	CommonTime  m_realOrbitstart;//打开程序时的时间
	CommonTime  m_realStart;
	CommonTime  m_realEnd;
	///////////////////////////////////////
	int m_map3dIndex;
	int m_coord3dShow;
	int m_orbitColorIndex;
	int m_coverColorIndex;
	int m_cover3dShow;
	int m_orbit3dShow;
	//////////////////////////////////////////////////////
	int    m_play;
	double m_playTimeAdd;
	int    m_simuPlayVIndex;
	int    m_realSatPos;
	double m_realTimeLong;
//////////////////////////////////////////////
	int    m_timeFlag;
	int    m_realFlag;
	double elevation;
	double azimuth;
	int    light;
	/////////////////////////////////////
	HACCEL  m_hAccel;
	float  m_xRotation;
	float  m_yRotation;
	BOOL   m_LeftButtonDown;
	BOOL   m_RightButtonDown;
	CPoint m_LeftDownPos;
	CPoint m_RightDownPos;
public:
    vector<Point3D> m_orbit3D;
public:
	//void initialSet();
	double GetGMST(CommonTime m_realTime);
	void DrawEarth(CommonTime m_realTime);
	vector<Point3D> Orbit3DCompute(cJulian m_start,cJulian m_end,double m_spanTime);
	void DrawOrbit(CommonTime m_start,CommonTime m_end);
	void DrawSatCover(CommonTime m_realTime);
	void DrawScene();
	//////////////////////////////
	void InitData();
	void TimeControl();
	void LightControl();
	void Camera();
	void DrawCoord();
	/////////////////////////
	Point3D SunPos(CommonTime m_realTime);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnPaint();
public:
	afx_msg LRESULT UpdateData3D(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT UpdateDataOrbit3D(WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnZoomOut();
public:
	afx_msg void OnZoomIn();
public:
	virtual BOOL OnInitDialog();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
//	afx_msg void OnLookLeft();
public:
//	afx_msg void OnLookRight();
public:
//	afx_msg void OnLookUp();
public:
//	afx_msg void OnLookDown();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLightSun();
public:
	afx_msg void OnUpdateLightSun(CCmdUI *pCmdUI);
public:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
};
