#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _WIN32
#include <unistd.h>
#else
void usleep(unsigned int nanosec)
{
    Sleep(nanosec / 1000);
}
#endif

// *************** GLOBAL VARIABLES *************************
const float PI = 3.14159;
double eye_x = 0;
double eye_y = 0;
double eye_z = 275;
// --------------- USER INTERFACE VARIABLES -----------------
// Window settings
int windowID;   // Glut window ID (for display)
GLUI *glui; // Glui window (for controls)
int Win[2]; // window (x,y) size
// ---------------- ANIMATION VARIABLES ---------------------
// Animation settings
int animate_mode = 0;   // 0 = no anim, 1 = animate
int animation_frame = 0;    // Specify current frame of animation

// Main Bar translation parameters
const float MAIN_MIN = -60.0f;
const float MAIN_MAX = 60.0f;
float main_tran = -0.0f;

// Joint1 parameters
const float JOINT_MIN = -45.0f;
const float JOINT_MAX = 45.0f;
float joint_rot = -0.0f;

// Joint2 parameters
const float JOINT2_MIN = -66.0f;
const float JOINT2_MAX = 66.0f;
float joint2_rot = -0.0f;

// Joint3 parameters
const float JOINT3_MIN = -55.0f;
const float JOINT3_MAX = 55.0f;
float joint3_rot = -0.0f;

// Joint4 parameters
const float JOINT4_MIN = -50.0f;
const float JOINT4_MAX = 70.0f;
float joint4_rot = -0.0f;

// ***********  FUNCTION HEADER DECLARATIONS ****************
// Initialization functions
void initGlut(char* winName);
void initGlui();
void initGl();

// Callbacks for handling events in glut
void myReshape(int w, int h);
void animate();
void display(void);
void keyboard(unsigned char key, int x, int y);

// Callback for handling events in glui
void GLUI_Control(int id);

// Functions to help draw the object
void drawSquare(float size);
void drawF();
void drawR();
void drawE();
void drawD();
void drawCircle();

// ******************** FUNCTIONS ************************
// main() function
// Initializes the user interface (and any user variables)
// then hands over control to the event handler, which calls
// display() whenever the GL window needs to be redrawn.
int main(int argc, char** argv)
{
    // Process program arguments
    if (argc != 3) {
        printf("Usage: [width] [height]\n");
        printf("Using 500x400 window...\n");
        
        Win[0] = 500;
        Win[1] = 400;
    }
    else {
        Win[0] = atoi(argv[1]);
        Win[1] = atoi(argv[2]);
    }
    // Initialize glut, glui, and opengl
    glutInit(&argc, argv);
    initGlut(argv[0]);
    initGlui();
    initGl();
    // Invoke the standard GLUT main event loop
    glutMainLoop();
    return 0;         // never reached
}
// Initialize glut and create a window with the specified caption
void initGlut(char* winName)
{
    // Set video mode: double-buffered, color, depth-buffered
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Create window
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(Win[0], Win[1]);
    windowID = glutCreateWindow("Graphics");
    
    // Setup callback functions to handle events
    glutReshapeFunc(myReshape); // Call myReshape whenever window resized
    glutDisplayFunc(display);   // Call display whenever new frame needed
    glutKeyboardFunc(keyboard);
}
void keyboard(unsigned char key, int x, int y) {
    switch (key)
    {
        case 'a':
            eye_x += 10;
            break;
        case 'z':
            eye_x -= 10;
            break;
        case 's':
            eye_y += 10;
            
            break;
        case 'x':
            eye_y -= 10;
            break;
        case 'd':
            eye_z += 10;
            break;
        case 'c':
            eye_z -= 10;
        break; }
}
// Quit button handler.  Called when the "quit" button is pressed.
void quitButton(int)
{
    exit(0); }
// Animate button handler.  Called when the "animate" checkbox is pressed.
void animateButton(int)
{
    // synchronize variables that GLUT uses
    glui->sync_live();
    animation_frame = 0;
    if (animate_mode == 1) {
        // start animation
        GLUI_Master.set_glutIdleFunc(animate);
    }
    else {
        // stop animation
        GLUI_Master.set_glutIdleFunc(NULL);
    } }
