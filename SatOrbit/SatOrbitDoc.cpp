// SatOrbitDoc.cpp : CSatOrbitDoc ���ʵ��
//

#include "stdafx.h"
#include "SatOrbit.h"

#include "SatOrbitDoc.h"
#include "SatOrbitView.h"
#include "MainFrm.h"
#include "SatTLEImport.h"
#include "SatNote.h"
#include "SatOptionBase.h"
#include "SatOptionTwoD.h"
#include "SatOption3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSatOrbitDoc

IMPLEMENT_DYNCREATE(CSatOrbitDoc, CDocument)

BEGIN_MESSAGE_MAP(CSatOrbitDoc, CDocument)
//	ON_COMMAND(ID_TLE_IMPORT, &CSatOrbitDoc::OnTleImport)
	ON_COMMAND(ID_OPTION_BASE, &CSatOrbitDoc::OnOptionBase)
	ON_COMMAND(ID_OPTION_TWO, &CSatOrbitDoc::OnOptionTwo)
	ON_COMMAND(ID_ACTION_PLAY, &CSatOrbitDoc::OnActionPlay)
	ON_COMMAND(ID_ACTION_STOP, &CSatOrbitDoc::OnActionStop)
	ON_COMMAND(ID_ACTION_RESET, &CSatOrbitDoc::OnActionReset)
	ON_COMMAND(ID_ACTION_FORWARD, &CSatOrbitDoc::OnActionForward)
	ON_COMMAND(ID_ACTION_BACK, &CSatOrbitDoc::OnActionBack)
	ON_UPDATE_COMMAND_UI(ID_ACTION_RESET, &CSatOrbitDoc::OnUpdateActionReset)
	ON_UPDATE_COMMAND_UI(ID_ACTION_FORWARD, &CSatOrbitDoc::OnUpdateActionForward)
	ON_COMMAND(IDC_INFO_QUERY, &CSatOrbitDoc::OnInfoQuery)
	ON_UPDATE_COMMAND_UI(ID_ACTION_BACK, &CSatOrbitDoc::OnUpdateActionBack)
	ON_UPDATE_COMMAND_UI(ID_ACTION_PLAY, &CSatOrbitDoc::OnUpdateActionPlay)
	ON_UPDATE_COMMAND_UI(ID_ACTION_STOP, &CSatOrbitDoc::OnUpdateActionStop)
	ON_COMMAND(IDC_INFO_3D, &CSatOrbitDoc::OnInfo3d)
	ON_COMMAND(ID_OPTION_THREE, &CSatOrbitDoc::OnOptionThree)
	ON_COMMAND(ID_TLE_IN, &CSatOrbitDoc::OnTleIn)
	ON_COMMAND(ID_TLE_CLEAR, &CSatOrbitDoc::OnTleClear)
END_MESSAGE_MAP()


// CSatOrbitDoc ����/����

CSatOrbitDoc::CSatOrbitDoc()
{
	// TODO: �ڴ����һ���Թ������
    
	m_Control=InitializeOption(m_Control);
	

	sat_flag=0;
	m_play=0;

	m_tle.name="";
	m_tle.line1="";
	m_tle.line2="";

	 m_simuTime=m_Control.m_startCommonTime;

	m_satInfo=NULL;
	m_sat3D=NULL;
}

CSatOrbitDoc::~CSatOrbitDoc()
{
	if(!m_satInfo)//�Ի�������Դ���ͷ�
		delete m_satInfo;
	if(!m_sat3D)
		delete m_sat3D;
}

BOOL CSatOrbitDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CSatOrbitDoc ���л�

void CSatOrbitDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CSatOrbitDoc ���

#ifdef _DEBUG
void CSatOrbitDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSatOrbitDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSatOrbitDoc ����
//////////////////////////////////////////////////////

