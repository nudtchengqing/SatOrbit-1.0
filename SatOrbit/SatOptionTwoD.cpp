// SatOptionTwoD.cpp : 实现文件
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "SatOptionTwoD.h"


// CSatOptionTwoD 对话框

IMPLEMENT_DYNAMIC(CSatOptionTwoD, CDialog)

CSatOptionTwoD::CSatOptionTwoD(CWnd* pParent /*=NULL*/)
	: CDialog(CSatOptionTwoD::IDD, pParent)
	, m_gridShow(1)
	, m_satShow(1)
	, m_sunShow(0)
	, m_satPointShow(1)
	, m_minAngle(10)
	, m_satCoverShow(0)
	,colorIndex(0)
{

}

CSatOptionTwoD::~CSatOptionTwoD()
{
}

void CSatOptionTwoD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TWOD_MAP, m_mapShow);
	DDX_Control(pDX, IDC_TWOD_COVCOLOR, m_satCoverColor);
	DDX_Check(pDX, IDC_CHECK_GRID, m_gridShow);
	DDX_Check(pDX, IDC_CHECK_SAT, m_satShow);
	DDX_Check(pDX, IDC_CHECK_SUN, m_sunShow);
	DDX_Check(pDX, IDC_CHECK_SATPOINT, m_satPointShow);

	DDX_Check(pDX, IDC_CHECK_COVER, m_satCoverShow);
	DDX_Text(pDX,IDC_TWOD_MINANG,m_minAngle);

}


BEGIN_MESSAGE_MAP(CSatOptionTwoD, CDialog)
	ON_CBN_SELCHANGE(IDC_TWOD_COVCOLOR, &CSatOptionTwoD::OnCbnSelchangeTwodCovcolor)
END_MESSAGE_MAP()


// CSatOptionTwoD 消息处理程序

BOOL CSatOptionTwoD::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
  /* ((CComboBox*)GetDlgItem(IDC_TWOD_MAP))->AddString("World");
	((CComboBox*)GetDlgItem(IDC_TWOD_MAP))->AddString("world_blue");
	((CComboBox*)GetDlgItem(IDC_TWOD_MAP))->AddString("land_shallow");

	((CComboBox*)GetDlgItem(IDC_TWOD_MAP))->SetCurSel(mapIndex);
*/
	((CComboBox*)GetDlgItem(IDC_TWOD_COVCOLOR))->AddString("white");
	((CComboBox*)GetDlgItem(IDC_TWOD_COVCOLOR))->AddString("yellow");
	((CComboBox*)GetDlgItem(IDC_TWOD_COVCOLOR))->AddString("red");

	((CComboBox*)GetDlgItem(IDC_TWOD_COVCOLOR))->SetCurSel(colorIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSatOptionTwoD::OnCbnSelchangeTwodCovcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	colorIndex=m_satCoverColor.GetCurSel();
}
