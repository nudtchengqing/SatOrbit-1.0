// SatInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "SatInfo.h"
#include "MainFrm.h"
#include "SatOrbitView.h"
#include "SatOrbitDoc.h"
#include "cSite.h"


// CSatInfo 对话框

IMPLEMENT_DYNAMIC(CSatInfo, CDialog)

CSatInfo::CSatInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CSatInfo::IDD, pParent)
	, m_time(_T(""))
	, m_timeState(0)
	, m_query(0)
	, m_satName(_T(""))
	, m_longitude(0)
	, m_latitude(0)
	, m_altitude(0)
{
	fileNum=0;
	filePath="";

}

CSatInfo::~CSatInfo()
{
}

void CSatInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Radio(pDX,IDC_INFO_TIMESTATE,m_timeState);
	DDX_Text(pDX,IDC_INFO_TIME,m_time);
	DDX_Control(pDX, IDC_INFO_LIST, m_satInfoList);

	DDX_Text(pDX,IDC_SITE_NAME,m_satName);
	DDX_Text(pDX,IDC_SITE_LON,m_longitude);
	DDX_Text(pDX,IDC_SITE_LAT,m_latitude);
	DDX_Text(pDX,IDC_SITE_ALT,m_altitude);

	DDX_Check(pDX, IDC_INFO_QUERY, m_query);
}


BEGIN_MESSAGE_MAP(CSatInfo, CDialog)
	ON_BN_CLICKED(IDC_TIME_APPLY, &CSatInfo::OnBnClickedTimeApply)
//	ON_BN_CLICKED(IDC_QUERY_APPLY, &CSatInfo::OnBnClickedQueryApply)
//	ON_BN_CLICKED(IDC_INFO_QUERY, &CSatInfo::OnBnClickedInfoQuery)
ON_BN_CLICKED(IDC_BUTTON3, &CSatInfo::OnBnClickedButton3)
ON_BN_CLICKED(IDC_QUERY_APPLY, &CSatInfo::OnBnClickedQueryApply)
//ON_BN_CLICKED(IDC_REPORT_DISPLAY, &CSatInfo::OnBnClickedReportDisplay)
//ON_BN_CLICKED(IDC_REPORT_DISPLAY, &CSatInfo::OnBnClickedReportDisplay)
ON_BN_CLICKED(IDC_BUTTON_R, &CSatInfo::OnBnClickedButtonR)
END_MESSAGE_MAP()


// CSatInfo 消息处理程序
void CSatInfo::InitData()
{
	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
   CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //得到文档的指针

   tle          =pDoc->m_tle;
   m_commonTime =pDoc->m_Control.m_startCommonTime;
   m_time       =pDoc->CommonTime2CString(m_commonTime);
   m_timeState  =pDoc->m_Control.m_timeState;
   GetDlgItem(IDC_INFO_TIME)->SetWindowTextA(m_time);

    CString sat;//卫星名的显示
	char *pp = (char *)tle.name.c_str();  //卫星名称
	sat.Format("%s",pp);
    GetDlgItem(IDC_INFO_SAT)->SetWindowTextA(sat);

}

