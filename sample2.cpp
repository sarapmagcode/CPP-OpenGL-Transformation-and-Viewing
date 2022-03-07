#define GLEW_STATIC
#define BUFFER_OFFSET(i) ((void*)(i))

#include<GL/glew.h>
#include<GL/glut.h>
#include <GL/freeglut_ext.h>
#include<iostream>

using namespace std;

void init();
void displayObject();
void displayTriangle();
void animateTriangle(int value);
void GLAPIENTRY
MessageCallback( GLenum source,
GLenum type,
GLuint id,
GLenum severity,
GLsizei length,
const GLchar* message,
const void* userParam );

GLuint VBOid;
void keyMonitor(unsigned char key, int x, int y);
bool initTriangle = true;
float angle = 1.0f;
GLfloat vertices[] =  {
	//------------------- Triangle
	0.0f, 1.0f, 0.0f, 
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,

	0.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
};

GLfloat colorstriangle[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
	
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	
	1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,
	0.0f,1.0f,0.0f
};


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Setup Test");
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	
	init();
	glutDisplayFunc(displayObject);
	glutKeyboardFunc(keyMonitor);
	
	GLenum err = glewInit();
	if (err == GLEW_OK) {
		glEnable ( GL_DEBUG_OUTPUT );
		glDebugMessageCallback(MessageCallback, 0);
		printf("OpenGL version supported by this platform (%s): \n", 
		glGetString(GL_VERSION));
		printf("OpenGL vendor (%s): \n", glGetString(GL_VENDOR));
		glutMainLoop();
	} else {
		cout << "Error in initializing GLEW extension library...";
	}
	return 0;
}

void displayTriangle(){
	if (initTriangle) {
		glGenBuffers(1, &VBOid);
		glBindBuffer(GL_ARRAY_BUFFER, VBOid);
		glBufferData(GL_ARRAY_BUFFER, ((9 * 4) + (9 * 4)) * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, (9 * 4) * sizeof(GLfloat),(9 * 4) * sizeof(GLfloat), colorstriangle);
		
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET((9 * 4) * sizeof(GLfloat)));
		
		initTriangle = false;
	}
	
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
	glPushMatrix();
	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	
	glBegin(GL_TRIANGLES); //Displaying Six Sides
	for (int i = 0; i < 12; i++) {
		glArrayElement(i);
	}
	
	glEnd(); 
	glFlush();
	glPopMatrix();
}

void animateTriangle(int value){
	if (angle > 360) {
		angle = 0;	
	}
	angle++;
	glutPostRedisplay();
}

void keyMonitor(unsigned char key, int x, int y){
	if (key == 'x') {
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VBOid);
		exit(0);
	}
}

void displayObject(){
	glClear(GL_COLOR_BUFFER_BIT);
	displayTriangle();
	glutTimerFunc(50, animateTriangle, 1);
	glFlush();
}

void GLAPIENTRY MessageCallback(GLenum source,
GLenum type,
GLuint id,
GLenum severity,
GLsizei length,
const GLchar* message,
const void* userParam )
{
fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ), type, severity, message );
}

void init(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4.0,4.0,-4.0,4.0, -4.0,4.0);
}
