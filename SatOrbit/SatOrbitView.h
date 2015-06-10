// SatOrbitView.h : CSatOrbitView ��Ľӿ�
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
protected: // �������л�����
	CSatOrbitView();
	DECLARE_DYNCREATE(CSatOrbitView)

// ����
public:
	CSatOrbitDoc* GetDocument() const;
public:
	unsigned int listBase;//����һ��������ʾ�б�Ļ�׼ID
public:
	TLE m_tle;
	int sat_flag;
	ControlParameter m_Control;
	CommonTime  m_realOrbitstart;//�򿪳���ʱ��ʱ��,����ʵʱʱ�̵Ĺ����ʾ
	CommonTime  m_realStart;//ʵʱ����Ŀ�ʼ�ͽ���ʱ��
	CommonTime  m_realEnd;
public:
	CommonTime m_realTime;//ʵʱϵͳʱ��
	CommonTime m_simuTime;//ģ��ʱ��
	int        m_timeFlag;//ʵʱ��ʱ����ģ�ⶨʱ�����õĿ��Ʊ���
	int        m_realFlag;
	int        m_play;//1���� 2ֹͣ 0���� 3ǰ��  4����
	double     m_playTimeAdd; //ģ��ʱ�����ٶ�
public:
	vector<Point2D> m_orbitData;
// ����
public:
    CClientDC *m_pDC;//opengl��������
	HGLRC m_hglrc;

	int m_iViewWidth; //�ӿڵĿ��
	int m_iViewHeight;//�ӿڵĸ߶�

	AUX_RGBImageRec *m_pImageBack; //����λͼ�ṹָ��
	BYTE *m_pImage;      //λͼRGB��������

	int m_iImgWidth;//����λͼ�Ŀ��
	int m_iImgHeight;//����λͼ�ĸ߶�
public:
	ControlParameter InitializeOption(ControlParameter m_Control);//��ʼ������
	void DrawBackground();//��ͼ��������
	void DrawBackgroundGrid();//���ƾ�γ������
	void DrawBackgroundText();//��ʾ��γ����
public:
    Point2D Geo2Grid(double width,double height,Point2D geo);//��������ת������������
	CString CommonTime2CString(CommonTime m_commonTime);//ͨ��ʱ��ת�����ַ�����ʽ
	Point2D GetViewRec();//�õ���ͼ�Ĵ�С
	void GetDocData(CSatOrbitDoc* pDoc);
public:
    vector<Point2D> SatOrbitTranslate(vector<Point2D> m_data);
    vector<Point2D> SatOrbitCompute(cJulian m_start,cJulian m_end,double spanTime);
	void SatOrbitDisplay(CommonTime m_start,CommonTime m_end);//����Ļ���
	Point2D SatObjectCompute(cJulian m_realTime);//���ǵ������������
	void SatObjectDisplay(CommonTime m_realTime);//���ǵĻ���
    vector<Point2D> SatCoverCompute(cJulian m_realTime ,double minAngle,int n);//���Ǹ��Ǽ���
	void SatCoverDisplay(CommonTime m_realTime);//���Ǹ��ǵĻ���
	void SatSunDisplay(CommonTime m_realTime);//̫���Ļ���
	void SetEditdata(CString str);//ģ��ʱ�����ʾ
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CSatOrbitView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // SatOrbitView.cpp �еĵ��԰汾
inline CSatOrbitDoc* CSatOrbitView::GetDocument() const
   { return reinterpret_cast<CSatOrbitDoc*>(m_pDocument); }
#endif

