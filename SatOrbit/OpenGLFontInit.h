#ifndef _OPENGLFONTINIT_H_
#define _OPENGLFONTINIT_H_

unsigned int CreateBitmapFont(char *fontName,int fontSize,HDC g_HDC);
void PrintString(unsigned int base,char *str);
void ClearFont(unsigned int base);
unsigned int InitializeFont(char *fontName,int fontSize,HDC g_HDC);

#endif