#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace std;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glScalef(5, 5, 5);
	glBegin(GL_QUADS);
	glColor3ub(255, 0, 0);
	glVertex2f(-1, -1);
	glColor3ub(0, 255, 0);
	glVertex2f(1, -1);
	glColor3ub(0, 0, 255);
	glVertex2f(1, 1);
	glColor3ub(255, 255, 255);
	glVertex2f(-1, 1);
	glEnd();

	glutSwapBuffers();
}

void mouse(int x, int y)
{
	// 4 bytes per pixel (RGBA), 1x1 bitmap
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int width = glutGet(GLUT_WINDOW_WIDTH);

	unsigned char* colorArr = new unsigned char[width*height * 4];//(3 * 3 * 4);
	glReadPixels
		(
		0, 0,
		height, width,
		GL_RGBA, GL_UNSIGNED_BYTE, colorArr
		);
	
	cout << "x: " << x << " y: " << y << std::endl;

	cv::Mat scene(height, width, CV_8UC3);
	cv::Vec3b* sceneData = (cv::Vec3b*)scene.data;

	for (int i = 0; i < width*height * 4; i++)
	{
		if (colorArr[i] < 0)
			colorArr[i] = 255;;
	}

	for (int i = 0; i < width*height; i++)
	{
		sceneData[i][0] = colorArr[i * 4];
		sceneData[i][1] = colorArr[i * 4 + 1];
		sceneData[i][2] = colorArr[i * 4 + 2];
	}
	cv::namedWindow("scene", 0);
	cv::imshow("scene", scene);
	cv::waitKey(10);
}

int sdfamain(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutInitWindowSize(800, 600);
	glutCreateWindow("glReadPixels()");

	glutDisplayFunc(display);
	glutPassiveMotionFunc(mouse);
	glutMainLoop();
	return 0;
}