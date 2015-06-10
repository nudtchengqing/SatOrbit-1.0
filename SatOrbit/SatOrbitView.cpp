// SatOrbitView.cpp : CSatOrbitView ���ʵ��
//

#include "stdafx.h"
#include "SatOrbit.h"

#include "SatOrbitDoc.h"
#include "SatOrbitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSatOrbitView

IMPLEMENT_DYNCREATE(CSatOrbitView, CView)

BEGIN_MESSAGE_MAP(CSatOrbitView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_ORBITUPDATE,UpdateOrbit2D)
	ON_MESSAGE(WM_DOCUPDATE,DocUpdate)
//	ON_WM_PAINT()
//ON_WM_PAINT()
ON_WM_TIMER()
END_MESSAGE_MAP()

// CSatOrbitView ����/����

CSatOrbitView::CSatOrbitView()
{   
	m_Control=InitializeOption(m_Control);
	
	m_pImage=NULL;//����λͼ��Ϣ������ʼ��
	m_pImageBack=NULL;

	m_tle.name="12";
	m_tle.line1="";
	m_tle.line2="";
	sat_flag=0;
  
	m_timeFlag=0;
	m_realFlag=0;

	 CTime mt=CTime::GetCurrentTime(); 
	 m_realTime.year=mt.GetYear();
	 m_realTime.month=mt.GetMonth();
	 m_realTime.day=mt.GetDay();
	 m_realTime.hour=mt.GetHour();
	 m_realTime.minute=mt.GetMinute();
	 m_realTime.second=mt.GetSecond();

	 
     m_realOrbitstart=m_realTime;
	 m_simuTime=m_Control.m_startCommonTime;
	 m_realStart=m_realTime;
	 m_realEnd=CommonTimeAddMinute(m_realTime,m_Control.m_realTimeLong*60);
	 m_play=0; 
	 m_playTimeAdd=600/600.0;
}

CSatOrbitView::~CSatOrbitView()
{
	if(m_pImage!=NULL)//�����������ͷ���Դ
           delete[] m_pImage;
    if(m_pImageBack!=NULL)
		{
			delete[] m_pImageBack->data;
			delete m_pImageBack;
		}
}

BOOL CSatOrbitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
	return CView::PreCreateWindow(cs);
}

// CSatOrbitView ����

void CSatOrbitView::OnDraw(CDC* /*pDC*/)
{
	CSatOrbitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	/////////���õ�ǰ��ͼ�豸
	if(m_hglrc)
		wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else
		return;
	//GetDocData(pDoc);//���ĵ����еõ�����
    DrawBackground();//��ά����ͼ

	if(m_Control.m_satState==0)//ʵʱ
	{  
		if(m_Control.m_satPointOrbitShow==1)
	         SatOrbitDisplay(m_realStart,m_realEnd);//�����µ�켣
	
		if(m_realFlag==0)
		{
			SetTimer(0,1000,NULL);
			m_realFlag=1;
		}

	   if(m_Control.m_sunShow==1)//��̫��
		SatSunDisplay(m_realTime);
	   SatObjectDisplay(m_realTime);//������
	  
	   
	}
	else//ģ��
	{
		if(m_Control.m_satPointOrbitShow==1)
	         SatOrbitDisplay(m_Control.m_startCommonTime,m_Control.m_endCommonTime);//�����µ�켣
		if(m_realFlag==1)
		{
			KillTimer(0);
			m_realFlag=0;
		}
		if(m_play==0)
		{
			if(m_timeFlag==1)
			{
				KillTimer(1);
				m_timeFlag=0;
			}

			m_simuTime=m_Control.m_startCommonTime;
			SetEditdata(CommonTime2CString(m_simuTime));
		}
		
		if(m_play==1&&m_timeFlag!=1)
		{
			SetTimer(1, 100, NULL);
		    m_timeFlag=1;
		}
		if(m_play==2&&m_timeFlag==1)
		{
			    KillTimer(1);
				m_timeFlag=0;
		}
		if(m_play==3)
		{
			if(m_timeFlag==1)
			{
				KillTimer(1);
				m_timeFlag=0;
			}
			
			m_simuTime=CommonTimeAddMinute(m_simuTime,m_playTimeAdd);
			SetEditdata(CommonTime2CString(m_simuTime));
			m_play=2;
		}
		if(m_play==4)
		{
			if(m_timeFlag==1)
			{
				KillTimer(1);
				m_timeFlag=0;
			}
			m_simuTime=CommonTimeAddMinute(m_simuTime,-m_playTimeAdd);
			SetEditdata(CommonTime2CString(m_simuTime));
			m_play=2;
		}
		pDoc->m_simuTime=m_simuTime;
		
		if(m_Control.m_sunShow==1)//��̫��
		   SatSunDisplay(m_simuTime);
		SatObjectDisplay(m_simuTime);

		SetEditdata(CommonTime2CString(m_simuTime));
		
	}
	
    SwapBuffers(wglGetCurrentDC());//����ǰ�󻺴�
	glDrawBuffer(GL_FRONT);//����ǰ��
}


