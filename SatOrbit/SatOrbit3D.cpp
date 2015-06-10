// SatOrbit3D.cpp : 实现文件
//

#include "stdafx.h"
#include "SatOrbit.h"
#include "SatOrbit3D.h"

#include "SatOrbitDoc.h"
#include "MainFrm.h"
#include "OpenGLInit.h"
#include "set.h"
#include "cSun.h"
#include "CoordCovert.h"
//#include "cSatObject.h"
#include "cSatPointCover.h"


// CSatOrbit3D 对话框

IMPLEMENT_DYNAMIC(CSatOrbit3D, CDialog)

CSatOrbit3D::CSatOrbit3D(CWnd* pParent /*=NULL*/)
	: CDialog(CSatOrbit3D::IDD, pParent)
{
	scale      =0.001;
	sat_flag   =0;
	m_timeFlag =0;
	m_realFlag =0;
	//m_play=2;
	m_tle.line1="";
	m_tle.line2="";
	m_tle.name ="";

//	m_minAngle=10;

     CTime mt=CTime::GetCurrentTime(); 
	 m_realTime.year=mt.GetYear();
	 m_realTime.month=mt.GetMonth();
	 m_realTime.day=mt.GetDay();
	 m_realTime.hour=mt.GetHour();
	 m_realTime.minute=mt.GetMinute();
	 m_realTime.second=mt.GetSecond();
	 
	 m_simuPlayVIndex   =3;
	 m_playTimeAdd      =1/600.0;
	 m_realSatPos       =0;
	 m_realTimeLong     =2;
	 m_realOrbitstart   =m_realTime;
	 
	 m_realStart        =m_realTime;
	 m_realEnd          =CommonTimeAddMinute(m_realTime,m_realTimeLong*60);
	 
	 m_map3dIndex     =0;
	 m_coord3dShow    =1;
	 m_orbitColorIndex=0;
	 m_coverColorIndex=0;
	 m_cover3dShow    =0;
	 m_orbit3dShow    =1;
	
///////////////////////////////////////////////////////////////////////////
	 elevation           =0;
	 azimuth             =0;
     light               =0;
     m_LeftButtonDown    = FALSE;
	 m_RightButtonDown  = FALSE;
	 m_xRotation = 0.0f;
	 m_yRotation = 0.0f;


}

CSatOrbit3D::~CSatOrbit3D()
{
}

void CSatOrbit3D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSatOrbit3D, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SAT3D,UpdateData3D)
	ON_MESSAGE(WM_ORBIT3DUPDATE,UpdateDataOrbit3D)
	ON_WM_TIMER()
	ON_COMMAND(ID_ZOOM_OUT, &CSatOrbit3D::OnZoomOut)
	ON_COMMAND(ID_ZOOM_IN, &CSatOrbit3D::OnZoomIn)
//	ON_COMMAND(ID_LOOK_LEFT, &CSatOrbit3D::OnLookLeft)
//	ON_COMMAND(ID_LOOK_RIGHT, &CSatOrbit3D::OnLookRight)
//	ON_COMMAND(ID_LOOK_UP, &CSatOrbit3D::OnLookUp)
//	ON_COMMAND(ID_LOOK_DOWN, &CSatOrbit3D::OnLookDown)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_LIGHT_SUN, &CSatOrbit3D::OnLightSun)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SUN, &CSatOrbit3D::OnUpdateLightSun)
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()


// CSatOrbit3D 消息处理程序

int CSatOrbit3D::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_pDC = new CClientDC(this);		//Windows当前的绘图设备上下文对象指针
	ASSERT(m_pDC!=NULL);
    if(!OpenGLInit(m_pDC,m_hglrc))		//初始化OpenGL的显示情况
		return -1;

	InitData();//初始化数据
	listBaseFont=InitializeFont("宋体",15,wglGetCurrentDC());//创建一个字体显示列表的基准ID

	return 0;
}

void CSatOrbit3D::OnDestroy()
{
	CDialog::OnDestroy();
		OpenGLExit(m_pDC,m_hglrc);				//释放资源
       ClearFont(listBaseFont);//销毁文字
	   ///////////////////////销毁定时器
	if(m_realFlag==1)
		KillTimer(3);
	if(m_timeFlag==1)
	   KillTimer(4);

	// TODO: 在此处添加消息处理程序代码
}

