/*
Description: Rubik's Cube
Instructions:
	1.) Use your mouse to click and rotate the entire cube
	2.) Use the following character keys to rotate cube slices:
			'q', 'a', 'w', 's', 'e', 'd', 'r', 'f', 't', 'g', 'y', and 'h'
	3.) Use character key 'o' to reverse all moves and solve the cube
	4.) Use symbol keys '+' and '-' to increase/decrease speed of rotations respectively
	5.) Use character key 'z' to zoom in and character key 'x' to zoom out
*/

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

char*s = "1.) Use your mouse to click and rotate the entire cube";
char*s1 = "2.) Character keys to rotate cube slices: q, a, w, s, e, d, r, f, t, g, y, h";
char*s2 = "3.) Use character key 'o' to solve the cube.";
char*s3 = "4.) Use symbol keys '+' and '-' to increase/decrease speed of rotations";
char*s4 = "5.) Use character key 'z' to zoom in and character key 'x' to zoom out";


//to control camera zoom
int eye_z = 22;

//for speed of rotations
static float speed = 1.5;

//arrays to hold data for the 6 faces of the cube
static int
top[3][3] = { { 0,0,0 },{ 0,0,0 },{ 0,0,0 } },
right[3][3] = { { 1,1,1 },{ 1,1,1 },{ 1,1,1 } },
front[3][3] = { { 2,2,2 },{ 2,2,2 },{ 2,2,2 } },
back[3][3] = { { 3,3,3 },{ 3,3,3 },{ 3,3,3 } },
bottom[3][3] = { { 4,4,4 },{ 4,4,4 },{ 4,4,4 } },
left[3][3] = { { 5,5,5 },{ 5,5,5 },{ 5,5,5 } },

temp[3][3];

int solve[300];
int count = 0;
int solve1 = 0;
static int rotation = 0;
int rotationcomplete = 0;
static GLfloat theta = 0.0;
static GLint axis = 0;
static GLfloat p = 0.0, q = 0.0, r = 0.0;
static GLint inverse = 0;
static GLfloat angle = 0.0;
int beginx = 0, beginy = 0;
int moving = 0;

