// SatTLEImport.cpp : 实现文件
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
// CSatTLEImport 对话框

IMPLEMENT_DYNAMIC(CSatTLEImport, CDialog)

CSatTLEImport::CSatTLEImport(CWnd* pParent /*=NULL*/)
	: CDialog(CSatTLEImport::IDD, pParent)
{
	strpath=""; //卫星名初始化
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


// CSatTLEImport 消息处理程序

void CSatTLEImport::OnBnClickedBImport()
{
	// TODO: 在此添加控件通知处理程序代码
	//打开星历文件，并载入
	 
	char   currentpath[50];   
   GetCurrentDirectory(50,currentpath);//这里设置当前路径   
    // SetCurrentDirectory(currentpath);
	 CFileDialog fileEphemeris(TRUE);//公用对话框
    
     fileEphemeris.m_ofn.lpstrTitle="TLE星历";
	 fileEphemeris.m_ofn.lpstrFilter="Text File(*.txt)\0*.txt\0\0";
	 fileEphemeris.m_ofn.lpstrInitialDir=".\\TLE\\";
   
	 ////////////////////////////////////////////////////////////
	 string strname,strline1,strline2;
     
     ifstream inf;//C++方式打开星历文件
    if(IDOK==fileEphemeris.DoModal())
	{
     
	strpath=fileEphemeris.GetPathName();//得到文件的路径
    //inf.open(_TEXT(strpath));//通过文件路径打开文件
	locale::global(locale(""));//将全局区域设为操作系统默认区域
	inf.open(strpath);//通过文件路径打开文件
	locale::global(locale("C"));//还原全局区域设定
    CString sat_name;
	int i=0;
	TLE sat_tle;
    while(!inf.eof())
	 {
	
	getline(inf,strname);
	getline(inf,strline1);
	getline(inf,strline2);//读星历各行数据
   
    char *p = (char *)strname.c_str();  //卫星名称

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
    // m_satlist.DeleteItem(i-1);//由于用getline()会多读一行
	 //原因：按常理eof该是最后一个位置。实则不然，是文件最后一个位置的下一个位置。
	 //当文件读完时，并不是eof,而再读一次的位置才是eof(文件末尾的下一个位置)
     
     inf.close();//关闭文件
	}
   SetCurrentDirectory(currentpath);//这里还原当前路径，主要是为了后面位图加载路径的正确
     //this->PostMessage( WM_PAINT);
   // InvalidateRect(NULL,TRUE);//刷新绘图区
}

void CSatTLEImport::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	for(int nIndex=m_ListBox.GetCount()-1;nIndex>=0;nIndex--)
	{
		delete (TLE *)m_ListBox.GetItemDataPtr(nIndex);
	}
}

void CSatTLEImport::OnBnClickedBAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nIndex=m_ListBox.GetCurSel();
	if(nIndex!=LB_ERR)
	{
		
		TLE *a_tle=(TLE *)m_ListBox.GetItemDataPtr(nIndex);
		TLE b_tle;
		b_tle.name=a_tle->name;
		b_tle.line1=a_tle->line1;
		b_tle.line2=a_tle->line2;

		CString sat_nameList;
		char *pp = (char *)b_tle.name.c_str();  //卫星名称
	    sat_nameList.Format("%s",pp);
	    sat_nameList.TrimLeft();
	    sat_nameList.TrimRight();
		if((m_satDisplay.FindString(-1,sat_nameList))!=LB_ERR)
		{
			MessageBox("已经添加了该项");
		}
    	else
		{
		m_tle.push_back(b_tle);
		sat_flag++;
        
		CString sat_nameDisplay;
		char *p = (char *)b_tle.name.c_str();  //卫星名称
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
		MessageBox("没有选择卫星","提示",MB_ICONWARNING|MB_OK);

	

}