ControlParameter CSatOrbitDoc::InitializeOption(ControlParameter m_Control)
{
	m_Control.m_startCommonTime.year=2009;
	m_Control.m_startCommonTime.month=4;
	m_Control.m_startCommonTime.day=10;
	m_Control.m_startCommonTime.hour=0;
	m_Control.m_startCommonTime.minute=0;
	m_Control.m_startCommonTime.second=0;
	
	m_Control.m_endCommonTime.year=2009;
	m_Control.m_endCommonTime.month=4;
	m_Control.m_endCommonTime.day=10;
	m_Control.m_endCommonTime.hour=2;
	m_Control.m_endCommonTime.minute=0;
	m_Control.m_endCommonTime.second=0;
	m_Control.m_spanTime=2;
	m_Control.m_satState=0;
	m_Control.m_timeState=0;
	m_Control.m_simuPlayVIndex=3;
	m_Control.m_realTimeLong=2;
	m_Control.m_realSatPos=0;
	///////////////////////////////////////////////////
	m_Control.m_gridShow=1;
	m_Control.m_satShow=1;
	m_Control.m_sunShow=0;
	m_Control.m_satPointOrbitShow=1;
	m_Control.m_minAngle=10;
	m_Control.m_satCoverShow=0;
	m_Control.m_satCoverColorIndex=0;
	////////////////////////////////////////////////
	
	m_Control.m_map3dIndex=0;
	m_Control.m_coord3dShow=1;
	m_Control.m_orbitColorIndex=0;
	m_Control.m_coverColorIndex=0;
	m_Control.m_cover3dShow=0;
	m_Control.m_orbit3dShow=1;

	return m_Control;
}
//������������
void CSatOrbitDoc::OnOptionBase()
{
	// TODO: �ڴ���������������
	CSatOptionBase m_satOptionBase;
	m_satOptionBase.m_startJulian=CommonTime2CString(m_Control.m_startCommonTime);
	m_satOptionBase.m_endJulian=CommonTime2CString(m_Control.m_endCommonTime);
	m_satOptionBase.m_spanTime=m_Control.m_spanTime;
	m_satOptionBase.m_state=m_Control.m_satState;
	m_satOptionBase.m_timeState=m_Control.m_timeState;
	m_satOptionBase.m_simuPlayVIndex=m_Control.m_simuPlayVIndex;
	m_satOptionBase.m_realTimeLong=m_Control.m_realTimeLong;
	m_satOptionBase.m_realSatPos=m_Control.m_realSatPos;

	if(IDOK==m_satOptionBase.DoModal())
	{
		if(IsCStringTime(m_satOptionBase.m_startJulian)==false)
			AfxMessageBox("��ʼʱ�������ʽ����\r\n�밴0000-00-00 00:00:00����");
		else
		{
			if(IsCStringTime(m_satOptionBase.m_endJulian)==false)
				AfxMessageBox("����ʱ�������ʽ����\r\n�밴0000-00-00 00:00:00����");
			if(m_satOptionBase.m_realTimeLong<0)
                AfxMessageBox("ʵʱʱ�䳤�Ȳ���Ϊ������");

			{
				m_Control.m_startCommonTime = CString2CommonTime(m_satOptionBase.m_startJulian);
				m_Control.m_endCommonTime   = CString2CommonTime(m_satOptionBase.m_endJulian);
				m_Control.m_spanTime    = m_satOptionBase.m_spanTime;
				m_Control.m_satState    = m_satOptionBase.m_state;
				m_Control.m_timeState   = m_satOptionBase.m_timeState;
				m_Control.m_simuPlayVIndex=m_satOptionBase.m_simuPlayVIndex;
				m_Control.m_realTimeLong=m_satOptionBase.m_realTimeLong;
	            m_Control.m_realSatPos=m_satOptionBase.m_realSatPos;
			}
		}
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	    CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
		pv->SendMessage(WM_ORBITUPDATE);
		pv->SendMessage(WM_DOCUPDATE);

		//UpdateAllViews(NULL);
		if(m_sat3D!=NULL)
		{//m_sat3D->SendMessage(WM_SAT3D);
		m_sat3D->SendMessage(WM_ORBIT3DUPDATE);}
	}
}

