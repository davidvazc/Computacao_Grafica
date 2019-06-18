
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

//--------------------------------- Definir cores
#define AZUL     0.0, 0.0, 1.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0, 1.0
#define AMARELO  1.0, 1.0, 0.0, 1.0
#define VERDE    0.0, 1.0, 0.0, 1.0
#define LARANJA  0.8, 0.6, 0.1, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY1    0.2, 0.2, 0.2, 1.0
#define GRAY2    0.9, 0.9, 0.9, 1.0
#define PRETO    0.0, 0.0, 0.0, 1.0

#define MAX_PARTICLES 10000

#include "RgbImage.h"
#define   PI 3.14159

//================================================================================
//===========================================================Variaveis e constantes

int loop;
float slowdown = 0.95;
float speed = 0.0;
bool isRaining;

typedef struct
{
    float   size;
    float   life;
    float   fade;
    float   r,g,b;
    float   x,y,z;
    float   vel;
    float   gravity;
}Particle;

Particle particles[MAX_PARTICLES];


int cam_flag=0;
//------------------------------------------------------------ Sistema Coordenadas + objectos
GLint		wScreen=1600, hScreen=1000;		//.. janela (pixeis)
GLfloat		xC=10.0, yC=10.0, zC=10.0;		//.. Mundo  (unidades mundo)

//------------------------------------------------------------ Observador
GLfloat  rVisao=3.0, aVisao=0.5*PI, ayVisao=0.5*PI, incVisao=0.1;
GLfloat  angPersp=109.0;
GLfloat  obsPini[] ={1, 1.0, 0.5*xC};
GLfloat  obsPfin[] ={obsPini[0]-rVisao*cos(aVisao), obsPini[1], obsPini[2]-rVisao*sin(aVisao)};


//=========================================================== FACES DA MESA
GLboolean   frenteVisivel=1;
static GLuint     cima[] = {8,11, 10,  9};
//?? falta a esquerda
//?? falta a direita
//=================================================================== TEXTURAS

//------------------------------------------------------------ Skybox
GLUquadricObj*  sky  = gluNewQuadric ( );
GLfloat         sizeSky  = 50;

//------------------------------------------------------------ Texturas
GLuint   texture[10];
RgbImage imag;

//luzes
GLint   noite=1;
GLfloat luzGlobalCor[4]={1.0,1.0,1.0,1.0};

GLint   ligaLuz=1;



GLfloat materialColorObjetos[] = {1.0f, 1.0f, 1.0f, 1.0f};
void initLights(void){
    //ÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖ Ambiente
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);

    //ÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖÖ Tecto
    GLfloat localCor[4] ={0.6,0.6,0,1.0};
    GLfloat localCorDif[4] ={ 1, 1, 1, 1.0};
    GLfloat localPos[4] ={-15, 30.0, 0, 1.0};//posica luz
    GLfloat localAttCon =1.0;
    GLfloat localAttLin =0.05;
    GLfloat localAttQua =0.0;

    glLightfv(GL_LIGHT0, GL_POSITION,      localPos );
    glLightfv(GL_LIGHT0, GL_AMBIENT,       localCor );
    glLightfv(GL_LIGHT0, GL_DIFFUSE,       localCorDif );
    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, localAttCon);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION,   localAttLin) ;
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION,localAttQua) ;

}

GLfloat tam=2.0;
static GLfloat vertices[]={
	// x=tam (Esquerda)
        -tam,  -tam,  tam,	// 0
        -tam,   tam,  tam,	// 1
        -tam,   tam, -tam,	// 2
        -tam,  -tam, -tam,	// 3
	// Direita
         tam,  -tam,  tam,	// 4
         tam,   tam,  tam,	// 5
         tam,   tam, -tam,	// 6
         tam,  -tam, -tam,	// 7
	// (Cima
        -tam,  tam,  tam,	// 8
        -tam,  tam, -tam,	// 9
         tam,  tam, -tam,	// 10
         tam,  tam,  tam,	// 11
};

