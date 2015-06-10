// StaticCounter.cpp : implementation file
//

#include "StaticTime.h"
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticTime

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
CStaticTime::CStaticTime()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	// Set default background
	m_crBackground = 0;	//::GetSysColor(COLOR_BTNFACE);
	m_brBackground.CreateSolidBrush(m_crBackground);

	// Set default foreground
	m_crForeground = 0x0000FF00;	//::GetSysColor(COLOR_BTNFACE);
	m_brForeground.CreateSolidBrush(m_crForeground);

	m_strDisplay = "0";

	m_bSpecifiedFadeColour = false;
	m_bDrawFadedNotches = true;
	m_bGotMetrics = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
CStaticTime::~CStaticTime()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::SetColourBackGround(COLORREF cr)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	// Set new background color
	if (cr != 0xffffffff)
		m_crBackground = cr;
	else // Set default background color
		m_crBackground = ::GetSysColor(COLOR_BTNFACE);

    m_brBackground.DeleteObject();
    m_brBackground.CreateSolidBrush(m_crBackground);

	Update();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::SetColourForeGround(COLORREF cr)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	// Set new foreground color
	if (cr != 0xffffffff)
	{
		m_crForeground = cr;
	}
	else // Set default foreground color
	{
		m_crForeground = ::GetSysColor(COLOR_BTNTEXT);
	}

	Update();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::SetColourFaded(COLORREF cr)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	m_bSpecifiedFadeColour = true;
	m_crDimForeground = cr;
	Update();
}

