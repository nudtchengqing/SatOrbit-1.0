// SatOrbit3D.cpp : ʵ���ļ�
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


// CSatOrbit3D �Ի���

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


// CSatOrbit3D ��Ϣ�������

int CSatOrbit3D::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_pDC = new CClientDC(this);		//Windows��ǰ�Ļ�ͼ�豸�����Ķ���ָ��
	ASSERT(m_pDC!=NULL);
    if(!OpenGLInit(m_pDC,m_hglrc))		//��ʼ��OpenGL����ʾ���
		return -1;

	InitData();//��ʼ������
	listBaseFont=InitializeFont("����",15,wglGetCurrentDC());//����һ��������ʾ�б�Ļ�׼ID

	return 0;
}

void CSatOrbit3D::OnDestroy()
{
	CDialog::OnDestroy();
		OpenGLExit(m_pDC,m_hglrc);				//�ͷ���Դ
       ClearFont(listBaseFont);//��������
	   ///////////////////////���ٶ�ʱ��
	if(m_realFlag==1)
		KillTimer(3);
	if(m_timeFlag==1)
	   KillTimer(4);

	// TODO: �ڴ˴������Ϣ����������
}

void CSatOrbit3D::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//�����ӿڴ�С
	glViewport(0,0,cx,cy);

	glMatrixMode(GL_PROJECTION);	//ͶӰ�任
	glLoadIdentity();
   
	if(cx <= cy)	//���ݴ��ڴ�С��������ͶӰ����
		glOrtho(-20.0,20.0,-20.0*(GLfloat)cy/(GLfloat)cx,
			20.0*(GLfloat)cy/(GLfloat)cx,-50.0,50.0);
	else
		glOrtho(-20.0*(GLfloat)cx/(GLfloat)cy,
			20.0*(GLfloat)cx/(GLfloat)cy,-20.0,20.0,-50.0,50.0);


	glMatrixMode(GL_MODELVIEW);		//����䲻���٣�����ͼ�ο�����Ч��
	glLoadIdentity();			//���ñ任ģʽΪģ�ͱ任
}

void CSatOrbit3D::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	if(m_hglrc)					//���õ�ǰ��ͼ�豸ΪOpenGL���豸�龰����
		wglMakeCurrent(m_pDC->GetSafeHdc(),m_hglrc);
	else
		return;
	
	///////////////////////////////////////////////////////////////////////////////////////
	      InitGL();                     //��ʼOPENGL����
		  glDrawBuffer(GL_BACK);		//ָ���ں�̨�����л���ͼ��
	    //  glLoadIdentity();			//��ʼ���任����
		    TimeControl();              //ʱ����ƣ���Ҫ�Ƕ�ʱ��������
		  	glPushMatrix();
		  Camera();                     //�ӵ����
          DrawScene();                   //������Ⱦ
		  glDisable(GL_BLEND);
          LightControl();                 //�ƹ���ƣ���Ҫ��Ĭ�Ϲ�Դ��̫����Դ
          glPopMatrix();
		SwapBuffers(wglGetCurrentDC());	//����ǰ�󻺴�
}
void CSatOrbit3D::InitGL()
{
	////////////////////////////////////////////////////////
	GLfloat mat_ambient[]= { 0.2f, 0.2f, 0.2f, 1.0f };//���ʵ�����
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
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//�������
	glEnable(GL_BLEND);                              //����͸��
    glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);                        //������Ȳ���
	switch(m_map3dIndex)                            //3Dλͼ����ͼ��ѡ�����
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
		FILE *Filebitmap=NULL;									// �ļ����
	if (!Filename)										// ȷ���ļ������ṩ
	{  
		return NULL;									// ���û�ṩ������ NULL
	}

	Filebitmap=fopen(Filename,"r");	// ���Դ��ļ�
	
	if (Filebitmap)	// �ļ�����ô?
	{  
	
		fclose(Filebitmap);	
								// �رվ��
		return auxDIBImageLoadA(Filename);				// ����λͼ������ָ��
	}
	return NULL;										// �������ʧ�ܣ����� NULL
}

int CSatOrbit3D::LoadGLTextures(CHAR *Filename)
{
      	int Status=FALSE;									// ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage[1];					// ��������Ĵ洢�ռ�
	memset(TextureImage,0,sizeof(void *)*1);			// ��ָ����Ϊ NULL
	// ����λͼ��������޴������λͼû�ҵ����˳�
	//if (TextureImage[0]=LoadBMP("images/earth.bmp"))
	if (TextureImage[0]=LoadBMP(Filename))
	{
		Status=TRUE;									// �� Status ��Ϊ TRUE
		glGenTextures(1, &texture[0]);					// ��������
     
		// ʹ������λͼ�������� �ĵ�������
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// ��������

		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
	}

	if (TextureImage[0])								// �����Ƿ����
	{
		if (TextureImage[0]->data)						// ����ͼ���Ƿ����
		{
			free(TextureImage[0]->data);				// �ͷ�����ͼ��ռ�õ��ڴ�
		}

		free(TextureImage[0]);							// �ͷ�ͼ��ṹ
	}
	return Status;	
	// ���� Status
}