static GLfloat normais[] = {
    // x=tam (Esquerda)
	  -1.0,  0.0,  0.0,
      -1.0,  0.0,  0.0,
      -1.0,  0.0,  0.0,
      -1.0,  0.0,  0.0,
	// x=tam (Direita)
	   1.0,  0.0,  0.0,
       1.0,  0.0,  0.0,
       1.0,  0.0,  0.0,
       1.0,  0.0,  0.0,
	// y=tam (Cima)
	   0.0,  1.0,  0.0,
       0.0,  1.0,  0.0,
       0.0,  1.0,  0.0,
       0.0,  1.0,  0.0,
};
//------------------------------------------------------------ Cores
static GLfloat cores[]={
0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 1.000000, 0.000000
};

static GLfloat arrayTexture[]={
0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1,  0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1, 0,0, 1,0, 1,1, 0,1
};
void initTexturas()
{
	//----------------------------------------- SKY
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
  imag.LoadBmpFile("Sky.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, 3,
	imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	//----------------------------------------- CLARA
     glGenTextures(1, &texture[0]);
     glBindTexture(GL_TEXTURE_2D, texture[0]);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
     imag.LoadBmpFile("clara.bmp");
     glTexImage2D(GL_TEXTURE_2D, 0, 3,
                  imag.GetNumCols(),
                  imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                  imag.ImageData());
     // textura partes de cima

     glGenTextures(1, &texture[1]);
     glBindTexture(GL_TEXTURE_2D, texture[1]);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     imag.LoadBmpFile("escura.bmp");
     glTexImage2D(GL_TEXTURE_2D, 0, 3,
                  imag.GetNumCols(),
                  imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                  imag.ImageData());

     //----------------------------------------- textura paredes departamentos
     glGenTextures(1, &texture[2]);
     glBindTexture(GL_TEXTURE_2D, texture[2]);
     glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
     imag.LoadBmpFile("DEI_PORTA.bmp");
     glTexImage2D(GL_TEXTURE_2D, 0, 3,
                  imag.GetNumCols(),
                  imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                  imag.ImageData());




}

void init_particles(int i)
{
    particles[i].life = 1.0f;
    particles[i].fade = float(rand()%100)/1000.0f+0.003f;
    
    // Posição
    
    particles[i].x = (float) -(xC * 4 ) + (rand() % 60) ;
    particles[i].y = 60.0f;
    particles[i].z = (float) -(xC * 4 ) + (rand() % 60) ;
    
    particles[i].r = 1.0f;
    particles[i].g = 1.0f;
    particles[i].b = 1.0f;
    
    particles[i].vel = speed;
    particles[i].gravity = -0.8;//-0.8;
}

void draw_rain() {
    float x, y, z;
    for (loop = 0; loop < MAX_PARTICLES; loop=loop+1) {
        if (particles[loop].life > 0.0f) {
            x = particles[loop].x;
            y = particles[loop].y;
            z = particles[loop].z;
            
            
            if ( (x > -(0.1) and x < (0.1) ) and ((z > -(0.1) and z < (0.1) )) )
            {
                init_particles(loop);
                continue;
            }
        
            
            
            
            // Draw particles
            glLineWidth(1);
            glColor4f(1.0,1.0,1.0,1.0);
            glBegin(GL_LINES);
            glVertex3f(x, y, z);
            glVertex3f(x, y+0.3, z);
            glEnd();
            
            // Velocidade de descida com gravidade
            particles[loop].y += particles[loop].vel / (slowdown*100);
            particles[loop].vel += particles[loop].gravity;
            // Fade
            particles[loop].life -= particles[loop].fade;
            
            if (particles[loop].y <= -0) {
                particles[loop].life = -1.0;
            }
            //Revive
            if (particles[loop].life < 0.0) {
                init_particles(loop);
            }
        }
    }
}





//================================================================================
//=========================================================================== INIT
void inicializa(void)
{
	glClearColor(BLACK);		//Apagar
	glEnable(GL_DEPTH_TEST);	//Profundidade
	glShadeModel(GL_SMOOTH);	//Interpolacao de cores

//  glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
  // glEnable ( GL_COLOR_MATERIAL ) ;
   //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  initTexturas();
  glEnable(GL_TEXTURE_2D);
	//glEnable(GL_CULL_FACE);		//Faces visiveis
	//glCullFace(GL_BACK);		//Mostrar so as da frente
/*
	glVertexPointer(3, GL_FLOAT, 0, vertices); //Vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glNormalPointer(GL_FLOAT, 0, normais);
    glEnableClientState(GL_NORMAL_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, cores);
    glEnableClientState(GL_COLOR_ARRAY);
    */

    initLights();

    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        init_particles(loop);
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);




}


