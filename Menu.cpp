#include "Menu.h"

void menu(int num) {
	value = num;
	glutPostRedisplay();
}

void createMenu(void) {
	menuId = glutCreateMenu(menu);
	glutAddMenuEntry("Wireframe", 0);
	glutAddMenuEntry("Texture", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menuProcess(bool& wired) {
	switch (value) {
	case 0: {
		// Wireframe
		wired = true;
		break;
	}
	case 1:
		// Texture
		wired = false;
		break;
	}
	value = -1;
}