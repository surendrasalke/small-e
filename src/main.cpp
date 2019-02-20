#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib> //for usleep()
#include "smalle.hpp"
#include "PPMImageIO.h"
// Defining the ESCAPE Key Code
#define ESCAPE 27
// Defining the DELETE Key Code
#define DELETE 127
#define PARAMETER_NUMBER 30
using namespace std;
Smalle smalle;
GLvoid InitGL();
GLvoid ReshapeGL (GLsizei Width, GLsizei Height);
GLvoid DisplayGL();
GLvoid KeyPressedGL(unsigned char key, GLint x, GLint y);
GLvoid SpecialKeyPressedGL(GLint key, GLint x, GLint y);
GLvoid RenderGL(int argc, char** argv);
GLvoid RenderParallelopiped(GLfloat x,GLfloat y,GLfloat z,GLfloat texcoor);
void play();
float l = -20.0f, r = 20.0f, b = -10.0f, t = 10.0f, n = -5.0f, f = -35.0f;
int cam_smalle;
float curr_x,curr_y,curr_z;
static GLuint texName[5];//unsigned binary int. atleast 32 bit
GLubyte *tex1;
bool lightON0=false,lightON1=false,lightON2=false;
FILE *fp,*fp1;
int cnt1=0,cnt2=0;
typedef struct
{
  float x,y,z;
  float ax,ay,az;
}camera_t;
camera_t camera;
void InitGL (void)
{
  camera.x=0;camera.y=0;camera.z=n+8;
  camera.ax=0;camera.ay=0;camera.az=0;
  cam_smalle=0;
  curr_x=(l+r)/2;curr_y=b;curr_z=(n+f)/2;
  glClearColor(0.0, 0.0, 0.0, 1.0f);
  glClearDepth(1.0);
  glutFullScreen();
  glEnable(GL_DEPTH_TEST); // Enable depth test (z buffering)
  //glShadeModel(GL_SMOOTH);
  glPolygonMode(GL_FRONT,GL_FILL);
  //glEnable(GL_CULL_FACE); // Enable polygon face culling
  //glCullFace(GL_BACK); // Cull surfaces which is oriented toward back (z direction)
  //prevent white light
  glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
  glEnable ( GL_COLOR_MATERIAL );
 
  // Now enable lighting
  glEnable(GL_LIGHTING);

  const GLfloat whiteLightColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
  glGenTextures(5,texName);  //glGenTextures returns n texture names in textures
  glBindTexture(GL_TEXTURE_2D, texName[0]); // glBindTexture(target,texture)    apply texture to target
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//void glTexParameterf(target,  symbolic_pname,  value_of_pname);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  int height,width;
  char filename[]="wall.ppm";
  bool success = PPMReader(filename,height,width,tex1);
  /*glTexImage2D(target_tex,i level,i internalFormat,  GLsizei  width,  GLsizei  height,  GLint  border,  
    GLenum  format,  GLenum  type,  const GLvoid *  data);
    level:Specifies the level-of-detail number. Level 0 is the base image level. Level n is the nth mipmap reduction image.
    internalFormat:Number of color components [1 2 3 4]
    width/height : in the order of 2^n+2
    border:[0/1]
    format:Specifies the format of the pixel data.
    type:datatype of pixel data
    data:Specifies a pointer to the image data in memory.
                
  */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1;

  glBindTexture(GL_TEXTURE_2D, texName[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  char filename1[]="floor.ppm";
  success = PPMReader(filename1,height,width,tex1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1;

  glBindTexture(GL_TEXTURE_2D, texName[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  char filename2[]="ceiling.ppm";
  success = PPMReader(filename2,height,width,tex1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1; 

  glBindTexture(GL_TEXTURE_2D, texName[3]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  char filename3[]="table.ppm";
  success = PPMReader(filename3,height,width,tex1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1;

  smalle.init_list();
}


GLvoid ReshapeGL (GLsizei Width, GLsizei Height)
{
  //Prevent a divison by zero
  if (Height == 0) Height=1;
  //Set the viewport to the whole of the current window
  glViewport (0, 0, Width, Height);
  glMatrixMode (GL_PROJECTION);
  //Load the identity matrix
  glLoadIdentity ();
  //Setup a perspective projection
  //Parameters are in the order -
  //vertical field of view angle - 60 degrees
  //Aspect Ratio i.e. width/height of window - 1.0
  //Near clipping plane distance - -1.0
  //Far clipping plane distance - -100.0
  gluPerspective(70.0f,(GLfloat)Width/(GLfloat)Height,1.0f,100.0f);
  //Change the matrix mode to modelview
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  //gluLookAt(0,0,-1,0,0,-3,0,1,0);
}
//GL display callback - does all the drawing
GLvoid light()
{
  /*by tuning light off and setting lights at the eyes of a smalle we can create amasing animation*/
  // set up a white light
  if(lightON0==1){
    glEnable(GL_LIGHT0); // Turn on the light 0
  }
  else{
    glDisable(GL_LIGHT0);
  }
  GLfloat light_ambient0[] = { 0.5, 0.5, 1.0, 1.0 };
  GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };
 
  float x,y,z,ax,ay,az;
  x=curr_x+smalle.xpos;
  y=curr_y+5+smalle.ypos;
  z=curr_z+smalle.zpos;
  ax=x+sin(-smalle.yrot*3.14/180)*-100+sin(-smalle.head_rot_side*3.14/180)*-100+sin(-smalle.head_rot_lr*3.14/180)*100;
  ay=y+sin(-(smalle.l_neck_rot+smalle.u_neck_rot)*3.14/180)*100+cos(smalle.head_rot_side*3.14/180)*10+sin(-smalle.head_rot_fb*3.14/180)*100;
  az=z+cos(smalle.yrot*3.14/180)*100+cos((smalle.l_neck_rot+smalle.u_neck_rot)*3.14/180)*10+cos(smalle.head_rot_fb*3.14/180)*10+cos(smalle.head_rot_lr*3.14/180)*-10;
  //GLfloat light_position0[] = { 0,5,0,1 };
  GLfloat light_position0[] = {x,y,z,1};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE,light_diffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR,light_specular0);
  glLightfv(GL_LIGHT0, GL_POSITION,light_position0);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0);
  //GLfloat spot_direction[] = { 0.0, -0.5, -1.0 };
  GLfloat spot_direction[] = {ax,ay,az};
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
  if(lightON1==1){
    glEnable(GL_LIGHT1); // Turn on the light 0
  }
  else{
    glDisable(GL_LIGHT1);
  }
  GLfloat light_ambient1[] = { 0.4, 0.4, 0.4, 1.0 };
  GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position1[] = { 0,5,0,0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE,light_diffuse1);
  glLightfv(GL_LIGHT1, GL_SPECULAR,light_specular1);
  glLightfv(GL_LIGHT1, GL_POSITION,light_position1);
  
 
  //Render the light source
  float iradius_light = 0.0, oradius_light = (r-l)/25;
  int lslices = 20;
  int lrings = 20;
  float lwhite[] = {1.0f,1.0f,1.0f};
  GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  glPushMatrix();
  glTranslatef((l+r)/2,t,(n+f)/2);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,lwhite);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, lightSpecular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 800);
  glutSolidSphere(oradius_light,32,32);
  glPopMatrix();

  if(lightON2==1){
    glEnable(GL_LIGHT2); // Turn on the light 0
  }
  else{
    glDisable(GL_LIGHT2);
  }
  GLfloat light_ambient2[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse2[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_specular2[] = { 1.0, 0.0, 0.0, 1.0 };
  GLfloat light_position2[] = {(l+r)/2,t+5,(n+f)/2 };
  glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE,light_diffuse2);
  glLightfv(GL_LIGHT2, GL_SPECULAR,light_specular2);
  glLightfv(GL_LIGHT2, GL_POSITION,light_position2);
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 20.0);
  GLfloat spot_direction2[] = { 0.0, -0.5, 0.0 };
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2);
}
GLvoid RenderRoom()
{
  float white[] = {1.0f,1.0f,1.0f};  
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texName[1]);
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,white);
  GLfloat roomSpecular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
  glMaterialfv(GL_FRONT, GL_SPECULAR, roomSpecular);
  glMaterialf(GL_FRONT, GL_SHININESS, 5);


  //reference: opengl  forum
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  int wslices = 100, wstrips = 100;
  float qwidth = (f-n)/wslices, qheight = (t-b)/wstrips;
  float twidth = 1.0/wslices, theight = 1.0/wstrips;

  // Render the right wall
  glNormal3f(-1.0,0.0,0.0);
  for(int i = 0; i < wstrips; i++)
    {
      glBegin(GL_QUAD_STRIP);	
      for(int j = 0; j <= wslices; j++)
	{
	  glTexCoord2f(i*theight,j*twidth);
	  glVertex3f(r,b+i*qheight,n+j*qwidth);
	  glTexCoord2f((i+1)*theight,j*twidth);
	  glVertex3f(r,b+(i+1)*qheight,n+j*qwidth);
	}
      glEnd();
    }
	
  // Render the left wall
  glNormal3f(1.0,0.0,0.0);
  for(int i = 0; i < wstrips; i++)
    {
      glBegin(GL_QUAD_STRIP);	
      for(int j = 0; j <= wslices; j++)
	{
	  glTexCoord2f((i+1)*theight,j*twidth);
	  glVertex3f(l,b+(i+1)*qheight,n+j*qwidth);
	  glTexCoord2f(i*theight,j*twidth);
	  glVertex3f(l,b+i*qheight,n+j*qwidth);
	}
      glEnd();
    }
  // Render the rear wall
  qwidth = (r-l)/wslices, qheight = (t-b)/wstrips;
  glNormal3f(0.0,0.0,1.0);
  for(int j = 0; j < wstrips; j++)
    {
      glBegin(GL_QUAD_STRIP);	
      for(int i = 0; i <= wslices; i++)
	{
	  glTexCoord2f(i*twidth,(j+1)*theight);
	  glVertex3f(l+i*qwidth,b+(j+1)*qheight,f);
	  glTexCoord2f(i*twidth,j*theight);
	  glVertex3f(l+i*qwidth,b+j*qheight,f);
	}
      glEnd();
    }

  qwidth = (f-n)/wslices; qheight = (r-l)/wstrips;
 
  // Render the ceiling
  glBindTexture(GL_TEXTURE_2D, texName[2]);
  glNormal3f(0.0,1.0,0.0);
  for(int i = 0; i < wstrips; i++)
    {
      glBegin(GL_QUAD_STRIP);	
      for(int j = 0; j <= wslices; j++)
	{
	  glTexCoord2f((i+1)*theight,j*twidth);
	  glVertex3f(l+(i+1)*qheight,t,n+j*qwidth);
	  glTexCoord2f(i*theight,j*twidth);
	  glVertex3f(l+i*qheight,t,n+j*qwidth);
	}
      glEnd();
    }
  // Render the floor 
  glBindTexture(GL_TEXTURE_2D, texName[1]);
  glNormal3f(0.0,1.0,0.0);
  for(int i = 0; i < wstrips; i++)
    {
      glBegin(GL_QUAD_STRIP);	
      for(int j = 0; j <= wslices; j++)
	{
	  glTexCoord2f((i+1)*theight,j*twidth);
	  glVertex3f(l+(i+1)*qheight,b,n+j*qwidth);
	  glTexCoord2f(i*theight,j*twidth);
	  glVertex3f(l+i*qheight,b,n+j*qwidth);
	}
      glEnd();
    }
  //drawing board
  glPushMatrix();
    glTranslatef(-5,-5,-34.8);
    RenderParallelopiped(1,2,0.2,1);
  glPopMatrix();
  glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(-5.5,0.5-5,-34.3);
    glutSolidCube(0.4);
  glPopMatrix();
  glPushMatrix();
    glColor3f(1,1,0);
    glTranslatef(-4.5,0.5-5,-34.3);
    glutSolidCube(0.4);
  glPopMatrix();
}
GLvoid RenderParallelopiped(GLfloat x,GLfloat y,GLfloat z,GLfloat texcoor)
{
  GLfloat nx,ny,nz;
  nx=-1*x;
  nz=-1*z;
  glEnable(GL_TEXTURE_2D);
  glMaterialf(GL_FRONT, GL_SHININESS, 10); 
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0);
  glVertex3f(nx,0,z);

  glTexCoord2f(0.0,texcoor);
  glVertex3f(x,0,z);
    
  glTexCoord2f(texcoor,texcoor);
  glVertex3f(x,0,nz);
    
  glTexCoord2f(texcoor,0.0);
  glVertex3f(nx,0,nz);
  glEnd();
  glBegin(GL_QUADS);
  glTexCoord2f(0.0,0.0);
  glVertex3f(nx,y,z);

  glTexCoord2f(0.0,texcoor);
  glVertex3f(x,y,z);
    
  glTexCoord2f(texcoor,texcoor);
  glVertex3f(x,y,nz);
    
  glTexCoord2f(texcoor,0.0);
  glVertex3f(nx,y,nz);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glTexCoord2f(0.0,0.0);
  glVertex3f(nx,0,z);
  glTexCoord2f(0.0,texcoor);
  glVertex3f(nx,y,z);
  glTexCoord2f(texcoor,texcoor);
  glVertex3f(x,0,z);
  glTexCoord2f(texcoor,0.0);
  glVertex3f(x,y,z);
  glTexCoord2f(0.0,0.0);
  glVertex3f(x,0,nz);
  glTexCoord2f(0.0,texcoor);
  glVertex3f(x,y,nz);
  glTexCoord2f(texcoor,texcoor);
  glVertex3f(nx,0,nz);
  glTexCoord2f(texcoor,0.0);
  glVertex3f(nz,y,nz);
  glTexCoord2f(0.0,0.0);
  glVertex3f(nx,0,z);
  glTexCoord2f(0.0,texcoor);
  glVertex3f(nx,y,z);
  glEnd();
  
  glDisable(GL_TEXTURE_2D);	
}
GLvoid RenderTable(GLfloat texcoor)
{ 
  glBindTexture(GL_TEXTURE_2D, texName[3]);  
  //surface
  RenderParallelopiped(1,0.2,1,texcoor);
  //four legs
  glPushMatrix();
  glTranslatef(0.85,-1.2,0.85);
  RenderParallelopiped(0.1,1.2,0.1,texcoor);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-0.85,-1.2,0.85);
  RenderParallelopiped(0.1,1.2,0.1,texcoor);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.85,-1.2,-0.85);
  RenderParallelopiped(0.1,1.2,0.1,texcoor);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-0.85,-1.2,-0.85);
  RenderParallelopiped(0.1,1.2,0.1,texcoor);
  glPopMatrix();
}
GLvoid RenderChair(GLfloat texcoor)
{
  glPushMatrix();
  glScalef(0.5,1,0.5);
  RenderTable(texcoor);
  glPopMatrix();
  glPushMatrix();
  glRotatef(80,1,0,0);
  glTranslatef(0,-0.45,-1.2);
  glScalef(1,1,2);
  RenderParallelopiped(0.5,0.1,0.5,texcoor);
  glPopMatrix();
}
GLvoid DisplayGL()
{ 
  //Clear the frame and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  if(cam_smalle!=0){
    camera.x=curr_x+smalle.xpos*0.8;
    camera.y=curr_y+5+smalle.ypos*0.8;
    camera.z=curr_z+smalle.zpos*0.8;
    camera.ax=curr_x+smalle.xpos*0.8;
    camera.ay=curr_y+5+smalle.ypos*0.8;
    camera.az=curr_z+smalle.zpos*0.8+0.1;
    
    glRotatef((smalle.l_neck_rot+smalle.u_neck_rot),1,0,0);
    glRotatef(smalle.head_rot_fb,1,0,0);
    glRotatef(-1*smalle.head_rot_lr,0,1,0);
    glRotatef(-1*smalle.head_rot_side,0,0,1);
    glRotatef(-1*(smalle.yrot),0.0,1.0,0.0); 
  } 
  gluLookAt(camera.x,camera.y,camera.z,camera.ax,camera.ay,camera.az,0,1,0);  
  light();
  RenderRoom();
  double y = (r-l)/10; 
  glPushMatrix();
  glTranslatef((l+r)/2+12, b+2.4, (n+f)/2+5);
  glScalef(3,2,3);
  RenderTable(1);
  glPopMatrix();
  glPushMatrix();
  glTranslatef((l+r)/2+12, b+1.8, (n+f)/2);
  glScalef(3,1.5,3);
  RenderChair(1);
  glPopMatrix();
  glPushMatrix(); 
  glTranslatef((l+r)/2,b+2, (n+f)/2);
  glScalef(y/5,y/5,y/5);
  smalle.Draw_smalle();	
  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

