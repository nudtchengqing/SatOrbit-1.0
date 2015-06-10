// SatOrbitDoc.h : CSatOrbitDoc ��Ľӿ�
//


#pragma once

//
#include "SatTLEImport.h"
#include "cJulian.h"
#include "set.h"
#include "SatInfo.h"
#include "SatOrbit3D.h"

//#define WM_SAT3D  WM_USER+1
//#define WM_ORBITUPDATE WM_USER+2

class CSatOrbitDoc : public CDocument
{
protected: // �������л�����
	CSatOrbitDoc();
	DECLARE_DYNCREATE(CSatOrbitDoc)

// ����
public:
     vector<TLE> m_tlelist;//���е�����������������
	 TLE         m_tle;//��ǰ��������
	 int         sat_flag;//����������
	 ControlParameter m_Control;
	 CommonTime m_simuTime;//ģ��ʱ��,����ͼ��ã��ٴ��ݸ�3D��ͼ���Ա�֤ģ��ʱ��ģ��ʱ����ͬ

public://����
	int           m_play;//=1Ϊ���ţ�=2Ϊֹͣ��=0���� =3Ϊǰ�� =4Ϊ����
    CSatInfo *m_satInfo; //��Ϣ��ѯ
	CSatOrbit3D *m_sat3D;//��ά��ʾ
// ����	
public:
	ControlParameter InitializeOption(ControlParameter m_Control);//��ʼ������
	CommonTime CString2CommonTime(CString str);//ͨ��ʱʱ���ʽ �ṹ�����ַ���֮���ת��
    CString    CommonTime2CString(CommonTime m_commontime);
	bool       IsCStringTime(CString str);//ʱ���ʽ�Ƿ���ȷ
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CSatOrbitDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTleImport();
public:
	afx_msg void OnOptionBase();
public:
	afx_msg void OnOptionTwo();
public:
	afx_msg void OnActionPlay();
public:
	afx_msg void OnActionStop();
public:
	afx_msg void OnActionReset();
public:
	afx_msg void OnActionForward();
public:
	afx_msg void OnActionBack();
public:
	afx_msg void OnUpdateActionReset(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateActionForward(CCmdUI *pCmdUI);
public:
	afx_msg void OnInfoQuery();
public:
	afx_msg void OnUpdateActionBack(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateActionPlay(CCmdUI *pCmdUI);
public:
	afx_msg void OnUpdateActionStop(CCmdUI *pCmdUI);
public:
	afx_msg void OnInfo3d();
public:
	afx_msg void OnOptionThree();
public:
	afx_msg void OnTleIn();
public:
	afx_msg void OnTleClear();
	
};


