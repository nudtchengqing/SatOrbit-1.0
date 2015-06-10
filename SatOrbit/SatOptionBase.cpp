// SatOptionBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "SatOptionBase.h"


// CSatOptionBase �Ի���

IMPLEMENT_DYNAMIC(CSatOptionBase, CDialog)

CSatOptionBase::CSatOptionBase(CWnd* pParent /*=NULL*/)
	: CDialog(CSatOptionBase::IDD, pParent)
	, m_startJulian(_T("2009-04-01 00:00:00"))
	, m_endJulian(_T("2009-04-01 01:00:00"))
	, m_spanTime(2)
	, m_state(0)
	, m_timeState(0)
	, m_realTimeLong(2)
	, m_realSatPos(0)
{
m_simuPlayVIndex=3;
}

CSatOptionBase::~CSatOptionBase()
{
}

void CSatOptionBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX,IDC_RADIO_TIME,m_timeState);
	DDX_Text(pDX,IDC_BASE_STARTT,m_startJulian);
	DDX_Text(pDX,IDC_BASE_ENDT,m_endJulian);
	DDX_Text(pDX,IDC_BASE_SPANT,m_spanTime);
	DDX_Radio(pDX,IDC_RADIO_STATE,m_state);
	DDX_Control(pDX, IDC_BASE_PLAYV, m_simuPlayV);

	DDX_Text(pDX,IDC_REAL_LONG,m_realTimeLong);
	DDX_Radio(pDX,IDC_RADIO_POS,m_realSatPos);
}


BEGIN_MESSAGE_MAP(CSatOptionBase, CDialog)
	ON_CBN_SELCHANGE(IDC_BASE_PLAYV, &CSatOptionBase::OnCbnSelchangeBasePlayv)
END_MESSAGE_MAP()


// CSatOptionBase ��Ϣ�������

BOOL CSatOptionBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    ((CComboBox*)GetDlgItem(IDC_BASE_PLAYV))->AddString("10");
	((CComboBox*)GetDlgItem(IDC_BASE_PLAYV))->AddString("60");
	((CComboBox*)GetDlgItem(IDC_BASE_PLAYV))->AddString("100");
	((CComboBox*)GetDlgItem(IDC_BASE_PLAYV))->AddString("600");
    ((CComboBox*)GetDlgItem(IDC_BASE_PLAYV))->AddString("1200");
	((CComboBox*)GetDlgItem(IDC_BASE_PLAYV))->AddString("1800");


	((CComboBox*)GetDlgItem(IDC_BASE_PLAYV))->SetCurSel(m_simuPlayVIndex);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSatOptionBase::OnCbnSelchangeBasePlayv()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_simuPlayVIndex=m_simuPlayV.GetCurSel();
}