// CSatOrbitView ���

#ifdef _DEBUG
void CSatOrbitView::AssertValid() const
{
	CView::AssertValid();
}

void CSatOrbitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSatOrbitDoc* CSatOrbitView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSatOrbitDoc)));
	return (CSatOrbitDoc*)m_pDocument;
}
#endif //_DEBUG


// CSatOrbitView ��Ϣ�������

int CSatOrbitView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_pDC=new CClientDC(this);
	ASSERT(m_pDC!=NULL);
    if(!OpenGLInit(m_pDC,m_hglrc))		//��ʼ��OpenGL����ʾ���
		return -1;

	//����λͼ
	if(m_pImageBack!=NULL)
	{
	delete[] m_pImageBack->data;
	delete m_pImageBack;
	m_pImageBack=NULL;
	}
	if(m_pImage!=NULL)
		delete[] m_pImage;
	//����λͼ�ļ�
	
	m_pImageBack=auxDIBImageLoad(_T("res/World.bmp"));
		
    if(m_pImageBack==NULL)
		return TRUE;

	//��¼λͼ�Ĵ�С
	m_iImgWidth=m_pImageBack->sizeX;
	m_iImgHeight=m_pImageBack->sizeY;
  
	//����λͼ������ռ���ڴ�
	m_pImage=new BYTE[(2*m_iImgWidth)*(2*m_iImgHeight)*3];
   //��λͼ�����ݰ���1��1�ı������͵�������
	gluScaleImage(GL_RGB,m_pImageBack->sizeX,m_pImageBack->sizeY,
		GL_UNSIGNED_BYTE,m_pImageBack->data,
		m_iImgWidth,m_iImgHeight,GL_UNSIGNED_BYTE,m_pImage);
//////////////////////////////////////////////////////////////////////
    
	listBase=InitializeFont("����",15,wglGetCurrentDC());//����Ĵ���
	
	return 0;
}

void CSatOrbitView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	OpenGLExit(m_pDC,m_hglrc);
	ClearFont(listBase);//��������
	if(m_realFlag==1)//���ٶ�ʱ��
		KillTimer(0);
	if(m_timeFlag==1)
	   KillTimer(1);
}

void CSatOrbitView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if(m_hglrc)
		wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else 
		return;
	m_iViewWidth=cx;
	m_iViewHeight=cy;
	glViewport(0,0,cx,cy);//�����ӿڴ�С
    glMatrixMode(GL_PROJECTION);//���ñ任ģʽΪͶӰģʽ
	glLoadIdentity();//��ʼ��ͶӰ�任����
    gluOrtho2D(0,m_iViewWidth,0,m_iViewHeight);//���ݸ���ֵ��������ͶӰ����
	glMatrixMode(GL_MODELVIEW);//���ñ任ģʽΪģ�ͱ任
	glLoadIdentity();//��ʼ��ģ�ͱ任����
	
}

BOOL CSatOrbitView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
     return true;
	//return CView::OnEraseBkgnd(pDC);
}


