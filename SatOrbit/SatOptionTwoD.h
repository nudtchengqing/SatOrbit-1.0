#pragma once
#include "afxwin.h"


// CSatOptionTwoD �Ի���

class CSatOptionTwoD : public CDialog
{
	DECLARE_DYNAMIC(CSatOptionTwoD)

public:
	CSatOptionTwoD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSatOptionTwoD();

// �Ի�������
	enum { IDD = IDD_OPTION_TWOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
