#pragma once

#include "set.h"
#include "afxwin.h"
// CSatTLEImport 对话框

class CSatTLEImport : public CDialog
{
	DECLARE_DYNAMIC(CSatTLEImport)

public:
	CSatTLEImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSatTLEImport();

// 对话框数据
	enum { IDD = IDD_TLE_IMP };
public:
	vector<TLE> m_tle;
	int         sat_flag;
	CString     strpath;//星历文件路径

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
