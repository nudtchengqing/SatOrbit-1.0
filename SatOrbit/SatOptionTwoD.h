#pragma once
#include "afxwin.h"


// CSatOptionTwoD 对话框

class CSatOptionTwoD : public CDialog
{
	DECLARE_DYNAMIC(CSatOptionTwoD)

public:
	CSatOptionTwoD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSatOptionTwoD();

// 对话框数据
	enum { IDD = IDD_OPTION_TWOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int /*mapIndex,*/colorIndex;
public:
	//CComboBox m_mapShow;
public:
	int m_gridShow;
public:
	int m_satShow;
public:
	int m_sunShow;
public:
	int m_satPointShow;
public:
	double m_minAngle;
public:
	int m_satCoverShow;
public:
	CComboBox m_satCoverColor;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeTwodCovcolor();
};