// Initialize GLUI and the user interface
void initGlui()
{
    GLUI_Master.set_glutIdleFunc(NULL);
    
    // Create GLUI window
    glui = GLUI_Master.create_glui("Glui Window", 0, Win[0] + 10, 0);
    
    // Main Bar Translation Control
    GLUI_Translation *main_translator
    = glui->add_translation("Main", GLUI_TRANSLATION_Z,
                            &main_tran);
    main_translator->set_speed(0.05);
    
    // Joint 1 Rotation Control
    GLUI_Spinner *joint_spinner
    = glui->add_spinner("Joint 1", GLUI_SPINNER_FLOAT,
                        &joint_rot);
    joint_spinner->set_speed(0.1);
    joint_spinner->set_float_limits(JOINT_MIN, JOINT_MAX,
                                    GLUI_LIMIT_CLAMP);
    
    //Joint 2 Rotation Control
    GLUI_Spinner *joint_spinner2
    = glui->add_spinner("Joint 2", GLUI_SPINNER_FLOAT,
                        &joint2_rot);
    joint_spinner2->set_speed(0.05);
    joint_spinner2->set_float_limits(JOINT2_MIN, JOINT2_MAX,
                                     GLUI_LIMIT_CLAMP);
    
    //Joint 3 Rotation Control
    GLUI_Spinner *joint_spinner3
    = glui->add_spinner("Joint 3", GLUI_SPINNER_FLOAT,
                        &joint3_rot);
    joint_spinner3->set_speed(0.15);
    joint_spinner3->set_float_limits(JOINT3_MIN, JOINT3_MAX,
                                     GLUI_LIMIT_CLAMP);
    //Joint 4 Rotation Control
    GLUI_Spinner *joint_spinner4
    = glui->add_spinner("Joint 4", GLUI_SPINNER_FLOAT,
                        &joint4_rot);
    joint_spinner4->set_speed(0.08);
    joint_spinner4->set_float_limits(JOINT4_MIN, JOINT4_MAX,
                                     GLUI_LIMIT_CLAMP);
    
    // Add button to specify animation mode
    glui->add_separator();
    glui->add_checkbox("Animate", &animate_mode, 0, animateButton);
    
    // Add "Quit" button
    glui->add_separator();
    glui->add_button("Quit", 0, quitButton);
    
    // Set the main window to be the "active" window
    glui->set_main_gfx_window(windowID);
}

// Performs most of the OpenGL intialization
void initGl(void)
{
    // glClearColor (red, green, blue, alpha)
    
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f, 0.7f, 0.9f, 1.0f);
}

// Callback idle function for animating the scene
void animate()
{
    // Update Main Bar geometry
    const double main_tran_speed = 0.05;
    double main_tran_t = (sin(animation_frame*main_tran_speed) + 1.0) / 2.0;
    main_tran = main_tran_t * MAIN_MIN + (1 - main_tran_t) * MAIN_MAX;
   
    // Update Joint 1 geometry
    const double joint_rot_speed = 0.1;
    double joint_rot_t = (sin(animation_frame*joint_rot_speed) + 1.0) / 2.0;
    joint_rot = joint_rot_t * JOINT_MIN + (1 - joint_rot_t) * JOINT_MAX;
    
    //Update Joint 2 geometry
    const double joint_rot_speed2 = 0.05;
    double joint_rot_t2 = (sin(animation_frame*joint_rot_speed2) + 1.0) / 2.0;
    joint2_rot = joint_rot_t2 * JOINT2_MIN + (1 - joint_rot_t2) * JOINT2_MAX;
    
    //Update Joint 3 geometry
    const double joint_rot_speed3 = 0.15;
    double joint_rot_t3 = (sin(animation_frame*joint_rot_speed3) + 1.0) / 2.0;
    joint3_rot = joint_rot_t3 * JOINT3_MIN + (1 - joint_rot_t3) * JOINT3_MAX;
    //Update Joint 3 geometry
    const double joint_rot_speed4 = 0.08;
    double joint_rot_t4 = (sin(animation_frame*joint_rot_speed4) + 1.0) / 2.0;
    joint4_rot = joint_rot_t4 * JOINT4_MIN + (1 - joint_rot_t4) * JOINT4_MAX;
    
    // Update user interface
    glui->sync_live();
    // Tell glut window to update itself.  This will cause the display()
    // callback to be called, which renders the object (once you've written
    // the callback).
    glutSetWindow(windowID);
    
    glutPostRedisplay();
    
    // increment the frame number.
    animation_frame++;
    
    // Wait 50 ms between frames (20 frames per second)
    usleep(50000);
}
// Handles the window being resized by updating the viewport
// and projection matrices
void myReshape(int w, int h)
{
    // Setup projection matrix for new window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    //gluOrtho2D(-w / 2, w / 2, -h / 2, h / 2);
    gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 10, 1600);
    
    // Update OpenGL viewport and internal variables
    glViewport(0, 0, w, h);
    Win[0] = w;
    Win[1] = h;
}