/////////////////////
//�õ����ֵ�ĸ������κ���ʱ����������ϵ�ĵ�����λͼ����ĵ���
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
//������ά����
void CSatOrbit3D::DrawEarth(CommonTime m_realTime)
{
	double m_GMST=rad2deg(GetGMST(m_realTime));

    glPushMatrix();
	glEnable(GL_TEXTURE_2D);//��������
	glColor3d(1.0,1.0,1.0); //��ɫ�����ã��������ʱ��ɫͬ�������ɫһ��
	glBindTexture(GL_TEXTURE_2D,texture[0]); //������

	glRotatef(90,0,0,1);//���ݵ�ͼ���������λ��
    glRotatef(GLfloat(m_GMST),0,0,1);
	 
	GLUquadricObj* q=gluNewQuadric();//����openGLʵ�ÿ��еĶ���������ƺ������л���
	gluQuadricDrawStyle(q,GLU_FILL);
	gluQuadricNormals(q,GLU_SMOOTH);
    gluQuadricTexture(q,GL_TRUE);
	gluSphere(q,scale*RE,32,32);

	gluDeleteQuadric(q);//�ͷŶ��������ָ��
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
//���ǹ���Ļ���
void CSatOrbit3D::DrawOrbit(CommonTime m_start,CommonTime m_end)
{
	if(sat_flag==0)
		return;
	else
	{   if(m_orbit3D.empty())
	         return;
		vector<Point3D> re=m_orbit3D;
		glPushMatrix();
		switch(m_orbitColorIndex)          //�����ɫ��ѡ��
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
//���Ǻ����ǶԵ��渲�ǵĻ���
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
	posX=m_satCover.getSatPosECI().getPoint3D().x;//���ǵ�����
	posY=m_satCover.getSatPosECI().getPoint3D().y;
	posZ=m_satCover.getSatPosECI().getPoint3D().z;
	/////////////////////////////////////////////////�йظ��ǻ��Ƶ�����ϵ�����Ĳ���
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
////////////////////////////////////////////���ǻ���
	 glTranslatef(0,0,GLfloat(r*scale));
	 glColor3f(0.5f,0.5f,0.0f);
  	 auxSolidCube(300*scale);
	 glColor3f(0.f,0.f,1.0f);
	 auxSolidBox(2000*scale,250*scale,0.1);
//////////////////////////////////////////
	 //���ǻ���
	 if(m_cover3dShow==1)
	 {
	 glTranslatef(0,0,GLfloat(-r*scale));

	 glTranslatef(0.0f,0.0f,GLfloat((traslateZ)*scale));
	 switch(m_coverColorIndex) //������ʾ��ɫѡ��
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
	 	//����Բ׶���ʾ����
	  GLUquadricObj* qq=gluNewQuadric();//����openGLʵ�ÿ��еĶ���������ƺ������л���
	  gluQuadricDrawStyle(qq,GLU_FILL);
	  gluQuadricNormals(qq,GLU_SMOOTH);
	  //gluQuadricTexture(qq,GL_TRUE);
      gluCylinder(qq,radii*scale,0,height*scale,256,100);
	  gluDeleteQuadric(qq);//�ͷŶ��������ָ��
	 }
	glPopMatrix();
	

}
/////////////////////////////////////////
//�����Ļ���
void CSatOrbit3D::DrawScene()
{
     CommonTime m_time;
	if(m_satState==0)///ʵʱ
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
//���ݳ�ʼ�� 
void CSatOrbit3D::InitData()
{
	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //�õ��ĵ���ָ��
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
//���ݸ���
LRESULT CSatOrbit3D::UpdateData3D(WPARAM wParam,LPARAM lParam)
{
	CMainFrame*   pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;  
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //�õ��ĵ���ָ��
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
    CSatOrbitDoc*   pDoc=(CSatOrbitDoc*)pFrame->GetActiveDocument();  //�õ��ĵ���ָ��
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
//ʱ��Ŀ���
void CSatOrbit3D::TimeControl()
{
	if(m_satState==0)//ʵʱ
	{
		if(m_realFlag==0)//���û�����ö�ʱ��
		{
			SetTimer(3,100,NULL);
			m_realFlag=1;
		}
	   
	}
	else     //ģ��
	{
		if(m_realFlag==1)//����Ѿ�������ʵʱ״̬ʱ�Ķ�ʱ��
		{
			KillTimer(3);
			m_realFlag=0;
		}
		if(m_play==0)  //ģ�����ã����ǻص�ģ��ʱ��Ŀ�ʼʱ�䴦
		{
			if(m_timeFlag==1)//����Ѿ�������ģ��״̬�Ķ�ʱ��
			{
				KillTimer(4);
				m_timeFlag=0;
			}

			m_simuTime=m_startCommonTime;
			
		}
		
		if(m_play==1&&m_timeFlag!=1)//ģ�ⲥ��
		{
			SetTimer(4, 100, NULL);
		    m_timeFlag=1;
		}
		if(m_play==2&&m_timeFlag==1)//ģ��ֹͣ
		{
			    KillTimer(4);
				m_timeFlag=0;
		}
		if(m_play==3) //ģ��ǰ��
		{
			if(m_timeFlag==1)//����Ѿ�������ģ��Ķ�ʱ��
			{
				KillTimer(4);
				m_timeFlag=0;
			}
			
			m_simuTime=CommonTimeAddMinute(m_simuTime,m_playTimeAdd);
			m_play=2;

		}
		if(m_play==4)//ģ�����
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
//�ƹ����
void CSatOrbit3D::LightControl()
{
	//glDisable(GL_LIGHTING);
	CommonTime m_time;
	if(m_satState==0)///ʵʱ
	     m_time=m_realTime;
	 else 
	{
		m_time=m_simuTime;
	}
	Point3D sunpos=SunPos(m_time);
	GLfloat light_diffuse[]= { 1.0, 1.0, 1.0, 1.0};//0�Ź�Դ(��̫����Դ)
	GLfloat light_position[] = {GLfloat(sunpos.x*scale),GLfloat(sunpos.y*scale),GLfloat(sunpos.z*scale), 0.0 };
	///////////////////////////////////////
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);//0�Ź�Դ����
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	if(0==light)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);//����0�Ź�Դ

}
//////////////////////////////////////////////
//////�ӵ�����,ͨ��������
void CSatOrbit3D::Camera()
{
	 glRotatef(m_xRotation, 1.0, 0.0, 0.0);
	 glRotatef(m_yRotation, 0.0, 1.0, 0.0);	 
}
/////////////////////////////////////////////////////////
//��ά����ϵ�Ļ��ƣ��ܸ���������ʾ�����λ��
void CSatOrbit3D::DrawCoord()
{
	glPushMatrix();
	glColor3d(1.0,0.0,0.0);//��ɫ����
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
//̫����ECI����ϵ�µ�λ�ü���
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
	///�ѵ�����һ�����崦��ͬ��ά�аѵ�����������������
	GeodeticToCartesian (&m_crd,&m_geo,RE,0);
    Point3D result;
	result.x=m_crd.x;
	result.y=m_crd.y;
	result.z=m_crd.z;
	return result;
}

void CSatOrbit3D::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
//�Ŵ�
void CSatOrbit3D::OnZoomOut()
{
	// TODO: �ڴ���������������
	scale+=0.0001;
	this->PostMessage( WM_PAINT);
}
//////////////////////////////////////////////
//��С
void CSatOrbit3D::OnZoomIn()
{
	// TODO: �ڴ���������������
	scale-=0.0001;
	this->PostMessage( WM_PAINT);
}
//����OnInitDialog��������Ҫ���������ӿ�ݷ�ʽ����Դ
BOOL CSatOrbit3D::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
  m_hAccel=::LoadAccelerators(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_SAT3DACCEL));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CSatOrbit3D::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
   if(m_hAccel!=NULL)
		if(::TranslateAccelerator(m_hWnd,m_hAccel,pMsg))
			return true;
	return CDialog::PreTranslateMessage(pMsg);
}


void CSatOrbit3D::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_LeftButtonDown = TRUE;
	m_LeftDownPos = point;

	CDialog::OnLButtonDown(nFlags, point);
}

void CSatOrbit3D::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_LeftButtonDown = FALSE;

	CDialog::OnLButtonUp(nFlags, point);
}

void CSatOrbit3D::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
//̫�����տ�����رյĿ���
void CSatOrbit3D::OnLightSun()
{
	// TODO: �ڴ���������������
	if(0==light)
		light=1;
	else
		light=0;
	InvalidateRect(NULL,FALSE);
// this->PostMessage( WM_PAINT);
}

void CSatOrbit3D::OnUpdateLightSun(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������

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



	// TODO: �ڴ˴������Ϣ����������
}
