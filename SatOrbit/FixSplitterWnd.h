#pragma once


// CFixSplitterWnd

class CFixSplitterWnd : public CSplitterWnd 
{
	DECLARE_DYNAMIC(CFixSplitterWnd)

public:
	CFixSplitterWnd();
	virtual ~CFixSplitterWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


