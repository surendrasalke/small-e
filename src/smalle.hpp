#ifndef _SMALLE_HPP_
#define _SMALLE_HPP_
class Smalle
{
  public:
  GLuint mylist;
  //texture variables
  GLuint texName[5];//unsigned binary int. atleast 32 bit
  GLubyte *tex1;
  /* Storage For Our Quadratic Objects */
  GLUquadricObj *IDquadric; 
  
  //parameters for left and right hand
  float rot_ud_l,rot_lr_l,rot_ud_r,rot_lr_r;
  float rot_ud_lim_u,rot_ud_lim_d,rot_lr_lim_l,rot_lr_lim_r;
  
  //parametes for left hand forearm expansion
  float expand_l,expand_r,expand_lim_x,expand_lim_c; 
  
  //parameters for neck
  float l_neck_rot,u_neck_rot,l_neck_lim,u_neck_lim;
  
  //parameters for head
  float head_rot_fb,head_rot_lr,head_rot_side;
  int eye_flag;
  float head_rot_fb_lim,head_rot_lr_lim,head_rot_side_lim;
  
  //parameters for gears
  float rot_gear,trans_smalle,roller_displacement;
  int dir_flag;
  
  //parameters for wrist
  float rot_wrist_ud_l,rot_wrist_lr_l,rot_wrist_axis_l,rot_wrist_ud_r,rot_wrist_lr_r,rot_wrist_axis_r;
  float rot_wrist_ud_lim,rot_wrist_lr_lim,rot_wrist_axis_lim;
  
  //parameters for fingers
  float rot_fing_ud_lr,rot_fing_ud_ll,rot_fing_ud_rr,rot_fing_ud_rl;
  float rot_fing_ud_lim; 
  
  //parameters for global translation and rotation
  float xpos,ypos,zpos;
  float xrot,yrot,zrot;
  int flag_trans,flag_rot;
  
  void init_list();
  void geartooth(void);
  void struct_torso(void);
  void struct_finger(void);
  void struct_palm(void);
  void struct_upper_arm(void);
  void struct_fore_arm(void);
  void struct_arm_sphere(void);
  void struct_neck_lower(void);
  void struct_neck_upper(void);
  void struct_left_head(void);
  void struct_right_head(void);
  void struct_eye(void);
  void struct_eye_strip(void);
  void struct_roller(void);
  void edge_triangle(void);
  void triangle(void);
  void struct_gears(void);
  void head(void);
  void Draw_smalle(void);
  void SetMaterial(GLfloat spec[], GLfloat amb[], GLfloat diff[], GLfloat shin[]);  
  void Box(GLdouble p_x, GLdouble p_y, GLdouble p_z);
};
#endif
