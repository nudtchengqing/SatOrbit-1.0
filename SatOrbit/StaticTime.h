/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStaticTime - CStatic derived numeric counter display
//
// Author: Jason Hattingh
// Email:  jhattingh@greystonefx.com
// Copyright 1999, Jason Hattingh
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// This class implements a LED style counter without the need for bitmap resources
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define WM_UPDATE_STATIC (WM_USER+0x410)		// user defined WM message

#if !defined(AFX_STATICCOUNTER_H__F666A491_3847_11D3_A58E_00805FC1DE10__INCLUDED_)
#define AFX_STATICCOUNTER_H__F666A491_3847_11D3_A58E_00805FC1DE10__INCLUDED_

#include "stdafx.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticCounter.h : header file
//

const DWORD STCOUNTER0    = 252;
const DWORD STCOUNTER1    = 96;
const DWORD STCOUNTER2    = 218;
const DWORD STCOUNTER3    = 242;
const DWORD STCOUNTER4    = 102;
const DWORD STCOUNTER5    = 182;
const DWORD STCOUNTER6    = 190;
const DWORD STCOUNTER7    = 224;
const DWORD STCOUNTER8    = 254;
const DWORD STCOUNTER9    = 246;
const DWORD STCOUNTER10  = 2;		// The minus sign [2]
const DWORD STCOUNTER11  = 256;	  // The "point"
const DWORD STCOUNTER12  = 257;	  // The "colon" (:)
const DWORD STCOUNTERALL  = 999;

const DWORD NOTCH1 = 128;
const DWORD NOTCH2 = 64;
const DWORD NOTCH3 = 32;
const DWORD NOTCH4 = 16;
const DWORD NOTCH5 = 8;
const DWORD NOTCH6 = 4;
const DWORD NOTCH7 = 2;		// The minus sign
const DWORD NOTCH8 = 1;		// Not used...

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//				 - 14/7/99 Added optional clip rect parameter [jgh]
//
// This class implements a memory Device Context

class CMemDC : public CDC {
private:
	CBitmap m_bitmap; // Offscreen bitmap
	CBitmap* m_oldBitmap; // bitmap originally found in CMemDC
	CDC* m_pDC; // Saves CDC passed in constructor
	CRect m_rect; // Rectangle of drawing area.
	BOOL m_bMemDC; // TRUE if CDC really is a Memory DC.
public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
	CMemDC(CDC* pDC, CRect rect = CRect(0,0,0,0)) : CDC(), m_oldBitmap(NULL), m_pDC(pDC)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		ASSERT(m_pDC != NULL); // If you asserted here, you passed in a NULL CDC.
		
		m_bMemDC = !pDC->IsPrinting();
		
		if (m_bMemDC){
			// Create a Memory DC
			CreateCompatibleDC(pDC);
			if ( rect == CRect(0,0,0,0) )
				pDC->GetClipBox(&m_rect);
			else
				m_rect = rect;

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);
			SetWindowOrg(m_rect.left, m_rect.top);
		} else {
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
	~CMemDC()
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		if (m_bMemDC) {
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);
			//Swap back the original bitmap.
			SelectObject(m_oldBitmap);
		} else {
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
			m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// Allow usage as a pointer
	CMemDC* operator->() {return this;}
	
	// Allow usage as a pointer
	operator CMemDC*() {return this;}
};

class CStaticTime : public CStatic
{
// Construction
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticTime)
	//}}AFX_VIRTUAL

// Implementation
public:
	CStaticTime();
	virtual ~CStaticTime();

	void DisplayCurrentTime( int flag,CString strFormat = "%H:%M:%S" );
	void SetDrawFaded(bool bState = true);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
	void Display( CString strDisplay )		
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{	
		m_strDisplay = strDisplay;		
		Invalidate(FALSE);	
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
	void Display(UINT uSecs=0, UINT uMins=0, UINT uHours=0, UINT uDays=0, CString strFormat="%D:%H:%M:%S")
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		CTimeSpan TimeSpan(uDays, uHours, uMins, uSecs);
		Display(TimeSpan.Format(strFormat));
	};

	void SetColourForeGround(COLORREF crColor = 0xffffffff);
	void SetColourBackGround(COLORREF crColor = 0xffffffff);
	void SetColourFaded(COLORREF crColor = 0xffffffff);

protected:
	void Update();
	void Draw( CMemDC* pDC, DWORD dwChar, int nCol);

	bool m_bDrawFadedNotches;
	bool m_bGotMetrics;
	bool m_bSpecifiedFadeColour;

	CString m_strDisplay;

	RECT m_recClient;

	int m_nNotchWidth;
	int m_nNotchLength;
	int m_nMargin;

	COLORREF m_crBackground;
	COLORREF m_crForeground;
	COLORREF m_crDimForeground;

	CBrush m_brBackground;
	CBrush m_brForeground;


	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticTime)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICCOUNTER_H__F666A491_3847_11D3_A58E_00805FC1DE10__INCLUDED_)
