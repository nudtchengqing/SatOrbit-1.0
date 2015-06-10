// SatOption3D.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "SatOption3D.h"


// CSatOption3D �Ի���

IMPLEMENT_DYNAMIC(CSatOption3D, CDialog)

CSatOption3D::CSatOption3D(CWnd* pParent /*=NULL*/)
	: CDialog(CSatOption3D::IDD, pParent)
	, m_orbit3dShow(1)
	, m_cover3dShow(0)
	, m_coord3dShow(1)
{
	m_map3dIndex=0;
	m_orbitColorIndex=0;
	m_coverColorIndex=0;

}

CSatOption3D::~CSatOption3D()
{
}

void CSatOption3D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MAP3D, m_map3d);
	DDX_Control(pDX, IDC_COMBO_ORBITCOLOR, m_orbitColor);
	DDX_Control(pDX, IDC_COMBO_COVERCOLOR, m_coverColor);

	DDX_Check(pDX, IDC_ORBIT3D_SHOW, m_orbit3dShow);
	DDX_Check(pDX, IDC_COVER3D_SHOW, m_cover3dShow);
	DDX_Check(pDX, IDC_COORD3D_SHOW, m_coord3dShow);
}


BEGIN_MESSAGE_MAP(CSatOption3D, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_MAP3D, &CSatOption3D::OnCbnSelchangeComboMap3d)
	ON_CBN_SELCHANGE(IDC_COMBO_ORBITCOLOR, &CSatOption3D::OnCbnSelchangeComboOrbitcolor)
	ON_CBN_SELCHANGE(IDC_COMBO_COVERCOLOR, &CSatOption3D::OnCbnSelchangeComboCovercolor)
END_MESSAGE_MAP()


// CSatOption3D ��Ϣ�������

BOOL CSatOption3D::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	((CComboBox*)GetDlgItem(IDC_COMBO_MAP3D))->AddString("earth");
	((CComboBox*)GetDlgItem(IDC_COMBO_MAP3D))->AddString("world_blue");
	((CComboBox*)GetDlgItem(IDC_COMBO_MAP3D))->AddString("land_shallow");

	((CComboBox*)GetDlgItem(IDC_COMBO_MAP3D))->SetCurSel(m_map3dIndex);

	((CComboBox*)GetDlgItem(IDC_COMBO_ORBITCOLOR))->AddString("white");
	((CComboBox*)GetDlgItem(IDC_COMBO_ORBITCOLOR))->AddString("yellow");
	((CComboBox*)GetDlgItem(IDC_COMBO_ORBITCOLOR))->AddString("red");
	((CComboBox*)GetDlgItem(IDC_COMBO_ORBITCOLOR))->AddString("green");
	((CComboBox*)GetDlgItem(IDC_COMBO_ORBITCOLOR))->AddString("blue");

	((CComboBox*)GetDlgItem(IDC_COMBO_ORBITCOLOR))->SetCurSel(m_orbitColorIndex);

	((CComboBox*)GetDlgItem(IDC_COMBO_COVERCOLOR))->AddString("yellow");
	((CComboBox*)GetDlgItem(IDC_COMBO_COVERCOLOR))->AddString("red");
	((CComboBox*)GetDlgItem(IDC_COMBO_COVERCOLOR))->AddString("green");
	((CComboBox*)GetDlgItem(IDC_COMBO_COVERCOLOR))->AddString("blue");
	((CComboBox*)GetDlgItem(IDC_COMBO_COVERCOLOR))->AddString("black");

	((CComboBox*)GetDlgItem(IDC_COMBO_COVERCOLOR))->SetCurSel(m_coverColorIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSatOption3D::OnCbnSelchangeComboMap3d()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_map3dIndex=m_map3d.GetCurSel();
}

void CSatOption3D::OnCbnSelchangeComboOrbitcolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_orbitColorIndex=m_orbitColor.GetCurSel();
}

void CSatOption3D::OnCbnSelchangeComboCovercolor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_coverColorIndex=m_coverColor.GetCurSel();
}