BOOL CSatInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    InitData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSatInfo::OnBnClickedTimeApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if(m_satInfoList.GetSelCount()!=0)
	{
		m_satInfoList.ResetContent();
	}

	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //得到文档的指针
    
	tle      =pDoc->m_tle;
	CString sat;//卫星名的显示
	char *pp = (char *)tle.name.c_str();  //卫星名称
	sat.Format("%s",pp);
    GetDlgItem(IDC_INFO_SAT)->SetWindowTextA(sat);
	if(pDoc->IsCStringTime(m_time)==false)
		MessageBox("时间输入格式不对\r\n请按0000-00-00 00:00:00输入");
   else{
	    m_commonTime=pDoc->CString2CommonTime(m_time);
	    cJulian m_julian(m_commonTime.year,m_commonTime.month,
					     m_commonTime.day ,m_commonTime.hour,
					     m_commonTime.minute,
					     m_commonTime.second);
	   if(m_timeState==0)
		   m_julian.addHour(-8);

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
		double p_motion=orbit.mnMotion()*2*PI/(24*3600);//平均角速度
        double a=pow(GM/(p_motion*p_motion),1/3.0);//长半径

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
       
	   m_satInfoList.AddString("卫星基本信息");
	   m_satInfoList.AddString(str1);
	   m_satInfoList.AddString(str8);
	   m_satInfoList.AddString(str9);
	   m_satInfoList.AddString("");
       m_satInfoList.AddString("卫星轨道形状");
	   m_satInfoList.AddString(str4);
	   m_satInfoList.AddString(str5);
	   m_satInfoList.AddString("");
       m_satInfoList.AddString("卫星轨道位置");
	   m_satInfoList.AddString(str2);
	   m_satInfoList.AddString(str3);
	   m_satInfoList.AddString("");
	   m_satInfoList.AddString("卫星位置");
	   m_satInfoList.AddString(str6);
	   m_satInfoList.AddString(str7);
	   m_satInfoList.AddString("");
	   
	   cSatOrbitKEY m_satorbit(tle);
	   cSatObject   m_satobject(m_satorbit,m_julian);
	   CString satECIx,satECIy,satECIz,satGEOx,satGEOy,satGEOz,satVx,satVy,satVz;
	   CString ECIx,ECIy,ECIz,GEOx,GEOy,GEOz,Vx,Vy,Vz;
	   ECIx="X:";
	   ECIy="Y:";
	   ECIz="Z:";
	   GEOx="经度：";
	   GEOy="纬度：";
	   GEOz="大地高：";
	   Vx="Xv:";
	   Vy="Yv:";
	   Vz="Zv:";
	   satECIx.Format("%.4lf",m_satobject.getSatPosECI().getPoint3D().x);
	   satECIy.Format("%.4lf",m_satobject.getSatPosECI().getPoint3D().y);
	   satECIz.Format("%.4lf",m_satobject.getSatPosECI().getPoint3D().z);
	   satGEOx.Format("%.4lf",m_satobject.getSatPosGEO().getPoint3D().x);
	   satGEOy.Format("%.4lf",m_satobject.getSatPosGEO().getPoint3D().y);
	   satGEOz.Format("%.4lf",m_satobject.getSatPosGEO().getPoint3D().z);
	   satVx.Format("%.4lf",m_satobject.getSatVelECI().getPoint3D().x);
	   satVy.Format("%.4lf",m_satobject.getSatVelECI().getPoint3D().y);
	   satVz.Format("%.4lf",m_satobject.getSatVelECI().getPoint3D().z);
       satECIx=ECIx+satECIx+"km";
	   satECIy=ECIy+satECIy+"km";
	   satECIz=ECIz+satECIz+"km";
	   satGEOx=GEOx+satGEOx+" 度";
	   satGEOy=GEOy+satGEOy+"度";
	   satGEOz=GEOz+satGEOz+"km";
	   satVx=Vx+satVx+"km/s";
	   satVy=Vy+satVy+"km/s";
	   satVz=Vz+satVz+"km/s";
	   m_satInfoList.AddString("卫星的地心惯性坐标(ECI)");
	   m_satInfoList.AddString(satECIx);
	   m_satInfoList.AddString(satECIy);
	   m_satInfoList.AddString(satECIz);
	   m_satInfoList.AddString("");
	   m_satInfoList.AddString("卫星的大地坐标");
	   m_satInfoList.AddString(satGEOx);
	   m_satInfoList.AddString(satGEOy);
	   m_satInfoList.AddString(satGEOz);
	   m_satInfoList.AddString("");
	   m_satInfoList.AddString("卫星在地心惯性坐标中的速度");
	   m_satInfoList.AddString(satVx);
	   m_satInfoList.AddString(satVy);
	   m_satInfoList.AddString(satVz);
	   m_satInfoList.AddString("");

	cSun sun(m_julian);
	double m_sunLon=sun.GetSunPos().getPoint3D().x;
	double m_sunLat=sun.GetSunPos().getPoint3D().y;
	CString m_lon,m_lat;
	m_lon.Format("%.3lf",m_sunLon);
	m_lat.Format("%.3lf",m_sunLat);
	m_lon=GEOx+m_lon+"度";
	m_lat=GEOy+m_lat+"度";
	m_satInfoList.AddString("太阳的位置");
	m_satInfoList.AddString(m_lon);
	m_satInfoList.AddString(m_lat);}
}


void CSatInfo::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString p="<p>";
	fileNum++;
	CString filen;
	filen.Format("%d",fileNum);
	CString str_file;
	//1.时间
	CString str_time ,str1;
	if(m_timeState==0)
	  str1="时间(北京时间)\r\n";
	else
       str1="时间(UTC)\r\n";
	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //得到文档的指针
	str_time   =pDoc->CommonTime2CString(m_commonTime);
    str_time   =p+str1+"</p>"+p+str_time+"</p>\r\n";
	
	CString str_sat,str2,str_satname;
	str2="卫星名:";
	char *pp = (char *)tle.name.c_str();  //卫星名称
	str_satname.Format("%s",pp);
	str_satname.TrimLeft();
	str_satname.TrimRight();
	//////////////////////////////////////////////////////////////
	str_sat=p+str2+"<b>"+str_satname+"</b></p>\r\n";
