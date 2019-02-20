#include <GL/glut.h>
#include<stdio.h>
#include<math.h>
#include "smalle.hpp"
#include "PPMImageIO.h"
extern bool lightON0;
void Smalle::init_list()
{  
  //texture
  int height,width;
  bool success;
  glGenTextures(5,texName);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  char filename[]="walle.ppm";
  success=PPMReader(filename,height,width,tex1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1;
  glBindTexture(GL_TEXTURE_2D, texName[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  char filename1[]="roller.ppm";
  success=PPMReader(filename1,height,width,tex1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1;
  glBindTexture(GL_TEXTURE_2D, texName[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  char filename2[]="gears.ppm";
  success=PPMReader(filename2,height,width,tex1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1;
  glBindTexture(GL_TEXTURE_2D, texName[3]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  char filename3[]="skin.ppm";
  success=PPMReader(filename3,height,width,tex1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height, width, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1);
  delete[] tex1;
  /*initialising*/ 
  IDquadric=gluNewQuadric(); 
  mylist = glGenLists (20);
  flag_trans=0;flag_rot=0;
  //restrictions to movements 
  rot_fing_ud_lim=45;
  rot_wrist_axis_lim=90;
  rot_wrist_ud_lim=60;
  rot_wrist_lr_lim=60;
  head_rot_fb_lim=20;
  head_rot_lr_lim=20;
  head_rot_side_lim=20;
  l_neck_lim=50;
  u_neck_lim=45;
  rot_ud_lim_d=40;
  rot_ud_lim_u=-60;
  rot_lr_lim_l=15;
  rot_lr_lim_r=-90;
  expand_lim_x=1.8;
  expand_lim_c=0;
      
  //creating display lists
  struct_torso();
  struct_fore_arm();
  struct_upper_arm();
  struct_arm_sphere();
  struct_neck_lower();
  struct_neck_upper();
  struct_left_head();
  struct_right_head();
  struct_eye();
  struct_eye_strip();
  struct_roller();
  struct_finger();
  struct_palm();
}
void Smalle::geartooth(void)
{

  float i,j;
  int sections = 20; //number of triangles to use to estimate a circle
  // (a higher number yields a more perfect circle)
  GLfloat radius = 0.8f; //radius
  GLfloat twoPi = 2.0f * 3.14159f;
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texName[2]);
  glBegin(GL_QUAD_STRIP);               
  for(i = 0; i <= sections;i+=2) { // make $section number of inner circles
    glTexCoord2f(0, 0);
    glVertex3f(radius * cos(i *  twoPi / sections), radius* sin(i * twoPi / sections),0);	
    glTexCoord2f(1, 0);
    glVertex3f(radius * cos(i *  twoPi / sections), radius* sin(i * twoPi / sections),radius*0.35f);
    glTexCoord2f(1, 1);
    glVertex3f(radius * cos((i+1) *  twoPi / sections), radius* sin((i+1) * twoPi / sections),0);		
    glTexCoord2f(0, 1);
    glVertex3f(radius * cos((i+1) *  twoPi / sections), radius* sin((i+1) * twoPi / sections),radius*0.35f);
  }
  glEnd();    
      
  for(i = 0; i <= sections;i++) { 
    glBegin(GL_QUADS);// 1st slop of gear tooth
    glTexCoord2f(0, 0);
    glVertex3f(radius * cos(i *  twoPi / sections), radius* sin(i * twoPi / sections),radius*0.35f);
    glTexCoord2f(1, 0);
    glVertex3f(radius * cos(i *  twoPi / sections), radius* sin(i * twoPi / sections),0);			
    glTexCoord2f(1, 1);
    glVertex3f(radius *1.15f* cos((i+0.25f)*  twoPi / sections), radius*1.15f* sin((i+0.25f)* twoPi / sections),0);			
    glTexCoord2f(0, 1);
    glVertex3f(radius *1.15f* cos((i+0.25f) *  twoPi / sections), radius*1.15f* sin((i+0.25f)* twoPi / sections),radius*0.35f);
    glEnd();
    glBegin(GL_QUADS);// middle flat thooth area
    glTexCoord2f(0, 0);
    glVertex3f(radius *1.15f* cos((i+0.25f) *  twoPi / sections), radius*1.15f* sin((i+0.25f)* twoPi / sections),radius*0.35f);
    glTexCoord2f(1, 0);
    glVertex3f(radius *1.15f* cos((i+0.25f)*  twoPi / sections), radius*1.15f* sin((i+0.25f)* twoPi / sections),0);
    glTexCoord2f(1, 1);
    glVertex3f(radius *1.15f* cos((i+0.75f)*  twoPi / sections), radius*1.15f* sin((i+0.75f)* twoPi / sections),0);
    glTexCoord2f(0, 1);
    glVertex3f(radius *1.15f* cos((i+0.75f)*  twoPi / sections), radius*1.15f* sin((i+0.75f)* twoPi / sections),radius*0.35f);
    glEnd();
    glBegin(GL_QUADS);// 2st slop of gear tooth
    glTexCoord2f(0, 0);
    glVertex3f(radius *1.15f* cos((i+0.75f)*  twoPi / sections), radius*1.15f* sin((i+0.75f)* twoPi / sections),radius*0.35f);
    glTexCoord2f(1, 0);
    glVertex3f(radius *1.15f* cos((i+0.75f)*  twoPi / sections), radius*1.15f* sin((i+0.75f)* twoPi / sections),0);
    glTexCoord2f(1, 1);
    glVertex3f(radius * cos((i+1)*  twoPi / sections), radius * sin((i+1)* twoPi / sections),0);
    glTexCoord2f(0, 1);
    glVertex3f(radius * cos((i+1)*  twoPi / sections), radius * sin((i+1)* twoPi / sections),radius*0.35f);
    glEnd();
    glBegin(GL_QUADS);// front face of gear tooth
    glTexCoord2f(0, 0);
    glVertex3f(radius * cos(i *  twoPi / sections), radius* sin(i * twoPi / sections),radius*0.35f);
    glTexCoord2f(1, 0);
    glVertex3f(radius *1.15f* cos((i+0.25f) *  twoPi / sections), radius*1.15f* sin((i+0.25f)* twoPi / sections),radius*0.35f);
    glTexCoord2f(1, 1);
    glVertex3f(radius *1.15f* cos((i+0.75f)*  twoPi / sections), radius*1.15f* sin((i+0.75f)* twoPi / sections),radius*0.35f);
    glTexCoord2f(0, 1);
    glVertex3f(radius * cos((i+1)*  twoPi / sections), radius * sin((i+1)* twoPi / sections),radius*0.35f);
    glEnd();
    glBegin(GL_QUADS);// back face of gear tooth					
    glTexCoord2f(0, 0);
    glVertex3f(radius *1.15f* cos((i+0.25f)*  twoPi / sections), radius*1.15f* sin((i+0.25f)* twoPi / sections),0);	
    glTexCoord2f(1, 0);
    glVertex3f(radius * cos(i *  twoPi / sections), radius* sin(i * twoPi / sections),0);	
    glTexCoord2f(1, 1);
    glVertex3f(radius * cos((i+1)*  twoPi / sections), radius * sin((i+1)* twoPi / sections),0);
    glTexCoord2f(0, 1);
    glVertex3f(radius *1.15f* cos((i+0.75f)*  twoPi / sections), radius*1.15f* sin((i+0.75f)* twoPi / sections),0);
    glEnd();
  }
  glDisable(GL_TEXTURE_2D);
}
void Smalle::struct_torso(void)
{
  GLfloat mat_specularGRAY[] ={0.75,0.75,0.75,1.0};
  GLfloat mat_ambientGRAY[] ={0.5,0.5,0.5,1.0};
  GLfloat mat_diffuseGRAY[] ={0.50,0.50,0.50,1.0};
  GLfloat mat_shininessGRAY[] ={128.0 * 0.6};

  glNewList(mylist,GL_COMPILE); 
  SetMaterial(mat_specularGRAY, mat_ambientGRAY, mat_diffuseGRAY, mat_shininessGRAY);
  glPushMatrix();
    glTranslatef(1.5,1.5,-1.5);
    glRotatef(180,0,0,1);
    Box(3,3,3);
  glPopMatrix();
  //for left and right sockets
  glPushMatrix();
  glTranslatef(1.7,0.5,0.5);
  glRotatef(-90,0.0,1.0,0.0);
  glTranslatef(-0.5,0,0);
  glColor3ub(0,0,255);
  gluCylinder(IDquadric,0.3,0.25,0.2,32,32);
  glTranslatef(0.0,0.0,-0.1);
  glColor3ub(0,255,0);
  glutSolidSphere(0.27,32,32);
  glTranslatef(0,0,-0.1);
  glRotatef(-180,0.0,1.0,0.0);
  glColor3ub(0,0,255);
  gluCylinder(IDquadric,0.3,0.25,0.45,32,32);
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-1.9,0.5,0.5);
  glRotatef(-90,0.0,1.0,0.0);
  glTranslatef(-0.5,0,0);
  gluCylinder(IDquadric,0.3,0.25,0.45,32,32);
  glTranslatef(0.0,0.0,-0.1);
  glColor3ub(0,255,0);
  glutSolidSphere(0.27,32,32);
  glTranslatef(0,0,-0.1);
  glRotatef(-180,0.0,1.0,0.0);
  glColor3ub(0,0,255);
  gluCylinder(IDquadric,0.3,0.25,0.2,32,32);
  glPopMatrix();
  //for neck socket
  glPushMatrix();
  glTranslatef(1.7,1.7,0.5);
  glRotatef(90,0,0,1.0);
  glTranslatef(0,1.7,0.0);
  glRotatef(-90,0.0,1.0,0.0);
  glTranslatef(-0.5,0,0);
  gluCylinder(IDquadric,0.3,0.25,0.2,32,32);
  glTranslatef(0.0,0.0,-0.1);
  glColor3ub(0,255,0);
  glutSolidSphere(0.27,32,32);
  glTranslatef(0,0,-0.1);
  glRotatef(-180,0.0,1.0,0.0);
  glColor3ub(0,0,255);
  gluCylinder(IDquadric,0.3,0.25,0.25,32,32);
  glPopMatrix();
  glEndList();
}
void Smalle::struct_finger(void)
{
  glNewList(mylist+3,GL_COMPILE); 	
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texName[3]);
  glBegin(GL_QUAD_STRIP);
  glTexCoord2f(0,0);
  glVertex3f(0.0, 0.1, 0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.0, 0.0, 0.0);
  glTexCoord2f(1,1);
  glVertex3f(0.0, 0.1, 0.2);
  glTexCoord2f(0,1);
  glVertex3f(0.0, 0.0, 0.2);
  glTexCoord2f(0,0);
  glVertex3f(0.4, 0.02, 0.2);
  glTexCoord2f(1,0);
  glVertex3f(0.4, 0.0, 0.2);
  glTexCoord2f(1,1);
  glVertex3f(0.4, 0.02, 0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.4, 0.0, 0.0);
  glTexCoord2f(0,0);
  glVertex3f(0.0, 0.1, 0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.0, 0.0, 0.0);
  glEnd();
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(0.4, 0.02, 0.2);
  glTexCoord2f(1,0);
  glVertex3f(0.4, 0.02, 0.0);
  glTexCoord2f(1,1);
  glVertex3f(0.0, 0.1, 0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.0, 0.1, 0.2);
  glEnd();
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(0.4, 0.0, 0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.4, 0.0, 0.2);
  glTexCoord2f(1,1);
  glVertex3f(0.0, 0.0, 0.2);
  glTexCoord2f(0,1);
  glVertex3f(0.0, 0.0, 0.0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glEndList();
}
void Smalle::struct_palm(void)
{
  glNewList(mylist+4,GL_COMPILE); 	
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texName[3]);
  glBegin(GL_QUAD_STRIP);
  glTexCoord2f(0,0);
  glVertex3f(0.0, 0.2, 0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.0, 0.0, 0.0);
  glTexCoord2f(1,1);
  glVertex3f(0.0, 0.2, 0.4);
  glTexCoord2f(0,1);
  glVertex3f(0.0, 0.0, 0.4);
  glTexCoord2f(0,0);
  glVertex3f(0.25, 0.2, 0.4);
  glTexCoord2f(1,0);
  glVertex3f(0.25, 0.1, 0.4);
  glTexCoord2f(1,1);
  glVertex3f(0.25, 0.2, 0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.25, 0.1, 0.0);
  glTexCoord2f(0,0);
  glVertex3f(0.0, 0.2, 0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.0, 0.0, 0.0);
  glEnd();
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(0.25, 0.2, 0.4);
  glTexCoord2f(1,0);
  glVertex3f(0.25, 0.2, 0.0);
  glTexCoord2f(1,1);
  glVertex3f(0.0, 0.2, 0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.0, 0.2, 0.4);
  glEnd();
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(0.25, 0.1, 0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.25, 0.1, 0.4);
  glTexCoord2f(1,1);
  glVertex3f(0.0, 0.0, 0.4);
  glTexCoord2f(0,1);
  glVertex3f(0.0, 0.0, 0.0);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glEndList();
}
void Smalle::struct_upper_arm(void)
{
  GLfloat mat_specular[] ={0.75,0.75,0.75,1.0};
  GLfloat mat_ambient[] ={0.0,0.0,1.0,1.0};
  GLfloat mat_diffuse[] ={0.50,0.50,0.50,1.0};
  GLfloat mat_shininess[] ={128.0};
  glNewList(mylist+1,GL_COMPILE);
  SetMaterial(mat_specular, mat_ambient, mat_diffuse, mat_shininess);
  glColor3ub(0,0,160);
  gluCylinder(IDquadric,0.2,0.2,2.5,32,32);
  glEndList();
}
void Smalle::struct_fore_arm(void)
{
  GLfloat mat_specular[] ={0.75,0.75,0.75,1.0};
  GLfloat mat_ambient[] ={0.0,0.0,1.0,1.0};
  GLfloat mat_diffuse[] ={0.1,0.1,0.1,1.0};
  GLfloat mat_shininess[] ={128.0};
  glNewList(mylist+12,GL_COMPILE); 
  SetMaterial(mat_specular, mat_ambient, mat_diffuse, mat_shininess);
  glColor3ub(60,60,60);
  gluCylinder(IDquadric,0.15,0.15,2,32,32);
  glEndList();
}
void Smalle::struct_arm_sphere(void)
{
  glNewList(mylist+2,GL_COMPILE);
  glColor3ub(255,0,0);
  glutSolidSphere(0.1,32,32);
  glEndList();
}
void Smalle::struct_neck_lower(void)
{
  glNewList(mylist+5,GL_COMPILE);
  glColor3ub(150,20,150);
  gluCylinder(IDquadric,0.25,0.25,0.9f,32,32);
  glEndList();
}
void Smalle::struct_neck_upper(void)
{
  glNewList(mylist+6,GL_COMPILE);
  glColor3ub(120,120,120);
  gluCylinder(IDquadric,0.25,0.25,0.8f,32,32);
  glEndList();
}
void Smalle::struct_left_head(void)
{
  GLfloat mat_specularGRAY[] ={0.75,0.75,0.75,1.0};
  GLfloat mat_ambientGRAY[] ={0.0,0.0,1.0,1.0};
  GLfloat mat_diffuseGRAY[] ={0.50,0.50,0.50,1.0};
  GLfloat mat_shininessGRAY[] ={128.0 * 0.6};
  glNewList(mylist+7,GL_COMPILE);
  SetMaterial(mat_specularGRAY, mat_ambientGRAY, mat_diffuseGRAY, mat_shininessGRAY);
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texName[3]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);
  glVertex3f(0.5,0.0,0.0);
  glTexCoord2f(1,0);
  glVertex3f(1.0,0.0,0.0);
  glTexCoord2f(1,1);
  glVertex3f(1.7,0.5,0.0);
  glTexCoord2f(0,1);
  glVertex3f(1.5,1.0,0.0);
  glTexCoord2f(0,0);
  glVertex3f(0.3,0.8,0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.1,0.3,0.0);
  glEnd();
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);
  glVertex3f(0.5,0.0,0.54);
  glTexCoord2f(1,0);
  glVertex3f(1.0,0.0,0.54);
  glTexCoord2f(1,1);
  glVertex3f(1.7,0.5,0.54);
  glTexCoord2f(0,1);
  glVertex3f(1.5,1.0,0.54);
  glTexCoord2f(0,0);
  glVertex3f(0.3,0.8,0.54);
  glTexCoord2f(1,0);
  glVertex3f(0.1,0.3,0.54);
  glEnd();
  glBegin(GL_QUAD_STRIP);
  glTexCoord2f(0,0);
  glVertex3f(0.5,0.0,0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.5,0.0,0.54);
  glTexCoord2f(1,1);
  glVertex3f(1.0,0.0,0.0);
  glTexCoord2f(0,1);
  glVertex3f(1.0,0.0,0.54);
  glTexCoord2f(0,0);
  glVertex3f(1.7,0.5,0.0);
  glTexCoord2f(1,0);
  glVertex3f(1.7,0.5,0.54);
  glTexCoord2f(1,1);
  glVertex3f(1.5,1.0,0.0);
  glTexCoord2f(0,1);
  glVertex3f(1.5,1.0,0.54);
  glTexCoord2f(0,0);
  glVertex3f(0.3,0.8,0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.3,0.8,0.54);
  glTexCoord2f(1,1);
  glVertex3f(0.1,0.3,0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.1,0.3,0.54);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glEndList();
}
void Smalle::struct_right_head(void)
{
  GLfloat mat_specularGRAY[] ={0.5,0.5,0.5,1.0};
  GLfloat mat_ambientGRAY[] ={0.0,0.0,1.0,1.0};
  GLfloat mat_diffuseGRAY[] ={0.50,0.50,0.50,1.0};
  GLfloat mat_shininessGRAY[] ={128.0 * 0.1};
    
  glNewList(mylist+8,GL_COMPILE);
  SetMaterial(mat_specularGRAY, mat_ambientGRAY, mat_diffuseGRAY, mat_shininessGRAY);
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texName[3]);
  glBegin(GL_POLYGON);
  glNormal3f(0,1,0);
  glTexCoord2f(0,0);
  glVertex3f(0.0,0.0,0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.5,0.0,0.0);
  glTexCoord2f(1,1);
  glVertex3f(0.9,0.3,0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.7,0.8,0.0);
  glTexCoord2f(0,0);
  glVertex3f(-0.5,1.0,0.0);
  glTexCoord2f(1,0);
  glVertex3f(-0.7,0.5,0.0);
  glEnd();
  //glColor3f(255,0,0);
  glBegin(GL_POLYGON);
  glNormal3f(0,-1,0);
  glTexCoord2f(0,0);
  glVertex3f(0.0,0.0,0.54);
  glTexCoord2f(1,0);
  glVertex3f(0.5,0.0,0.54);
  glTexCoord2f(1,1);
  glVertex3f(0.9,0.3,0.54);
  glTexCoord2f(0,1);
  glVertex3f(0.7,0.8,0.54);
  glTexCoord2f(0,0);
  glVertex3f(-0.5,1.0,0.5);
  glTexCoord2f(1,0);
  glVertex3f(-0.7,0.5,0.54);
  glEnd();
  //glColor3f(0,0,140);
  glBegin(GL_QUAD_STRIP);
  glTexCoord2f(0,0);
  glVertex3f(0.0,0.0,0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.0,0.0,0.54);
  glTexCoord2f(1,1);
  glVertex3f(0.5,0.0,0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.5,0.0,0.54);
  glTexCoord2f(0,0);
  glVertex3f(0.9,0.3,0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.9,0.3,0.54);
  glTexCoord2f(1,1);
  glVertex3f(0.7,0.8,0.0);
  glTexCoord2f(0,1);
  glVertex3f(0.7,0.8,0.54);
  glTexCoord2f(0,0);
  glVertex3f(-0.5,1.0,0.0);
  glTexCoord2f(1,0);
  glVertex3f(-0.5,1.0,0.54);
  glTexCoord2f(1,1);
  glVertex3f(-0.7,0.5,0.0);
  glTexCoord2f(0,1);
  glVertex3f(-0.7,0.5,0.54);
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glEndList();
}
void Smalle::struct_eye(void)
{
  GLfloat mat_specularRED[] ={0.75,0.75,0.75,1.0};
  GLfloat mat_ambientRED[] ={1.0,0.0,0.0,1.0};
  GLfloat mat_diffuseRED[] ={0.50,0.50,0.50,1.0};
  GLfloat mat_shininessRED[] ={128.0};
  glNewList(mylist+9,GL_COMPILE);
  SetMaterial(mat_specularRED, mat_ambientRED, mat_diffuseRED, mat_shininessRED);
  //glColor3ub(225,255,255);
  gluDisk(IDquadric,0.0,0.2,32,32);
  glEndList();
}
void Smalle::struct_eye_strip(void)
{
  glNewList(mylist+10,GL_COMPILE);
  glColor3f(255,255,255);
  glBegin(GL_QUADS);
  glVertex3f(0.0,0.0,0.25);
  glVertex3f(0.4,0.0,0.25);
  glVertex3f(0.4,0.4,0.25);
  glVertex3f(0.0,0.4,0.25);
  glEnd();
  glEndList();
}
void Smalle::struct_roller(void)
{
  //first roller
  glPushMatrix();/////////
  glTranslatef(-2.5,-2.5,0.0);
  glRotatef(-90,0.0,1.0,0.0);	
  glScalef(3.5,3.5,3.5);
  edge_triangle();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-1.5,-2.5,0.0);
  glRotatef(90,0.0,1.0,0.0);	
  glScalef(3.5,3.5,3.5);
  triangle();
  glPopMatrix();	
  //second roller
  glPushMatrix();
  glTranslatef(1.5,-2.5,0.0);
  glRotatef(-90,0.0,1.0,0.0);	
  glScalef(3.5,3.5,3.5);
  edge_triangle();
  glPopMatrix();
  glPushMatrix();
  //glLoadIdentity();
  glTranslatef(2.5,-2.5,0.0);
  glRotatef(90,0.0,1.0,0.0);	
  glScalef(3.5,3.5,3.5);
  triangle();
  glPopMatrix();
}
void Smalle::edge_triangle(void)
{
  glColor3f(255,255,255);
  glBegin(GL_POLYGON);
  glVertex3f(0.4, 0.0, 0.0);
  glVertex3f(0.5, 0.1, 0.0);
  glVertex3f(0.07, 0.6, 0.0);
  glVertex3f(-0.07, 0.6, 0.0);
  glVertex3f(-0.5, 0.1, 0.0);
  glVertex3f(-0.4, 0.0, 0.0);
  glEnd();
  glColor3f(120,120,120);
  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D,texName[1]);
  glBegin(GL_QUAD_STRIP);
  glTexCoord2f(0, 0);
  glVertex3f(0.4, 0.0, 0.0);
  glTexCoord2f(1, 0);
  glVertex3f(0.4, 0.0, -0.3);
  glTexCoord2f(1, 1);
  glVertex3f(0.5, 0.1, 0.0);
  glTexCoord2f(0, 1);        //this is taken care by below for loop
  glVertex3f(0.5, 0.1, -0.3);
		
  float x_cord = 0.5f;
  float y_cord = 0.1f;

  if(dir_flag==0)
    {
      if(roller_displacement >= 0.14)
	roller_displacement=0.041;
    }
  else
    {
      if(roller_displacement <= 0)
	roller_displacement=0.14;
    }

  float tempx=roller_displacement*cos(49.305*3.1428/180);
  float tempy=roller_displacement*cos(40.695*3.1428/180);
		
  x_cord-=tempx;
  y_cord+=tempy;
  int i,j;
  for(i=0,j=0; x_cord >= 0.07;x_cord-=0.0913 )	//strips for front slop of roller
    {
      glTexCoord2f(i, j);
      glVertex3f(x_cord, y_cord, 0.0);
      glTexCoord2f(i+1, j);
      glVertex3f(x_cord, y_cord, -0.3);
      y_cord+=0.1069;
      i++;
      j++;
    }		
  glTexCoord2f(0, 0);
  glVertex3f(0.07, 0.6, 0.0);
  glTexCoord2f(1,0);
  glVertex3f(0.07, 0.6, -0.3);
		
  glTexCoord2f(1, 1);
  glVertex3f(-0.07, 0.6, 0.0);
  glTexCoord2f(0, 1);
  glVertex3f(-0.07, 0.6, -0.3);

  x_cord = -0.07f;
  y_cord = 0.6f;
		
  x_cord-=tempx;
  y_cord-=tempy;
  for(i=0,j=0; x_cord >= -0.5;x_cord-=0.0913 )	//strips for back slop of roller
    {
      glTexCoord2f(i, j);
      glVertex3f(x_cord, y_cord, 0.0);
      glTexCoord2f(i+1, j);
      glVertex3f(x_cord, y_cord, -0.3);			
      y_cord-=0.1069;
      i++;
      j++;
    }		
  /*glTexCoord2f(0, 0);
  glVertex3f(-0.07, 0.6, 0.0);
  glTexCoord2f(1, 0);
  glVertex3f(-0.07, 0.6, -0.3);*/

  glTexCoord2f(0, 0);
  glVertex3f(-0.5, 0.1, 0.0);
  glTexCoord2f(0, 1);
  glVertex3f(-0.5, 0.1, -0.3);
  glTexCoord2f(1, 1);
  glVertex3f(-0.4, 0.0, 0.0);
  glTexCoord2f(0, 1);
  glVertex3f(-0.4, 0.0, -0.3);
  x_cord=-0.4;
  x_cord+=tempx;
  //y_cord-=tempy;
  for(i=0,j=0; x_cord <= 0.4;x_cord+=0.14 )	//strips for base of roller
    {
      glTexCoord2f(i, j);
      glVertex3f(x_cord, 0.0, 0.0);
      glTexCoord2f(1-i, j);
      glVertex3f(x_cord, 0.0, -0.3);
      i++;
      j++;
    }
  glTexCoord2f(0, 0);
  glVertex3f(0.4, 0.0, 0.0);
  glTexCoord2f(1,0 );
  glVertex3f(0.4, 0.0, -0.3);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
void Smalle::triangle(void)
{
  glBegin(GL_POLYGON);
  glVertex3f(0.4, 0.0, 0.0);
  glVertex3f(0.5, 0.1, 0.0);
  glVertex3f(0.07, 0.6, 0.0);
  glVertex3f(-0.07, 0.6, 0.0);
  glVertex3f(-0.5, 0.1, 0.0);
  glVertex3f(-0.4, 0.0, 0.0);
  glEnd();
}
void Smalle::struct_gears(void)
{
  //--------Gears for ROLLER 1-------------//
  glPushMatrix();//big gear roller1
  glTranslatef(-2.35,-1.9,-0.65);
  glRotatef(-90,0.0,1.0,0.0);	
  glScalef(0.6,0.6,1.6);		
  glRotatef(rot_gear,0,0,1);
  geartooth();
  glPopMatrix();
  glPushMatrix();//small gear roller1
  glTranslatef(-2.4,-2.1,1.3);
  glRotatef(-90,0.0,1.0,0.0);	
  glScalef(0.3,0.3,1.0);		
  glRotatef(rot_gear,0,0,1);
  geartooth();
  glPopMatrix();
  glPushMatrix();//medium gear roller1
  glTranslatef(-2.4,-0.85,0);
  glRotatef(-90,0.0,1.0,0.0);	
  glScalef(0.45,0.45,1.3);		
  glRotatef(rot_gear,0,0,1);
  geartooth();
  glPopMatrix();
  //--------------Gears for ROLLER 2-------//
  glPushMatrix();//big gear roller2
  glTranslatef(2.9,-1.9,-0.65);
  glRotatef(-90,0.0,1.0,0.0);	//we rotate with -90 not with 90 degree to prsevent opposite rotation
  glScalef(0.6,0.6,1.6);		//of gears of two roller belt
  glRotatef(rot_gear,0,0,1);
  geartooth();
  glPopMatrix();
  glPushMatrix();//small gear roller2
  glTranslatef(2.7,-2.1,1.3);
  glRotatef(-90,0.0,1.0,0.0);	
  glScalef(0.3,0.3,1.0);	
  glRotatef(rot_gear,0,0,1);
  geartooth();
  glPopMatrix();
  glPushMatrix();//medium gear roller2
  glTranslatef(2.8,-0.85,0);
  glRotatef(-90,0.0,1.0,0.0);	
  glScalef(0.45,0.45,1.3);
  glRotatef(rot_gear,0,0,1);
  geartooth();
  glPopMatrix();
}  
void Smalle::head(void)
{
  //drawing left head
  glPushMatrix();
  glTranslatef(-1.5,-0.4,-0.3);
  glCallList(mylist+7);
  glPushMatrix();//drawing left eye
  glTranslatef(0.9,0.5,0.551);
  if (eye_flag==1)
    glCallList(mylist+9);	//eye circle
  glPopMatrix();
  glPopMatrix();

  //drawing right head
  glPushMatrix();
  glTranslatef(0.9,-0.4,-0.3);
  glCallList(mylist+8);
  glPushMatrix();//drawing right eye
  glTranslatef(0.1,0.5,0.551);
  if (eye_flag==1)
    glCallList(mylist+9);	//eye circle
  glPopMatrix();
  glPopMatrix();
}
void Smalle::Draw_smalle(void)
{  
  glTranslatef(xpos,ypos,zpos);
  //glRotatef(xrot,1.0,0.0,0.0);
  //glRotatef(zrot,0.0,0.0,1.0);     
  glRotatef(yrot,0.0,1.0,0.0);
  //glTranslatef(0,0,trans_smalle);
  glCallList(mylist);//for belly
  glPushMatrix();
  struct_roller();
  glPushMatrix();
  struct_gears();
  glPopMatrix();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(-0.6,0.5,0);
  glTranslatef(-(1.5+0.15),0.0,0);
  glRotatef(rot_ud_l,1,0,0);
  glRotatef(rot_lr_l,0,1,0);
  glCallList(mylist+1);        //cylindrical arm
  glTranslatef(0.0,0.0,3.1);
  glTranslatef(0.0,0.0,-2.55+expand_l);
  glCallList(mylist+12);
  glTranslatef(0,0,2.05);
  glRotatef(rot_wrist_ud_l,1,0,0);
  glRotatef(rot_wrist_lr_l,0,1,0);
  glRotatef(rot_wrist_axis_l,0,0,1);
  glCallList(mylist+2);        //sphere after arm
  glTranslatef(0.2,-0.2,0.0);
  glRotatef(-90,0.0,1.0,0.0);//rotate palm
  glTranslatef(0.1,0.1,0.0);
  glCallList(mylist+4);	//palm
  glTranslatef(0.2,0.1,0.0);
  glPushMatrix();
  glRotatef(rot_fing_ud_ll,0,0,1);
  glCallList(mylist+3);	//finger1
  glPopMatrix();
  glTranslatef(0.0,0.0,0.21);
  glPushMatrix();
  glRotatef(rot_fing_ud_lr,0,0,1);
  glCallList(mylist+3);	//finger2
  glPopMatrix();
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0.6,0.5,0);
  glTranslatef((1.5+0.15),0.0,0.0);
  glRotatef(rot_ud_r,1,0,0);
  glRotatef(rot_lr_r,0,1,0);
  glCallList(mylist+1);        //cylindrical arm
  glTranslatef(0.0,0.0,3.1);
  glTranslatef(0.0,0.0,-2.55+expand_r);
  glCallList(mylist+12);
  glTranslatef(0,0,2.05);
  glRotatef(rot_wrist_ud_r,1,0,0);
  glRotatef(rot_wrist_lr_r,0,1,0);
  glRotatef(rot_wrist_axis_r,0,0,1);
  glCallList(mylist+2);        //sphere after arm
  glTranslatef(0.2,-0.2,0.0);
  glRotatef(-90,0.0,1.0,0.0);//rotate palm
  glTranslatef(0.1,0.1,0.0);

  glCallList(mylist+4);	//palm
  glTranslatef(0.2,0.1,0.0);
  glPushMatrix();
  glRotatef(rot_fing_ud_rl,0,0,1);
  glCallList(mylist+3);	//finger1
  glPopMatrix();
  glTranslatef(0.0,0.0,0.21);
  glPushMatrix();
  glRotatef(rot_fing_ud_rr,0,0,1);
  glCallList(mylist+3);	//finger2
  glPopMatrix();
  glPopMatrix();
  glPushMatrix();    
  //drawing neck
  glTranslatef(0.0,1.5+0.6,0.0);
  glRotatef(30,1.0,0.0,0.0);
  glRotatef(-90,1.0,0.0,0.0);
  glRotatef(l_neck_rot,1,0,0);
  glCallList(mylist+5);
  glTranslatef(0.0,0.0,1.0);
  glRotatef(-30,1.0,0.0,0.0);
  glRotatef(u_neck_rot,1,0,0);
  glCallList(mylist+6);
     
  //drawing eye strip
  glTranslatef(-0.2,-0.1,0.6);
  glRotatef(90,1.0,0,0);
  glRotatef(head_rot_fb,1,0,0);
  glRotatef(head_rot_lr,0,1,0);
  glRotatef(head_rot_side,0,0,1);
  glCallList(mylist+10);
  glPushMatrix();
    float lwhite[] = {1.0f,1.0f,1.0f};
    if(lightON0==true){
      glMaterialf(GL_FRONT, GL_SHININESS, 800);
    }
    else{
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,lwhite);
    }
    glTranslatef(0.2,0.8,0);
    glutSolidSphere(0.3,32,32);
  glPopMatrix();
  eye_flag=1;
  head();
  eye_flag=0;
  glTranslatef(0,0,-0.35);
  glScalef(0.9,0.9,0.2);
  head();
  /*binacular of smalle*/
  for (int i=0;i<15;i++)
    {
      glTranslatef(0,0,-0.5);
      glScalef(0.9,0.9,1);
      head();
    }
  glPopMatrix();
}
void Smalle::SetMaterial(GLfloat spec[], GLfloat amb[], GLfloat diff[], GLfloat shin[])
{

  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
  glMaterialfv(GL_FRONT, GL_SHININESS, shin);
  glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
}
void Smalle::Box(GLdouble p_x, GLdouble p_y, GLdouble p_z)
{
   GLdouble a[] = {0., 0., p_z};
   GLdouble b[] = {p_x, 0., p_z};
   GLdouble c[] = {p_x, p_y, p_z};
   GLdouble d[] = {0., p_y, p_z};
   GLdouble e[] = {0., 0., 0.};
   GLdouble f[] = {p_x, 0., 0.};
   GLdouble g[] = {p_x, p_y, 0.};
   GLdouble h[] = {0., p_y, 0.};

   // Front
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texName[0]);
   glBegin(GL_QUADS);
      glNormal3d(0, 0, 1);
      glTexCoord2f(0, 0);
      glVertex3dv(a);
      glTexCoord2f(1, 0);
      glVertex3dv(b);
      glTexCoord2f(1, 1);
      glVertex3dv(c);
      glTexCoord2f(0, 1);
      glVertex3dv(d);
   glEnd();
   glDisable(GL_TEXTURE_2D);
   
    // Right
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texName[0]);
   glBegin(GL_QUADS);
      glNormal3d(1, 0, 0);
      glTexCoord2f(0, 0);
      glVertex3dv(c);
      glTexCoord2f(1, 0);
      glVertex3dv(b);
      glTexCoord2f(1, 1);
      glVertex3dv(f);
      glTexCoord2f(0, 1);
      glVertex3dv(g);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   // Back
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texName[0]);
   glBegin(GL_QUADS);
      glNormal3d(0, 0, -1);
      glTexCoord2f(0, 0);
      glVertex3dv(h);
      glTexCoord2f(1, 0);
      glVertex3dv(g);
      glTexCoord2f(1, 1);
      glVertex3dv(f);
      glTexCoord2f(0, 1);
      glVertex3dv(e);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   // Left
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texName[0]);
   glBegin(GL_QUADS);
      glNormal3d(-1, 0, 0);
      glTexCoord2f(0, 0);
      glVertex3dv(d);
      glTexCoord2f(1, 0);
      glVertex3dv(h);
      glTexCoord2f(1, 1);
      glVertex3dv(e);
      glTexCoord2f(0, 1);
      glVertex3dv(a);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   // Top
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texName[0]);
   glBegin(GL_QUADS);
      glNormal3d(0, 1, 0);
      glTexCoord2f(0, 0);
      glVertex3dv(d);
      glTexCoord2f(1, 0);
      glVertex3dv(c);
      glTexCoord2f(1, 1);
      glVertex3dv(g);
      glTexCoord2f(0, 1);
      glVertex3dv(h);
   glEnd();
   glDisable(GL_TEXTURE_2D);

   // Bottom
   glEnable(GL_TEXTURE_2D);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glBindTexture(GL_TEXTURE_2D,texName[0]);
   glBegin(GL_QUADS);
      glNormal3d(0, -1, 0);
      glTexCoord2f(0, 0);
      glVertex3dv(e);
      glTexCoord2f(1, 0);
      glVertex3dv(f);
      glTexCoord2f(1, 1);
      glVertex3dv(b);
      glTexCoord2f(0, 1);
      glVertex3dv(a);
   glEnd();
   glDisable(GL_TEXTURE_2D);
}