void CSatOrbit3D::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//设置视口大小
	glViewport(0,0,cx,cy);

	glMatrixMode(GL_PROJECTION);	//投影变换
	glLoadIdentity();
   
	if(cx <= cy)	//根据窗口大小调整正射投影矩阵
		glOrtho(-20.0,20.0,-20.0*(GLfloat)cy/(GLfloat)cx,
			20.0*(GLfloat)cy/(GLfloat)cx,-50.0,50.0);
	else
		glOrtho(-20.0*(GLfloat)cx/(GLfloat)cy,
			20.0*(GLfloat)cx/(GLfloat)cy,-20.0,20.0,-50.0,50.0);


	glMatrixMode(GL_MODELVIEW);		//这二句不能少，否则图形看不出效果
	glLoadIdentity();			//设置变换模式为模型变换
}

void CSatOrbit3D::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	if(m_hglrc)					//设置当前绘图设备为OpenGL的设备情景对象
		wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else
		return;
	
	///////////////////////////////////////////////////////////////////////////////////////
	      InitGL();                     //初始OPENGL环境
		  glDrawBuffer(GL_BACK);		//指定在后台缓存中绘制图形
	    //  glLoadIdentity();			//初始化变换矩阵
		    TimeControl();              //时间控制，主要是定时器的设置
		  	glPushMatrix();
		  Camera();                     //视点控制
          DrawScene();                   //场景渲染
		  glDisable(GL_BLEND);
          LightControl();                 //灯光控制，主要有默认光源和太阳光源
          glPopMatrix();
		SwapBuffers(wglGetCurrentDC());	//交换前后缓存
}
void CSatOrbit3D::InitGL()
{
	////////////////////////////////////////////////////////
	GLfloat mat_ambient[]= { 0.2f, 0.2f, 0.2f, 1.0f };//材质的设置
	GLfloat mat_diffuse[]= { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat mat_specular[] = { 0.f, 0.f, 0.f, 1.0f };
	GLfloat mat_shininess[] = { 50.0f };
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glEnable(GL_COLOR_MATERIAL);
	////////////////////////////////////////
	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//启动混合
	glEnable(GL_BLEND);                              //启动透明
    glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);                        //启动深度测试
	switch(m_map3dIndex)                            //3D位图纹理图的选择加载
	{
	case 0:
		{LoadGLTextures("images/earth.bmp"); break;}   
	case 1:
		{LoadGLTextures("images/world_blue.bmp"); break;}
	case 2:
		{LoadGLTextures("images/land_shallow.bmp"); break;}
	default:
          {LoadGLTextures("images/earth.bmp"); break;}
	}

}
AUX_RGBImageRec *CSatOrbit3D::LoadBMP(CHAR *Filename)
{
		FILE *Filebitmap=NULL;									// 文件句柄
	if (!Filename)										// 确保文件名已提供
	{  
		return NULL;									// 如果没提供，返回 NULL
	}

	Filebitmap=fopen(Filename,"r");	// 尝试打开文件
	
	if (Filebitmap)	// 文件存在么?
	{  
	
		fclose(Filebitmap);	
								// 关闭句柄
		return auxDIBImageLoadA(Filename);				// 载入位图并返回指针
	}
	return NULL;										// 如果载入失败，返回 NULL
}

int CSatOrbit3D::LoadGLTextures(CHAR *Filename)
{
      	int Status=FALSE;									// 状态指示器
	AUX_RGBImageRec *TextureImage[1];					// 创建纹理的存储空间
	memset(TextureImage,0,sizeof(void *)*1);			// 将指针设为 NULL
	// 载入位图，检查有无错误，如果位图没找到则退出
	//if (TextureImage[0]=LoadBMP("images/earth.bmp"))
	if (TextureImage[0]=LoadBMP(Filename))
	{
		Status=TRUE;									// 将 Status 设为 TRUE
		glGenTextures(1, &texture[0]);					// 创建纹理
     
		// 使用来自位图数据生成 的典型纹理
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// 生成纹理

		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
	}

	if (TextureImage[0])								// 纹理是否存在
	{
		if (TextureImage[0]->data)						// 纹理图像是否存在
		{
			free(TextureImage[0]->data);				// 释放纹理图像占用的内存
		}

		free(TextureImage[0]);							// 释放图像结构
	}
	return Status;	
	// 返回 Status
}