void drawSkySphere(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[3]);
	glPushMatrix();
		glTranslatef(-15,0,-20);
		glRotatef (       -90, 1, 0, 0);
		gluQuadricDrawStyle ( sky, GLU_FILL   );
		gluQuadricNormals   ( sky, GLU_SMOOTH );
		gluQuadricTexture   ( sky, GL_TRUE    );
		gluSphere ( sky, sizeSky*1, 150, 150);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
};

static void drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;
  glEnable(GL_TEXTURE_2D);

  for (i = 5; i >= 0; i--) {
    if (i%2==0)
      glBindTexture(GL_TEXTURE_2D,texture[0]);
      else
      glBindTexture(GL_TEXTURE_2D,texture[1]);

    glPushMatrix();
      glBegin(type);
      glNormal3fv(&n[i][0]);
      glTexCoord2f(0,0);   glVertex3fv(&v[faces[i][0]][0]);
      glTexCoord2f(1,0);   glVertex3fv(&v[faces[i][1]][0]);
      glTexCoord2f(1,1);   glVertex3fv(&v[faces[i][2]][0]);
      glTexCoord2f(0,1);   glVertex3fv(&v[faces[i][3]][0]);
      glEnd();
    glPopMatrix();
  }
  glDisable(GL_TEXTURE_2D);
}
static void drawParedeEletro(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;
  glEnable(GL_TEXTURE_2D);

  for (i = 5; i >= 0; i--) {
    if (i%2==0)
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      else
      glBindTexture(GL_TEXTURE_2D,texture[1]);

    glPushMatrix();
      glBegin(type);
      glNormal3fv(&n[i][0]);
      glTexCoord2f(0,0);   glVertex3fv(&v[faces[i][3]][0]);
      glTexCoord2f(1,0);   glVertex3fv(&v[faces[i][2]][0]);
      glTexCoord2f(1,1);   glVertex3fv(&v[faces[i][1]][0]);
      glTexCoord2f(0,1);   glVertex3fv(&v[faces[i][0]][0]);
      glEnd();
    glPopMatrix();
  }
  glDisable(GL_TEXTURE_2D);
}
static void drawParedeDei(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;
  glEnable(GL_TEXTURE_2D);

  for (i = 5; i >= 0; i--) {
    if (i%2==0)
      glBindTexture(GL_TEXTURE_2D,texture[2]);
      else
      glBindTexture(GL_TEXTURE_2D,texture[1]);

    glPushMatrix();
      glBegin(type);
      glNormal3fv(&n[i][0]);
      glTexCoord2f(0,0);   glVertex3fv(&v[faces[i][0]][0]);
      glTexCoord2f(1,0);   glVertex3fv(&v[faces[i][1]][0]);
      glTexCoord2f(1,1);   glVertex3fv(&v[faces[i][2]][0]);
      glTexCoord2f(0,1);   glVertex3fv(&v[faces[i][3]][0]);
      glEnd();
    glPopMatrix();
  }
  glDisable(GL_TEXTURE_2D);
}

void APIENTRY glutCube(GLdouble size)
{
  drawBox(size, GL_QUADS);
}