BEGIN_MESSAGE_MAP(CStaticTime, CStatic)
	//{{AFX_MSG_MAP(CStaticTime)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticTime message handlers

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
HBRUSH CStaticTime::CtlColor(CDC* pDC, UINT nCtlColor) 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	pDC->SetTextColor(m_crForeground);
	pDC->SetBkColor(m_crBackground);
	
    return (HBRUSH)m_brBackground;	// Return non-NULL brush - the parent's handler is not called
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::OnPaint() 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	GetClientRect(&m_recClient);

	CPaintDC dc(this);
	CMemDC memDC(&dc, m_recClient);
	CMemDC* pDC = &memDC;

	CRect clip;
	pDC->GetClipBox(&clip);

	pDC->FillSolidRect(&m_recClient, m_crBackground );

	if (!m_bGotMetrics)
	{	// Calculate the character metrics in proportion to the size of the control:
		int nHeight = m_recClient.bottom;
		
		(nHeight * 0.06) < 1 ? m_nMargin = 1 : m_nMargin = (int)(nHeight * 0.06);
		(nHeight * 0.35) < 1 ? m_nNotchLength = 1 : m_nNotchLength = (int)(nHeight * 0.35);
		m_nNotchWidth = m_nMargin;
		m_bGotMetrics = true;
	}

	int nColPos = 0;

	for (int nCount = 0; nCount<m_strDisplay.GetLength(); nCount++)
	{
		// Calculate the position of the next character:

		if ( nCount > 0 )
		{
			if (m_strDisplay[nCount] == ':')	
				nColPos+= m_nNotchLength+m_nMargin;
			else if (m_strDisplay[nCount-1] == ':' )	
				nColPos+= m_nNotchLength+1;
			else
				nColPos += m_nNotchLength + (m_nMargin*4);
		}

		// First 'lay down' the faded notches:
		if (m_bDrawFadedNotches && m_strDisplay[nCount] != ':' )	Draw( pDC, STCOUNTERALL, nColPos );

		if		  ( m_strDisplay[nCount] == '0' ) Draw( pDC, STCOUNTER0, nColPos );
		else if ( m_strDisplay[nCount] == '1' )	Draw( pDC, STCOUNTER1, nColPos );
		else if ( m_strDisplay[nCount] == '2' )	Draw( pDC, STCOUNTER2, nColPos );
		else if ( m_strDisplay[nCount] == '3' )	Draw( pDC, STCOUNTER3, nColPos );
		else if ( m_strDisplay[nCount] == '4' )	Draw( pDC, STCOUNTER4, nColPos );
		else if ( m_strDisplay[nCount] == '5' )	Draw( pDC, STCOUNTER5, nColPos );
		else if ( m_strDisplay[nCount] == '6' )	Draw( pDC, STCOUNTER6, nColPos );
		else if ( m_strDisplay[nCount] == '7' )	Draw( pDC, STCOUNTER7, nColPos );
		else if ( m_strDisplay[nCount] == '8' )	Draw( pDC, STCOUNTER8, nColPos );
		else if ( m_strDisplay[nCount] == '9' )	Draw( pDC, STCOUNTER9, nColPos );
		else if ( m_strDisplay[nCount] == '-' )	Draw( pDC, STCOUNTER10, nColPos );
		else if ( m_strDisplay[nCount] == '.' )	Draw( pDC, STCOUNTER11, nColPos );
		else if ( m_strDisplay[nCount] == ':' )	Draw( pDC, STCOUNTER12, nColPos );
	}

 }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::Draw(CMemDC* pDC, DWORD dwChar, int nCol)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	COLORREF crNotchColor = m_crForeground;
	
	if (dwChar == STCOUNTERALL && !m_bSpecifiedFadeColour)	// The colour used will be a dim version of normal foreground
	{
		int r = GetRValue(m_crForeground)/3;
		int g = GetGValue(m_crForeground)/3;
		int b = GetBValue(m_crForeground)/3;
		crNotchColor = RGB(r,g,b);
	}
	else if (dwChar == STCOUNTERALL && m_bSpecifiedFadeColour)
		crNotchColor = m_crDimForeground;

	// Create the Pen accordingly
	CPen pen(PS_SOLID | PS_ENDCAP_ROUND, m_nNotchWidth, crNotchColor);
	CPen* pOldPen=pDC->SelectObject(&pen);


	if ( (dwChar & NOTCH1) || dwChar == STCOUNTERALL)	{	// should I draw the first bar in the display?
		pDC->MoveTo( nCol + m_nMargin*2, m_nMargin );
		pDC->LineTo( nCol + m_nNotchLength, m_nMargin );
	}

	if ( dwChar & NOTCH2 || dwChar == STCOUNTERALL)	{	// should I draw the 2nd bar in the display? [minus sign]
		pDC->MoveTo(nCol + m_nNotchLength + m_nMargin, m_nMargin*2);
		pDC->LineTo(nCol + m_nNotchLength + m_nMargin, m_nNotchLength + (m_nMargin*2) );
	}

	if ( dwChar & NOTCH3 || dwChar == STCOUNTERALL)	{	// should I draw the 3rd bar in the display?
		pDC->MoveTo(nCol + m_nNotchLength + m_nMargin, m_nNotchLength + (m_nMargin*4) );
		pDC->LineTo(nCol + m_nNotchLength + m_nMargin, m_nNotchLength*2 + (m_nMargin*3) );
	}

	if ( dwChar & NOTCH4 || dwChar == STCOUNTERALL)	{	// should I draw the 4th bar in the display?
		pDC->MoveTo( nCol + m_nMargin*2, m_nNotchLength*2 + (m_nMargin*4) );
		pDC->LineTo( nCol + m_nNotchLength, m_nNotchLength*2 + (m_nMargin*4) );
	}

	if ( dwChar & NOTCH5 || dwChar == STCOUNTERALL)	{	// should I draw the 5th bar in the display?
		pDC->MoveTo(nCol + m_nMargin, m_nNotchLength + (m_nMargin*4) );
		pDC->LineTo(nCol + m_nMargin, m_nNotchLength*2 + (m_nMargin*3) );
	}

	if ( dwChar & NOTCH6 || dwChar == STCOUNTERALL)	{	// should I draw the 6th bar in the display?
		pDC->MoveTo(nCol + m_nMargin, m_nMargin*2);
		pDC->LineTo(nCol + m_nMargin, m_nNotchLength + (m_nMargin*2) );
	}

	if ( dwChar & NOTCH7 || dwChar == STCOUNTERALL)	{	// should I draw the 7th bar in the display?
		pDC->MoveTo(nCol + m_nMargin*2, m_nNotchLength + (m_nMargin*3) );
		pDC->LineTo(nCol + m_nMargin + m_nNotchLength - m_nMargin, m_nNotchLength + (m_nMargin*3) );
	}

	if ( dwChar == STCOUNTER11 )	{	// should I draw the point?
		pDC->MoveTo( nCol + m_nMargin*2, m_nNotchLength*2 + (m_nMargin*4) );
		pDC->LineTo( nCol + (m_nNotchLength/2), m_nNotchLength*2 + (m_nMargin*4) );
	}

	if ( dwChar == STCOUNTER12 )	{	// should I draw the colon?
		// Upper dot:
		pDC->MoveTo( nCol + m_nMargin*2+(m_nMargin*2), m_nNotchLength );
		pDC->LineTo( nCol + (m_nNotchLength/2)+(m_nMargin*2), m_nNotchLength );

		// Lower dot:
		pDC->MoveTo( nCol + m_nMargin*2+(m_nMargin*2), m_nNotchLength*2 + (m_nMargin) );
		pDC->LineTo( nCol + (m_nNotchLength/2)+(m_nMargin*2), m_nNotchLength*2 + (m_nMargin) );
	}

	pDC->SelectObject(pOldPen);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::SetDrawFaded(bool bState)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	m_bDrawFadedNotches = bState;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::Update()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (::IsWindow(m_hWnd))
		Display(m_strDisplay);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticTime::DisplayCurrentTime(int flag, CString strFormat )
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	CTime tCurrent = CTime::GetCurrentTime();
	if(flag==0)
	m_strDisplay = tCurrent.Format(strFormat);
	else
		m_strDisplay = tCurrent.FormatGmt(strFormat);
	Update();
}