/////////////////////
//得到春分点的格林尼治恒星时，用于坐标系的调整，位图纹理的调整
double CSatOrbit3D::GetGMST(CommonTime m_realTime)
{
     cSatOrbitKEY satKEY(m_tle);
	 cJulian m_julian(m_realTime.year,m_realTime.month,
		              m_realTime.day,m_realTime.hour,
		              m_realTime.minute,m_realTime.second);
	 if(m_timeState==0)
		{
	      m_julian.addHour(-8);
		}
	 cSatObject satObject(satKEY,m_julian);
	 return satObject.getGMST();
	 

}
//////////////////////////////////////////////////////////
//绘制三维地球
void CSatOrbit3D::DrawEarth(CommonTime m_realTime)
{
	double m_GMST=rad2deg(GetGMST(m_realTime));

    glPushMatrix();
	glEnable(GL_TEXTURE_2D);//启用纹理
	glColor3d(1.0,1.0,1.0); //颜色的设置，避免更新时颜色同轨道的颜色一样
	glBindTexture(GL_TEXTURE_2D,texture[0]); //绑定纹理

	glRotatef(90,0,0,1);//根据地图调整地球的位置
    glRotatef(GLfloat(m_GMST),0,0,1);
	 
	GLUquadricObj* q=gluNewQuadric();//采用openGL实用库中的二次曲面绘制函数进行绘制
	gluQuadricDrawStyle(q,GLU_FILL);
	gluQuadricNormals(q,GLU_SMOOTH);
    gluQuadricTexture(q,GL_TRUE);
	gluSphere(q,scale*RE,32,32);

	gluDeleteQuadric(q);//释放二次曲面的指针
	glDisable(GL_TEXTURE_2D); 
    glPopMatrix(); 
}
////////////////////////////////////////////////////////////////
vector<Point3D> CSatOrbit3D::Orbit3DCompute(cJulian m_start,cJulian m_end,double m_spanTime)
{
	cSatOrbitKEY sat(m_tle);
	cSatTrack satTrack(sat,m_start,m_end,m_spanTime);
	return satTrack.getTrack3D();


}
//卫星轨道的绘制
void CSatOrbit3D::DrawOrbit(CommonTime m_start,CommonTime m_end)
{
	if(sat_flag==0)
		return;
	else
	{   if(m_orbit3D.empty())
	         return;
		vector<Point3D> re=m_orbit3D;
		glPushMatrix();
		switch(m_orbitColorIndex)          //轨道颜色的选择
		{
		case 0:
			{glColor3d(1.0,1.0,1.0);break;}
		case 1:
             {glColor3d(1.0,1.0,0.0);break;}
		case 2:
			{glColor3d(1.0,0.0,0.0);break;}
		case 3:
			{glColor3d(0.0,1.0,0.0);break;}
		case 4:
            {glColor3d(0.0,0.0,1.0);break;}
		default:
			{glColor3d(1.0,1.0,0.0);break;}
		}
	    glBegin(GL_LINE_STRIP); 
		for(int i=0;i<int(re.size()-1);i++)
		{
			glVertex3f((GLfloat)(re[i].x*scale),
				       (GLfloat)(re[i].y*scale),
					   (GLfloat)(re[i].z*scale));
		}
        glEnd();
		glPopMatrix();
	}
}