//GL keyboard callback
GLvoid KeyPressedGL(unsigned char key, GLint x, GLint y) 
{
  int i,flags[5],tempvar;
  int light_state0,light_state1,light_state2;
  float DOFArray[PARAMETER_NUMBER],DOFArray1[PARAMETER_NUMBER],DOFResult[PARAMETER_NUMBER];
  switch (key)
    {   
      //expand right hand's lower arm 
    case 'x':
      if(smalle.expand_l<=smalle.expand_lim_x) 
	{
	  smalle.expand_l+=0.1; 
	  glutPostRedisplay();
	}
      break;
      //compress right hand's lower arm 
    case 'c':
      if(smalle.expand_l>=smalle.expand_lim_c) 
	{
	  smalle.expand_l-=0.1; 
	  glutPostRedisplay();
	}
      break;
      //expand left hand's lower arm 
    case 'X':
      if(smalle.expand_r<=smalle.expand_lim_x) 
	{
	  smalle.expand_r+=0.1; 
	  glutPostRedisplay();
	}
      break;
      //compress left hand's lower arm 
    case 'C':
      if(smalle.expand_r>=smalle.expand_lim_c) 
	{
	  smalle.expand_r-=0.1; 
	  glutPostRedisplay();
	}
      break;
      //move right hand up
    case 'u':
      if(smalle.rot_ud_l>=smalle.rot_ud_lim_u)
	{  
	  smalle.rot_ud_l-=3; 
	  glutPostRedisplay();
	}  
      break;
      //move right hand down
    case 'd':
      if(smalle.rot_ud_l<=smalle.rot_ud_lim_d)
	{
	  smalle.rot_ud_l+=3; 
	  glutPostRedisplay();
	}
      break;
      //move left hand up
    case 'U':
      if(smalle.rot_ud_r>=smalle.rot_ud_lim_u)
	{  
	  smalle.rot_ud_r-=3; 
	  glutPostRedisplay();
	}
      break;
      //move left hand down
    case 'D':
      if(smalle.rot_ud_r<=smalle.rot_ud_lim_d)
	{  
	  smalle.rot_ud_r+=3; 
	  glutPostRedisplay();
	}
      break;
      //move right hand to left
    case 'l':
      if(smalle.rot_lr_l<=smalle.rot_lr_lim_l)
	{
	  smalle.rot_lr_l+=3;
	  glutPostRedisplay();
	}
      break;
      //move right hand to right
    case 'r':
      if(smalle.rot_lr_l>=smalle.rot_lr_lim_r)
	{
	  smalle.rot_lr_l-=3; 
	  glutPostRedisplay();
	}
      break;
      //move left hand to left
    case 'L':
      if(smalle.rot_lr_r<=(-1*smalle.rot_lr_lim_r))
	{
	  smalle.rot_lr_r+=3; 
	  glutPostRedisplay();
	}
      break;
      //move left hand to right
    case 'R':
      if(smalle.rot_lr_r>=(-1*smalle.rot_lr_lim_l))
	{
	  smalle.rot_lr_r-=3; 
	  glutPostRedisplay();
	}
      break;
      //move lower neck down 
    case 'n':
      if(smalle.l_neck_rot<=smalle.l_neck_lim)
	{
	  smalle.l_neck_rot+=3; 
	  glutPostRedisplay();
	}
      break;
      //move lower neck up
    case 'N':
      if(smalle.l_neck_rot>=0)
	{
	  smalle.l_neck_rot-=3; 
	  glutPostRedisplay();
	}
      break;
      //move upper neck down
    case 'g':
      if(smalle.u_neck_rot<=smalle.u_neck_lim)
	{
	  smalle.u_neck_rot+=3; 
	  glutPostRedisplay();
	}
      break;
      //move upper neck up
    case 'G':
      if(smalle.u_neck_rot>=-20)
	{
	  smalle.u_neck_rot-=3; 
	  glutPostRedisplay();
	}
      break;
      //move head down
    case 'f':
      if(smalle.head_rot_fb<=smalle.head_rot_fb_lim)
	{
	  smalle.head_rot_fb+=3; 
	  glutPostRedisplay();
	}
      break;
      //move head up
    case 'F':
      if(smalle.head_rot_fb>=(-1*smalle.head_rot_fb_lim))
	{
	  smalle.head_rot_fb-=3; 
	  glutPostRedisplay();
	}
      break;
      //tilt head from left
    case 's':
      if(smalle.head_rot_side<=smalle.head_rot_side_lim)
	{
	  smalle.head_rot_side+=3; 
	  glutPostRedisplay();
	}
      break;
      //tilt head form right
    case 'S':
      if(smalle.head_rot_side>=(-1*smalle.head_rot_side_lim))
	{
	  smalle.head_rot_side-=3; 
	  glutPostRedisplay();
	}
      break;
      //rotate head to left
    case 'q':
      if(smalle.head_rot_lr<=smalle.head_rot_lr_lim)
	{
	  smalle.head_rot_lr+=3; 
	  glutPostRedisplay();
	}
      break;
      //rotate head to right
    case 'Q':
      if(smalle.head_rot_lr>=(-1*smalle.head_rot_lr_lim))
	{
	  smalle.head_rot_lr-=3; 
	  glutPostRedisplay();
	}
      break;
      //move smalle backward
    case 'k':
      smalle.dir_flag=0;
      smalle.rot_gear+=5;
      smalle.trans_smalle-=0.2;//0.041;
      smalle.roller_displacement+=0.041;
      glutPostRedisplay();
      break;
      //move smalle forward
    case 'K':
      smalle.dir_flag=1;
      smalle.rot_gear-=5;
      smalle.trans_smalle+=0.2;//0.041;
      smalle.roller_displacement-=0.041;
      glutPostRedisplay();
      break;
      //move right palm down
    case 'i':
      if(smalle.rot_wrist_ud_l<=smalle.rot_wrist_ud_lim)
	{
	  smalle.rot_wrist_ud_l+=3; 
	  glutPostRedisplay();
	}
      break;
      //move right palm up
    case 'I':
      if(smalle.rot_wrist_ud_l>=(-1*smalle.rot_wrist_ud_lim))
	{
	  smalle.rot_wrist_ud_l-=3; 
	  glutPostRedisplay();
	}
      break;
      //move right palm left
    case 'o':
      if(smalle.rot_wrist_lr_l<=smalle.rot_wrist_lr_lim)
	{
	  smalle.rot_wrist_lr_l+=3; 
	  glutPostRedisplay();
	}
      break;
      // move right palm right
    case 'O':
      if(smalle.rot_wrist_lr_l>=(-1*smalle.rot_wrist_lr_lim))
	{
	  smalle.rot_wrist_lr_l-=3; 
	  glutPostRedisplay();
	}
      break;
      //rotate right palm axially clockwise 
    case 'p':
      if(smalle.rot_wrist_axis_l<=smalle.rot_wrist_axis_lim)
	{
	  smalle.rot_wrist_axis_l+=10; 
	  glutPostRedisplay();
	}
      break;
      //rotate right palm axially anticlockwise
    case 'P':
      if(smalle.rot_wrist_axis_l>=(-1*smalle.rot_wrist_axis_lim))
	{
	  smalle.rot_wrist_axis_l-=10; 
	  glutPostRedisplay();
	}
      break;
      //rotate left palm axially clockwise
    case 'y':
      if(smalle.rot_wrist_axis_r<=smalle.rot_wrist_axis_lim)
	{
	  smalle.rot_wrist_axis_r+=10; 
	  glutPostRedisplay();
	}
      break;
      //rotate left palm axially anticlockwise
    case 'Y':
      if(smalle.rot_wrist_axis_r>=(-1*smalle.rot_wrist_axis_lim))
	{
	  smalle.rot_wrist_axis_r-=10; 
	  glutPostRedisplay();
	}
      break;
      //move left palm down
    case 'z':
      if(smalle.rot_wrist_ud_r<=smalle.rot_wrist_ud_lim)
	{
	  smalle.rot_wrist_ud_r+=3; 
	  glutPostRedisplay();
	}
      break;
      //move left palm up
    case 'Z':
      if(smalle.rot_wrist_ud_r>=(-1*smalle.rot_wrist_ud_lim))
	{
	  smalle.rot_wrist_ud_r-=3; 
	  glutPostRedisplay();
	}
      break;
      //move left palm left
    case 't':
      if(smalle.rot_wrist_lr_r<=smalle.rot_wrist_lr_lim)
	{
	  smalle.rot_wrist_lr_r+=3; 
	  glutPostRedisplay();
	}
      break;
      //move left palm right
    case 'T':
      if(smalle.rot_wrist_lr_r>=(-1*smalle.rot_wrist_lr_lim))
	{
	  smalle.rot_wrist_lr_r-=3; 
	  glutPostRedisplay();
	}
      break;
      //move right finger of right hand up
    case 'm':
      if(smalle.rot_fing_ud_lr<=smalle.rot_fing_ud_lim)
	{
	  smalle.rot_fing_ud_lr+=10;
	  glutPostRedisplay();
	}
      break;
      //move right finger of right hand down
    case 'M':
      if(smalle.rot_fing_ud_lr>=(-1*smalle.rot_fing_ud_lim))
	{
	  smalle.rot_fing_ud_lr-=10;
	  glutPostRedisplay();
	}
      break;
      //move left finger of right hand up
    case 'b':
      if(smalle.rot_fing_ud_ll<=smalle.rot_fing_ud_lim)
	{
	  smalle.rot_fing_ud_ll+=10;
	  glutPostRedisplay();
	}
      break;
      //move left finger of right hand down
    case 'B':
      if(smalle.rot_fing_ud_ll>=(-1*smalle.rot_fing_ud_lim))
	{
	  smalle.rot_fing_ud_ll-=10;
	  glutPostRedisplay();
	}
      break;
      //move right finger of left hand up
    case 'j':
      if(smalle.rot_fing_ud_rr<=smalle.rot_fing_ud_lim)
	{
	  smalle.rot_fing_ud_rr+=10;
	  glutPostRedisplay();
	}
      break;
      //move right finger of left hand down
    case 'J':
      if(smalle.rot_fing_ud_rr>=(-1*smalle.rot_fing_ud_lim))
	{
	  smalle.rot_fing_ud_rr-=10;
	  glutPostRedisplay();
	}
      break;
      //move left finger of left hand up
    case 'e':
      if(smalle.rot_fing_ud_rl<=smalle.rot_fing_ud_lim)
	{
	  smalle.rot_fing_ud_rl+=10;
	  glutPostRedisplay();
	}
      break;
      //move left finger of left hand down
    case 'E':
      if(smalle.rot_fing_ud_rl>=(-1*smalle.rot_fing_ud_lim))
	{
	  smalle.rot_fing_ud_rl-=10;
	  glutPostRedisplay();
	}
      break;

    case 'V':
      if(cnt1==0){
	play();
	cnt1=1;
      }
      break;
    case 'v':     
      if(cnt1==0){
	if(cnt2==0){
	  fp=fopen("DOF.txt","w"); 
	  cnt2=1;
	}
	//parameters for left and right hand
	fprintf(fp,"%f %f %f %f ",smalle.rot_ud_l,smalle.rot_lr_l,smalle.rot_ud_r,smalle.rot_lr_r);
	//parametes for left hand forearm expansion
	fprintf(fp,"%f %f ",smalle.expand_l,smalle.expand_r);
	//parameters for neck
	fprintf(fp,"%f %f ",smalle.l_neck_rot,smalle.u_neck_rot);
	//parameters for head
	fprintf(fp,"%f %f %f ",smalle.head_rot_fb,smalle.head_rot_lr,smalle.head_rot_side);
	//parameters for gears
	fprintf(fp,"%f %f %f ",smalle.rot_gear,smalle.trans_smalle,smalle.roller_displacement);
	//parameters for wrist
	fprintf(fp,"%f %f %f %f",smalle.rot_wrist_ud_l,smalle.rot_wrist_lr_l,smalle.rot_wrist_axis_l,smalle.rot_wrist_ud_r);
	fprintf(fp,"%f %f ",smalle.rot_wrist_lr_r,smalle.rot_wrist_axis_r);
	//parameters for fingers
	fprintf(fp,"%f %f %f %f ",smalle.rot_fing_ud_lr,smalle.rot_fing_ud_ll,smalle.rot_fing_ud_rr,smalle.rot_fing_ud_rl);
	//parameters for global translation and rotation
	fprintf(fp,"%f %f %f ",smalle.xpos,smalle.ypos,smalle.zpos);
	fprintf(fp,"%f %f %f ",smalle. xrot,smalle.yrot,smalle.zrot);
	fprintf(fp,"%d %d %d %d %d ",smalle.flag_trans,smalle.flag_rot,smalle.eye_flag,smalle.dir_flag,cam_smalle);
	fprintf(fp,"%d %d %d ",(int)lightON0,(int)lightON1,(int)lightON2);
	fprintf(fp,"%f %f %f %f %f %f \n\n",camera.x,camera.y,camera.z,camera.ax,camera.ay,camera.az);
      }
      break;
	
      //quit
    case ESCAPE:
      exit(1);                   	
      break;
      //move along Z
    case '-' :smalle.zpos-=0.1;
      smalle.dir_flag=0;
      smalle.rot_gear+=5;	 
      smalle.roller_displacement+=0.041;
      glutPostRedisplay();
      break;
      //move along Z
    case '+' :smalle.zpos+=0.1; 
      smalle.dir_flag=1;
      smalle.rot_gear-=5;	 
      smalle.roller_displacement-=0.041;
      glutPostRedisplay();
      break;
      //rotate left about Y
    case DELETE:
      smalle.dir_flag=0;
      smalle.yrot-=2.0;
      smalle.roller_displacement+=0.041;
      smalle.rot_gear+=3;
      glutPostRedisplay();
      break;
    case '2':
      cam_smalle=0;
      camera.x=0;camera.y=0;camera.z=f;
      camera.ax=0;camera.ay=b/2;camera.az=0;
      glutPostRedisplay();
      break;
    case '3':
      cam_smalle=0;
      camera.x=r;camera.y=0;camera.z=(f-n)/2;
      camera.ax=0;camera.ay=0;camera.az=n+(f-n)/2;
      glutPostRedisplay();
      break;
    case '4':
      cam_smalle=0;
      camera.x=l;camera.y=0;camera.z=(f-n)/2;
      camera.ax=0;camera.ay=0;camera.az=n+(f-n)/2;
      glutPostRedisplay();
      break;
    case '1':
      cam_smalle=0;
      camera.x=0;camera.y=0;camera.z=n+8;
      camera.ax=0;camera.ay=0;camera.az=0;
      glutPostRedisplay();
      break;
    case '5':
      cam_smalle=0;
      camera.x=0;camera.y=t;camera.z=2*n+(f-n)/2;
      camera.ax=0;camera.ay=b;camera.az=(f-n)/2;
      glutPostRedisplay();
      break;
    case '6':
      cam_smalle=1;
      glutPostRedisplay();
      break;
    case '7':
      lightON0=not lightON0;
      glutPostRedisplay();
      break;
    case '8':
      lightON1=not lightON1;
      glutPostRedisplay();
      break;
    case '9':
      lightON2=not lightON2;
      glutPostRedisplay();
      break;
    default:
      break;
    }	
}