CommonTime CSatOrbitDoc::CString2CommonTime(CString str)
{
	CommonTime m_t;
	
	str.TrimLeft();
	str.TrimRight();
	m_t.year      = atoi(str.Mid(0,4).GetBuffer(str.Mid(0,4).GetLength()));
	m_t.month     = atoi(str.Mid(5,2).GetBuffer(str.Mid(5,2).GetLength()));
	m_t.day       = atoi(str.Mid(8,2).GetBuffer(str.Mid(8,2).GetLength()));
	m_t.hour      = atoi(str.Mid(11,2).GetBuffer(str.Mid(11,2).GetLength()));
	m_t.minute    = atoi(str.Mid(14,2).GetBuffer(str.Mid(14,2).GetLength()));
	m_t.second    = atof(str.Mid(17,2).GetBuffer(str.Mid(17,2).GetLength()));

	return m_t;
}
CString CSatOrbitDoc::CommonTime2CString(CommonTime m_commontime)
{
	CString year,month,day,hour,minute;
	CString second;
	CString m_("-");
	CString m_t(":");
	CString m_0("0");
	CString m(" ");
	year.Format("%d",m_commontime.year);
	month.Format("%d",m_commontime.month);
	if(month.GetLength()<2)
		month=m_0+month;
	day.Format("%d",m_commontime.day);
	if(day.GetLength()<2)
		day=m_0+day;
	hour.Format("%d",m_commontime.hour);
	if(hour.GetLength()<2)
		hour=m_0+hour;
	minute.Format("%d",m_commontime.minute);
	if(minute.GetLength()<2)
		minute=m_0+minute;
	second.Format("%d",m_commontime.second);
	if(second.GetLength()<2)
		second=m_0+second;
	return year+m_+month+m_+day+m+hour+m_t+minute+m_t+second;


}
bool CSatOrbitDoc::IsCStringTime(CString str)
{
	if(!str.Mid(4,1).Compare("-")&&!str.Mid(7,1).Compare("-")&&!str.Mid(10,1).Compare(" ")&&
		!str.Mid(13,1).Compare(":")&&!str.Mid(16,1).Compare(":"))
		{
		   CommonTime m=CString2CommonTime(str);
		if(m.month<1||m.month>12||m.day<1||m.day>31||m.hour<0||m.hour>24||
			m.minute<0||m.minute>60||m.second<0||m.second>60)
			return false;
		else 
			return true;
		}
		else
			return false;
}

/////////////////////////////////////////////////////////////////////////////////
//��ά��������
void CSatOrbitDoc::OnOptionTwo()
{
	// TODO: �ڴ���������������
	CSatOptionTwoD m_satOptionTwoD;
    m_satOptionTwoD.m_gridShow      =m_Control.m_gridShow;
    m_satOptionTwoD.m_satShow       =m_Control.m_satShow;
    m_satOptionTwoD.m_sunShow       =m_Control.m_sunShow;
	m_satOptionTwoD.m_satPointShow  =m_Control.m_satPointOrbitShow;
	m_satOptionTwoD.m_minAngle      =m_Control.m_minAngle;
	m_satOptionTwoD.m_satCoverShow  =m_Control.m_satCoverShow;
	m_satOptionTwoD.colorIndex=m_Control.m_satCoverColorIndex;
	
	if(IDOK==m_satOptionTwoD.DoModal())
	{   
		if(m_satOptionTwoD.m_minAngle<0|| m_satOptionTwoD.m_minAngle>90)
				AfxMessageBox("��������С����λ��0--90��֮�䣡");
		m_Control.m_gridShow    =  m_satOptionTwoD.m_gridShow;
		m_Control.m_satShow     =  m_satOptionTwoD.m_satShow;
		m_Control.m_sunShow     =  m_satOptionTwoD.m_sunShow;
		m_Control.m_satPointOrbitShow=  m_satOptionTwoD.m_satPointShow;
		m_Control.m_minAngle    =  m_satOptionTwoD.m_minAngle;
		m_Control.m_satCoverShow=  m_satOptionTwoD.m_satCoverShow;
		m_Control.m_satCoverColorIndex=m_satOptionTwoD.colorIndex;
		//UpdateAllViews(NULL);
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	    CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
        pv->SendMessage(WM_DOCUPDATE);
		if(m_sat3D!=NULL)
		   m_sat3D->SendMessage(WM_ORBIT3DUPDATE);
	};
}