// display callback
void display(void)
{
    // glClearColor (red, green, blue, alpha)
    // Ignore the meaning of the 'alpha' value for now
    glClearColor(0.7f, 0.7f, 0.9f, 1.0f);
    
    //Clear the screen with the background colour
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Setup the model-view transformation matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
    
    // Draw object
    const float BODY_WIDTH = 300.0f;
    const float BODY_LENGTH = 30.0f;
    const float BODY_DEPTH = 20.0f;
    const float ARM_LENGTH = 50.0f;
    const float ARM_WIDTH = 10.0f;
    
    // Push the current transformation matrix on the stack
    glPushMatrix();
    
    // Draw the main bar
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    
    // Scale square to size of body
    glScalef(BODY_WIDTH, BODY_LENGTH, 15.0);
    
    // Set the color to white
    glColor3f(1.0, 1.0, 1.0);
    
    // Draw the square for the body
    glutWireCube(1.0);
    //drawSquare(1.0);
    glPopMatrix();
    /*----------------------------------------------*/
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    // Draw the 'F' and translate to position on bar
    glTranslatef(-100.0, -0.5, 0.0);
    // Move to the joint hinge
    glTranslatef(0.0, -BODY_LENGTH / 2 + ARM_WIDTH, 0.0);
    // Rotate along the hinge
    glRotatef(joint_rot, 0.0, 0.0, 1.0);
    // Scale the size of the arm
    glScalef(ARM_WIDTH, ARM_LENGTH, 15.0);
    // Move to center location of arm, under previous rotation
    glTranslatef(0.0, -0.5, 0.0);
    // Draw the square for the arm
    glColor3f(0.8, 0.2, 0.2);
    glutWireCube(1.0);
    //drawSquare(1.0);
    //make 'F' blue
    glColor3f(0.0, 0.0, 1.0);
    drawF();
    // Retrieve the previous state of the transformation stack
    glPopMatrix();
    //end of 'F'
  /*----------------------------------------------*/
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    // Draw the 'R' and put in position on main bar
    
    glTranslatef(-33.0, -0.5, 0.0);
    // Move to the joint hinge
    glTranslatef(0.0, -BODY_LENGTH / 2 + ARM_WIDTH, 0.0);
    // Rotate along the hinge
    glRotatef(joint2_rot, 0.0, 0.0, 1.0);
    // Scale the size of the arm
    glScalef(ARM_WIDTH, ARM_LENGTH, 15.0);
    // Move to center location of arm, under previous rotation
    glTranslatef(0.0, -0.5, 0.0);
    // Draw the square for the arm
    glColor3f(0.8, 0.2, 0.2);
    glutWireCube(1.0);
    //drawSquare(1.0);
    //make 'R' blue
    glColor3f(0.0, 0.0, 1.0);
    drawR();
    // Retrieve the previous state of the transformation stack
    glPopMatrix();
    //end of 'R'
  /*----------------------------------------------*/
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    // Draw the 'E' and put in position on main bar
    glTranslatef(33.0, -0.5, 0.0);
    // Move to the joint hinge
    glTranslatef(0.0, -BODY_LENGTH / 2 + ARM_WIDTH, 0.0);
    // Rotate along the hinge
    glRotatef(joint3_rot, 0.0, 0.0, 1.0);
    // Scale the size of the arm
    glScalef(ARM_WIDTH, ARM_LENGTH, 15.0);
    // Move to center location of arm, under previous rotation
    glTranslatef(0.0, -0.5, 0.0);
    // Draw the square for the arm
    glColor3f(0.8, 0.2, 0.2);
    glutWireCube(1.0);
    //drawSquare(1.0);
    //make 'E' blue
    glColor3f(0.0, 0.0, 1.0);
    drawE();
    // Retrieve the previous state of the transformation stack
    glPopMatrix();
    //end of 'E'
  /*----------------------------------------------*/
    glPushMatrix();
    //translate according to main translator
    
    glTranslatef(main_tran, main_tran, 0.0);
    // Draw the 'D'
    glTranslatef(100.0, -0.5, 0.0);
    // Move to the joint hinge
    glTranslatef(0.0, -BODY_LENGTH / 2 + ARM_WIDTH, 0.0);
    // Rotate along the hinge
    glRotatef(joint4_rot, 0.0, 0.0, 1.0);
    // Scale the size of the arm
    glScalef(ARM_WIDTH, ARM_LENGTH, 15.0);
    // Move to center location of arm, under previous rotation
    glTranslatef(0.0, -0.5, 0.0);
    // Draw the square for the arm
    glColor3f(0.8, 0.2, 0.2);
    glutWireCube(1.0);
    //drawSquare(1.0);
    //make 'D' blue
    glColor3f(0.0, 0.0, 1.0);
    drawD();
    // Retrieve the previous state of the transformation stack
    glPopMatrix();
    //end of 'D'
    /*----------------------------------------------*/
    //circle for hinge 'F'
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    //put in position on bar
    glTranslatef(-100.0, -0.5, 0.0);
    //make circle black
    glColor3f(0.0, 0.0, 0.0);
    drawCircle();
    glPopMatrix();
    
    //circle for hinge R
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    //put in position on bar
    glTranslatef(-33.0, -0.5, 0.0);
    //make circle black
    glColor3f(0.0, 0.0, 0.0);
    drawCircle();
    glPopMatrix();
    
    //circle for hinge E
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    
    //put in position on bar
    glTranslatef(33.0, -0.5, 0.0);
    //make circle black
    glColor3f(0.0, 0.0, 0.0);
    drawCircle();
    glPopMatrix();
    //circle for hinge D
    glPushMatrix();
    //translate according to main translator
    glTranslatef(main_tran, main_tran, 0.0);
    //put in position on bar
    glTranslatef(100.0, -0.5, 0.0);
    //make circle black
    glColor3f(0.0, 0.0, 0.0);
    drawCircle();
    glPopMatrix();
    /*----------------------------------------------*/
    // Execute any GL functions that are in the queue just to be safe
    glFlush();
    
    // Now, show the frame buffer that we just drew into.
    // (this prevents flickering).
    glutSwapBuffers();
}

