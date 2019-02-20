#include <GL/gl.h>
#include <GL/glu.h>
bool PPMReader(char* filename, int& height, int& width, GLubyte*& pixels); 
bool PPMWriter(char* filename, int height, int width, GLubyte* pixels); 