//vertices for cubes
GLfloat vertices[][3] = {

	//center
	{ -1.0,-1.0,-1.0 },
	{ 1.0,-1.0,-1.0 },
	{ 1.0,1.0,-1.0 },
	{ -1.0,1.0,-1.0 },
	{ -1.0,-1.0,1.0 },
	{ 1.0,-1.0,1.0 },
	{ 1.0,1.0,1.0 },
	{ -1.0,1.0,1.0 },

	//bottom center
	{ -1.0,-3.0,-1.0 },
	{ 1.0,-3.0,-1.0 },
	{ 1.0,-1.0,-1.0 },
	{ -1.0,-1.0,-1.0 },
	{ -1.0,-3.0,1.0 },
	{ 1.0,-3.0,1.0 },
	{ 1.0,-1.0,1.0 },
	{ -1.0,-1.0,1.0 },

	//left center
	{ -3.0,-1.0,-1.0 },
	{ -1.0,-1.0,-1.0 },
	{ -1.0,1.0,-1.0 },
	{ -3.0,1.0,-1.0 },
	{ -3.0,-1.0,1.0 },
	{ -1.0,-1.0,1.0 },
	{ -1.0,1.0,1.0 },
	{ -3.0,1.0,1.0 },

	// right center
	{ 1.0,-1.0,-1.0 },
	{ 3.0,-1.0,-1.0 },
	{ 3.0,1.0,-1.0 },
	{ 1.0,1.0,-1.0 },
	{ 1.0,-1.0,1.0 },
	{ 3.0,-1.0,1.0 },
	{ 3.0,1.0,1.0 },
	{ 1.0,1.0,1.0 },

	// top center
	{ -1.0,1.0,-1.0 },
	{ 1.0,1.0,-1.0 },
	{ 1.0,3.0,-1.0 },
	{ -1.0,3.0,-1.0 },
	{ -1.0,1.0,1.0 },
	{ 1.0,1.0,1.0 },
	{ 1.0,3.0,1.0 },
	{ -1.0,3.0,1.0 },

	//front center
	{ -1.0,-1.0,1.0 },
	{ 1.0,-1.0,1.0 },
	{ 1.0,1.0,1.0 },
	{ -1.0,1.0,1.0 },
	{ -1.0,-1.0,3.0 },
	{ 1.0,-1.0,3.0 },
	{ 1.0,1.0,3.0 },
	{ -1.0,1.0,3.0 },

	//back center
	{ -1.0,-1.0,-3.0 },
	{ 1.0,-1.0,-3.0 },
	{ 1.0,1.0,-3.0 },
	{ -1.0,1.0,-3.0 },
	{ -1.0,-1.0,-1.0 },
	{ 1.0,-1.0,-1.0 },
	{ 1.0,1.0,-1.0 },
	{ -1.0,1.0,-1.0 },

	// top left center
	{ -3.0,1.0,-1.0 },
	{ -1.0,1.0,-1.0 },
	{ -1.0,3.0,-1.0 },
	{ -3.0,3.0,-1.0 },
	{ -3.0,1.0,1.0 },
	{ -1.0,1.0,1.0 },
	{ -1.0,3.0,1.0 },
	{ -3.0,3.0,1.0 },

	// top right center
	{ 1.0,1.0,-1.0 },
	{ 3.0,1.0,-1.0 },
	{ 3.0,3.0,-1.0 },
	{ 1.0,3.0,-1.0 },
	{ 1.0,1.0,1.0 },
	{ 3.0,1.0,1.0 },
	{ 3.0,3.0,1.0 },
	{ 1.0,3.0,1.0 },

	// top front center
	{ -1.0,1.0,1.0 },
	{ 1.0,1.0,1.0 },
	{ 1.0,3.0,1.0 },
	{ -1.0,3.0,1.0 },
	{ -1.0,1.0,3.0 },
	{ 1.0,1.0,3.0 },
	{ 1.0,3.0,3.0 },
	{ -1.0,3.0,3.0 },

	// top back center
	{ -1.0,1.0,-3.0 },
	{ 1.0,1.0,-3.0 },
	{ 1.0,3.0,-3.0 },
	{ -1.0,3.0,-3.0 },
	{ -1.0,1.0,-1.0 },
	{ 1.0,1.0,-1.0 },
	{ 1.0,3.0,-1.0 },
	{ -1.0,3.0,-1.0 },

	// bottom left center
	{ -3.0,-3.0,-1.0 },
	{ -1.0,-3.0,-1.0 },
	{ -1.0,-1.0,-1.0 },
	{ -3.0,-1.0,-1.0 },
	{ -3.0,-3.0,1.0 },
	{ -1.0,-3.0,1.0 },
	{ -1.0,-1.0,1.0 },
	{ -3.0,-1.0,1.0 },

	//bottom right center
	{ 1.0,-3.0,-1.0 },
	{ 3.0,-3.0,-1.0 },
	{ 3.0,-1.0,-1.0 },
	{ 1.0,-1.0,-1.0 },
	{ 1.0,-3.0,1.0 },
	{ 3.0,-3.0,1.0 },
	{ 3.0,-1.0,1.0 },
	{ 1.0,-1.0,1.0 },

	//bottom front center
	{ -1.0,-3.0,1.0 },
	{ 1.0,-3.0,1.0 },
	{ 1.0,-1.0,1.0 },
	{ -1.0,-1.0,1.0 },
	{ -1.0,-3.0,3.0 },
	{ 1.0,-3.0,3.0 },
	{ 1.0,-1.0,3.0 },
	{ -1.0,-1.0,3.0 },

	//bottom back center
	{ -1.0,-3.0,-3.0 },
	{ 1.0,-3.0,-3.0 },
	{ 1.0,-1.0,-3.0 },
	{ -1.0,-1.0,-3.0 },
	{ -1.0,-3.0,-1.0 },
	{ 1.0,-3.0,-1.0 },
	{ 1.0,-1.0,-1.0 },
	{ -1.0,-1.0,-1.0 },

	// top left back
	{ -3.0,1.0,-3.0 },
	{ -1.0,1.0,-3.0 },
	{ -1.0,3.0,-3.0 },
	{ -3.0,3.0,-3.0 },
	{ -3.0,1.0,-1.0 },
	{ -1.0,1.0,-1.0 },
	{ -1.0,3.0,-1.0 },
	{ -3.0,3.0,-1.0 },

	// top left front
	{ -3.0,1.0,1.0 },
	{ -1.0,1.0,1.0 },
	{ -1.0,3.0,1.0 },
	{ -3.0,3.0,1.0 },
	{ -3.0,1.0,3.0 },
	{ -1.0,1.0,3.0 },
	{ -1.0,3.0,3.0 },
	{ -3.0,3.0,3.0 },

	// top right back
	{ 1.0,1.0,-3.0 },
	{ 3.0,1.0,-3.0 },
	{ 3.0,3.0,-3.0 },
	{ 1.0,3.0,-3.0 },
	{ 1.0,1.0,-1.0 },
	{ 3.0,1.0,-1.0 },
	{ 3.0,3.0,-1.0 },
	{ 1.0,3.0,-1.0 },

	// top right front
	{ 1.0,1.0,1.0 },
	{ 3.0,1.0,1.0 },
	{ 3.0,3.0,1.0 },
	{ 1.0,3.0,1.0 },
	{ 1.0,1.0,3.0 },
	{ 3.0,1.0,3.0 },
	{ 3.0,3.0,3.0 },
	{ 1.0,3.0,3.0 },

	// ceneter left back
	{ -3.0,-1.0,-3.0 },
	{ -1.0,-1.0,-3.0 },
	{ -1.0,1.0,-3.0 },
	{ -3.0,1.0,-3.0 },
	{ -3.0,-1.0,-1.0 },
	{ -1.0,-1.0,-1.0 },
	{ -1.0,1.0,-1.0 },
	{ -3.0,1.0,-1.0 },

	// center left front
	{ -3.0,-1.0,1.0 },
	{ -1.0,-1.0,1.0 },
	{ -1.0,1.0,1.0 },
	{ -3.0,1.0,1.0 },
	{ -3.0,-1.0,3.0 },
	{ -1.0,-1.0,3.0 },
	{ -1.0,1.0,3.0 },
	{ -3.0,1.0,3.0 },

	// center right back
	{ 1.0,-1.0,-3.0 },
	{ 3.0,-1.0,-3.0 },
	{ 3.0,1.0,-3.0 },
	{ 1.0,1.0,-3.0 },
	{ 1.0,-1.0,-1.0 },
	{ 3.0,-1.0,-1.0 },
	{ 3.0,1.0,-1.0 },
	{ 1.0,1.0,-1.0 },

	// center right front
	{ 1.0,-1.0,1.0 },
	{ 3.0,-1.0,1.0 },
	{ 3.0,1.0,1.0 },
	{ 1.0,1.0,1.0 },
	{ 1.0,-1.0,3.0 },
	{ 3.0,-1.0,3.0 },
	{ 3.0,1.0,3.0 },
	{ 1.0,1.0,3.0 },

	// bottom left back
	{ -3.0,-3.0,-3.0 },
	{ -1.0,-3.0,-3.0 },
	{ -1.0,-1.0,-3.0 },
	{ -3.0,-1.0,-3.0 },
	{ -3.0,-3.0,-1.0 },
	{ -1.0,-3.0,-1.0 },
	{ -1.0,-1.0,-1.0 },
	{ -3.0,-1.0,-1.0 },

	// bottom left front
	{ -3.0,-3.0,1.0 },
	{ -1.0,-3.0,1.0 },
	{ -1.0,-1.0,1.0 },
	{ -3.0,-1.0,1.0 },
	{ -3.0,-3.0,3.0 },
	{ -1.0,-3.0,3.0 },
	{ -1.0,-1.0,3.0 },
	{ -3.0,-1.0,3.0 },

	// bottom right back
	{ 1.0,-3.0,-3.0 },
	{ 3.0,-3.0,-3.0 },
	{ 3.0,-1.0,-3.0 },
	{ 1.0,-1.0,-3.0 },
	{ 1.0,-3.0,-1.0 },
	{ 3.0,-3.0,-1.0 },
	{ 3.0,-1.0,-1.0 },
	{ 1.0,-1.0,-1.0 },

	// bottom right front
	{ 1.0,-3.0,1.0 },
	{ 3.0,-3.0,1.0 },
	{ 3.0,-1.0,1.0 },
	{ 1.0,-1.0,1.0 },
	{ 1.0,-3.0,3.0 },
	{ 3.0,-3.0,3.0 },
	{ 3.0,-1.0,3.0 },
	{ 1.0,-1.0,3.0 },
};