// Draw a square of the specified size, centered at the current location
void drawSquare(float width)
{
    // Draw the square
    glBegin(GL_POLYGON);
    glVertex2d(-width / 2, -width / 2);
    glVertex2d(width / 2, -width / 2);
    glVertex2d(width / 2, width / 2);
    glVertex2d(-width / 2, width / 2);
    glEnd();
}

//function to compute and draw circles
void drawCircle() {
    float x, y;
    const float radius = 6.0f;
    float twoPI = 2.0f * PI;
    glBegin(GL_LINE_LOOP);
    for (float t = 0; t <= 360; t++)
    {
        x = radius * cos(t * twoPI / 360);
        
        y = radius * sin(t * twoPI / 360);
        glVertex2f(x, y);
    }
    glEnd(); }
//function to draw 'F' using 3d cubes
void drawF() {
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glutWireCube(1.0);
    glTranslatef(1.5, 0.4, 0.0);
    glScalef(2.0, 0.2, 1.0);
    glutWireCube(1.0);
    glTranslatef(0.0, -1.5, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
}
//function to draw 'R' using 3d cubes
void drawR() {
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glutWireCube(1.0);
    glTranslatef(1.5, 0.4, 0.0);
    glScalef(2.0, 0.2, 1.0);
    glutWireCube(1.0);
    glTranslatef(0.0, -1.8, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.0, -1.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
}
//function to draw 'E' using 3d cubes
void drawE() {
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glutWireCube(1.0);
    
    glTranslatef(1.5, 0.4, 0.0);
    glScalef(2.0, 0.2, 1.0);
    glutWireCube(1.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutWireCube(1.0);
    glTranslatef(0.0, -2.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
}
//function to draw 'D' using 3d cubes
void drawD() {
    glPushMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glutWireCube(1.0);
    glTranslatef(1.5, 0.4, 0.0);
    glScalef(2.0, 0.2, 1.0);
    glutWireCube(1.0);
    glTranslatef(0.0, -4.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(2.25, -1.0, 0.0);
    glutWireCube(1.0);
    glPopMatrix();
}