//////////////////////////////////////////////////////////////////////////
	CString str_info("-----------------------卫星信息---------------------</b></p>\r\n<p>状态:");
	if(m_satInfoList.GetCount()==0)
     {
		str_info=str_info+"<b>"+"无信息</b></p>\r\n";
		str_file=str_sat+str_time+str_info;
	}
	else
	{
		str_info=p+"<b>"+str_info+"<b>"+"正常</b></p>\r\n";
	/////////////////////////////////////////////////
		CString str("<p>---------------------------------------------------------------------------------------------------</p>\r\n");
	CString str_base("卫星基本信息\r\n");
	CString str_launchYear,str_perod,str_perigee;
	m_satInfoList.GetText(1,str_launchYear);
	m_satInfoList.GetText(2,str_perod);
	m_satInfoList.GetText(3,str_perigee);

	str_base=p+"<b>"+str_base+"</b></p>&nbsp&nbsp&nbsp"+p+str_launchYear+"&nbsp&nbsp"
		     +"&nbsp&nbsp&nbsp"+str_perod+"&nbsp&nbsp"+"&nbsp&nbsp&nbsp"+str_perigee+"</p>\r\n";
	///////////////////////////////////////////卫星六轨道根数
	CString str_orbit("<p><b>卫星六轨道根数</b></p>\r\n");
	CString str1,str2,str3;
	CString str_a,str_e,str_RAAN,str_i,str_argperigee,str_mnanomaly;
	m_satInfoList.GetText(5,str1);
	m_satInfoList.GetText(6,str_a);
	m_satInfoList.GetText(7,str_e);
	m_satInfoList.GetText(9,str2);
	m_satInfoList.GetText(10,str_RAAN);
	m_satInfoList.GetText(11,str_i);
	m_satInfoList.GetText(13,str3);
	m_satInfoList.GetText(14,str_argperigee);
	m_satInfoList.GetText(15,str_mnanomaly);
	str_orbit=str_orbit+
		      p+str1+"</p>"+
		      "\r\n  "+p+"&nbsp"+str_a+"&nbsp&nbsp&nbsp&nbsp"+str_RAAN+"</p>"+
			  p+str2+"</p>"+p+"&nbsp"+str_argperigee+"&nbsp&nbsp&nbsp&nbsp"+str_e+"</p>"+
			  p+str3+"</p>"+p+"&nbsp"+str_i+"&nbsp&nbsp&nbsp&nbsp"+str_mnanomaly+
			  +"</p>\r\n";
///////////////////////////////////////////卫星位置和速度
	CString str_satpos("<p><b>卫星位置和速度</b></p>\r\n");
	CString str11,str22,str33;
	CString ecix,eciy,eciz,geox,geoy,geoz,vx,vy,vz;
	m_satInfoList.GetText(17,str11);
	m_satInfoList.GetText(18,ecix);
	m_satInfoList.GetText(19,eciy);
	m_satInfoList.GetText(20,eciz);
	m_satInfoList.GetText(22,str22);
	m_satInfoList.GetText(23,geox);
	m_satInfoList.GetText(24,geoy);
	m_satInfoList.GetText(25,geoz);
	m_satInfoList.GetText(27,str33);
	m_satInfoList.GetText(28,vx);
	m_satInfoList.GetText(29,vy);
	m_satInfoList.GetText(30,vz);
	str_satpos=str_satpos+p+"&nbsp"+str11+"</p>"+
		       p+"&nbsp&nbsp&nbsp"+ecix+"</p><p>&nbsp&nbsp&nbsp"+eciy+"</p><p>&nbsp&nbsp&nbsp"+
			   eciz+"</p>"+
			   p+"&nbsp"+str22+"</p>"+
			   p+"&nbsp&nbsp&nbsp"+geox+"</p><p>&nbsp&nbsp&nbsp"+geoy+"</p><p>&nbsp&nbsp&nbsp"+
			   geoz+"</p>"+
			   p+"&nbsp"+str33+"</p>"+
			   p+"&nbsp&nbsp&nbsp"+vx+"</p><p>&nbsp&nbsp&nbsp"+vy+"<p></p>&nbsp&nbsp&nbsp"+
			   vz+"</p>\r\n";

	CString str_query("<p><b>卫星站心坐标查询</b></p>\r\n");
	if(m_query==0)
	{
		str_query+="<p>无查询</p>\r\n";
	}
	else
	{
		str_query+="<p>查询正常</p>\r\n";
		CString sat,lon,lat,alt;
		CString az,el,ra,visble;
		GetDlgItemText(IDC_SITE_NAME,sat);
		GetDlgItemText(IDC_SITE_LON,lon);
		GetDlgItemText(IDC_SITE_LAT,lat);
		GetDlgItemText(IDC_SITE_ALT,alt);
		GetDlgItemText(IDC_TCS_AZ,az);
		GetDlgItemText(IDC_TCS_EL,el);
		GetDlgItemText(IDC_TCS_RA,ra);
		GetDlgItemText(IDC_TCS_STATE,visble);
		str_query=str_query+"</p>卫星对地面站"+"<b>"+visble+"</b></p>\r\n"+"<p>地面站名称："+sat+"</p>\r\n"+p+
			      "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp地面站心坐标&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp"+
                    "卫星方位  </p>\r\n"+
					p+ "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp经度:"+lon+"度"+
					"&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp"+
					"方位角: "+az+"度"+"</p>\r\n"+
					p+"&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp纬度:"+lat+"度"+"&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp------------->&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp"+"高度角: "+el+"度"+"</p>\r\n"+
					p+"&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp站心大地高:"+alt+"km"+"&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp"+"极距"+ra+"km"+"</p>\r\n";

	}

	CString m_head="<html><head><title>"+filen+"</title><body bgcolor=\"#F5F5DC\">\r\n";	
	str_file=m_head+str_sat+str_time+str_info+str+str_base
		+str+str_orbit+str+str_satpos+str+str_query+"</body></html>";
}
       CString pa(".\\Report\\");
	   CString path;
	   path=pa+str_satname+"_"+filen+".html";
       filePath=path;
       CFile file(path,CFile::modeCreate | CFile::modeWrite);
	   file.Write(str_file,str_file.GetLength());
	   file.Close();
		MessageBox("报告生成完成");

}