//colors for cubes
GLfloat color[][3] = {
	{ 1.0,1.0,1.0 },  //white
	{ 1.0,0.0,0.5 },  //purple
	{ 0.0,0.0,1.0 },  //blue
	{ 0.0,1.0,0.0 },  //green
	{ 1.0,1.0,0.0 },  //yellow
	{ 1.0,0.0,0.0 }, //red
};

//creates border and panel for cubes
void panel(int a, int b, int c, int d, int e)
{
	//outline of panel
	glColor3f(0, 0, 0);
	glLineWidth(3.0);
	glBegin(GL_LINE_LOOP);

	//passes a pointer to an array of 3 float values
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glVertex3fv(vertices[e]);
	glEnd();

	//1 panel
	glColor3fv(color[a]);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glVertex3fv(vertices[e]);
	glEnd();
}

//start creation of cubes, using panel function
// center piece
void cube1()
{
	panel(6, 0, 3, 2, 1);
	panel(6, 2, 3, 7, 6);
	panel(6, 0, 4, 7, 3);
	panel(6, 1, 2, 6, 5);
	panel(6, 4, 5, 6, 7);
	panel(6, 0, 1, 5, 4);
}

// bottom center
void cube2()
{
	panel(6, 8, 11, 10, 9);
	panel(6, 10, 11, 15, 14);
	panel(6, 8, 12, 15, 11);
	panel(6, 9, 10, 14, 13);
	panel(6, 12, 13, 14, 15);
	panel(bottom[1][1], 8, 9, 13, 12);
}

// left center
void cube3()
{
	panel(6, 16, 19, 18, 17);
	panel(6, 18, 19, 23, 22);
	panel(left[1][1], 16, 20, 23, 19);
	panel(6, 17, 18, 22, 21);
	panel(6, 20, 21, 22, 23);
	panel(6, 16, 17, 21, 20);
}

// right center
void cube4()
{
	panel(6, 24, 27, 26, 25);
	panel(6, 26, 27, 31, 30);
	panel(6, 24, 28, 31, 27);
	panel(right[1][1], 25, 26, 30, 29);
	panel(6, 28, 29, 30, 31);
	panel(6, 24, 25, 29, 28);
}