ControlParameter CSatOrbitView::InitializeOption(ControlParameter m_Control)//��ʼ������
{
	CSatOrbitDoc* pDoc = GetDocument();
	return pDoc->InitializeOption(m_Control);

}
void CSatOrbitView::DrawBackground()
{
    glDrawBuffer(GL_BACK);
///////////////////////////////////////////////
	glLoadIdentity();//��ʼ���任����

	glClearColor(1.0,1.0,1.0,1.0f);//���������ɫ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_pImage!=NULL)

	{
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);//ָ���ں�̨�����л���
	
	glPixelZoom(float(m_iViewWidth)/m_iImgWidth,//////�����ӿں�λͼ������λͼ�����ű���
		float(m_iViewHeight)/m_iImgHeight);

	glRasterPos2i(0,0);////ָ��λͼ�Ļ���λ��

	glDrawPixels(m_iImgWidth,m_iImgHeight,GL_RGB,GL_UNSIGNED_BYTE,m_pImage);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}
  if(m_Control.m_gridShow==1)
	{
	  DrawBackgroundGrid();
	  DrawBackgroundText();
	}
   glFinish();//������������
}

void CSatOrbitView::DrawBackgroundGrid()
{
	double width=GetViewRec().x;
	double height=GetViewRec().y;

	glPushMatrix();
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1,0xCCCC);
	glColor3ub(128,128,128);
	glBegin(GL_LINES);
    for(int i=0;i<12;i++)//����ѭ��������
	  {
	      glVertex2d(i*width /12.0,0);
	      glVertex2d(i*width /12.0,height);
	    }
	   ////////////////////////////////////////////////////////
	  for(int j=0;j<6;j++)//����ѭ����γ��
	  {
	      glVertex2d(0,j*height/6);
	      glVertex2d(width,j*height/6);
	  }
    glEnd();
	glDisable(GL_LINE_STIPPLE);
    glPopMatrix();
}

void CSatOrbitView::DrawBackgroundText()
{   
	   double width = GetViewRec().x;
	   double height= GetViewRec().y;
       glLoadIdentity();
	   glColor3f(255,255,255);
       glTranslatef(0.0f,0.0f,-1.0f);
	   int longitude=-150;//����������ʾ���м��������
	   int latitude=-60;
       int flag=0;
	   int flat=0;
	   CString strl,strla;
       char *lon;
	   for(int k=1;k<12;k++)//����
	   {
		   strl.Format("%d",longitude);
		   lon= (LPTSTR)(LPCTSTR)strl;
           glRasterPos2f(GLfloat(k*width/12.0-10),0.0);
           PrintString(listBase,lon);
		   longitude+=30;
	   }
	   for(int h=1;h<=5;h++)//γ��
	   {
		   strla.Format("%d",latitude);
		   lon= (LPTSTR)(LPCTSTR)strla;
           glRasterPos2f(0.0,GLfloat(h*height/6.0-5));
           PrintString(listBase,lon);
		   latitude+=30;
	   }  
}
Point2D CSatOrbitView::Geo2Grid(double width, double height, Point2D geo)
{
	Point2D m_grid;
	m_grid.x=width/2.0+(geo.x/360.0)*width;
	m_grid.y=height/2.0+(geo.y/180.0)*height;
	return m_grid;
}

CString CSatOrbitView::CommonTime2CString(CommonTime m_commonTime)
{
	CSatOrbitDoc* pDoc = GetDocument();
	return pDoc->CommonTime2CString(m_commonTime);
}

