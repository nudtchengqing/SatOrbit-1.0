#include "stdafx.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "OpenGLInit.h"

BOOL OpenGLInit(CClientDC* pDC,HGLRC& hglrc)
{
	int n;								//当前选择的像素格式索引

	static PIXELFORMATDESCRIPTOR pfd=
	{									//像素点格式信息描述结构体
		sizeof(PIXELFORMATDESCRIPTOR),// pfd结构的大小
			1,                        // 版本号
			PFD_DRAW_TO_WINDOW |		//支持Windows与OpenGL二环境绘图
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,			//使用双缓存交换显示图形
			PFD_TYPE_RGBA,			    // RGBA 颜色模式 
			24,							// 24 位颜色深度 
			0, 0, 0, 0, 0, 0,			// 忽略颜色位
			0,							// 没有非透明度缓存 
			0,							// 忽略移位位 
			0,							// 无累加缓存 
			0, 0, 0, 0,					// 忽略累加位
			32,							// 32 位深度缓存     
			0,							// 无模板缓存 
			0,							// 无辅助缓存 
			PFD_MAIN_PLANE,				// 主层
			0,							// 保留
			0, 0, 0						// 忽略层,可见性和损毁掩模 
	};

	int pixelformat;
	if((pixelformat=ChoosePixelFormat(pDC->GetSafeHdc(),&pfd))==0)
	{
		AfxMessageBox(_T("选择像素格式化失败"));
		return FALSE;
	}

	if(SetPixelFormat(pDC->GetSafeHdc(),pixelformat,&pfd)==FALSE)
	{
		AfxMessageBox(_T("设置像素格式化失败"));
		return FALSE;
	}
	
	n=::GetPixelFormat(pDC->GetSafeHdc());
	::DescribePixelFormat(pDC->GetSafeHdc(),n,sizeof(pfd),&pfd);

	hglrc = wglCreateContext(pDC->GetSafeHdc());	//创建绘制情景对象

	wglMakeCurrent(pDC->GetSafeHdc(),hglrc);		//选择绘制情景对象

	//glClearDepth(1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glMatrixMode(GL_MODELVIEW);						//设置变换模式为模型变换
//	glLoadIdentity();								//初始化模型变换矩阵
	
	return TRUE;
}

BOOL OpenGLExit(CClientDC* pDC,HGLRC& hglrc)
{
	::wglMakeCurrent(NULL,NULL);		//将当前绘图情景对象置空
	
	if(hglrc)
		::wglDeleteContext(hglrc);		//删除当前绘图情景对象

	if(pDC)
		delete pDC;					//删除创建的windows设备上下文

	return TRUE;
}