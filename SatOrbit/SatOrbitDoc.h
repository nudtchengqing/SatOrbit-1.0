// SatOrbitDoc.h : CSatOrbitDoc 类的接口
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
protected: // 仅从序列化创建
	CSatOrbitDoc();
	DECLARE_DYNCREATE(CSatOrbitDoc)

// 属性
public:
     vector<TLE> m_tlelist;//所有的卫星星历数据向量
	 TLE         m_tle;//当前卫星星历
	 int         sat_flag;//所含卫星数
	 ControlParameter m_Control;
	 CommonTime m_simuTime;//模拟时间,从视图获得，再传递给3D视图，以保证模拟时的模拟时间相同

public://动作
	int           m_play;//=1为播放，=2为停止，=0重置 =3为前进 =4为后退
    CSatInfo *m_satInfo; //信息查询
	CSatOrbit3D *m_sat3D;//三维显示
// 操作	
public:
	ControlParameter InitializeOption(ControlParameter m_Control);//初始化参数
	CommonTime CString2CommonTime(CString str);//通用时时间格式 结构体与字符串之间的转换
    CString    CommonTime2CString(CommonTime m_commontime);
	bool       IsCStringTime(CString str);//时间格式是否正确
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CSatOrbitDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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