void CSatOrbitDoc::OnActionPlay()
{
	// TODO: �ڴ���������������
	//CtoolApp *pApp=(CToolApp *)AfxGetApp();
	//CMainFrame *pFrame=(CMainFrame *)pApp->m_pMainWnd;
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
	
	m_play=1;
	//UpdateAllViews(NULL);
	//pv->InvalidateRect(NULL,FALSE);
	
    pv->SendMessage(WM_DOCUPDATE);
	if(m_sat3D!=NULL)
		   m_sat3D->SendMessage(WM_SAT3D);

}

void CSatOrbitDoc::OnActionStop()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
	m_play=2;
	//UpdateAllViews(NULL);
	//pv->InvalidateRect(NULL,FALSE);
	pv->SendMessage(WM_DOCUPDATE);
	if(m_sat3D!=NULL)
		   m_sat3D->SendMessage(WM_SAT3D);
}

void CSatOrbitDoc::OnActionReset()
{
	// TODO: �ڴ���������������
    CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
	m_play=0;
	//UpdateAllViews(NULL);
	//pv->InvalidateRect(NULL,FALSE);
	pv->SendMessage(WM_DOCUPDATE);
	if(m_sat3D!=NULL)
		   m_sat3D->SendMessage(WM_SAT3D);
}

void CSatOrbitDoc::OnActionForward()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
	m_play=3;
	//pv->InvalidateRect(NULL,FALSE);
	pv->SendMessage(WM_DOCUPDATE);
	//UpdateAllViews(NULL);
	if(m_sat3D!=NULL)
		   m_sat3D->SendMessage(WM_SAT3D);
	
	//UpdateAllViews(pSatOrbitView);

}

void CSatOrbitDoc::OnActionBack()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
	m_play=4;
	//pv->InvalidateRect(NULL,FALSE);
	//UpdateAllViews(NULL);
	pv->SendMessage(WM_DOCUPDATE);
	if(m_sat3D!=NULL)
		   m_sat3D->SendMessage(WM_SAT3D);

}


void CSatOrbitDoc::OnUpdateActionReset(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_Control.m_satState==0)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CSatOrbitDoc::OnUpdateActionForward(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_Control.m_satState==0)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}



void CSatOrbitDoc::OnInfoQuery()
{
	// TODO: �ڴ���������������
	if(sat_flag==0)
		MessageBox(NULL,"û���������","��ʾ",MB_OK);
	else
	{
			if(m_satInfo==NULL)
				{
				m_satInfo=new CSatInfo;
				m_satInfo->Create(IDD_SAT_INFO,NULL);
				m_satInfo->ShowWindow(SW_SHOW);
				}
			else
			   {
					m_satInfo->SetActiveWindow();
				   m_satInfo->ShowWindow(SW_SHOW);
				}
	}
}

void CSatOrbitDoc::OnUpdateActionBack(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_Control.m_satState==0)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);

}

