#pragma once


// CActionToolBar

class CActionToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CActionToolBar)

public:
	CActionToolBar();
	virtual ~CActionToolBar();
public:
	bool CreateStatic( DWORD dwStyle, CEdit *pEdit, UINT nID,
		int nWidth, int nHeight );
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR);
};