void drawEixos()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo X
	glColor4f(VERMELHO);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0);
		glVertex3i(10, 0, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Y
	glColor4f(VERDE);
	glBegin(GL_LINES);
		glVertex3i(0,  0, 0);
		glVertex3i(0, 10, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo Z
	glColor4f(AZUL);
	glBegin(GL_LINES);
		glVertex3i( 0, 0, 0);
		glVertex3i( 0, 0,10);
	glEnd();

}
GLfloat sizeEscadasNormais=1,scaleEscadasNormais=10;
GLfloat sizeEscadasBig=3,scaleEscadasBig=10;
GLfloat scaleEscadasMini=scaleEscadasNormais/3;

GLfloat translateEscada =(sizeEscadasNormais*scaleEscadasNormais)/2;
GLfloat translateBigEscada = ((sizeEscadasBig*scaleEscadasBig)/2);
GLfloat translateEscadaMini = (sizeEscadasNormais*scaleEscadasMini)/2;
int nmr_escadas = 30;

void drawChao(){

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Chao y=0
	glEnable(GL_TEXTURE_2D);
	glColor4f(VERDE);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);  	 glVertex3i( -40,  -1, -1 );
			glTexCoord2f(1.0f, 0.0f); 	 glVertex3i( -40,   -1,  8 );
			glTexCoord2f(1.0f, 1.0f);    glVertex3i(  10,   -1,  8);
			glTexCoord2f(0.0f, 1.0f);    glVertex3i(  10,     -1,  -1);
		glEnd();
	glPopMatrix();


	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);  	 glVertex3i( 3,  16.5, -23 );
			glTexCoord2f(1.0f, 0.0f); 	 glVertex3i( 3,   16.5,  -16.5 );
			glTexCoord2f(1.0f, 1.0f);    glVertex3i(  10,   16.5,  -16.5);
			glTexCoord2f(0.0f, 1.0f);    glVertex3i(  10,     16.5,  -23);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);  	 glVertex3i( -40,  17, -23 );
			glTexCoord2f(1.0f, 0.0f); 	 glVertex3i( -40,   17,  -16 );
			glTexCoord2f(1.0f, 1.0f);    glVertex3i(  -33,   17,  -16);
			glTexCoord2f(0.0f, 1.0f);    glVertex3i(  -33,     17,  -23);
		glEnd();
	glPopMatrix();


	glDisable(GL_TEXTURE_2D);
}

void drawEscadaEsquerda(){
  glColor4f(VERMELHO);
  glTranslatef(-(2*translateBigEscada+translateEscada),0,-translateEscada);
  glScalef(scaleEscadasNormais,1,scaleEscadasNormais);
  glutCube(sizeEscadasNormais);
}
void drawEscadaEsquerdaMini(){
  glTranslatef(-(2*translateBigEscada+translateEscadaMini),0,-translateEscada);
  glScalef(scaleEscadasMini,1,scaleEscadasNormais);
  glutCube(sizeEscadasNormais);
}
void drawEscadaTraseiraEsquerda(){
  glColor4f(VERMELHO);
  glTranslatef(-(2*translateBigEscada+translateEscada),0,-translateEscada-6);
  glScalef(scaleEscadasNormais,1,scaleEscadasNormais);
  glutCube(sizeEscadasNormais);
}
void drawEscadaTraseira(){
  //glColor4f(AZUL);
  glTranslatef(translateEscada,0,-translateEscada-6);
  glScalef(scaleEscadasNormais,1,scaleEscadasNormais);
  glutCube(sizeEscadasNormais);
}
void drawEscada(){
  //glColor4f(AZUL);
  glTranslatef(translateEscada,0,-translateEscada);
  glScalef(scaleEscadasNormais,1,scaleEscadasNormais);
  glutCube(sizeEscadasNormais);
}
void drawEscadaMini(){
  glTranslatef(translateEscadaMini,0,-translateEscada);
  glScalef(scaleEscadasMini,1,scaleEscadasNormais);
  glutCube(sizeEscadasNormais);
}
void drawBigEscada(){
  glColor4f(AMARELO);
  glTranslatef(-translateBigEscada,1,-9);
  glScalef(scaleEscadasBig,1,6);

  glutCube(sizeEscadasBig);
}
void drawAllBigEscada(int x){
  int i;
  glPushMatrix();
  drawBigEscada();
  glPopMatrix();
  for(i=0;i<x;i++){
    glTranslatef(0,3,-3);
    glPushMatrix();
      //?? escala, rotacao, translacao ??7
    drawBigEscada();

    glPopMatrix();
  }
}

