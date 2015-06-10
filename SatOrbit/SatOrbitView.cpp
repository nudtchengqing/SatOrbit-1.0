// SatOrbitView.cpp : CSatOrbitView 类的实现
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

// CSatOrbitView 构造/析构

CSatOrbitView::CSatOrbitView()
{   
	m_Control=InitializeOption(m_Control);
	
	m_pImage=NULL;//背景位图信息变量初始化
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
	if(m_pImage!=NULL)//析构函数中释放资源
           delete[] m_pImage;
    if(m_pImageBack!=NULL)
		{
			delete[] m_pImageBack->data;
			delete m_pImageBack;
		}
}

BOOL CSatOrbitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN ;
	return CView::PreCreateWindow(cs);
}

// CSatOrbitView 绘制

void CSatOrbitView::OnDraw(CDC* /*pDC*/)
{
	CSatOrbitDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	/////////设置当前绘图设备
	if(m_hglrc)
		wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else
		return;
	//GetDocData(pDoc);//从文档类中得到数据
    DrawBackground();//二维背景图

	if(m_Control.m_satState==0)//实时
	{  
		if(m_Control.m_satPointOrbitShow==1)
	         SatOrbitDisplay(m_realStart,m_realEnd);//画星下点轨迹
	
		if(m_realFlag==0)
		{
			SetTimer(0,1000,NULL);
			m_realFlag=1;
		}

	   if(m_Control.m_sunShow==1)//画太阳
		SatSunDisplay(m_realTime);
	   SatObjectDisplay(m_realTime);//画卫星
	  
	   
	}
	else//模拟
	{
		if(m_Control.m_satPointOrbitShow==1)
	         SatOrbitDisplay(m_Control.m_startCommonTime,m_Control.m_endCommonTime);//画星下点轨迹
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
		
		if(m_Control.m_sunShow==1)//画太阳
		   SatSunDisplay(m_simuTime);
		SatObjectDisplay(m_simuTime);

		SetEditdata(CommonTime2CString(m_simuTime));
		
	}
	
    SwapBuffers(wglGetCurrentDC());//交换前后缓存
	glDrawBuffer(GL_FRONT);//绘制前景
}


// CSatOrbitView 诊断

#ifdef _DEBUG
void CSatOrbitView::AssertValid() const
{
	CView::AssertValid();
}

void CSatOrbitView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSatOrbitDoc* CSatOrbitView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSatOrbitDoc)));
	return (CSatOrbitDoc*)m_pDocument;
}
#endif //_DEBUG


// CSatOrbitView 消息处理程序

int CSatOrbitView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pDC=new CClientDC(this);
	ASSERT(m_pDC!=NULL);
    if(!OpenGLInit(m_pDC,m_hglrc))		//初始化OpenGL的显示情况
		return -1;

	//读入位图
	if(m_pImageBack!=NULL)
	{
	delete[] m_pImageBack->data;
	delete m_pImageBack;
	m_pImageBack=NULL;
	}
	if(m_pImage!=NULL)
		delete[] m_pImage;
	//读入位图文件
	
	m_pImageBack=auxDIBImageLoad(_T("res/World.bmp"));
		
    if(m_pImageBack==NULL)
		return TRUE;

	//记录位图的大小
	m_iImgWidth=m_pImageBack->sizeX;
	m_iImgHeight=m_pImageBack->sizeY;
  
	//分配位图数据所占的内存
	m_pImage=new BYTE[(2*m_iImgWidth)*(2*m_iImgHeight)*3];
   //将位图的数据按照1：1的比例传送到数组中
	gluScaleImage(GL_RGB,m_pImageBack->sizeX,m_pImageBack->sizeY,
		GL_UNSIGNED_BYTE,m_pImageBack->data,
		m_iImgWidth,m_iImgHeight,GL_UNSIGNED_BYTE,m_pImage);
//////////////////////////////////////////////////////////////////////
    
	listBase=InitializeFont("宋体",15,wglGetCurrentDC());//字体的创建
	
	return 0;
}

void CSatOrbitView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	OpenGLExit(m_pDC,m_hglrc);
	ClearFont(listBase);//销毁字体
	if(m_realFlag==1)//销毁定时器
		KillTimer(0);
	if(m_timeFlag==1)
	   KillTimer(1);
}

void CSatOrbitView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_hglrc)
		wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else 
		return;
	m_iViewWidth=cx;
	m_iViewHeight=cy;
	glViewport(0,0,cx,cy);//设置视口大小
    glMatrixMode(GL_PROJECTION);//设置变换模式为投影模式
	glLoadIdentity();//初始化投影变换矩阵
    gluOrtho2D(0,m_iViewWidth,0,m_iViewHeight);//根据赋的值调整正射投影矩阵
	glMatrixMode(GL_MODELVIEW);//设置变换模式为模型变换
	glLoadIdentity();//初始化模型变换矩阵
	
}