////////////////////////////////////////////////////////
//卫星和卫星对地面覆盖的绘制
void CSatOrbit3D::DrawSatCover(CommonTime m_realTime)
{
      if(sat_flag==0)
		return;
	cJulian m_real;
	 m_real=cJulianNormal(m_realTime,m_satState,m_timeState);
	cSatOrbitKEY m_sat(m_tle);
	cSatPointCover m_satCover(m_sat,m_real,m_minAngle,100);
	
	glPushMatrix();
	double posX,posY,posZ;
	posX=m_satCover.getSatPosECI().getPoint3D().x;//卫星的坐标
	posY=m_satCover.getSatPosECI().getPoint3D().y;
	posZ=m_satCover.getSatPosECI().getPoint3D().z;
	/////////////////////////////////////////////////有关覆盖绘制的坐标系调整的参数
	double x=AcTan(posY,posX);
	double y=AcTan(posZ,sqrt(posX*posX+posY*posY));
	double r=sqrt(posX*posX+posY*posY+posZ*posZ);
	/*double traslateZ=RE*RE/r;
	double radii=(r-traslateZ)*tan(m_satCover.getSatPointAngle());
    double height=r-traslateZ;*/
	double traslateZ=RE*cos(PI/2-m_satCover.getSatPointAngle()-deg2rad(m_satCover.getAngle()));
	double height=r-traslateZ;
	double radii=height*tan(m_satCover.getSatPointAngle());
     glRotatef(90, 0.f, 1.f, 0.f);
     glRotatef(GLfloat(-x*180/PI), 1.f, 0.f, 0.f);
	 glRotatef(GLfloat(-y*180/PI), 0.f, 1.f, 0.f);
////////////////////////////////////////////卫星绘制
	 glTranslatef(0,0,GLfloat(r*scale));
	 glColor3f(0.5f,0.5f,0.0f);
  	 auxSolidCube(300*scale);
	 glColor3f(0.f,0.f,1.0f);
	 auxSolidBox(2000*scale,250*scale,0.1);
//////////////////////////////////////////
	 //覆盖绘制
	 if(m_cover3dShow==1)
	 {
	 glTranslatef(0,0,GLfloat(-r*scale));

	 glTranslatef(0.0f,0.0f,GLfloat((traslateZ)*scale));
	 switch(m_coverColorIndex) //覆盖显示颜色选择
	 {
	 case 0:
		 {glColor4f(1.0f, 1.f, 0.f, 0.5f);break;}
	 case 1:
		 {glColor4f(1.0f, 0.f, 0.f, 0.5f);break;}
	 case 2:
		 {glColor4f(0.0f, 1.f, 0.f, 0.5f);break;}
	 case 3:
		 {glColor4f(0.0f, 0.f, 1.f, 0.5f);break;}
	 case 4:
		 {glColor4f(0.0f, 0.f, 0.f, 0.5f);break;}
	 default:
		 {glColor4f(1.0f, 1.f, 0.f, 0.5f);break;}
	 }
	 	//采用圆锥体表示覆盖
	  GLUquadricObj* qq=gluNewQuadric();//采用openGL实用库中的二次曲面绘制函数进行绘制
	  gluQuadricDrawStyle(qq,GLU_FILL);
	  gluQuadricNormals(qq,GLU_SMOOTH);
	  //gluQuadricTexture(qq,GL_TRUE);
      gluCylinder(qq,radii*scale,0,height*scale,256,100);
	  gluDeleteQuadric(qq);//释放二次曲面的指针
	 }
	glPopMatrix();
	

}
/////////////////////////////////////////
//场景的绘制
void CSatOrbit3D::DrawScene()
{
     CommonTime m_time;
	if(m_satState==0)///实时
	{
		m_time=m_realTime;
		if(m_orbit3dShow==1)
			DrawOrbit(m_realStart,m_realEnd);
	 }
	else 
	{
		m_time=m_simuTime;
		if(m_orbit3dShow==1)
			 DrawOrbit(m_startCommonTime,m_endCommonTime);
	}
   if(m_coord3dShow==1)
	  DrawCoord();
	DrawEarth(m_time);
	DrawSatCover(m_time);

}
////////////////////////////////////////
//数据初始化 
void CSatOrbit3D::InitData()
{
	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //得到文档的指针
	sat_flag             =pDoc->sat_flag;
	m_startCommonTime    =pDoc->m_Control.m_startCommonTime;
	m_endCommonTime      =pDoc->m_Control.m_endCommonTime;
	m_spanTime           =pDoc->m_Control.m_spanTime;
	m_satState           =pDoc->m_Control.m_satState;
	m_timeState          =pDoc->m_Control.m_timeState;
	m_tle                =pDoc->m_tle;	
	m_play               =pDoc->m_play;
	m_minAngle           =pDoc->m_Control.m_minAngle;
	////////////////////////////////////
	m_map3dIndex         =pDoc->m_Control.m_map3dIndex;
	m_coord3dShow        =pDoc->m_Control.m_coord3dShow;
	m_orbitColorIndex    =pDoc->m_Control.m_orbitColorIndex;;
	m_coverColorIndex   =pDoc->m_Control.m_coverColorIndex;
	m_cover3dShow       =pDoc->m_Control.m_cover3dShow;
	m_orbit3dShow       =pDoc->m_Control.m_orbit3dShow;
	/////////////////
	CTime mt=CTime::GetCurrentTime(); 
	 m_realTime.year  =mt.GetYear();
	 m_realTime.month =mt.GetMonth();
	 m_realTime.day   =mt.GetDay();
	 m_realTime.hour  =mt.GetHour();
	 m_realTime.minute=mt.GetMinute();
	 m_realTime.second=mt.GetSecond();
	 m_simuTime=m_startCommonTime;
    

	
}
//////////////////////////////////////////////////////////////////
//数据更新
LRESULT CSatOrbit3D::UpdateData3D(WPARAM wParam,LPARAM lParam)
{
	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //得到文档的指针
	/*sat_flag             =pDoc->sat_flag;
	m_startCommonTime    =pDoc->m_Control.m_startCommonTime;
	m_endCommonTime      =pDoc->m_Control.m_endCommonTime;
	m_spanTime           =pDoc->m_Control.m_spanTime;
	m_satState           =pDoc->m_Control.m_satState;
	m_timeState          =pDoc->m_Control.m_timeState;
	m_tle                =pDoc->m_tle;
	m_play               =pDoc->m_play;
	m_minAngle           =pDoc->m_Control.m_minAngle;
	////////////////////////////////////
	m_map3dIndex         =pDoc->m_Control.m_map3dIndex;
	m_coord3dShow        =pDoc->m_Control.m_coord3dShow;
	m_orbitColorIndex    =pDoc->m_Control.m_orbitColorIndex;;
	m_coverColorIndex   =pDoc->m_Control.m_coverColorIndex;
	m_cover3dShow       =pDoc->m_Control.m_cover3dShow;
	m_orbit3dShow       =pDoc->m_Control.m_orbit3dShow;
	m_simuPlayVIndex    =pDoc->m_Control.m_simuPlayVIndex;
	m_realSatPos        =pDoc->m_Control.m_realSatPos;
	m_realTimeLong      =pDoc->m_Control.m_realTimeLong;
	int n=0;
	switch(m_simuPlayVIndex)
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

	if(m_realSatPos==0)
	 {m_realStart=m_realOrbitstart;
	  m_realEnd=CommonTimeAddMinute(m_realOrbitstart,m_realTimeLong*60);
	 }
	 else if(m_realSatPos==1)
	 {m_realStart=CommonTimeAddMinute(m_realOrbitstart,-m_realTimeLong*60/2);
	  m_realEnd=CommonTimeAddMinute(m_realOrbitstart,m_realTimeLong*60/2);
     }
	 else
	 {m_realStart=CommonTimeAddMinute(m_realOrbitstart,-m_realTimeLong*60);
	   m_realEnd=m_realOrbitstart;
	 }*/

	m_play               =pDoc->m_play;
	 InvalidateRect(NULL,FALSE);
	return 0;
}