//-------------------------------------------------------------------------

//GL Keyboard callback for special keys
GLvoid SpecialKeyPressedGL(GLint key, GLint x, GLint y) 
{
  switch (key)
    {   
      //Move along Y
    case GLUT_KEY_UP:
      smalle.dir_flag=0;
      smalle.rot_gear+=5;	 
      smalle.roller_displacement+=0.041;

      smalle.ypos+=0.1; 
      glutPostRedisplay();
      break;
      //Move along Y
    case GLUT_KEY_DOWN:smalle.ypos-=0.2; 
      smalle.dir_flag=1;
      smalle.rot_gear-=5;	 
      smalle.roller_displacement-=0.041;
	 
      glutPostRedisplay();
      break;
      //Move along X
    case GLUT_KEY_LEFT:smalle.xpos-=0.1; 
      smalle.dir_flag=0;
      smalle.rot_gear+=5;	 
      smalle.roller_displacement+=0.041;
      glutPostRedisplay();
      break;
      //Move along X
    case GLUT_KEY_RIGHT:smalle.xpos+=0.1;
      smalle.dir_flag=1;
      smalle.rot_gear-=5;	 
      smalle.roller_displacement-=0.041;
	 
      glutPostRedisplay();
      break;
      //Rotate about X
    case GLUT_KEY_HOME:smalle.xrot+=1.0;
      glutPostRedisplay();
      break;
      //Rotate about X
    case GLUT_KEY_END:smalle.xrot-=1.0;
      glutPostRedisplay();
      break;
      //Rotate about Y
    case GLUT_KEY_INSERT:smalle.zrot+=1.0;
      glutPostRedisplay();
      break;
      //Rotate about Y
    case GLUT_KEY_PAGE_UP:smalle.zrot-=1.0;
      glutPostRedisplay();
      break;
    case GLUT_KEY_PAGE_DOWN:
      smalle.yrot+=2.0;
      smalle.rot_gear+=3;
      smalle.roller_displacement+=0.041;
      glutPostRedisplay();
      break;
    default:
      break;
    }	
}

