#pragma once

#include "set.h"
#include "afxwin.h"
// CSatTLEImport �Ի���

class CSatTLEImport : public CDialog
{
	DECLARE_DYNAMIC(CSatTLEImport)

public:
	CSatTLEImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSatTLEImport();

// �Ի�������
	enum { IDD = IDD_TLE_IMP };
public:
	vector<TLE> m_tle;
	int         sat_flag;
	CString     strpath;//�����ļ�·��

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBImport();
public:
	CListBox m_ListBox;
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedBAdd();
public:
	CListBox m_satDisplay;
};