Point2D CSatOrbitView::GetViewRec()
{
	RECT rect;
	GetClientRect(&rect);
	Point2D viewRec;
	viewRec.x=rect.right;
	viewRec.y=rect.bottom;
	return viewRec;
}
void CSatOrbitView::GetDocData(CSatOrbitDoc *pDoc)
{
	m_tle=pDoc->m_tle;	
	sat_flag=pDoc->sat_flag;
	m_Control=pDoc->m_Control;
	m_play=pDoc->m_play;
	if(m_play==0)
       m_simuTime=m_Control.m_startCommonTime;
    int n=0;
	switch(m_Control.m_simuPlayVIndex)
	{
	case 0:
		{n=10;break;}
	case 1:
		{n=60;break;}
	case 2:
		{n=100;break;}
	case 3:
		{n=600;break;}
	case 4:
		{n=1200;break;}
	case 5:
		{n=1800;break;}
	default:
		{n=600;break;}
	}
	m_playTimeAdd=n/600.0;

	 
	 if(m_Control.m_realSatPos==0)
	 {m_realStart=m_realOrbitstart;
	  m_realEnd=CommonTimeAddMinute(m_realOrbitstart,m_Control.m_realTimeLong*60);
	 }
	 else if(m_Control.m_realSatPos==1)
	 {m_realStart=CommonTimeAddMinute(m_realOrbitstart,-m_Control.m_realTimeLong*60/2);
	  m_realEnd=CommonTimeAddMinute(m_realOrbitstart,m_Control.m_realTimeLong*60/2);
     }
	 else
	 {m_realStart=CommonTimeAddMinute(m_realOrbitstart,-m_Control.m_realTimeLong*60);
	   m_realEnd=m_realOrbitstart;
	 }

}
vector<Point2D> CSatOrbitView::SatOrbitTranslate(vector<Point2D> m_data)
{
	double width=GetViewRec().x;
	double height=GetViewRec().y;
	vector<Point2D> geoTrack=m_data;
	vector<Point2D> gridTrack;
	int geoSize=int(geoTrack.size());
	Point2D front,back;
	Point2D mid;
	Point2D a1;
	for(int i=0;i<geoSize-1;i++)
	{
		front=GeoToGrid(width,height,geoTrack[i]);
		back=GeoToGrid(width,height,geoTrack[i+1]);
		gridTrack.push_back(front);
		if(sqrt(pow(front.x-back.x,2)+pow(front.y-back.y,2))>3*width/4)
		{
			if(front.x<width/2&&back.x>width/2)
			{
				if(i>0)
				{   mid=GeoToGrid(width,height,geoTrack[i-1]);
					a1.x=0;
				    a1.y=-front.x*(front.y-mid.y)/(front.x-mid.x)+front.y;
					gridTrack.push_back(a1);
                }
				if(i<geoSize-2)
                {   mid=GeoToGrid(width,height,geoTrack[i+2]);
					a1.x=width;
				    a1.y= (width-back.x)*(back.y-mid.y)/(back.x-mid.x)+back.y;
					gridTrack.push_back(a1);
				}
			}
			if(front.x>width/2&&back.x<width/2)
			{
				if(i>0)
				{
					mid=GeoToGrid(width,height,geoTrack[i-1]);
					a1.x=width;
					a1.y=(width-front.x)*(front.y-mid.y)/(front.x-mid.x)+front.y;
					gridTrack.push_back(a1);
                }
				if(i<geoSize-2)
				{
					mid=GeoToGrid(width,height,geoTrack[i+2]);
					a1.x=0;
					a1.y=-back.x*(back.y-mid.y)/(back.x-mid.x)+back.y;
					gridTrack.push_back(a1);
				}

			}
		}
	}
	return gridTrack;
}
//�������
vector<Point2D> CSatOrbitView::SatOrbitCompute(cJulian m_start, cJulian m_end, double spanTime)
{
	    cSatOrbitKEY sat(m_tle);
		cSatTrack satTrack(sat,m_start,m_end,spanTime);
        return satTrack.getTrack2D();
		//return satTrack.getTrack2dGrid(GetViewRec().x,GetViewRec().y);
}

void CSatOrbitView::SatOrbitDisplay(CommonTime m_start,CommonTime m_end)
{
	if(sat_flag==0)
		return;
	else
	{  if(m_orbitData.empty())
	        return ;
		vector<Point2D> re=SatOrbitTranslate(m_orbitData);
        Point2D frontPoint,backPoint;//����ʱ��������һ��
        glPushMatrix();
        glColor3d(1.0,1.0,0.0);//��ɫ����
		for(int i=0;i<int(re.size()-1);i++)
		{
			//frontPoint=Geo2Grid(GetViewRec().x,GetViewRec().y,re[i]);
			//backPoint=Geo2Grid(GetViewRec().x,GetViewRec().y,re[i+1]);
			frontPoint=re[i];
			backPoint=re[i+1];

			glBegin(GL_LINE_STRIP);
			if(sqrt(pow((frontPoint.x-backPoint.x),2)+pow((frontPoint.y-backPoint.y),2))>GetViewRec().x*3/4)//������Ӵ����еľ������̫Զ������
			{	glVertex2d(frontPoint.x,frontPoint.y);
				glVertex2d(frontPoint.x,frontPoint.y);
			}
			else
			{
				glVertex2d(frontPoint.x,frontPoint.y);
				glVertex2d(backPoint.x,backPoint.y);
			}
			glEnd();
		}
        glPopMatrix();
		//glFlush();
	}
}