// top center
void cube5()
{
	panel(6, 32, 35, 34, 33);
	panel(top[1][1], 34, 35, 39, 38);
	panel(6, 32, 36, 39, 35);
	panel(6, 33, 34, 38, 37);
	panel(6, 36, 37, 38, 39);
	panel(6, 32, 33, 37, 36);
}

// front center
void cube6()
{
	panel(6, 40, 43, 42, 41);
	panel(6, 42, 43, 47, 46);
	panel(6, 40, 44, 47, 43);
	panel(6, 41, 42, 46, 45);
	panel(front[1][1], 44, 45, 46, 47);
	panel(6, 40, 41, 45, 44);
}

//back center
void cube7()
{
	panel(back[1][1], 48, 51, 50, 49);
	panel(6, 50, 51, 55, 54);
	panel(6, 48, 52, 55, 51);
	panel(6, 49, 50, 54, 53);
	panel(6, 52, 53, 54, 55);
	panel(6, 48, 49, 53, 52);
}

// top left center
void cube8()
{
	panel(6, 56, 59, 58, 57);
	panel(top[1][0], 58, 59, 63, 62);
	panel(left[0][1], 56, 60, 63, 59);
	panel(6, 57, 58, 62, 61);
	panel(6, 60, 61, 62, 63);
	panel(6, 56, 57, 61, 60);
}

// top right center
void cube9()
{
	panel(6, 64, 67, 66, 65);
	panel(top[1][2], 66, 67, 71, 70);
	panel(6, 64, 68, 71, 67);
	panel(right[0][1], 65, 66, 70, 69);
	panel(6, 68, 69, 70, 71);
	panel(6, 64, 65, 69, 68);
}

// top front center
void cube10()
{
	panel(6, 72, 75, 74, 73);
	panel(top[2][1], 74, 75, 79, 78);
	panel(6, 72, 76, 79, 75);
	panel(6, 73, 74, 78, 77);
	panel(front[0][1], 76, 77, 78, 79);
	panel(6, 72, 73, 77, 76);
}

// top back center
void cube11()
{
	panel(back[0][1], 80, 83, 82, 81);
	panel(top[0][1], 82, 83, 87, 86);
	panel(6, 80, 84, 87, 83);
	panel(6, 81, 82, 86, 85);
	panel(6, 84, 85, 86, 87);
	panel(6, 80, 81, 85, 84);
}

// bottom left center
void cube12()
{
	panel(6, 80 + 8, 83 + 8, 82 + 8, 81 + 8);
	panel(6, 82 + 8, 83 + 8, 87 + 8, 86 + 8);
	panel(left[2][1], 80 + 8, 84 + 8, 87 + 8, 83 + 8);
	panel(6, 81 + 8, 82 + 8, 86 + 8, 85 + 8);
	panel(6, 84 + 8, 85 + 8, 86 + 8, 87 + 8);
	panel(bottom[1][0], 80 + 8, 81 + 8, 85 + 8, 84 + 8);
}

// bottom right center
void cube13()
{
	panel(6, 80 + 16, 83 + 16, 82 + 16, 81 + 16);
	panel(6, 82 + 16, 83 + 16, 87 + 16, 86 + 16);
	panel(6, 80 + 16, 84 + 16, 87 + 16, 83 + 16);
	panel(right[2][1], 81 + 16, 82 + 16, 86 + 16, 85 + 16);
	panel(6, 84 + 16, 85 + 16, 86 + 16, 87 + 16);
	panel(bottom[1][2], 80 + 16, 81 + 16, 85 + 16, 84 + 16);
}

// bottom front center
void cube14()
{
	panel(6, 80 + 24, 83 + 24, 82 + 24, 81 + 24);
	panel(6, 82 + 24, 83 + 24, 87 + 24, 86 + 24);
	panel(6, 80 + 24, 84 + 24, 87 + 24, 83 + 24);
	panel(6, 81 + 24, 82 + 24, 86 + 24, 85 + 24);
	panel(front[2][1], 84 + 24, 85 + 24, 86 + 24, 87 + 24);
	panel(bottom[0][1], 80 + 24, 81 + 24, 85 + 24, 84 + 24);
}

// bottom back center
void cube15()
{
	panel(back[2][1], 112, 115, 114, 113);
	panel(6, 114, 115, 119, 118);
	panel(6, 112, 116, 119, 115);
	panel(6, 113, 114, 118, 117);
	panel(6, 116, 117, 118, 119);
	panel(bottom[2][1], 112, 113, 117, 116);
}

// top left back
void cube16()
{
	panel(back[0][2], 120, 123, 122, 121);
	panel(top[0][0], 122, 123, 127, 126);
	panel(left[0][0], 120, 124, 127, 123);
	panel(6, 121, 122, 126, 125);
	panel(6, 124, 125, 126, 127);
	panel(6, 120, 121, 125, 124);
}

