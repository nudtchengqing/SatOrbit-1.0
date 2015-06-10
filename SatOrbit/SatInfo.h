#pragma once

#include "set.h"
#include "afxwin.h"
// CSatInfo 对话框

class CSatInfo  : public CDialog
{
	DECLARE_DYNAMIC(CSatInfo)

public:
	CSatInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSatInfo();

// 对话框数据
	enum { IDD = IDD_SAT_INFO };
public:
	TLE tle;
	CommonTime m_commonTime;
	//int sat_flag;
	int fileNum;
	CString filePath;
public:
	void InitData();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_time;
	
public:
	int m_timeState;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedTimeApply();
public:
	CListBox m_satInfoList;
public:
	int m_query;
public:
	CString m_satName;
public:
	double m_longitude;
public:
	double m_latitude;
public:
	double m_altitude;
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedQueryApply();
public:
//	afx_msg void OnBnClickedReportDisplay();
public:
//	afx_msg void OnBnClickedReportDisplay();
public:
	afx_msg void OnBnClickedButtonR();
};
