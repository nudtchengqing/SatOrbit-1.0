#pragma once
#include "afxwin.h"


// CSatOption3D �Ի���

class CSatOption3D : public CDialog
{
	DECLARE_DYNAMIC(CSatOption3D)

public:
	CSatOption3D(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSatOption3D();

// �Ի�������
	enum { IDD = IDD_OPTION_THREED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_map3d;
	int  m_map3dIndex;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnCbnSelchangeComboMap3d();
public:
	CComboBox m_orbitColor;
	int  m_orbitColorIndex;
public:
	afx_msg void OnCbnSelchangeComboOrbitcolor();
public:
	CComboBox m_coverColor;
	int m_coverColorIndex;
public:
	afx_msg void OnCbnSelchangeComboCovercolor();
public:
	int m_orbit3dShow;
public:
	int m_cover3dShow;
public:
	int m_coord3dShow;
};