void desenhaParedes(){
	glPushMatrix();
  //DEI
  	glTranslatef(10,16,-18);
  	glScalef(1,35,54);
  	//glutCube(sizeEscadasNormais);
    drawParedeDei(sizeEscadasNormais, GL_QUADS);
  	glPopMatrix();

//eletro
  	glPushMatrix();
  	glTranslatef(-40,16,-18);
  	glScalef(1,35,54);
  	//glutCube(sizeEscadasNormais);
    drawParedeEletro(sizeEscadasNormais, GL_QUADS);
  	glPopMatrix();

}

void drawAllEscada(int x){
  int i;

  glPushMatrix();
  drawEscada();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-15,-1,4);
  glScalef(50,1,10);
  glutCube(sizeEscadasNormais);
  glPopMatrix();
  for(i=0;i<x;i++){

    if(i>15){
      glTranslatef(0,1,-1);
      glPushMatrix();
        //?? escala, rotacao, translacao ??7
      drawEscadaMini();
      glPopMatrix();
      glPushMatrix();
        //?? escala, rotacao, translacao ??7
      drawEscadaTraseira();
      glPopMatrix();
    }else{
      glTranslatef(0,1,-1);
      glPushMatrix();
        //?? escala, rotacao, translacao ??7
      drawEscada();
      glPopMatrix();
    }


  }
}

void drawAllEscadaEsquerda(int x){
  int i;
  glPushMatrix();
  drawEscadaEsquerda();
  glPopMatrix();
  for(i=0;i<x;i++){
    if(i>15){
      glTranslatef(0,1,-1);
      glPushMatrix();
        //?? escala, rotacao, translacao ??7
      drawEscadaEsquerdaMini();
      glPopMatrix();

      glPushMatrix();
        //?? escala, rotacao, translacao ??7
      drawEscadaTraseiraEsquerda();
      glPopMatrix();
    }else{
      glTranslatef(0,1,-1);
      glPushMatrix();
        //?? escala, rotacao, translacao ??7
      drawEscadaEsquerda();
      glPopMatrix();
    }

  }
}


void escadas_normais(){

  glPushMatrix();
  drawAllEscada(nmr_escadas-1);
  glPopMatrix();

  glPushMatrix();
  drawAllBigEscada(nmr_escadas/3-1);
  glPopMatrix();

  glPushMatrix();
  drawAllEscadaEsquerda(nmr_escadas-1);
  glPopMatrix();
}
void drone(){
  glPushMatrix();
  glTranslatef(obsPini[0],obsPini[1],obsPini[2]);

  glEnable(GL_CULL_FACE);		//Faces visiveis
  glutCube(1);
  glCullFace(GL_BACK);		//Mostrar so as da frente
  glDisable(GL_CULL_FACE);

  glPopMatrix();
}

void drawScene(){
  //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, materialColorObjetos);

  
  escadas_normais();
  desenhaParedes();
  draw_rain();
  drone();

}


