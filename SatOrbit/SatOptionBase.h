#pragma once
#include "afxwin.h"

// CSatOptionBase �Ի���

class CSatOptionBase : public CDialog
{
	DECLARE_DYNAMIC(CSatOptionBase)

public:
	CSatOptionBase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSatOptionBase();

// �Ի�������
	enum { IDD = IDD_OPTION_BASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
