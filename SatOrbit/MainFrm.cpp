// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "SatOrbit.h"

#include "MainFrm.h"

#include "SatNote.h"
#include "SatOrbitView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_ACTION, &CMainFrame::OnViewAction)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ACTION, &CMainFrame::OnUpdateViewAction)
	ON_COMMAND(ID_HELP, &CMainFrame::OnHelp)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	// TODO: �������Ҫ��������ͣ������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if (!m_actionToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_actionToolBar.LoadToolBar(IDR_ACTION))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_actionToolBar.CreateStatic( WS_CHILD | CBS_DROPDOWNLIST | 
		 CBS_HASSTRINGS, &m_simutime,
		ID_ACTION_TIME, 150, 20 ))
	{
		TRACE0("Failed to create static box.\n");
		return -1;
	}

    m_actionToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_actionToolBar);
	 RecalcLayout(TRUE);   
    
	  CRect   rect;   
	  m_wndToolBar.GetWindowRect(&rect);   
	  rect.OffsetRect(1,0);   
	  DockControlBar(&m_actionToolBar,   AFX_IDW_DOCKBAR_TOP,   rect);   

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
    cs.style   &=   ~FWS_ADDTOTITLE;//ȥ���ޱ���   
    m_strTitle="SATORBIT";//�����µı���
	return TRUE;
}


// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame ��Ϣ�������




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_fixSplitter.CreateStatic(this,1,2);
	m_fixSplitter.CreateView(0,0,RUNTIME_CLASS(CSatNote),CSize(200,600),pContext);
	m_fixSplitter.CreateView(0,1,RUNTIME_CLASS(CSatOrbitView),CSize(100,0),pContext);
	 return true;
  
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnViewAction()
{
	// TODO: �ڴ���������������
	ShowControlBar(&m_actionToolBar,!m_actionToolBar.IsWindowVisible(),FALSE);
}

void CMainFrame::OnUpdateViewAction(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(m_actionToolBar.IsWindowVisible());
}

void CMainFrame::OnHelp()
{
	// TODO: �ڴ���������������
	::ShellExecute(NULL,"open","doc\\help.chm",NULL,NULL,SW_SHOW  ); 
}