LRESULT CSatOrbit3D::UpdateDataOrbit3D(WPARAM wParam,LPARAM lParam)
{
	//UpdateData3D(wParam,lParam);
    CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //得到文档的指针
	sat_flag             =pDoc->sat_flag;
	m_startCommonTime    =pDoc->m_Control.m_startCommonTime;
	m_endCommonTime      =pDoc->m_Control.m_endCommonTime;
	m_spanTime           =pDoc->m_Control.m_spanTime;
	m_satState           =pDoc->m_Control.m_satState;
	m_timeState          =pDoc->m_Control.m_timeState;
	m_tle                =pDoc->m_tle;
	m_play               =pDoc->m_play;
	m_minAngle           =pDoc->m_Control.m_minAngle;
	////////////////////////////////////
	m_map3dIndex         =pDoc->m_Control.m_map3dIndex;
	m_coord3dShow        =pDoc->m_Control.m_coord3dShow;
	m_orbitColorIndex    =pDoc->m_Control.m_orbitColorIndex;;
	m_coverColorIndex   =pDoc->m_Control.m_coverColorIndex;
	m_cover3dShow       =pDoc->m_Control.m_cover3dShow;
	m_orbit3dShow       =pDoc->m_Control.m_orbit3dShow;
	m_simuPlayVIndex    =pDoc->m_Control.m_simuPlayVIndex;
	m_realSatPos        =pDoc->m_Control.m_realSatPos;
	m_realTimeLong      =pDoc->m_Control.m_realTimeLong;

	m_simuTime=pDoc->m_simuTime;

	int n=0;
	switch(m_simuPlayVIndex)
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

	if(m_realSatPos==0)
	 {m_realStart=m_realOrbitstart;
	  m_realEnd=CommonTimeAddMinute(m_realOrbitstart,m_realTimeLong*60);
	 }
	 else if(m_realSatPos==1)
	 {m_realStart=CommonTimeAddMinute(m_realOrbitstart,-m_realTimeLong*60/2);
	  m_realEnd=CommonTimeAddMinute(m_realOrbitstart,m_realTimeLong*60/2);
     }
	 else
	 {m_realStart=CommonTimeAddMinute(m_realOrbitstart,-m_realTimeLong*60);
	   m_realEnd=m_realOrbitstart;
	 }

	//this->PostMessage( WM_PAINT);
	 

	cJulian a1,a2;
	if(m_satState==0)
	{
	   a1=cJulianNormal(m_realStart,m_satState,m_timeState);
	   a2=cJulianNormal(m_realEnd,m_satState,m_timeState);
	}
	else
	{
       a1=cJulianNormal(m_startCommonTime,m_satState,m_timeState);
	   a2=cJulianNormal(m_endCommonTime,m_satState,m_timeState);
	}
	

	m_orbit3D=Orbit3DCompute(a1,a2,m_spanTime);
	InvalidateRect(NULL,FALSE);
	return 0;

}
//////////////////////////////////////////////////////////
//时间的控制
void CSatOrbit3D::TimeControl()
{
	if(m_satState==0)//实时
	{
		if(m_realFlag==0)//如果没有设置定时器
		{
			SetTimer(3,100,NULL);
			m_realFlag=1;
		}
	   
	}
	else     //模拟
	{
		if(m_realFlag==1)//如果已经设置了实时状态时的定时器
		{
			KillTimer(3);
			m_realFlag=0;
		}
		if(m_play==0)  //模拟重置，卫星回到模拟时间的开始时间处
		{
			if(m_timeFlag==1)//如果已经设置了模拟状态的定时器
			{
				KillTimer(4);
				m_timeFlag=0;
			}

			m_simuTime=m_startCommonTime;
			
		}
		
		if(m_play==1&&m_timeFlag!=1)//模拟播放
		{
			SetTimer(4, 100, NULL);
		    m_timeFlag=1;
		}
		if(m_play==2&&m_timeFlag==1)//模拟停止
		{
			    KillTimer(4);
				m_timeFlag=0;
		}
		if(m_play==3) //模拟前进
		{
			if(m_timeFlag==1)//如果已经设置了模拟的定时器
			{
				KillTimer(4);
				m_timeFlag=0;
			}
			
			m_simuTime=CommonTimeAddMinute(m_simuTime,m_playTimeAdd);
			m_play=2;

		}
		if(m_play==4)//模拟后退
		{
			if(m_timeFlag==1)
			{
				KillTimer(4);
				m_timeFlag=0;
			}
			m_simuTime=CommonTimeAddMinute(m_simuTime,-m_playTimeAdd);
			m_play=2;
			

		}
		
	}
}

