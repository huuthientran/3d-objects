#include <Windows.h>
#include "Mesh.h"
#include "Menu.h"

static const char title[] = "20120584 - Lab 04 - 3D Drawing";
GLfloat rotationAngle = 0.0f; // Rotational angle for cube 
int refreshMills = 15; // refresh interval in milliseconds 
bool wired = true;

/* Initialize OpenGL Graphics */
void initGL() {
	loadGLTextures();
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClearDepth(1.0f); // Set background depth to farthest
	glEnable(GL_DEPTH_TEST); // Enable depth testing for z-culling
	glDepthFunc(GL_LEQUAL); // Set the type of depth-test
	glShadeModel(GL_SMOOTH); // Enable smooth shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
}
/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
	menuProcess(wired);

	if (wired) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
	glMatrixMode(GL_MODELVIEW); // To operate on model-view matrix
	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity(); // Reset the model-view matrix

	glColor3f(1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, -1);
	
	if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[0]);
	Sphere sphere(2.0f, 36, 36);
	sphere.draw(-12.0f, 4.0f, -15.0f, rotationAngle);

	Cube cube(2.0f);
	cube.draw(-4.0f, 4.0f, -15.0f, rotationAngle, wired);
	
	Cylinder cylinder(2.5f, 5.0f, 36, 18);
	cylinder.draw(4.0f, 4.0f, -15.0f, rotationAngle, wired);

	Cone cone(3.0f, 5.0f, 36);
	cone.draw(12.0f, 4.0f, -15.0f, rotationAngle, wired);

	if (!wired) glBindTexture(GL_TEXTURE_2D, textureIDList[4]);
	Disk disk(2.0f, 36, { 0, 0, 0 }, {0, 1, 0});
	disk.draw(-12.0f, -4.0f, -15.0f, rotationAngle);

	Torus torus(1.0f, 2.0f, 36, 36);
	torus.draw(-4.0f, -4.0f, -15.0f, rotationAngle, wired);
	
	Hyperboloid hyperboloid(3.0f, 5.0f, 36, 100);
	hyperboloid.draw(4.0f, -4.0f, -15.0f, rotationAngle, wired);

	Paraboloid paraboloid(3.0f, 5.0f, 36, 10);
	paraboloid.draw(12.0f, -4.0f, -15.0f, rotationAngle, wired);

	glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
	// Update the rotational angle after each refresh 
	rotationAngle += 0.15f;
}

void timer(int value) {
	glutPostRedisplay(); // Post re-paint request to activate display()
	glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
}
/* Handler for window re-size event. Called back when the window first appears and
whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) { // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1; // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
	glLoadIdentity(); // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(90.0f, aspect, 0.1f, 100.0f);
}
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
	glutInit(&argc, argv); // Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
	glutInitWindowSize(640, 480); // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow(title); // Create window with the given title
	glutDisplayFunc(display); // Register callback handler for window re-paint event
	glutReshapeFunc(reshape); // Register callback handler for window re-size event
	initGL(); // Our own OpenGL initialization
	createMenu();
	glutTimerFunc(0, timer, 0); // First timer call immediately 
	glutMainLoop(); // Enter the infinite event-processing loop
	return 0;
}
