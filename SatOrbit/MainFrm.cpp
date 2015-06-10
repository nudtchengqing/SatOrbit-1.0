// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	if (!m_actionToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_actionToolBar.LoadToolBar(IDR_ACTION))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
    cs.style   &=   ~FWS_ADDTOTITLE;//去掉无标题   
    m_strTitle="SATORBIT";//设置新的标题
	return TRUE;
}


// CMainFrame 诊断

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


// CMainFrame 消息处理程序




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_fixSplitter.CreateStatic(this,1,2);
	m_fixSplitter.CreateView(0,0,RUNTIME_CLASS(CSatNote),CSize(200,600),pContext);
	m_fixSplitter.CreateView(0,1,RUNTIME_CLASS(CSatOrbitView),CSize(100,0),pContext);
	 return true;
  
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnViewAction()
{
	// TODO: 在此添加命令处理程序代码
	ShowControlBar(&m_actionToolBar,!m_actionToolBar.IsWindowVisible(),FALSE);
}

void CMainFrame::OnUpdateViewAction(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_actionToolBar.IsWindowVisible());
}

void CMainFrame::OnHelp()
{
	// TODO: 在此添加命令处理程序代码
	::ShellExecute(NULL,"open","doc\\help.chm",NULL,NULL,SW_SHOW  ); 
}
