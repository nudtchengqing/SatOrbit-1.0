// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
#include "FixSplitterWnd.h"
#include "ActionToolBar.h"
class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
    CFixSplitterWnd m_fixSplitter;
// ����
public:
	CEdit m_simutime;

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CActionToolBar    m_actionToolBar;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnViewAction();
public:
	afx_msg void OnUpdateViewAction(CCmdUI *pCmdUI);
public:
	afx_msg void OnHelp();
};


