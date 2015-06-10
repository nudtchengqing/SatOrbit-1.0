// SatNote.cpp : 实现文件
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "SatNote.h"
#include "MainFrm.h"
#include "SatOrbitView.h"




// CSatNote

IMPLEMENT_DYNCREATE(CSatNote, CFormView)

CSatNote::CSatNote()
	: CFormView(CSatNote::IDD)
	, m_sunLon(0)
	, m_sunLat(0)
{
	CTime mt=CTime::GetCurrentTime(); 
	 m_realTime.year=mt.GetYear();
	 m_realTime.month=mt.GetMonth();
	 m_realTime.day=mt.GetDay();
	 m_realTime.hour=mt.GetHour();
	 m_realTime.minute=mt.GetMinute();
	 m_realTime.second=mt.GetSecond();

}

CSatNote::~CSatNote()
{
}

void CSatNote::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NOTE_SAT, m_noteSatList);
	DDX_Control(pDX, IDC_NOTE_SATINFO, m_satInfo);
	DDX_Control(pDX, IDC_NOTE_TIME, m_noteTime);
	DDX_Control(pDX, IDC_NOTE_DAY, m_noteDay);
	DDX_Text(pDX,IDC_NOTE_LONG,m_sunLon);
	DDX_Text(pDX,IDC_NOTE_LAT,m_sunLat);
	DDX_Control(pDX, IDC_BUTTON_UTC, m_timeN);
}

BEGIN_MESSAGE_MAP(CSatNote, CFormView)
	ON_CBN_SELCHANGE(IDC_NOTE_SAT, &CSatNote::OnCbnSelchangeNoteSat)
//	ON_COMMAND(ID_FILE_DE, &CSatNote::OnFileDe)
//	ON_COMMAND(ID_TLE_IMPORT, &CSatNote::OnTleImport)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_UTC, &CSatNote::OnBnClickedButtonUtc)
END_MESSAGE_MAP()


// CSatNote 诊断

#ifdef _DEBUG
void CSatNote::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSatNote::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSatNote 消息处理程序

void CSatNote::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	
}

void CSatNote::OnCbnSelchangeNoteSat()
{
	// TODO: 在此添加控件通知处理程序代码
	CSatOrbitDoc* pDoc = (CSatOrbitDoc *)GetDocument();
	int nChoice=m_noteSatList.GetCurSel();
	
	pDoc->m_tle=pDoc->m_tlelist[nChoice];
	m_satnoteTLE=pDoc->m_tlelist[nChoice];
	ListInitial(pDoc->m_tle);

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
		pv->SendMessage(WM_ORBITUPDATE);

	pDoc->UpdateAllViews(this);
	if(pDoc->m_sat3D!=NULL)
		pDoc->m_sat3D->SendMessage(WM_ORBIT3DUPDATE);

}


void CSatNote::ListInitial(TLE tle)
{
	if(m_satInfo.GetSelCount()!=0)
	{
		m_satInfo.ResetContent();
	}
	   cTle tle1(tle.name,tle.line1,tle.line2);//采用SDP模型计算
       cOrbit       orbit(tle1);
///////////////////////////////////////////////////////
	   string syear= tle.line1.substr(9,2);
       char *cyear = (char *)syear.c_str();  
       int year=atoi(cyear);
       if(year>57)    //判断具体年份
	      year+=1900;
	   else
		   year+=2000;
	   //////////////////////////////////////
		double p_motion =orbit.mnMotion()*2*PI/(24*3600);//平均角速度
        double a        =pow(GM/(p_motion*p_motion),1/3.0);//长半径

	///////////////////////////////////////////
	   CString str1,str2,str3,str4,str5,str6,str7,str8,str9;
	   CString str11,str22,str33,str44,str55,str66,str77,str88,str99;//字符相加不能以常字符开始
       str11="发射年份:  ";
	   str22="升交点赤经:";
	   str33="轨道倾角:  ";
	   str44="长半轴:    ";
	   str55="偏心率:    ";
	   str66="近地点角距:";
	   str77="平近点角:  ";
	   str88="周期:      ";
	   str99="近地点距离:";
	   str1.Format("%d",year);
	   str1=str11+str1;
	   str2.Format("%.4lf",orbit.RAAN()*180/PI);
	   str2=str22+str2+"度";
	   str3.Format("%.4lf",orbit.Inclination()*180/PI);
	   str3=str33+str3+"度";
	   str4.Format("%.4lf",a);
	   str4=str44+str4+"km";
	   str5.Format("%.7lf",orbit.Eccentricity());
	   str5=str55+str5;
	   str6.Format("%.4lf",orbit.ArgPerigee()*180/PI);
	   str6=str66+str6+"度";
	   str7.Format("%.4lf",orbit.mnAnomaly()*180/PI);
	   str7=str77+str7+"度";
	   str8.Format("%.4lf",orbit.Period()/(60*60*24.0));
	   str8=str88+str8+"天";
	   str9.Format("%.4lf",orbit.Perigee());
	   str9=str99+str9+"km";
       
	   m_satInfo.AddString(str1);
	   m_satInfo.AddString(str2);
	   m_satInfo.AddString(str3);
	   m_satInfo.AddString(str4);
	   m_satInfo.AddString(str5);
	   m_satInfo.AddString(str6);
	   m_satInfo.AddString(str7);
	   m_satInfo.AddString(str8);
	   m_satInfo.AddString(str9);
}