void CSatOrbitDoc::OnUpdateActionPlay(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_Control.m_satState==0)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CSatOrbitDoc::OnUpdateActionStop(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	if(m_Control.m_satState==0)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

void CSatOrbitDoc::OnInfo3d()
{
	// TODO: �ڴ���������������
	if(sat_flag==0)
		MessageBox(NULL,"û���������","��ʾ",MB_OK);
	else
	{
			if(m_sat3D==NULL)
				{
				m_sat3D=new CSatOrbit3D;
				m_sat3D->Create(IDD_SAT_3D,NULL);
				m_sat3D->ShowWindow(SW_SHOW);
				m_sat3D->SendMessage(WM_ORBIT3DUPDATE);
				}
			else
			   {
					m_sat3D->SetActiveWindow();
				   m_sat3D->ShowWindow(SW_SHOW);
				}
	}

}
///////////////////////////////////////////////////
//��ά��ʾ����������
void CSatOrbitDoc::OnOptionThree()
{
	// TODO: �ڴ���������������
	CSatOption3D m_satOption3D;
	m_satOption3D.m_map3dIndex=m_Control.m_map3dIndex;
	m_satOption3D.m_orbitColorIndex=m_Control.m_orbitColorIndex;
	m_satOption3D.m_coverColorIndex=m_Control.m_coverColorIndex;
	m_satOption3D.m_orbit3dShow=m_Control.m_orbit3dShow;
	m_satOption3D.m_cover3dShow=m_Control.m_cover3dShow;
	m_satOption3D.m_coord3dShow=m_Control.m_coord3dShow;
	
	if(IDOK==m_satOption3D.DoModal())
	{
		m_Control.m_map3dIndex          =  m_satOption3D.m_map3dIndex;
		m_Control.m_orbitColorIndex     =  m_satOption3D.m_orbitColorIndex;
		m_Control.m_coverColorIndex     =  m_satOption3D.m_coverColorIndex;
		m_Control.m_orbit3dShow    =  m_satOption3D.m_orbit3dShow;
		m_Control.m_cover3dShow    =  m_satOption3D.m_cover3dShow;
		m_Control.m_coord3dShow    =  m_satOption3D.m_coord3dShow;
      
		UpdateAllViews(NULL);

		if(m_sat3D!=NULL)
		   m_sat3D->SendMessage(WM_ORBIT3DUPDATE);
		
	}

}

void CSatOrbitDoc::OnTleIn()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatNote *pN=(CSatNote *)pFrame->m_fixSplitter.GetPane(0,0);
    
	//CSatOrbitDoc* pDoc = (CSatOrbitDoc *)GetDocument();
	CSatTLEImport  sat_tleImport;
	if(IDOK==sat_tleImport.DoModal())
	{
		if(sat_tleImport.sat_flag!=0)
		{
			//pDoc->sat_flag=sat_tleImport.sat_flag;
			//pDoc->m_tlelist=sat_tleImport.m_tle;
			//pDoc->m_tle=sat_tleImport.m_tle[0];
			sat_flag=sat_tleImport.sat_flag;
			m_tlelist=sat_tleImport.m_tle;
			m_tle=sat_tleImport.m_tle[0];

			pN->m_satnoteTLE=sat_tleImport.m_tle[0];
			TLE tle;

		if(pN->m_noteSatList.GetCount()!=0)
				pN->m_noteSatList.ResetContent();
		for(int i=0;i<sat_tleImport.sat_flag;i++)
		{
			tle=sat_tleImport.m_tle[i];
			
			CString sat;
			char *pp = (char *)tle.name.c_str();  //��������
			sat.Format("%s",pp);
		if(sat.Compare(""))
		{
			
		((CComboBox*)(pN->GetDlgItem(IDC_NOTE_SAT)))->AddString(sat);
		((CComboBox*)(pN->GetDlgItem(IDC_NOTE_SAT)))->SetCurSel(0);
		
		}}
       pN->ListInitial(pN->m_satnoteTLE);
		}
		else
			MessageBox(NULL,"û���������","��ʾ",MB_OK);
		
	}
	//UpdateAllViews(NULL);
	pN->InvalidateRect(NULL,FALSE);
	

	CSatOrbitView *pv=(CSatOrbitView *)pFrame->m_fixSplitter.GetPane(0,1);
		
		pv->SendMessage(WM_DOCUPDATE);
		pv->SendMessage(WM_ORBITUPDATE);
	if(m_sat3D!=NULL)
		m_sat3D->SendMessage(WM_ORBIT3DUPDATE);


}

void CSatOrbitDoc::OnTleClear()
{
	// TODO: �ڴ���������������
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CSatNote *pN=(CSatNote *)pFrame->m_fixSplitter.GetPane(0,0);
	pN->m_noteSatList.ResetContent();
	pN->m_satInfo.ResetContent();
	m_tle.name="";
	m_tle.line1="";
	m_tle.line2="";
	sat_flag=0;
	UpdateAllViews(NULL);
	if(m_sat3D!=NULL)
		m_sat3D->ShowWindow(SW_HIDE);
	if(m_satInfo!=NULL)
		m_satInfo->ShowWindow(SW_HIDE);
}

