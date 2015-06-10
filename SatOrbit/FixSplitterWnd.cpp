// FixSplitterWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "FixSplitterWnd.h"


// CFixSplitterWnd

IMPLEMENT_DYNAMIC(CFixSplitterWnd, CSplitterWnd )

CFixSplitterWnd::CFixSplitterWnd()
{

}

CFixSplitterWnd::~CFixSplitterWnd()
{
}


BEGIN_MESSAGE_MAP(CFixSplitterWnd, CSplitterWnd )
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CFixSplitterWnd 消息处理程序



void CFixSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CWnd::OnLButtonDown(nFlags,point);

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}