Point2D CSatOrbitView::SatObjectCompute(cJulian m_realTime)
{
	cSatOrbitKEY m_sat(m_tle);
	cSatObject m_satObject(m_sat,m_realTime);
    
	Point2D sat_geo={m_satObject.getSatPosGEO().getPoint3D().x,
	                 m_satObject.getSatPosGEO().getPoint3D().y};
	return Geo2Grid(GetViewRec().x,GetViewRec().y,sat_geo);//����ת��

}
void CSatOrbitView::SatObjectDisplay(CommonTime m_realTime)
{
	   if(sat_flag==0)
		  return;
	   cJulian m_real;
	   m_real=cJulianNormal(m_realTime,m_Control.m_satState,m_Control.m_timeState);
	 /////////////////////////////////////////////////////////////

	Point2D sat_grid=SatObjectCompute(m_real);
	glPushMatrix();
    glPointSize(8);//���С����
	glBegin(GL_POINTS); //��һ������ʾ����
	  glColor3f (1.0f, 0.0f, 0.0f); 
	  glVertex2f(GLfloat(sat_grid.x),GLfloat(sat_grid.y)); 
    glEnd();
    glPopMatrix();

    if(m_Control.m_satShow==1)//�������Ƶ���ʵ
	{
	  char *p = (char *)m_tle.name.c_str();  //��������
	  CString strname;
	  strname.Format("%s",p);
	  int length=strname.GetLength();
	  double x=sat_grid.x;
      if(sat_grid.x+length*2>GetViewRec().x)
             x-=(length*2);
	  glColor3f(127,0,255);
	  glRasterPos2f(GLfloat(x),GLfloat(sat_grid.y));
	  PrintString(listBase,(char *)m_tle.name.c_str());
	 }
	 if(m_Control.m_satCoverShow==1)//���Ǹ����Ƿ���ʾ
		   SatCoverDisplay(m_realTime);
}

vector<Point2D> CSatOrbitView::SatCoverCompute(cJulian m_realTime ,double minAngle,int n)
{
	    cSatOrbitKEY m_sat(m_tle);
		cSatPointCover m_satCover(m_sat,m_realTime,minAngle,n);
		return m_satCover.getValidCover();

}
void CSatOrbitView::SatCoverDisplay(CommonTime m_realTime)
{	
	    if(sat_flag==0)
		   return;
		cJulian m_real;
	   m_real=cJulianNormal(m_realTime,m_Control.m_satState,m_Control.m_timeState);
	  ///////////////////////////////////////////////////////////////////
		vector<Point2D> cover=SatCoverCompute(m_real,m_Control.m_minAngle,200);
        Point2D frontPoint,backPoint;

		glPushMatrix();
        switch(m_Control.m_satCoverColorIndex)//������ɫ��ѡ��
		{
		case 0:
			{glColor3d(1.0,1.0,1.0);break;}
		case 1:
			{glColor3d(1.0,1.0,0.0);break;}
		default:
			{glColor3d(1.0,0.0,0.0);break;}
		}
		for(int i=0;i<int(cover.size()-1);i++)
		{
			frontPoint=Geo2Grid(GetViewRec().x,GetViewRec().y,cover[i]);
			backPoint=Geo2Grid(GetViewRec().x,GetViewRec().y,cover[i+1]);
            
			glBegin(GL_LINE_STRIP);
			if(sqrt(pow((frontPoint.x-backPoint.x),2)+pow((frontPoint.x-backPoint.x),2))>GetViewRec().x)//������Ӵ����еľ������̫Զ������
			{
				glVertex2d(frontPoint.x,frontPoint.y);
				glVertex2d(frontPoint.x,frontPoint.y);
			}
			else
			{
				glVertex2d(frontPoint.x,frontPoint.y);
				glVertex2d(backPoint.x,backPoint.y);
			}
			
			glEnd();
		}
        glPopMatrix();
       
}