void CSatInfo::OnBnClickedQueryApply()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_query==0)
		MessageBox("无法查询\t\n请选择查询!");
	else
	{
		CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //得到文档的指针
	//m_commonTime=pDoc->CString2CommonTime(m_time);
	cJulian m_julian(m_commonTime.year,m_commonTime.month,
					 m_commonTime.day ,m_commonTime.hour,
					 m_commonTime.minute,
					 m_commonTime.second);
	if(m_timeState==0)
		m_julian.addHour(-8);

	cTle tleSDP4(tle.name,tle.line1,tle.line2);
    cOrbit orbitSDP4(tleSDP4);
    cEci eciSDP4;
    orbitSDP4.getPosition((m_julian.getDate()-orbitSDP4.Epoch().getDate())*24*60,
		                  &eciSDP4);

   cSite siteEquator(m_latitude, m_longitude, m_altitude); // 0.00 N, 100.00 W, 0 km altitude
   cCoordTopo topoLook = siteEquator.getLookAngle(eciSDP4);

  
   CString str1,str2;
   str1="可见";
   str2="不可见";
   if(topoLook.m_El<0)
   {
	   GetDlgItem(IDC_TCS_STATE)->SetWindowTextA(str2);
   }
   else
   {
	   GetDlgItem(IDC_TCS_STATE)->SetWindowTextA(str1); 

   }
       CString m_Az,m_El,m_alt;
	   m_Az.Format("%.4lf",rad2deg(topoLook.m_Az));
	   m_El.Format("%.4lf",rad2deg(topoLook.m_El));
	   m_alt.Format("%.4lf",topoLook.m_Range);
	   GetDlgItem(IDC_TCS_AZ)->SetWindowTextA(m_Az);
	   GetDlgItem(IDC_TCS_EL)->SetWindowTextA(m_El);
	   GetDlgItem(IDC_TCS_RA)->SetWindowTextA(m_alt);
	}
}

void CSatInfo::OnBnClickedButtonR()
{
	// TODO: 在此添加控件通知处理程序代码
	if(filePath.Compare(""))
	{   
		/*CFile reportfile(filePath,CFile::modeRead);
		char *pBuf;
		DWORD dwFileLen;
		dwFileLen=DWORD(reportfile.GetLength());
		pBuf=new char[dwFileLen+1];
		pBuf[dwFileLen]=0;
		reportfile.Read(pBuf,dwFileLen);
		reportfile.Close();
		::MessageBox(NULL,pBuf,"report",MB_OK);*/
		::ShellExecute(NULL,"open",filePath,NULL,NULL,SW_MAXIMIZE  );
	}
	else
		AfxMessageBox("没有生成报告！");
}
