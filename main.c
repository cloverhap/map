#include "global.h"
#include "gl_wrapper.h"
#include "key.h"
#include "database.h"
#include "controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/***
 * Draw and Render
 ****/
static void draw_chars() {
    int i = 0;
    while (chars[i] != 0) {
        player *p1 = chars[i];
        glPushMatrix();
        glTranslatef(p1->position[0],p1->position[1],p1->position[2]);
        glRotatef(0,0.0,0.0,1.0);
        glScalef(1.5,1.5,1.5);
        GLfloat clear_color[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE,clear_color);  // Colour it the background color (trying to make the player invisible)
        glColor3fv(clear_color);
        draw_cube(GL_FILL,0.5);
        glPopMatrix();
        i++;
    }
}

static void draw_objects() {
    int i = 0;
    while (objects[i] != 0) {
        object *p1 = objects[i];
        glPushMatrix();
        glTranslatef(p1->position[0],p1->position[1],p1->position[2]);
        glRotatef(0,0.0,0.0,1.0);
        glScalef(p1->dimension[0],p1->dimension[1],p1->dimension[2]);
        glColor3f(0.0,0.0,1.0);
        draw_cube(GL_LINE,0.5);
        glPopMatrix();
        i++;
    }
}

static void render_scene()
{
    glMatrixMode(GL_MODELVIEW);
    // clear all pixels to clear colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw things
    draw_chars();
    draw_objects();

    glFlush();  // draw graphics in buffer immediately
    glutSwapBuffers();  // switch buffers for double buffering
}

/***
 * Window Resize callback
 ****/
static void resize_scene(int w, int h)
{
    if (h == 0)
        h = 1;  // DON'T divide by zero - please?
    GLfloat ratio = 1.0 * w / h;
    glViewport(0,0,w,h);  // set the viewport to be the entire window

    // reset the coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // set the correct perspective
    gluPerspective(45,ratio,1,1000);
    //glFrustum(-ratio,ratio,-1.0,1.0,2.0,100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // perspective transformation
    // LookAt(eyeX,eyeY,eyeZ,             eye point
    //        centerX,centerY,centerZ,    reference point
    //        upX,upY,upZ)                "up" vector direction
    gluLookAt(eye_pos[0],eye_pos[1],eye_pos[2],
              center_pos[0],center_pos[1],center_pos[2],
              up_pos[0],up_pos[1],up_pos[2]);

    glutPostRedisplay();
}

/***
 * Idle Function callback
 ****/
static void check_goal() {
    if (pow(pow(chars[0]->position[0] - objects[0]->position[0],2) +
            pow(chars[0]->position[1] - objects[0]->position[1],2) +
            pow(chars[0]->position[2] - objects[0]->position[2],2),
            1.0/2.0)
          < 1) {
        exit_glut("You have reached the goal!");
    }
}

static void idle_event()
{
    check_goal();
    glutPostRedisplay();
}

/***
 * Initialization
 ****/
void init()
{
    // Read Database on initialization
    read_db();

    // OpenGL values
    glClearColor(1.0,1.0,1.0,0.0);
    //glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    /*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Ortho(left,right,bottom,top,nearClipDepth,farClipDepth)
    glOrtho(-50.0,50.0,-50.0,50.0,-1.0,10.0);
    */

    // set up perspective
    // scene values
    eye_pos[0] = chars[0]->position[0];     // eye is here
    eye_pos[1] = chars[0]->position[1];
    eye_pos[2] = chars[0]->position[2];
    center_pos[0] = eye_pos[0];  // looking at the direction of this point
    center_pos[1] = eye_pos[1];
    center_pos[2] = eye_pos[2] - 5;
    up_pos[0] = 0.0;  // up vector
    up_pos[1] = 1.0;
    up_pos[2] = 0.0;

    glPushAttrib(GL_MATRIX_MODE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // LookAt(eyeX,eyeY,eyeZ,             eye point(camera)
    //        centerX,centerY,centerZ,    reference point (where to look)
    //        upX,upY,upZ)                "up" vector direction
    gluLookAt(eye_pos[0],eye_pos[1],eye_pos[2],
              center_pos[0],center_pos[1],center_pos[2],
              up_pos[0],up_pos[1],up_pos[2]);
    glPopAttrib();

    speed = 0.5;
    angular_speed = (1.0/12.0)*PI;

    kb_layout = KB_QWERTY;
}

/***
 * exit_glut
 * Takes care of cleanup and other exiting procedures as necessary
 * and exits the main loop
 ****/
void exit_glut(const char* s) {
    printf("%s\n",s);
    glutLeaveMainLoop();
}

int main(int argc, char *argv[])
{
    // initialize (free)GLUT/OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    GLint init_win_size_x = 640, init_win_size_y = 480;
    glutInitWindowSize(init_win_size_x, init_win_size_y);
    GLint init_win_pos_x = (glutGet(GLUT_SCREEN_WIDTH) - init_win_size_x) / 2;
    GLint init_win_pos_y = (glutGet(GLUT_SCREEN_HEIGHT) - init_win_size_y) / 2;
    printf("Screen Size: %i %i\n", glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
    printf("Window Size: %i %i\n", init_win_size_x, init_win_size_y);
    printf("Window Coords: %i %i\n", init_win_pos_x, init_win_pos_y);
    glutInitWindowPosition(init_win_pos_x, init_win_pos_y);
    GLint window_id = glutCreateWindow("Walking Around");

    init();

    // register callbacks
    glutDisplayFunc(render_scene);
    glutReshapeFunc(resize_scene);
    glutKeyboardFunc(process_key);
    glutSpecialFunc(process_special_key);
    glutMouseFunc(process_mouse);
    glutIdleFunc(idle_event);

    glutMainLoop();
    glutDestroyWindow(window_id);

    //system("PAUSE");
    getchar();
    return 0;
}