// top left front
void cube17()
{
	panel(6, 128, 131, 130, 129);
	panel(top[2][0], 130, 131, 135, 134);
	panel(left[0][2], 128, 132, 135, 131);
	panel(6, 129, 130, 134, 133);
	panel(front[0][0], 132, 133, 134, 135);
	panel(6, 128, 129, 133, 132);
}


// top right back
void cube18()
{
	panel(back[0][0], 136, 139, 138, 137);
	panel(top[0][2], 138, 139, 143, 142);
	panel(6, 136, 140, 143, 139);
	panel(right[0][2], 137, 138, 142, 141);
	panel(6, 140, 141, 142, 143);
	panel(6, 136, 137, 141, 140);
}

// top right front
void cube19()
{
	panel(6, 144, 147, 146, 145);
	panel(top[2][2], 146, 147, 151, 150);
	panel(6, 144, 148, 151, 147);
	panel(right[0][0], 145, 146, 150, 149);
	panel(front[0][2], 148, 149, 150, 151);
	panel(6, 144, 145, 149, 148);
}

//center left back
void cube20()
{
	panel(back[1][2], 152, 155, 154, 153);
	panel(6, 154, 155, 159, 158);
	panel(left[1][0], 152, 156, 159, 155);
	panel(6, 153, 154, 158, 157);
	panel(6, 156, 157, 158, 159);
	panel(6, 152, 153, 157, 156);
}

// center left front
void cube21()
{
	panel(6, 160, 163, 162, 161);
	panel(6, 162, 163, 167, 166);
	panel(left[1][2], 160, 164, 167, 163);
	panel(6, 161, 162, 166, 165);
	panel(front[1][0], 164, 165, 166, 167);
	panel(6, 160, 161, 165, 164);
}

// center right back
void cube22()
{
	panel(back[1][0], 168, 171, 170, 169);
	panel(6, 170, 171, 175, 174);
	panel(6, 168, 172, 175, 171);
	panel(right[1][2], 169, 170, 174, 173);
	panel(6, 172, 173, 174, 175);
	panel(6, 168, 169, 173, 172);
}

//center right front
void cube23()
{
	panel(6, 176, 179, 178, 177);
	panel(6, 178, 179, 183, 182);
	panel(6, 176, 180, 183, 179);
	panel(right[1][0], 177, 178, 182, 181);
	panel(front[1][2], 180, 181, 182, 183);
	panel(6, 176, 177, 181, 180);
}

// bottom left back
void cube24()
{
	panel(back[2][2], 184, 187, 186, 185);
	panel(6, 186, 187, 191, 190);
	panel(left[2][0], 184, 188, 191, 187);
	panel(6, 185, 186, 190, 189);
	panel(6, 188, 189, 190, 191);
	panel(bottom[2][0], 184, 185, 189, 188);
}

// bottom left front
void cube25()
{
	panel(6, 192, 195, 194, 193);
	panel(6, 194, 195, 199, 198);
	panel(left[2][2], 192, 196, 199, 195);
	panel(6, 193, 194, 198, 197);
	panel(front[2][0], 196, 197, 198, 199);
	panel(bottom[0][0], 192, 193, 197, 196);
}

// bottom right back
void cube26()
{
	panel(back[2][0], 200, 203, 202, 201);
	panel(6, 202, 203, 207, 206);
	panel(6, 200, 204, 207, 203);
	panel(right[2][2], 201, 202, 206, 205);
	panel(6, 204, 205, 206, 207);
	panel(bottom[2][2], 200, 201, 205, 204);
}

// bottom right front
void cube27()
{
	panel(6, 208, 211, 210, 209);
	panel(6, 210, 211, 215, 214);
	panel(6, 208, 212, 215, 211);
	panel(right[2][0], 209, 210, 214, 213);
	panel(front[2][2], 212, 213, 214, 215);
	panel(bottom[0][2], 208, 209, 213, 212);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0, 0, eye_z, 0, 0, 0, 0, 1, 0);

	glColor3fv(color[0]);

	glPushMatrix();
	
	//to rotate the cube (start head on with blue side facing)
	glRotatef(30.0 + p, 1.0, 0.0, 0.0);
	glRotatef(45.0 + q, 0.0, 1.0, 0.0);
	glRotatef(0.0 + r, 0.0, 0.0, 1.0);

