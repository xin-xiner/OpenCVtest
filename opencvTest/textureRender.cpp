#include <glut.h>
#include <gl.h>
#include <glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <zpr.h>
#include <iostream>
#include <string>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include 
/*  Create checkerboard texture  */
#define checkImageWidth 1242
#define checkImageHeight 375
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;


GLfloat* vertexs;
GLfloat* texcoors;

void makeCheckImage(std::string& imageFileName)
{
	int i, j, c;

	cv::Mat image = cv::imread(imageFileName);

	cv::Vec3b* imageData = (cv::Vec3b*)image.data;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			int idx = i*checkImageWidth + j;
			checkImage[i][j][0] = (GLubyte)imageData[idx][0];
			checkImage[i][j][1] = (GLubyte)imageData[idx][1];
			checkImage[i][j][2] = (GLubyte)imageData[idx][2];
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}




void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);
	

	vertexs = new GLfloat[3 * 8];
	GLfloat tempV[3 * 8] = { -2.0, -1.0, 0.0, -2.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0, 2.41421, 1.0, -1.41421, 2.41421, -1.0, -1.41421 };
	texcoors = new GLfloat[2 * 8];
	GLfloat tempT[2 * 8] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	for (int i = 0; i < 3 * 8; i++)
	{
		vertexs[i] = tempV[i];
		texcoors[i] = tempT[i];
	}


}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	
	

	/*glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0); 
	glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

	glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
	glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);*/

	for (int i = 0; i < 8; i++)
	{
		glTexCoord2f(texcoors[i*2], texcoors[i*2+1]);
		glVertex3f(vertexs[i * 3], vertexs[i * 3+1], vertexs[i * 3+2]);
	}
	


	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, vertexs);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_TEXTURE);
	//glTexCoordPointer(2, GL_FLOAT, 0, texcoors);
	//glDrawArrays(GL_TRIANGLES, 0, 8);
	//glDrawElements(GL_TRIANGLE_STRIP, 4,GL_UNSIGNED_BYTE, indices);
	//glDrawArrays(GL_LINES, 0, 8);
	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_VERTEX_ARRAY);
	//glDisable(GL_TEXTURE_COORD_ARRAY);
	//glDisable(GL_TEXTURE);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.6);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	std::string textureName = "D:\\นคื๗\\probability fusion\\kitti\\2011_09_26_drive_0048_sync\\2011_09_26\\2011_09_26_drive_0048_sync\\image_02\\data\\0000000000.png";
	makeCheckImage(textureName);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(250, 250);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);

	zprInit();
	zprSelectionFunc(display);     /* Selection mode draw function */
	//zprPickFunc(pick);              /* Pick event client callback   */

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	delete[] vertexs;
	delete[] texcoors;
	return 0;
}