BOOL CSatOrbitView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
     return true;
	//return CView::OnEraseBkgnd(pDC);
}


ControlParameter CSatOrbitView::InitializeOption(ControlParameter m_Control)//初始化参数
{
	CSatOrbitDoc* pDoc = GetDocument();
	return pDoc->InitializeOption(m_Control);

}
void CSatOrbitView::DrawBackground()
{
    glDrawBuffer(GL_BACK);
///////////////////////////////////////////////
	glLoadIdentity();//初始化变换矩阵

	glClearColor(1.0,1.0,1.0,1.0f);//清除背景颜色
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_pImage!=NULL)

	{
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);//指定在后台缓存中绘制
	
	glPixelZoom(float(m_iViewWidth)/m_iImgWidth,//////根据视口和位图来调整位图的缩放比例
		float(m_iViewHeight)/m_iImgHeight);

	glRasterPos2i(0,0);////指定位图的绘制位置

	glDrawPixels(m_iImgWidth,m_iImgHeight,GL_RGB,GL_UNSIGNED_BYTE,m_pImage);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}
  if(m_Control.m_gridShow==1)
	{
	  DrawBackgroundGrid();
	  DrawBackgroundText();
	}
   glFinish();//结束整个绘制
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
    for(int i=0;i<12;i++)//利用循环画经线
	  {
	      glVertex2d(i*width /12.0,0);
	      glVertex2d(i*width /12.0,height);
	    }
	   ////////////////////////////////////////////////////////
	  for(int j=0;j<6;j++)//利用循环画纬线
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
	   int longitude=-150;//便于数据显示的中间变量参数
	   int latitude=-60;
       int flag=0;
	   int flat=0;
	   CString strl,strla;
       char *lon;
	   for(int k=1;k<12;k++)//经度
	   {
		   strl.Format("%d",longitude);
		   lon= (LPTSTR)(LPCTSTR)strl;
           glRasterPos2f(GLfloat(k*width/12.0-10),0.0);
           PrintString(listBase,lon);
		   longitude+=30;
	   }
	   for(int h=1;h<=5;h++)//纬度
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
//轨道计算
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
        Point2D frontPoint,backPoint;//划线时的起点和下一点
        glPushMatrix();
        glColor3d(1.0,1.0,0.0);//颜色设置
		for(int i=0;i<int(re.size()-1);i++)
		{
			//frontPoint=Geo2Grid(GetViewRec().x,GetViewRec().y,re[i]);
			//backPoint=Geo2Grid(GetViewRec().x,GetViewRec().y,re[i+1]);
			frontPoint=re[i];
			backPoint=re[i+1];

			glBegin(GL_LINE_STRIP);
			if(sqrt(pow((frontPoint.x-backPoint.x),2)+pow((frontPoint.y-backPoint.y),2))>GetViewRec().x*3/4)//如果在视窗口中的距离相隔太远则不相连
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
	return Geo2Grid(GetViewRec().x,GetViewRec().y,sat_geo);//坐标转换

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
    glPointSize(8);//点大小设置
	glBegin(GL_POINTS); //以一个红点表示卫星
	  glColor3f (1.0f, 0.0f, 0.0f); 
	  glVertex2f(GLfloat(sat_grid.x),GLfloat(sat_grid.y)); 
    glEnd();
    glPopMatrix();

    if(m_Control.m_satShow==1)//卫星名称的现实
	{
	  char *p = (char *)m_tle.name.c_str();  //卫星名称
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
	 if(m_Control.m_satCoverShow==1)//卫星覆盖是否显示
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
        switch(m_Control.m_satCoverColorIndex)//覆盖颜色的选择
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
			if(sqrt(pow((frontPoint.x-backPoint.x),2)+pow((frontPoint.x-backPoint.x),2))>GetViewRec().x)//如果在视窗口中的距离相隔太远则不相连
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
        glColor4f(1.0,1.0,0.0,0.5);//黄色
		glTranslatef(GLfloat(sun_grid.x),GLfloat(sun_grid.y),-0.0); //平移函数，确定圆心
		int n=100;
        glBegin(GL_POLYGON); //以一个黄色的圆代替太阳         
          for(int i=0;i<n ;i++)    
		     glVertex2f(GLfloat(10*cos(2*PI/n*i)),GLfloat( 10*sin(2*PI/n*i)));  //根据角度画圆
        glEnd();
		glPopMatrix();
}
void CSatOrbitView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	GetDocData(pDoc);//从文档类中得到数据
  //SatOrbitDisplay(m_realStart,m_realEnd);//画星下点轨迹
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
	  //注意类的赋值是否准确
	m_orbitData=SatOrbitCompute(a1,a2,m_Control.m_spanTime);
	return 0;
}

LRESULT CSatOrbitView::DocUpdate(WPARAM wParam,LPARAM lParam)
{
	CSatOrbitDoc* pDoc = GetDocument();
	GetDocData(pDoc);//从文档类中得到数据
    Invalidate(FALSE);
	return 0;
}

