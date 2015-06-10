#pragma once
#include "afxwin.h"

#include "SatOrbitDoc.h"
#include "cOrbit.h"
#include "StaticTime.h"
#include "cSatObject.h"
#include "cSun.h"

#include "BtnST.h"


// CSatNote 窗体视图

class CSatNote : public CFormView
{
	DECLARE_DYNCREATE(CSatNote)

protected:
	CSatNote();           // 动态创建所使用的受保护的构造函数
	virtual ~CSatNote();
public:
	TLE m_satnoteTLE;
	CommonTime m_realTime;
public:
	void ListInitial(TLE tle);
	void SunPosShow(CommonTime m_realTime);
	//void SatPosShow(CommonTime m_realTime)

public:
	enum { IDD = IDD_SAT_NOTE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	CBrush m_brush;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_noteSatList;
	//vector<TLE> m_notetle;
protected:
	virtual void OnDraw(CDC* /*pDC*/);
public:
	afx_msg void OnCbnSelchangeNoteSat();
public:
//	afx_msg void OnFileDe();
public:
//	afx_msg void OnTleImport();
public:
	CListBox m_satInfo;
public:
	CStaticTime m_noteTime;
	CStaticTime m_noteDay;
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	double m_sunLon;
public:
	double m_sunLat;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnBnClickedButtonUtc();
public:
	CButtonST m_timeN;
};