void CSatNote::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_noteTime.SetColourFaded(RGB(0,40,0));
	m_noteDay.SetColourFaded(RGB(0,40,0));

	m_brush.CreateSolidBrush(RGB(0,0,0)); 

	m_timeN.SetActiveBgColor(RGB(0,0,0));
	m_timeN.SetActiveFgColor(RGB(0,255,0));

	m_timeN.SetInactiveBgColor(RGB(0,0,0));
	m_timeN.SetInactiveFgColor(RGB(255,0,0));


	SetTimer( 0, 500, NULL);

	// TODO: 在此添加专用代码和/或调用基类
}

void CSatNote::SunPosShow(CommonTime m_realTime)
{
	cJulian m_real(m_realTime.year,m_realTime.month,m_realTime.day,
		           m_realTime.hour,m_realTime.minute,m_realTime.second);
	m_real.addHour(-8);
	cSun sun(m_real);
	m_sunLon=sun.GetSunPos().getPoint3D().x;
	m_sunLat=sun.GetSunPos().getPoint3D().y;
	
	CString m_lon,m_lat;
	m_lon.Format("%.3lf",m_sunLon);
	m_lat.Format("%.3lf",m_sunLat);
	CString str1,str2;
	str1="经度：";
	str2="纬度：";
	m_lon=str1+m_lon+"度";
	m_lat=str2+m_lat+"度";
	GetDlgItem(IDC_NOTE_LONG)->SetWindowText(m_lon);
	GetDlgItem(IDC_NOTE_LAT)->SetWindowText(m_lat);


}


void CSatNote::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	    CTime mt=CTime::GetCurrentTime();
		m_realTime.year=mt.GetYear();
		 m_realTime.month=mt.GetMonth();
		 m_realTime.day=mt.GetDay();
		 m_realTime.hour=mt.GetHour();
		 m_realTime.minute=mt.GetMinute();
		 m_realTime.second=mt.GetSecond();
		
		int flagg=0;
	    CString str;
			if(GetDlgItemText(IDC_BUTTON_UTC,str),str=="北京时间")
			{
				//SetDlgItemText(IDC_BUTTON1,"Button2");
				flagg=0;
			}
			else
			{
				//SetDlgItemText(IDC_BUTTON1,"Button1");
				flagg=1;
			}
	
	        m_noteTime.DisplayCurrentTime(flagg);
		    m_noteDay.DisplayCurrentTime(flagg,"%Y-%m-%d");
		    SunPosShow(m_realTime);
      
	CFormView::OnTimer(nIDEvent);
}

HBRUSH CSatNote::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
		
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//
  if(pWnd->GetDlgCtrlID()==IDC_STATIC)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0,0,255));
		return m_brush;
	}
  if(pWnd->GetDlgCtrlID()==IDC_NOTE_LONG)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0,0,255));
		return m_brush;
	}
  if(pWnd->GetDlgCtrlID()==IDC_NOTE_LAT)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0,0,255));
		return m_brush;
	}
  if(pWnd->GetDlgCtrlID()==IDC_NOTE_SAT)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0,0,255));
		return m_brush;
	}
  if(pWnd->GetDlgCtrlID()==IDC_NOTE_SATINFO)
	{
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetBkColor(RGB(0,0,255));
		return m_brush;
	}
  
  return hbr;
}

BOOL CSatNote::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP);

	BITMAP bmp;
	bitmap.GetBitmap(&bmp);

	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(pDC);

	dcCompatible.SelectObject(&bitmap);
	CRect rect;
	GetClientRect(&rect);
	pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,
		0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
	return TRUE;

	//return CFormView::OnEraseBkgnd(pDC);
}

void CSatNote::OnBnClickedButtonUtc()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	if(GetDlgItemText(IDC_BUTTON_UTC,str),str=="北京时间")
	{
		SetDlgItemText(IDC_BUTTON_UTC,"UTC");
		
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_UTC,"北京时间");
	
	}
}
