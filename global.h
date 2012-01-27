#ifndef _GLOBAL__H_
#define _GLOBAL__H_

#ifndef max
#define max(a,b) ( ((a) > (b)) ? (a) : (b) )
#endif
#ifndef min
#define min(a,b) ( ((a) < (b)) ? (a) : (b) )
#endif

// #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#define GLUT_DISABLE_ATEXIT_HACK
#define FREEGLUT_STATIC
#define DEBUG_VALUE  // function values
#define DEBUG_POS  // position values
//#define DEBUG_FUNC  // function count
#define DEBUG_KEY // key press

#define PI 3.14
#define MAX_DB_LINE_LENGTH 1024
#define MAX_CHARS 100
#define MAX_AREAS 50
#define MAX_OBJECTS 100

// enum for direction
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

#include "GL/freeglut.h"

// forward declarations
typedef struct player player;

// structs
typedef struct {
    GLint _id;
    GLbyte name[256];
    GLdouble dimension[3];
    GLint location;
} area;

typedef struct {
    GLint _id;
    GLbyte name[256];
    GLdouble dimension[3];
    GLdouble position[3];
    GLint map_id;
} object;

// global variables
GLdouble eye_pos[3];
GLdouble center_pos[3];
GLdouble up_pos[3];
GLdouble speed;           // moving speed
GLdouble angular_speed;   // rotating speed
player * chars[MAX_CHARS];  // list of characters (logic will need to be changed eventually to allow dynamic char creation)
area * areas[MAX_AREAS];  // same as above
object * objects[MAX_OBJECTS]; // ditto

// global functions
GLint GLmax(GLint, GLint);
void exit_glut();
void init();

#endif
