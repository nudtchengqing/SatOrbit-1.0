// FixSplitterWnd.cpp : ʵ���ļ�
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



// CFixSplitterWnd ��Ϣ�������



void CFixSplitterWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CWnd::OnLButtonDown(nFlags,point);

	//CSplitterWnd::OnLButtonDown(nFlags, point);
}
