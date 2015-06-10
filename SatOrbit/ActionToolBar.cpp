// ActionToolBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "ActionToolBar.h"


// CActionToolBar

IMPLEMENT_DYNAMIC(CActionToolBar, CToolBar)

CActionToolBar::CActionToolBar()
{

}

CActionToolBar::~CActionToolBar()
{
}

bool CActionToolBar::CreateStatic(DWORD dwStyle, CEdit *pEdit, UINT nID, int nWidth, int nHeight)
{
	// set the size of combo-control
	CRect pRect(-nWidth, -nHeight, 0, 0);
	pRect.right -= 2;

	// make the button, that is selected to be the combo-control,
	// a separator and resize that separator
	ASSERT(CommandToIndex(nID) >= 0); // make sure the id is valid
	SetButtonInfo( CommandToIndex( nID ), nID, TBBS_SEPARATOR, nWidth );

	// create the combo-control itself, reposition it in the
	// client-area and show it
	if (!pEdit->Create( dwStyle, pRect, this, nID ))
	{
		  TRACE("Failed to create the combo-box %p .\n", nID);
		  return FALSE;
	}

	GetItemRect( CommandToIndex(nID), &pRect );

	pRect.left += 2;
	pRect.right = pRect.left + nWidth;

	pEdit->SetWindowPos(0, pRect.left, pRect.top, 0, 0,
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS );

//	pComboBox->SetFont( &m_GuiFont );
	//pEdit->SetReadOnly(true);
	pEdit->ShowWindow( SW_SHOW );
	
	return true;
}


BEGIN_MESSAGE_MAP(CActionToolBar, CToolBar)
END_MESSAGE_MAP()



// CActionToolBar ��Ϣ�������



//BOOL CActionToolBar::Create(CWnd* pParentWnd, DWORD dwStyle , UINT nID)
//{
//	// TODO: �ڴ����ר�ô����/����û���
//
//	return CToolBar::Create(pParentWnd, dwStyle, nID);
//}