//display colors, rotations (displaying 27 cubes at any time)
	if (rotation == 0)
	{
		cube1();
		cube2();
		cube3();
		cube4();
		cube5();
		cube6();
		cube7();
		cube8();
		cube9();
		cube10();
		cube11();
		cube12();
		cube13();
		cube14();
		cube15();
		cube16();
		cube17();
		cube18();
		cube19();
		cube20();
		cube21();
		cube22();
		cube23();
		cube24();
		cube25();
		cube26();
		cube27();
	}

	if (rotation == 1)
	{
		cube1();
		cube2();
		cube3();
		cube4();
		cube6();
		cube7();
		cube12();
		cube13();
		cube14();
		cube15();
		cube20();
		cube21();
		cube22();
		cube23();
		cube24();
		cube25();
		cube26();
		cube27();

		if (inverse == 0)
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(-theta, 0.0, 1.0, 0.0);
		}

		else
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(theta, 0.0, 1.0, 0.0);
		}

		cube5();
		cube8();
		cube9();
		cube10();
		cube11();
		cube16();
		cube17();
		cube18();
		cube19();
	}

	if (rotation == 2)
	{
		cube1();
		cube2();
		cube3();
		cube5();
		cube6();
		cube7();
		cube8();
		cube10();
		cube11();
		cube12();
		cube14();
		cube15();
		cube16();
		cube17();
		cube20();
		cube21();
		cube24();
		cube25();

		if (inverse == 0)
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(-theta, 1.0, 0.0, 0.0);
		}

		else
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(theta, 1.0, 0.0, 0.0);
		}

		cube4();
		cube9();
		cube13();
		cube18();
		cube19();
		cube22();
		cube23();
		cube26();
		cube27();
	}

	if (rotation == 3)
	{
		cube1();
		cube2();
		cube3();
		cube4();
		cube5();
		cube7();
		cube8();
		cube9();
		cube11();
		cube12();
		cube13();
		cube15();
		cube16();
		cube18();
		cube20();
		cube22();
		cube24();
		cube26();

		if (inverse == 0)
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(-theta, 0.0, 0.0, 1.0);
		}

		else
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(theta, 0.0, 0.0, 1.0);
		}

		cube6();
		cube10();
		cube14();
		cube17();
		cube19();
		cube21();
		cube23();
		cube25();
		cube27();
	}

	if (rotation == 4)
	{
		cube1();
		cube2();
		cube4();
		cube5();
		cube6();
		cube7();
		cube9();
		cube10();
		cube11();
		cube13();
		cube14();
		cube15();
		cube18();
		cube19();
		cube22();
		cube23();
		cube26();
		cube27();

		if (inverse == 0)
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(theta, 1.0, 0.0, 0.0);
		}

		else
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(-theta, 1.0, 0.0, 0.0);
		}

		cube3();
		cube8();
		cube12();
		cube16();
		cube17();
		cube20();
		cube21();
		cube24();
		cube25();
	}

	if (rotation == 5)
	{
		cube1();
		cube2();
		cube3();
		cube4();
		cube5();
		cube6();
		cube8();
		cube9();
		cube10();
		cube12();
		cube13();
		cube14();
		cube17();
		cube19();
		cube21();
		cube23();
		cube25();
		cube27();

		if (inverse == 0)
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(theta, 0.0, 0.0, 1.0);

		}

		else
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(-theta, 0.0, 0.0, 1.0);
		}

		cube7();
		cube11();
		cube15();
		cube16();
		cube18();
		cube20();
		cube22();
		cube24();
		cube26();
	}

	if (rotation == 6)
	{
		cube1();
		cube3();
		cube4();
		cube5();
		cube6();
		cube7();
		cube8();
		cube9();
		cube10();
		cube11();
		cube16();
		cube17();
		cube18();
		cube19();
		cube20();
		cube21();
		cube22();
		cube23();

		if (inverse == 0)
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();
			glRotatef(theta, 0.0, 1.0, 0.0);
		}

		else
		{
			glPushMatrix();
			glColor3fv(color[0]);
			glPopMatrix();

			glRotatef(-theta, 0.0, 1.0, 0.0);
		}

		cube2();
		cube12();
		cube13();
		cube14();
		cube15();
		cube24();
		cube25();
		cube26();
		cube27();

	}
	
	glPopMatrix();

	/*display directions for user*/
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos3f(-9.5, 10, 0);
	for (int i = 0; i<strlen(s); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s[i]);
	glRasterPos3f(-9.5, 9, 0);
	for (int i = 0; i<strlen(s1); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s1[i]);
	glRasterPos3f(-9.5, 8, 0);
	for (int i = 0; i<strlen(s2); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s2[i]);
	glRasterPos3f(-9.5, 7, 0);
	for (int i = 0; i<strlen(s3); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s3[i]);
	glRasterPos3f(-9.5, 6, 0);
	for (int i = 0; i<strlen(s4); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, s4[i]);
	glPopMatrix();
	/*------------------------------------------------------*/

	glFlush();

	glutSwapBuffers();

}


