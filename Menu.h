#pragma once
#include <gl/glut.h>

static int menuId;
static int value = 0;

void menu(int num);
void createMenu(void);
void menuProcess(bool&);