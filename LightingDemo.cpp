#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <GL/glut.h>

enum {
    BRASS, RED_PLAST
    IC, EMERALD, SLATE
} MaterialType;
enum {
    F_MATERIAL = 1
} MaterialDisplayList;

enum {
    LIGHT_OFF, LIGHT_PURPLE, LIGHT_WHITE, LIGHT_YELLOW
} LightValues;

GLfloat
purple_light[] = { 1.0, 0.0, 1.0, 1.0 },
yellow_light[] = { 1.0, 1.0, 0.0, 1.0 },
white_light[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat left_light_position[] = { -1.0, 0.0, 1.0, 0.0 },
        right_light_position[] = { 1.0, 0.0, 1.0, 0.0 };

GLfloat brass_ambient[] = { 0.33, 0.22, 0.03, 1.0 },
        brass_diffuse[] = { 0.78, 0.57, 0.11, 1.0 },
        brass_specular[] = { 0.99, 0.91, 0.81, 1.0 },
        brass_shininess = 27.8;

GLfloat red_plastic_ambient[] = { 0.0, 0.0, 0.0 },
        red_plastic_diffuse[] = { 0.5, 0.0, 0.0 },
        red_plastic_specular[] = { 0.7, 0.6, 0.6 },
        red_plastic_shininess = 32.0;

GLfloat emerald_ambient[] = { 0.0215, 0.1745, 0.0215 },
        emerald_diffuse[] = { 0.07568, 0.61424, 0.07568 },
        emerald_specular[] = { 0.633, 0.727811, 0.633 },
        emerald_shininess = 76.8;

GLfloat slate_ambient[] = { 0.02, 0.02, 0.02 },
        slate_diffuse[] = { 0.02, 0.01, 0.01 },
        slate_specular[] = { 0.4, 0.4, 0.4 },
        slate_shininess = .78125;

int shade_model = GL_SMOOTH;
char *left_light, *right_light;
char *f_material;

void output(GLfloat x, GLfloat y, char *format, ...)
{
    va_list args;
    char buffer[200], *p;
    
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    glPushMatrix();
    glTranslatef(x, y, 0);
    for (p = buffer; *p; p++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    glPopMatrix();
}
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    //move F into position
    glTranslatef(-0.25, 0.0, 0.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    
    //draw top bar of F
    glPushMatrix();
    glTranslatef(1.0, 1.25, 0.0);
    glScalef(1.5, 0.5, 0.5);
    glCallList(F_MATERIAL);
    glutSolidCube(1.0);
    glPopMatrix();
    
    //draw bottom bar of F
    glPushMatrix();
    glTranslatef(0.85, 0.0, 0.0);
    glScalef(1.35, 0.5, 0.5);
    glCallList(F_MATERIAL);
    glutSolidCube(1.0);
    glPopMatrix();
    
    //draw vertical bar of F
    glPushMatrix();
    glScalef(0.5, 3.0, 0.5);
    glCallList(F_MATERIAL);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 3000, 0, 3000);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    output(180, 2850, "Caleb Sarikey");
    output(880, 2550 , "'F' Material: %s", f_material);
    output(80, 250, "Left light: %s", left_light);
    output(1700, 250, "Right light: %s", right_light);
    output(850, 100, "Shade model: %s",
           shade_model == GL_SMOOTH ? "smooth" : "flat");
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glutSwapBuffers();
}
void light_select(GLenum which, int value, char **label)
{
    glEnable(which);
    switch (value) {
        case LIGHT_OFF:
            *label = "off";
            glDisable(which);
            break;
        case LIGHT_PURPLE:
            *label = "purple";
            glLightfv(which, GL_DIFFUSE, purple_light);
            break;
        case LIGHT_WHITE:
            *label = "white";
            glLightfv(which, GL_DIFFUSE, white_light);
            break;
        case LIGHT_YELLOW:
            *label = "yellow";
            glLightfv(which, GL_DIFFUSE, yellow_light);
            break;
    }
    glutPostRedisplay();
}

void left_light_select(int value)
{
    light_select(GL_LIGHT0, value, &left_light);
}

void right_light_select(int value)
{
    light_select(GL_LIGHT1, value, &right_light);
}

void material(int dlist, GLfloat * ambient, GLfloat * diffuse, GLfloat * specular, GLfloat shininess)
{
    glNewList(dlist, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    glEndList();
}

char *material_select(int object, int value)
{
    glutPostRedisplay();
    switch (value) {
        case BRASS:
            material(object, brass_ambient, brass_diffuse, brass_specular, brass_shininess);
            return "brass";
        case RED_PLASTIC:
            material(object, red_plastic_ambient, red_plastic_diffuse, red_plastic_specular, red_plastic_shininess);
            return "red plastic";
        case EMERALD:
            material(object, emerald_ambient, emerald_diffuse, emerald_specular, emerald_shininess);
            return "emerald";
        case SLATE:
            material(object, slate_ambient, slate_diffuse, slate_specular, slate_shininess);
            return "slate";
    }
    return NULL;// avoid bogus warning!
}

    void f_select(int value)
    {
        f_material = material_select(F_MATERIAL, value);
    }

    void main_menu_select(int value)
    {
        if (value == 666)
            exit(0);
        glShadeModel(shade_model = value);
        glutPostRedisplay();
    }

    int main(int argc, char **argv)
    {
        int left_light_m, right_light_m, f_m;
        
        glutInitWindowSize(400, 400);
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutCreateWindow("Caleb Sarikey");
        glutDisplayFunc(display);
        
#define LIGHT_MENU_ENTRIES() \
    glutAddMenuEntry("Disable", LIGHT_OFF); \
    glutAddMenuEntry("Purple", LIGHT_PURPLE); \
    glutAddMenuEntry("White", LIGHT_WHITE); \
    glutAddMenuEntry("Yellow", LIGHT_YELLOW);
        
#define MATERIAL_MENU_ENTRIES() \
    glutAddMenuEntry("Brass", BRASS); \
    glutAddMenuEntry("Red Plastic", RED_PLASTIC); \
    glutAddMenuEntry("Emerald", EMERALD); \
    glutAddMenuEntry("Slate", SLATE);
        
        left_light_m = glutCreateMenu(left_light_select);
        LIGHT_MENU_ENTRIES();
        right_light_m = glutCreateMenu(right_light_select);
        LIGHT_MENU_ENTRIES();
        f_m = glutCreateMenu(f_select);
        MATERIAL_MENU_ENTRIES();
        
        glutCreateMenu(main_menu_select);
        glutAddMenuEntry("Smooth shading", GL_SMOOTH);
        glutAddMenuEntry("Flat shading", GL_FLAT);
        glutAddSubMenu("Left light", left_light_m);
        glutAddSubMenu("Right light", right_light_m);
        glutAddSubMenu("F", f_m);
        glutAddMenuEntry("Quit", 666);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        
        glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
        glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
        glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
        glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
        left_light_select(LIGHT_WHITE);
        right_light_select(LIGHT_YELLOW);
        f_select(SLATE);
        
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
        
        glEnable(GL_NORMALIZE);
        glLineWidth(1.0);
        glMatrixMode(GL_PROJECTION);
        gluPerspective( /* degrees field of view */ 50.0,/* aspect ratio */ 1.0, /* Z near */ 1.0, /* Z far */ 10.0);
        glMatrixMode(GL_MODELVIEW);
        gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
                  0.0, 0.0, 0.0,      /* center is at (0,0,0) */
                  0.0, 1.0, 0.);      /* up is in positive Y direction */
        glTranslatef(0.0, 0.0, -1.0);
        
        glutMainLoop();
        return 0;           /* ANSI C requires main to return int. */
    }
