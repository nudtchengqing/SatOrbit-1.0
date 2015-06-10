#include "stdafx.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include "OpenGLFontInit.h"

unsigned int CreateBitmapFont(char *fontName,int fontSize,HDC g_HDC)
{
	HFONT hFont;
	unsigned int base;
	base = glGenLists(96);

	hFont=CreateFont(fontSize,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,
		             ANSI_CHARSET,OUT_TT_PRECIS,
		             CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,
					 FF_DONTCARE|DEFAULT_PITCH,
					 fontName);
	if(!hFont)
		return 0;
	SelectObject(g_HDC,hFont);
	wglUseFontBitmaps(g_HDC,32,96,base);
    return base;
}

void PrintString(unsigned int base,char *str)
{
	if((base==0)||(str==NULL))
		return ;

	glPushAttrib(GL_LIST_BIT);
	glListBase(base-32);
	glCallLists((GLsizei)strlen(str),GL_UNSIGNED_BYTE,str);
	glPopAttrib();
}

void ClearFont(unsigned int base)
{
	if(base!=0)
		glDeleteLists(base,96);
}

unsigned int InitializeFont(char *fontName,int fontSize,HDC g_HDC)
{
	return CreateBitmapFont(fontName,fontSize,g_HDC);
}