void display(void){
	draw_rain();
  if (ligaLuz)
    glEnable(GL_LIGHT0);
  else
    glDisable(GL_LIGHT0);

	//================================================================= APaga ecran/profundidade
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	//================================================================= viewport 1
	glViewport (600, 0, 1000, 500);								// ESQUECER PoR AGORA
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angPersp, (float)wScreen/hScreen, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(obsPini[0], obsPini[1], obsPini[2], obsPfin[0], obsPfin[1], obsPfin[2], 0, 1, 0);
												// ESQUECER PoR AGORA
                        //desenhar objetos
  //drawChao();
  //drawEixos();
  glDisable(GL_DEPTH_TEST);
  drawSkySphere();
  glEnable(GL_DEPTH_TEST);
  drawScene();

  											// ESQUECER PoR AGORA
	//================================================================= view port 2
  glViewport(0,0,600,400);
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glOrtho(-50,50,-50,50,-50,50);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(-15,20,0,-15,0,0,0,0,-1);
  									// ESQUECER PoR AGORA


  //drawEixos();
	drawScene();

	drawSkySphere();
  //================================================================= viewport 3

  glViewport (0, 500, 800, 500);								// ESQUECER PoR AGORA
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (float)wScreen/hScreen, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(8, 30, 10, -20,0, -20, 0, 1, 0);
                        // ESQUECER PoR AGORA
                        //desenhar objetos

  //drawEixos();
  drawScene();

  //================================================================= cam 2

  drawSkySphere();

  glViewport (800, 500, 800, 500);								// ESQUECER PoR AGORA
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90, (float)wScreen/hScreen, 0.1, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(-38,30 ,10,0 ,0, -20, 0, 1, 0);
                        // ESQUECER PoR AGORA
                        //desenhar objetos



  //drawEixos();
  
  drawScene();
  
drawSkySphere();



	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Actualizacao
	glutSwapBuffers();

}
void updateVisao(){

	obsPfin[0] =obsPini[0]+rVisao*cos(aVisao);
	obsPfin[2] =obsPini[2]-rVisao*sin(aVisao);
	glutPostRedisplay();
}
//======================================================= EVENTOS
void keyboard(unsigned char key, int x, int y){


	switch (key) {
  case 'n':
  case 'N':

    noite=!noite;
    if (noite)
    { luzGlobalCor[0]=1.0;   luzGlobalCor[1]=1.0;   luzGlobalCor[2]=1.0; }
    else
    { luzGlobalCor[0]=0.4;   luzGlobalCor[1]=0.4;   luzGlobalCor[2]=0.4; }
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCor);
    glutPostRedisplay();
    break;
  case 'l':
  case 'L':
    ligaLuz=!ligaLuz;
    glutPostRedisplay();
    break;
	case 'f':
	case 'F':
		frenteVisivel=!frenteVisivel;
		//glutPostVERMELHOisplay();
		break;

	case 'A':
	case 'a':
		//??
		obsPini[1]--;
    obsPfin[1]--;
	break;

	case 'S':
	case 's':
		//??
	obsPini[1]++;
  obsPfin[1]++;
	break;

	case 'e':
	case 'E':
		ayVisao=ayVisao-0.1;
		obsPfin[2] =obsPini[2]+rVisao*cos(ayVisao)*PI;
		obsPfin[1] =obsPini[1]-rVisao*sin(ayVisao)*PI;
	break;

	case 'd':
	case 'D':
		ayVisao=ayVisao+0.1;
		obsPfin[2] =obsPini[2]+rVisao*cos(ayVisao)*PI;
		obsPfin[1] =obsPini[1]-rVisao*sin(ayVisao)*PI;
	break;
  case 'i':
  case 'I':
    obsPini[0]++;
    obsPini[0]++;
  break;
  case 'k':
  case 'K':
    obsPini[0]--;
    obsPini[0]--;
  break;




//--------------------------- Escape
	case 27:
		exit(0);
		break;
  }

  updateVisao();

}



void teclasNotAscii(int key, int x, int y){

  if(key == GLUT_KEY_UP) {
    obsPini[0]=obsPini[0]+incVisao*cos(aVisao);
    obsPini[2]=obsPini[2]-incVisao*sin(aVisao);
  }
  if(key == GLUT_KEY_DOWN) {
    obsPini[0]=obsPini[0]-incVisao*cos(aVisao);
    obsPini[2]=obsPini[2]+incVisao*sin(aVisao);
  }
  if(key == GLUT_KEY_LEFT)
    aVisao = (aVisao + 0.1) ;
  if(key == GLUT_KEY_RIGHT)
    aVisao = (aVisao - 0.1) ;



  updateVisao();



}

GLvoid resize(GLsizei width, GLsizei height)
{
    wScreen=width;
    hScreen=height;
    draw_rain();
}


//======================================================= MAIN
//======================================================= MAIN
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize (wScreen, hScreen);
	glutInitWindowPosition (300, 100);
	glutCreateWindow ("{jh,pjmm}@dei.uc.pt|       |FaceVisivel:'f'|      |Observador:'SETAS'|        |Andar-'a/s'|        |Rodar -'e/d'| ");

	inicializa();


	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
