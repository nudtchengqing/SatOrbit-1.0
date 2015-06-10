#include "stdafx.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "OpenGLInit.h"

BOOL OpenGLInit(CClientDC* pDC,HGLRC& hglrc)
{
	int n;								//��ǰѡ������ظ�ʽ����

	static PIXELFORMATDESCRIPTOR pfd=
	{									//���ص��ʽ��Ϣ�����ṹ��
		sizeof(PIXELFORMATDESCRIPTOR),// pfd�ṹ�Ĵ�С
			1,                        // �汾��
			PFD_DRAW_TO_WINDOW |		//֧��Windows��OpenGL��������ͼ
			PFD_SUPPORT_OPENGL |
			PFD_DOUBLEBUFFER,			//ʹ��˫���潻����ʾͼ��
			PFD_TYPE_RGBA,			    // RGBA ��ɫģʽ 
			24,							// 24 λ��ɫ��� 
			0, 0, 0, 0, 0, 0,			// ������ɫλ
			0,							// û�з�͸���Ȼ��� 
			0,							// ������λλ 
			0,							// ���ۼӻ��� 
			0, 0, 0, 0,					// �����ۼ�λ
			32,							// 32 λ��Ȼ���     
			0,							// ��ģ�建�� 
			0,							// �޸������� 
			PFD_MAIN_PLANE,				// ����
			0,							// ����
			0, 0, 0						// ���Բ�,�ɼ��Ժ������ģ 
	};

	int pixelformat;
	if((pixelformat=ChoosePixelFormat(pDC->GetSafeHdc(),&pfd))==0)
	{
		AfxMessageBox(_T("ѡ�����ظ�ʽ��ʧ��"));
		return FALSE;
	}

	if(SetPixelFormat(pDC->GetSafeHdc(),pixelformat,&pfd)==FALSE)
	{
		AfxMessageBox(_T("�������ظ�ʽ��ʧ��"));
		return FALSE;
	}
	
	n=::GetPixelFormat(pDC->GetSafeHdc());
	::DescribePixelFormat(pDC->GetSafeHdc(),n,sizeof(pfd),&pfd);

	hglrc = wglCreateContext(pDC->GetSafeHdc());	//���������龰����

	wglMakeCurrent(pDC->GetSafeHdc(),hglrc);		//ѡ������龰����

	//glClearDepth(1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glMatrixMode(GL_MODELVIEW);						//���ñ任ģʽΪģ�ͱ任
//	glLoadIdentity();								//��ʼ��ģ�ͱ任����
	
	return TRUE;
}

BOOL OpenGLExit(CClientDC* pDC,HGLRC& hglrc)
{
	::wglMakeCurrent(NULL,NULL);		//����ǰ��ͼ�龰�����ÿ�
	
	if(hglrc)
		::wglDeleteContext(hglrc);		//ɾ����ǰ��ͼ�龰����

	if(pDC)
		delete pDC;					//ɾ��������windows�豸������

	return TRUE;
}