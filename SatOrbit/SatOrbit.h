// SatOrbit.h : SatOrbit Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSatOrbitApp:
// �йش����ʵ�֣������ SatOrbit.cpp
//

class CSatOrbitApp : public CWinApp
{
public:
	CSatOrbitApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CSatOrbitApp theApp;