// The main rendering function
GLvoid RenderGL(int argc, char** argv)
{
  //Initialize GLUT
  glutInit(&argc, argv);
  //Initialize GLUT display
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  //Window top-left corner position
  glutInitWindowPosition(0,0 );
  //Window size
  glutInitWindowSize( 1200, 800 );
  //Window title
  glutCreateWindow("The Small E");

  //Our Init function
  
  InitGL();

  //Register the callbacks  
  glutDisplayFunc(&DisplayGL);
  glutReshapeFunc(&ReshapeGL);   

  glutKeyboardFunc(&KeyPressedGL);
  glutSpecialFunc(&SpecialKeyPressedGL);
  //Start the GLUT event handling loop
  glutMainLoop();
}

//-------------------------------------------------------------------------
void play()
{
  int i,flags[10],tempvar;
  int light_state0,light_state1,light_state2;
  float DOFArray[PARAMETER_NUMBER],DOFArray1[PARAMETER_NUMBER],DOFResult[PARAMETER_NUMBER];
  float cam_cord[10];
  fp=fopen("DOF.txt","r"); 
  fp1=fopen("DOF.txt","r");
  int tint;
  float tfloat;
  for(i=0;i<PARAMETER_NUMBER;i++){
    fscanf(fp1,"%f",&tfloat);
    // printf("%f ",DOFArray1[i]);
  }
  fscanf(fp1,"%d %d %d %d %d",&tint,&tint,&tint,&tint,&tint);
  fscanf(fp1,"%d %d %d",&tint,&tint,&tint);
  fscanf(fp1,"%f %f %f %f %f %f",&tfloat,&tfloat,&tfloat,&tfloat,&tfloat,&tfloat);
  while(!feof(fp1)){
    //base snapshot
    //fp1=fp;
    //parameters for left and right hand
    fscanf(fp,"%f %f %f %f ",&smalle.rot_ud_l,&smalle.rot_lr_l,&smalle.rot_ud_r,&smalle.rot_lr_r);
    //parametes for left hand forearm expansion
    fscanf(fp,"%f %f ",&smalle.expand_l,&smalle.expand_r);
    //parameters for neck
    fscanf(fp,"%f %f ",&smalle.l_neck_rot,&smalle.u_neck_rot);
    //parameters for head
    fscanf(fp,"%f %f %f ",&smalle.head_rot_fb,&smalle.head_rot_lr,&smalle.head_rot_side);
    //parameters for gears
    fscanf(fp,"%f %f %f ",&smalle.rot_gear,&smalle.trans_smalle,&smalle.roller_displacement);
    //parameters for wrist
    fscanf(fp,"%f %f %f %f ",&smalle.rot_wrist_ud_l,&smalle.rot_wrist_lr_l,&smalle.rot_wrist_axis_l,&smalle.rot_wrist_ud_r);
    fscanf(fp,"%f %f ",&smalle.rot_wrist_lr_r,&smalle.rot_wrist_axis_r);
    //parameters for fingers
    fscanf(fp,"%f %f %f %f ",&smalle.rot_fing_ud_lr,&smalle.rot_fing_ud_ll,&smalle.rot_fing_ud_rr,&smalle.rot_fing_ud_rl);
    //parameters for global translation and rotation
    fscanf(fp,"%f %f %f ",&smalle.xpos,&smalle.ypos,&smalle.zpos);
    fscanf(fp,"%f %f %f ",&smalle. xrot,&smalle.yrot,&smalle.zrot);
    fscanf(fp,"%d %d %d %d %d",&smalle.flag_trans,&smalle.flag_rot,&smalle.eye_flag,&smalle.dir_flag,&cam_smalle);
    fscanf(fp,"%d %d %d ",&light_state0,&light_state1,&light_state2);
    fscanf(fp,"%f %f %f %f %f %f \n\n",&camera.x,&camera.y,&camera.z,&camera.ax,&camera.ay,&camera.az);
    //PRINTING BASIC SNAPSHOT
    printf("pointer 0 snapshot\n");
	 
    //parameters for left and right hand
    printf("%f %f %f %f ",smalle.rot_ud_l,smalle.rot_lr_l,smalle.rot_ud_r,smalle.rot_lr_r);
    //parametes for left hand forearm expansion
    printf("%f %f ",smalle.expand_l,smalle.expand_r);
    //parameters for neck
    printf("%f %f ",smalle.l_neck_rot,smalle.u_neck_rot);
    //parameters for head
    printf("%f %f %f ",smalle.head_rot_fb,smalle.head_rot_lr,smalle.head_rot_side);
    //parameters for gears
    printf("%f %f %f ",smalle.rot_gear,smalle.trans_smalle,smalle.roller_displacement);
    //parameters for wrist
    printf("%f %f %f %f ",smalle.rot_wrist_ud_l,smalle.rot_wrist_lr_l,smalle.rot_wrist_axis_l,smalle.rot_wrist_ud_r);
    printf("%f %f ",smalle.rot_wrist_lr_r,smalle.rot_wrist_axis_r);
    //parameters for fingers
    printf("%f %f %f %f ",smalle.rot_fing_ud_lr,smalle.rot_fing_ud_ll,smalle.rot_fing_ud_rr,smalle.rot_fing_ud_rl);
    //parameters for global translation and rotation
    printf("%f %f %f ",smalle.xpos,smalle.ypos,smalle.zpos);
    printf("%f %f %f ",smalle. xrot,smalle.yrot,smalle.zrot);
    printf("%d %d %d %d %d ",smalle.flag_trans,smalle.flag_rot,smalle.eye_flag,smalle.dir_flag,cam_smalle);
    printf("%d %d %d \n\n",light_state0,light_state1,light_state2);
    printf("%f %f %f %f %f %f \n\n",camera.x,camera.y,camera.z,camera.ax,camera.ay,camera.az);
    //first snapshot
    printf("pointer-1 1st snapshot\n");
	
	 
    DOFArray[0]=smalle.rot_ud_l; DOFArray[1]=smalle.rot_lr_l; DOFArray[2]=smalle.rot_ud_r; DOFArray[3]=smalle.rot_lr_r; 
	  
	   
    DOFArray[4]=smalle.expand_l; DOFArray[5]=smalle.expand_r;  
	   
    DOFArray[6]=smalle.l_neck_rot; DOFArray[7]=smalle.u_neck_rot;  
	   
    DOFArray[8]=smalle.head_rot_fb; DOFArray[9]=smalle.head_rot_lr; DOFArray[10]=smalle.head_rot_side;  
	 
	    
    DOFArray[11]=smalle.rot_gear; DOFArray[12]=smalle.trans_smalle; DOFArray[13]=smalle.roller_displacement; 
	    
    DOFArray[14]=smalle.rot_wrist_ud_l; DOFArray[15]=smalle.rot_wrist_lr_l; DOFArray[16]=smalle.rot_wrist_axis_l; DOFArray[17]=smalle.rot_wrist_ud_r; 
    DOFArray[18]=smalle.rot_wrist_lr_r; DOFArray[19]=smalle.rot_wrist_axis_r;  
	  
	   
    DOFArray[20]=smalle.rot_fing_ud_lr; DOFArray[21]=smalle.rot_fing_ud_ll; DOFArray[22]=smalle.rot_fing_ud_rr; DOFArray[23]=smalle.rot_fing_ud_rl;
	   
    DOFArray[24]=smalle.xpos; DOFArray[25]=smalle.ypos; DOFArray[26]=smalle.zpos; 
    DOFArray[27]=smalle. xrot; DOFArray[28]=smalle.yrot; DOFArray[29]=smalle.zrot; 
    flags[0]=smalle.flag_trans; flags[1]=smalle.flag_rot; flags[2]=smalle.eye_flag; flags[3]=smalle.dir_flag;flags[4]=cam_smalle;
    lightON0=(bool)light_state0;lightON1=(bool)light_state1;lightON2=(bool)light_state2;
    cam_cord[0]=camera.x;cam_cord[1]=camera.y;cam_cord[2]=camera.z;cam_cord[3]=camera.ax;cam_cord[4]=camera.ay;cam_cord[5]=camera.az;
    for(i=0;i<PARAMETER_NUMBER;i++)
      {
	//fscanf(fp1,"%f",&DOFArray[i]);
	printf("%f ",DOFArray[i]);
      }
    //fscanf(fp1,"%d %d ",&flags[0],&flags[1]);
    //second snapshot
    printf("\n\n");
    printf("pointer-1 2st snapshot\n");
    for(i=0;i<PARAMETER_NUMBER;i++)
      {
	fscanf(fp1,"%f",&DOFArray1[i]);
	printf("%f ",DOFArray1[i]);
      }
    fscanf(fp1,"%d %d %d %d %d",&flags[0],&flags[1],&flags[2],&flags[3],&flags[4]);
    fscanf(fp1,"%d %d %d ",&light_state0,&light_state1,&light_state2);	
    fscanf(fp1,"%f %f %f %f %f %f \n\n",&cam_cord[0],&cam_cord[1],&cam_cord[2],&cam_cord[3],&cam_cord[4],&cam_cord[5]);
    //Normalised differnce between two
    printf("\n\n NORMALISED DIFF\n\n");
    for(i=0;i<PARAMETER_NUMBER;i++)
      {
	DOFResult[i]=DOFArray1[i]-DOFArray[i];	 
	//printf("%f ",DOFResult[i]);
	if(DOFResult[i] != 0.0f)
	  {
	    DOFResult[i]/=10;
	    printf("%f ",DOFResult[i]);
	  }
      }
	
    //interpolating over Difference interval
     smalle.flag_trans=flags[0];smalle.flag_rot=flags[1];smalle.eye_flag=flags[2];smalle.dir_flag=flags[3];cam_smalle=flags[4];
     lightON0=(bool)light_state0;lightON1=(bool)light_state1;lightON2=(bool)light_state2;
     if(cam_smalle==0)
       camera.x=cam_cord[0];camera.y=cam_cord[1];camera.z=cam_cord[2];camera.ax=cam_cord[3];camera.ay=cam_cord[4];camera.az=cam_cord[5];
     for(i=0;i<10;i++){
      //parameters for left and right hand
      smalle.rot_ud_l+=DOFResult[0];smalle.rot_lr_l+=DOFResult[1];smalle.rot_ud_r+=DOFResult[2];smalle.rot_lr_r+=DOFResult[3];
      //parametes for left hand forearm expansion
      smalle.expand_l+=DOFResult[4];smalle.expand_r+=DOFResult[5];
      //parameters for neck
      smalle.l_neck_rot+=DOFResult[6];smalle.u_neck_rot+=DOFResult[7];
      //parameters for head
      smalle.head_rot_fb+=DOFResult[8];smalle.head_rot_lr+=DOFResult[9];smalle.head_rot_side+=DOFResult[10];
      //parameters for gears
      smalle.rot_gear+=DOFResult[11];smalle.trans_smalle+=DOFResult[12];smalle.roller_displacement+=DOFResult[13];
      //parameters for wrist
      smalle.rot_wrist_ud_l+=DOFResult[14];smalle.rot_wrist_lr_l+=DOFResult[15];smalle.rot_wrist_axis_l+=DOFResult[16];smalle.rot_wrist_ud_r+=DOFResult[17];
      smalle.rot_wrist_lr_r+=DOFResult[18];smalle.rot_wrist_axis_r+=DOFResult[19];
      //parameters for fingers
      smalle.rot_fing_ud_lr+=DOFResult[20];smalle.rot_fing_ud_ll+=DOFResult[21];smalle.rot_fing_ud_rr+=DOFResult[22];smalle.rot_fing_ud_rl+=DOFResult[23];
      //parameters for global translation and rotation
      smalle.xpos+=DOFResult[24];smalle.ypos+=DOFResult[25];smalle.zpos+=DOFResult[26];
      smalle. xrot+=DOFResult[27];smalle.yrot+=DOFResult[28];smalle.zrot+=DOFResult[29];
      DisplayGL();
      usleep(10000);
    }
  }
}
//---------------------------------------------------------------------------
int main(int argc, char** argv)
{
  RenderGL(argc,argv);
}

