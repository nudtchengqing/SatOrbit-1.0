#pragma once
#include "afxwin.h"

// CSatOptionBase 对话框

class CSatOptionBase : public CDialog
{
	DECLARE_DYNAMIC(CSatOptionBase)

public:
	CSatOptionBase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSatOptionBase();

// 对话框数据
	enum { IDD = IDD_OPTION_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    
public:
	CString m_startJulian;
public:
	CString m_endJulian;
public:
	double m_spanTime;
public:
	int m_state;
public:
	int m_timeState;
public:
	CComboBox m_simuPlayV;
	int m_simuPlayVIndex;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeBasePlayv();
public:
	double m_realTimeLong;
public:
	int m_realSatPos;
};