/////////////////////////////////////////////////////////
//灯光控制
void CSatOrbit3D::LightControl()
{
	//glDisable(GL_LIGHTING);
	CommonTime m_time;
	if(m_satState==0)///实时
	     m_time=m_realTime;
	 else 
	{
		m_time=m_simuTime;
	}
	Point3D sunpos=SunPos(m_time);
	GLfloat light_diffuse[]= { 1.0, 1.0, 1.0, 1.0};//0号光源(即太阳光源)
	GLfloat light_position[] = {GLfloat(sunpos.x*scale),GLfloat(sunpos.y*scale),GLfloat(sunpos.z*scale), 0.0 };
	///////////////////////////////////////
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//0号光源设置
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	if(0==light)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);//开启0号光源

}
//////////////////////////////////////////////
//////视点设置,通过鼠标控制
void CSatOrbit3D::Camera()
{
	 glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	 glRotatef(m_yRotation, 0.0, 1.0, 0.0);	 
}
/////////////////////////////////////////////////////////
//三维坐标系的绘制，能更清晰的显示物体的位置
void CSatOrbit3D::DrawCoord()
{
	glPushMatrix();
	glColor3d(1.0,0.0,0.0);//颜色设置
	glBegin(GL_LINES); 
	glVertex3f(0.0f,   0.0f,   0.0f); 
	glVertex3f(GLfloat(10000*scale),   0.0f,   0.0f); 
	glEnd(); 
	glRasterPos3f(GLfloat(10000*scale),   0.0f,   0.0f);
           PrintString(listBaseFont,"X");
	glBegin(GL_LINES); 
	glVertex3f(0.0f,   0.0f,   0.0f); 
	glVertex3f(0.0f,   GLfloat(10000*scale),   0.0f); 
	glEnd(); 
	glRasterPos3f(0.0f,   GLfloat(10000*scale),   0.0f);
           PrintString(listBaseFont,"Y");
	glBegin(GL_LINES); 
	glVertex3f(0.0f,   0.0f,   0.0f); 
	glVertex3f(0.0f,   0.0f,   GLfloat(10000*scale)); 
	glEnd(); 
	glRasterPos3f(0.0f,   0.0f,   GLfloat(10000*scale));
           PrintString(listBaseFont,"Z");
glPopMatrix();

}
///////////////////////////////////////////////////////
//太阳的ECI坐标系下的位置计算
Point3D CSatOrbit3D::SunPos(CommonTime m_realTime)
{
	cJulian m_j;
	   m_j=cJulianNormal(m_realTime,m_satState,m_timeState);
	cSun m_sun(m_j);
    CRDGEODETIC m_geo;
	m_geo.longitude =m_sun.GetSunPos().getPoint3D().x;
	m_geo.latitude  =m_sun.GetSunPos().getPoint3D().y;
    m_geo.height    =10*RE;

	double m_gmst=rad2deg(GetGMST(m_realTime));
	m_geo.longitude+=m_gmst;
	if(m_geo.longitude<-180)
        m_geo.longitude+=360;
	if(m_geo.longitude>180)
		m_geo.longitude-=360;
   m_geo.longitude  =deg2rad(m_geo.longitude);
   m_geo.latitude   =deg2rad(m_geo.latitude);
	CRDCARTESIAN m_crd;
	///把地球看做一个球体处理，同二维中把地球看做椭球处理存在误差
	GeodeticToCartesian (&m_crd,&m_geo,RE,0);
    Point3D result;
	result.x=m_crd.x;
	result.y=m_crd.y;
	result.z=m_crd.z;
	return result;
}