void CSatOrbitView::SatSunDisplay(CommonTime m_realTime)
{
	 
	  cJulian m_real;
	   m_real=cJulianNormal(m_realTime,m_Control.m_satState,m_Control.m_timeState);
	  //////////////////////////////////////////////////////////////////
		Point2D sun_geo,sun_grid;
		cSun sun(m_real);
		sun_geo.x=sun.GetSunPos().getPoint3D().x;
		sun_geo.y=sun.GetSunPos().getPoint3D().y;
        
		sun_grid=Geo2Grid(GetViewRec().x,GetViewRec().y,sun_geo);

        glPushMatrix();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor4f(1.0,1.0,0.0,0.5);//��ɫ
		glTranslatef(GLfloat(sun_grid.x),GLfloat(sun_grid.y),-0.0); //ƽ�ƺ�����ȷ��Բ��
		int n=100;
        glBegin(GL_POLYGON); //��һ����ɫ��Բ����̫��         
          for(int i=0;i<n ;i++)    
		     glVertex2f(GLfloat(10*cos(2*PI/n*i)),GLfloat( 10*sin(2*PI/n*i)));  //���ݽǶȻ�Բ
        glEnd();
		glPopMatrix();
}
void CSatOrbitView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(nIDEvent==0)
	{
		CTime mt=CTime::GetCurrentTime();
		 m_realTime.year=mt.GetYear();
		 m_realTime.month=mt.GetMonth();
		 m_realTime.day=mt.GetDay();
		 m_realTime.hour=mt.GetHour();
		 m_realTime.minute=mt.GetMinute();
		 m_realTime.second=mt.GetSecond();
	}
	else
	{
		m_simuTime=CommonTimeAddMinute(m_simuTime,m_playTimeAdd);
		if(CommonTimeCompare(m_simuTime,m_Control.m_endCommonTime)==0)
			 m_simuTime=m_Control.m_startCommonTime;

	}
    //InvalidateRect(NULL,FALSE);
	//SatObjectDisplay(m_simuTime);
    Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}

void CSatOrbitView::SetEditdata(CString str)
{
	CMainFrame* frame=(CMainFrame*)AfxGetMainWnd();
	frame->m_simutime.SetWindowTextA(str);
	
}

LRESULT CSatOrbitView::UpdateOrbit2D(WPARAM wParam,LPARAM lParam)
{
	CSatOrbitDoc* pDoc = GetDocument();
	GetDocData(pDoc);//���ĵ����еõ�����
  //SatOrbitDisplay(m_realStart,m_realEnd);//�����µ�켣
	if(sat_flag==0)
		return 0;
	cJulian a1,a2;
	if(m_Control.m_satState==0)
	{
	
	a1=cJulianNormal(m_realStart,m_Control.m_satState,m_Control.m_timeState);
	a2=cJulianNormal(m_realEnd,m_Control.m_satState,m_Control.m_timeState);
	}
	else
	{
	a1=cJulianNormal(m_Control.m_startCommonTime,m_Control.m_satState,m_Control.m_timeState);
	a2=cJulianNormal(m_Control.m_endCommonTime,m_Control.m_satState,m_Control.m_timeState);
    }
	  //ע����ĸ�ֵ�Ƿ�׼ȷ
	m_orbitData=SatOrbitCompute(a1,a2,m_Control.m_spanTime);
	return 0;
}

LRESULT CSatOrbitView::DocUpdate(WPARAM wParam,LPARAM lParam)
{
	CSatOrbitDoc* pDoc = GetDocument();
	GetDocData(pDoc);//���ĵ����еõ�����
    Invalidate(FALSE);
	return 0;
}

