// SatTLEImport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "SatTLEImport.h"

#include <stdio.h>
#include "cTle.h"
#include "cEci.h"
#include "cOrbit.h"
#include "cSite.h"
#include <iostream>
#include   <fstream>
// CSatTLEImport �Ի���

IMPLEMENT_DYNAMIC(CSatTLEImport, CDialog)

CSatTLEImport::CSatTLEImport(CWnd* pParent /*=NULL*/)
	: CDialog(CSatTLEImport::IDD, pParent)
{
	strpath=""; //��������ʼ��
	/*m_tle.name="";
	m_tle.line1="";
	m_tle.line2="";*/
	sat_flag=0;

}

CSatTLEImport::~CSatTLEImport()
{
}

void CSatTLEImport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAT_LIST, m_ListBox);
	DDX_Control(pDX, IDC_SAT_DISPLAY, m_satDisplay);
}


BEGIN_MESSAGE_MAP(CSatTLEImport, CDialog)
	ON_BN_CLICKED(IDC_B_IMPORT, &CSatTLEImport::OnBnClickedBImport)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_B_ADD, &CSatTLEImport::OnBnClickedBAdd)
END_MESSAGE_MAP()


// CSatTLEImport ��Ϣ�������

void CSatTLEImport::OnBnClickedBImport()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�������ļ���������
	 
	char   currentpath[50];   
   GetCurrentDirectory(50,currentpath);//�������õ�ǰ·��   
    // SetCurrentDirectory(currentpath);
	 CFileDialog fileEphemeris(TRUE);//���öԻ���
    
     fileEphemeris.m_ofn.lpstrTitle="TLE����";
	 fileEphemeris.m_ofn.lpstrFilter="Text File(*.txt)\0*.txt\0\0";
	 fileEphemeris.m_ofn.lpstrInitialDir=".\\TLE\\";
   
	 ////////////////////////////////////////////////////////////
	 string strname,strline1,strline2;
     
     ifstream inf;//C++��ʽ�������ļ�
    if(IDOK==fileEphemeris.DoModal())
	{
     
	strpath=fileEphemeris.GetPathName();//�õ��ļ���·��
    //inf.open(_TEXT(strpath));//ͨ���ļ�·�����ļ�
	locale::global(locale(""));//��ȫ��������Ϊ����ϵͳĬ������
	inf.open(strpath);//ͨ���ļ�·�����ļ�
	locale::global(locale("C"));//��ԭȫ�������趨
    CString sat_name;
	int i=0;
	TLE sat_tle;
    while(!inf.eof())
	 {
	
	getline(inf,strname);
	getline(inf,strline1);
	getline(inf,strline2);//��������������
   
    char *p = (char *)strname.c_str();  //��������

	// char p[100]=strname.c_str();
	//char *dd="sfs";
	sat_name.Format("%s",p);
	sat_name.TrimLeft();
	sat_name.TrimRight();
	int nIndex=m_ListBox.AddString(sat_name);
	//MessageBox(sat_name);

	sat_tle.name=strname;
	sat_tle.line1=strline1;
	sat_tle.line2=strline2;
	m_ListBox.SetItemDataPtr(nIndex,new TLE(sat_tle));
//	m_ListBox.SetItemData(nIndex,sat_tle);
	i++;
	
	}
	delete (TLE *)m_ListBox.GetItemDataPtr(m_ListBox.GetCount()-1);
	m_ListBox.DeleteString(m_ListBox.GetCount()-1);
    // m_satlist.DeleteItem(i-1);//������getline()����һ��
	 //ԭ�򣺰�����eof�������һ��λ�á�ʵ��Ȼ�����ļ����һ��λ�õ���һ��λ�á�
	 //���ļ�����ʱ��������eof,���ٶ�һ�ε�λ�ò���eof(�ļ�ĩβ����һ��λ��)
     
     inf.close();//�ر��ļ�
	}
   SetCurrentDirectory(currentpath);//���ﻹԭ��ǰ·������Ҫ��Ϊ�˺���λͼ����·������ȷ
     //this->PostMessage( WM_PAINT);
   // InvalidateRect(NULL,TRUE);//ˢ�»�ͼ��
}

void CSatTLEImport::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	for(int nIndex=m_ListBox.GetCount()-1;nIndex>=0;nIndex--)
	{
		delete (TLE *)m_ListBox.GetItemDataPtr(nIndex);
	}
}

void CSatTLEImport::OnBnClickedBAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nIndex=m_ListBox.GetCurSel();
	if(nIndex!=LB_ERR)
	{
		
		TLE *a_tle=(TLE *)m_ListBox.GetItemDataPtr(nIndex);
		TLE b_tle;
		b_tle.name=a_tle->name;
		b_tle.line1=a_tle->line1;
		b_tle.line2=a_tle->line2;

		CString sat_nameList;
		char *pp = (char *)b_tle.name.c_str();  //��������
	    sat_nameList.Format("%s",pp);
	    sat_nameList.TrimLeft();
	    sat_nameList.TrimRight();
		if((m_satDisplay.FindString(-1,sat_nameList))!=LB_ERR)
		{
			MessageBox("�Ѿ�����˸���");
		}
    	else
		{
		m_tle.push_back(b_tle);
		sat_flag++;
        
		CString sat_nameDisplay;
		char *p = (char *)b_tle.name.c_str();  //��������
	    sat_nameDisplay.Format("%s",p);
	    sat_nameDisplay.TrimLeft();
	    sat_nameDisplay.TrimRight();
	    int nIndex=m_satDisplay.AddString(sat_nameDisplay);
		}
		/*CString sd;
		sd.Format("%d",sat_flag);
		MessageBox(sd);*/
		UpdateData(false);

		//OnOK();
	}
	else
		MessageBox("û��ѡ������","��ʾ",MB_ICONWARNING|MB_OK);

	

}