//position of cubes (front, back, left, right, top, bottom)
void position(char a)
{

	if (a == 'r')
	{
		int temp;
		temp = right[0][0];
		right[0][0] = right[2][0];
		right[2][0] = right[2][2];
		right[2][2] = right[0][2];
		right[0][2] = temp;
		temp = right[1][0];
		right[1][0] = right[2][1];
		right[2][1] = right[1][2];
		right[1][2] = right[0][1];
		right[0][1] = temp;
	}

	if (a == 't')
	{
		int temp;
		temp = top[0][0];
		top[0][0] = top[2][0];
		top[2][0] = top[2][2];
		top[2][2] = top[0][2];
		top[0][2] = temp;
		temp = top[1][0];
		top[1][0] = top[2][1];
		top[2][1] = top[1][2];
		top[1][2] = top[0][1];
		top[0][1] = temp;
	}

	if (a == 'f')
	{
		int temp;
		temp = front[0][0];
		front[0][0] = front[2][0];
		front[2][0] = front[2][2];
		front[2][2] = front[0][2];
		front[0][2] = temp;
		temp = front[1][0];
		front[1][0] = front[2][1];
		front[2][1] = front[1][2];
		front[1][2] = front[0][1];
		front[0][1] = temp;
	}
	
	if (a == 'l')
	{
		int temp;
		temp = left[0][0];
		left[0][0] = left[2][0];
		left[2][0] = left[2][2];
		left[2][2] = left[0][2];
		left[0][2] = temp;
		temp = left[1][0];
		left[1][0] = left[2][1];
		left[2][1] = left[1][2];
		left[1][2] = left[0][1];
		left[0][1] = temp;
	}

	if (a == 'k')
	{
		int temp;
		temp = back[0][0];
		back[0][0] = back[2][0];
		back[2][0] = back[2][2];
		back[2][2] = back[0][2];
		back[0][2] = temp;
		temp = back[1][0];
		back[1][0] = back[2][1];
		back[2][1] = back[1][2];
		back[1][2] = back[0][1];
		back[0][1] = temp;
	}

	if (a == 'b')
	{
		int temp;
		temp = bottom[0][0];
		bottom[0][0] = bottom[2][0];
		bottom[2][0] = bottom[2][2];
		bottom[2][2] = bottom[0][2];
		bottom[0][2] = temp;
		temp = bottom[1][0];
		bottom[1][0] = bottom[2][1];
		bottom[2][1] = bottom[1][2];
		bottom[1][2] = bottom[0][1];
		bottom[0][1] = temp;
	}
	
}

void topc()
{
	position('t');
	int temp1 = front[0][0];
	int temp2 = front[0][1];
	int temp3 = front[0][2];

	front[0][0] = right[0][0];
	front[0][1] = right[0][1];
	front[0][2] = right[0][2];

	right[0][0] = back[0][0];
	right[0][1] = back[0][1];
	right[0][2] = back[0][2];

	back[0][0] = left[0][0];
	back[0][1] = left[0][1];
	back[0][2] = left[0][2];

	left[0][0] = temp1;
	left[0][1] = temp2;
	left[0][2] = temp3;
}

void frontc()
{
	position('f');
	int temp1 = left[0][2];
	int temp2 = left[1][2];
	int temp3 = left[2][2];

	left[0][2] = bottom[0][0];
	left[1][2] = bottom[0][1];
	left[2][2] = bottom[0][2];

	bottom[0][0] = right[2][0];
	bottom[0][1] = right[1][0];
	bottom[0][2] = right[0][0];

	right[2][0] = top[2][2];
	right[1][0] = top[2][1];
	right[0][0] = top[2][0];

	top[2][2] = temp1;
	top[2][1] = temp2;
	top[2][0] = temp3;
}

void rightc()
{
	position('r');
	int temp1 = top[0][2];
	int temp2 = top[1][2];
	int temp3 = top[2][2];

	top[0][2] = front[0][2];
	top[1][2] = front[1][2];
	top[2][2] = front[2][2];

	front[0][2] = bottom[0][2];
	front[1][2] = bottom[1][2];
	front[2][2] = bottom[2][2];


	bottom[0][2] = back[2][0];
	bottom[1][2] = back[1][0];
	bottom[2][2] = back[0][0];

	back[2][0] = temp1;
	back[1][0] = temp2;
	back[0][0] = temp3;
}

void leftc()
{
	position('l');
	int temp1 = front[0][0];
	int temp2 = front[1][0];
	int temp3 = front[2][0];

	front[0][0] = top[0][0];
	front[1][0] = top[1][0];
	front[2][0] = top[2][0];

	top[0][0] = back[2][2];
	top[1][0] = back[1][2];
	top[2][0] = back[0][2];

	back[2][2] = bottom[0][0];
	back[1][2] = bottom[1][0];
	back[0][2] = bottom[2][0];


	bottom[0][0] = temp1;
	bottom[1][0] = temp2;
	bottom[2][0] = temp3;
}

void backc()
{
	position('k');
	int temp1 = top[0][0];
	int temp2 = top[0][1];
	int temp3 = top[0][2];

	top[0][0] = right[0][2];
	top[0][1] = right[1][2];
	top[0][2] = right[2][2];

	right[0][2] = bottom[2][2];
	right[1][2] = bottom[2][1];
	right[2][2] = bottom[2][0];

	bottom[2][2] = left[2][0];
	bottom[2][1] = left[1][0];
	bottom[2][0] = left[0][0];

	left[2][0] = temp1;
	left[1][0] = temp2;
	left[0][0] = temp3;
}