void CSatOrbit3D::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent==3)
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
		if(CommonTimeCompare(m_simuTime,m_endCommonTime)==0)
			 m_simuTime=m_startCommonTime;
    }

   // this->PostMessage( WM_PAINT);
	InvalidateRect(NULL,FALSE);

	CDialog::OnTimer(nIDEvent);
}
////////////////////////////////////////////
//放大
void CSatOrbit3D::OnZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	scale+=0.0001;
	this->PostMessage( WM_PAINT);
}
//////////////////////////////////////////////
//缩小
void CSatOrbit3D::OnZoomIn()
{
	// TODO: 在此添加命令处理程序代码
	scale-=0.0001;
	this->PostMessage( WM_PAINT);
}
//重载OnInitDialog函数，主要是添加能添加快捷方式的资源
BOOL CSatOrbit3D::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
  m_hAccel=::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SAT3DACCEL));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CSatOrbit3D::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
   if(m_hAccel!=NULL)
		if(::TranslateAccelerator(m_hWnd,m_hAccel,pMsg))
			return true;
	return CDialog::PreTranslateMessage(pMsg);
}


void CSatOrbit3D::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void CSatOrbit3D::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LeftButtonDown = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CSatOrbit3D::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_LeftButtonDown)
	{
		m_yRotation -= (float)(m_LeftDownPos.x - point.x)/3.0f;
		m_xRotation -= (float)(m_LeftDownPos.y - point.y)/3.0f;
		m_LeftDownPos = point;
		InvalidateRect(NULL,FALSE);
	}

	CDialog::OnMouseMove(nFlags, point);
}
///////////////////////////////////////////
//太阳光照开启与关闭的控制
void CSatOrbit3D::OnLightSun()
{
	// TODO: 在此添加命令处理程序代码
	if(0==light)
		light=1;
	else
		light=0;
	InvalidateRect(NULL,FALSE);
// this->PostMessage( WM_PAINT);
}

void CSatOrbit3D::OnUpdateLightSun(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码

	  pCmdUI->SetCheck(light==1);
}

void CSatOrbit3D::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	//CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	   ASSERT(pPopupMenu   !=   NULL);   
          //   Check   the   enabled   state   of   various   menu   items.   
    
          CCmdUI   state;   
          state.m_pMenu   =   pPopupMenu;   
          ASSERT(state.m_pOther   ==   NULL);   
          ASSERT(state.m_pParentMenu   ==   NULL);   
    
          //   Determine   if   menu   is   popup   in   top-level   menu   and   set   m_pOther   to   
          //   it   if   so   (m_pParentMenu   ==   NULL   indicates   that   it   is   secondary   popup).   
          HMENU   hParentMenu;   
          if   (AfxGetThreadState()->m_hTrackingMenu   ==   pPopupMenu->m_hMenu)   
                  state.m_pParentMenu   =   pPopupMenu;         //   Parent   ==   child   for   tracking   popup.   
          else   if   ((hParentMenu   =   ::GetMenu(m_hWnd))   !=   NULL)   
          {   
                  CWnd*   pParent   =   this;   
                        //   Child   windows   don't   have   menus--need   to   go   to   the   top!   
                  if   (pParent   !=   NULL   &&   
                        (hParentMenu   =   ::GetMenu(pParent->m_hWnd))   !=   NULL)   
                  {   
                        int   nIndexMax   =   ::GetMenuItemCount(hParentMenu);   
                        for   (int   nIndex   =   0;   nIndex   <   nIndexMax;   nIndex++)   
                        {   
                          if   (::GetSubMenu(hParentMenu,   nIndex)   ==   pPopupMenu->m_hMenu)   
                          {   
                                  //   When   popup   is   found,   m_pParentMenu   is   containing   menu.   
                                  state.m_pParentMenu   =   CMenu::FromHandle(hParentMenu);   
                                  break;   
                          }   
                        }   
                  }   
          }   
    
          state.m_nIndexMax   =   pPopupMenu->GetMenuItemCount();   
          for   (state.m_nIndex   =   0;   state.m_nIndex   <   state.m_nIndexMax;   
              state.m_nIndex++)   
          {   
                  state.m_nID   =   pPopupMenu->GetMenuItemID(state.m_nIndex);   
                  if   (state.m_nID   ==   0)   
                        continue;   //   Menu   separator   or   invalid   cmd   -   ignore   it.   
    
                  ASSERT(state.m_pOther   ==   NULL);   
                  ASSERT(state.m_pMenu   !=   NULL);   
                  if   (state.m_nID   ==   (UINT)-1)   
                  {   
                        //   Possibly   a   popup   menu,   route   to   first   item   of   that   popup.   
                        state.m_pSubMenu   =   pPopupMenu->GetSubMenu(state.m_nIndex);   
                        if   (state.m_pSubMenu   ==   NULL   ||   
                          (state.m_nID   =   state.m_pSubMenu->GetMenuItemID(0))   ==   0   ||   
                          state.m_nID   ==   (UINT)-1)   
                        {   
                          continue;               //   First   item   of   popup   can't   be   routed   to.   
                        }   
                        state.DoUpdate(this,   TRUE);       //   Popups   are   never   auto   disabled.   
                  }   
                  else   
                  {   
                        //   Normal   menu   item.   
                        //   Auto   enable/disable   if   frame   window   has   m_bAutoMenuEnable   
                        //   set   and   command   is   _not_   a   system   command.   
                        state.m_pSubMenu   =   NULL;   
                        state.DoUpdate(this,   FALSE);   
                  }   
    
                  //   Adjust   for   menu   deletions   and   additions.   
                  UINT   nCount   =   pPopupMenu->GetMenuItemCount();   
                  if   (nCount   <   state.m_nIndexMax)   
                  {   
                        state.m_nIndex   -=   (state.m_nIndexMax   -   nCount);   
                        while   (state.m_nIndex   <   nCount   &&   
                          pPopupMenu->GetMenuItemID(state.m_nIndex)   ==   state.m_nID)   
                        {   
                          state.m_nIndex++;   
                        }   
                  }   
                  state.m_nIndexMax   =   nCount;   
          }   



	// TODO: 在此处添加消息处理程序代码
}