void bottomc()
{
	position('b');
	int temp1 = front[2][0];
	int temp2 = front[2][1];
	int temp3 = front[2][2];

	front[2][0] = left[2][0];
	front[2][1] = left[2][1];
	front[2][2] = left[2][2];

	left[2][0] = back[2][0];
	left[2][1] = back[2][1];
	left[2][2] = back[2][2];

	back[2][0] = right[2][0];
	back[2][1] = right[2][1];
	back[2][2] = right[2][2];

	right[2][0] = temp1;
	right[2][1] = temp2;
	right[2][2] = temp3;
}

//to rotate the cubes
void cubeRotate()
{
	theta += 0.5 + speed;
	if (theta == 360.0)
		theta -= 360.0;
	if (theta >= 90.0)
	{
		rotationcomplete = 1;
		glutIdleFunc(NULL);

		if (rotation == 1 && inverse == 0)
		{
			topc();
		}
		if (rotation == 1 && inverse == 1)
		{
			topc();
			topc();
			topc();
		}

		if (rotation == 2 && inverse == 0)
		{
			rightc();

		}
		if (rotation == 2 && inverse == 1)
		{
			rightc();
			rightc();
			rightc();

		}
		if (rotation == 3 && inverse == 0)
		{
			frontc();
		}
		if (rotation == 3 && inverse == 1)
		{
			frontc();
			frontc();
			frontc();
		}
		if (rotation == 4 && inverse == 0)
		{
			leftc();
		}
		if (rotation == 4 && inverse == 1)
		{
			leftc();
			leftc();
			leftc();

		}
		if (rotation == 5 && inverse == 0)
		{
			backc();
		}
		if (rotation == 5 && inverse == 1)
		{
			backc();
			backc();
			backc();
		}
		if (rotation == 6 && inverse == 0)
		{
			bottomc();
		}
		if (rotation == 6 && inverse == 1)
		{
			bottomc();
			bottomc();
			bottomc();

		}

		rotation = 0;
		theta = 0;
	}

	glutPostRedisplay();
}

//motion from mouse click
void motion(int x, int y)
{
	if (moving) {

		q = q + (x - beginx);
		beginx = x;
		p = p + (y - beginy);
		beginy = y;
		glutPostRedisplay();
	}
}

//mouse click&drag handler
void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		moving = 1;
		beginx = x;
		beginy = y;
	}
}

//keyboard function to control each slice rotation
static void keyboard(unsigned char key, int x, int y)
{
	if (key == 'a'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 1;
		inverse = 0;
		solve[++count] = 1;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'q'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 1;
		inverse = 1;
		solve[++count] = -1;
		glutIdleFunc(cubeRotate);
	}

	if (key == 's'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 2;
		inverse = 0;
		solve[++count] = 2;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'w'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 2;
		inverse = 1;
		solve[++count] = -2;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'd'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 3;
		inverse = 0;
		solve[++count] = 3;
		glutIdleFunc(cubeRotate);

	}

	if (key == 'e'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 3;
		inverse = 1;
		solve[++count] = -3;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'f'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 4;
		inverse = 0;
		solve[++count] = 4;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'r'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 4;
		inverse = 1;
		solve[++count] = -4;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'g'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 5;
		inverse = 0;
		solve[++count] = 5;
		glutIdleFunc(cubeRotate);

	}
	if (key == 't'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 5;
		inverse = 1;
		solve[++count] = -5;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'h'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 6;
		inverse = 0;
		solve[++count] = 6;
		glutIdleFunc(cubeRotate);
	}

	if (key == 'y'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		rotation = 6;
		inverse = 1;
		solve[++count] = -6;
		glutIdleFunc(cubeRotate);
	}

	
	//increase speed of rotations
	if (key == '+'&&rotationcomplete == 1)
	{
		if (speed<5.1)
		{
			speed = speed + 0.3;
		}
		glutPostRedisplay();
	}
	
	//decrease speed
	if (key == '-'&&rotationcomplete == 1)
	{
		if (speed >= 0.3)
		{
			speed = speed - 0.3;
		}
		glutPostRedisplay();
	}

	//to reverse steps back to original cube
	if (key == 'o'&&rotationcomplete == 1)
	{
		rotationcomplete = 0;
		if (count >= 0)
		{
			if (solve[count]<0)
			{
				rotation = -1 * solve[count];
				inverse = 0;
				glutIdleFunc(cubeRotate);

			}
			if (solve[count]>0)
			{
				rotation = solve[count];
				inverse = 1;
				glutIdleFunc(cubeRotate);
			}

			count--;
		}

		glutIdleFunc(cubeRotate);
	}

	//zoom in
	if (key == 'z') {
		if (eye_z>19)
			eye_z -= 1.5;
		glutPostRedisplay();
	}

	//zoom out
	if (key == 'x') {
		if (eye_z <= 40)
			eye_z += 1.5;
		glutPostRedisplay();
	}

}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 10, 1000);

	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	GLint i;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Rubik's Cube by Lozach, Peck, and Sarikey");
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(cubeRotate);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}
