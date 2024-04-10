#pragma comment(lib, "glaux")
#pragma comment(lib, "legacy_stdio_definitions")
#include <windows.h>
#include <glut.h>
#include <iostream>
#include <stdio.h>
#include <glaux.h>
#include <GL.h>
#include <GLU.h>

#define COLOR_TEXTURE_NUM 16
#define IMAGE_TEXTURE_NUM 16

GLuint texture[2];
GLUquadricObj* body;
GLUquadricObj* head;

int viewport;
GLfloat gYAngle, gXAngle, gZAngle;
GLint moveX, moveY;

#define ZERO 0
#define INT_ONE 1
#define FlOAT_ONE 1.0
#define ESC 27
#define GL_PI 3.1415f

void reshape(int w, int h)
{
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glRectf(30.0, 30.0, 50.0, 50.0);
	glutSwapBuffers();
}

void myLight(void) {
	float AmbientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };        //주변광
	float DiffuseColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };        //분산광
	float SpecularColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };        //방사광
	float EmissionColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };        //물체
	float Position[] = { 100.0f, 100.0f, 400.0f, 1.0f };  //조명 위치
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_TEXTURE_2D);
	// 가려진 면 제거
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// 조명 활성화
	glEnable(GL_LIGHTING);

	//재질을 설정
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientColor);       // 주변광 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseColor);       // 분산광 
	glLightfv(GL_LIGHT0, GL_EMISSION, EmissionColor);
	glLightfv(GL_LIGHT0, GL_POSITION, Position);          // 광원

	// LIGHT0을 켠다.
	glEnable(GL_LIGHT0);
}

/* 비트맵 이미지 읽어오는 함수 */
AUX_RGBImageRec* loadBMP(const char* filename)
{
	FILE* fp = NULL;
	if (!filename)
	{
		printf("filename error\n");
		return NULL;
	}
	fopen_s(&fp, filename, "r");
	if (fp)
	{
		printf("fp is not null\n");
		fclose(fp);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

int loadGLTextures(void) {
	int Status = FALSE;
	head = gluNewQuadric();
	gluQuadricDrawStyle(head, GLU_FILL);
	gluQuadricTexture(head, GL_TRUE);
	body = gluNewQuadric();
	gluQuadricDrawStyle(body, GLU_FILL);
	gluQuadricTexture(body, GL_TRUE);
	AUX_RGBImageRec* TextureImage[2];

	memset(TextureImage, 0, sizeof(void*) * 1);

	if (TextureImage[0] = loadBMP("head24.bmp"))
	{
		Status = TRUE;
		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	}
	else {
		printf("loadBMP fail\n");
	}
	
	/* TODO: body
	if (TextureImage[1] = loadBMP("body.bmp"))
	{
		Status = TRUE;
		glGenTextures(1, &texture[1]);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
	}
	*/

	if (TextureImage[0])
	{
		if (TextureImage[0]->data)
			free(TextureImage[0]->data);
		free(TextureImage[0]);
	}

	/* TODO: body
	if (TextureImage[1])
	{
		if (TextureImage[1]->data)
			free(TextureImage[1]->data);
		free(TextureImage[1]);
	}
	*/
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	return Status;
}

void resizeWindow(int width, int height)
{
	if (height == ZERO)
		height = INT_ONE;

	float ratio = FlOAT_ONE * width / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35, ratio, INT_ONE, 500);
	glMatrixMode(GL_MODELVIEW);
	glViewport(ZERO, ZERO, width, height);
}

void myDisplay(void) {
	// 물체의 색상 표현
	GLfloat d1[] = { 0.0,0.5,0.83, 1.0 };
	GLfloat d2[] = { 1.0,1.0,1.0, 1.0 };
	GLfloat d3[] = { 0.7,0.0,0.0, 1.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0 + viewport, 0 + viewport, 600 - 2 * viewport, 600 - 2 * viewport);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// 마우스 드래그로 인한 물체의 회전
	glRotatef(gYAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(gXAngle, 1.0f, 0.0f, 1.0f);
	glRotatef(gZAngle, 0.0f, 0.0f, 0.0f);
	// gluCylinder 함수 사용을 위한 설정 
	GLUquadricObj* obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GL_SMOOTH);

	// 머리
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(head, 0.3, 50, 30);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	glTranslatef(0.0, -0.43, 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(head, 0.209, 30, 20);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	glutSwapBuffers();
	gluDeleteQuadric(obj);
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	// 디스플레이 모드를 설정합니다. 여기서는 깊이 버퍼(GLUT_DEPTH), 더블 버퍼링(GLUT_DOUBLE), 그리고 RGBA 컬러 모드(GLUT_RGBA)를 사용하도록 지정합니다.

	glutInitWindowSize(800, 800);	// 윈도우의 초기 크기를 설정합니다.
	glutCreateWindow("anpanman");

	glClearColor(1.0, 1.0, 1.0, 1.0); // 배경 하얀색으로 설정.

	// 조명 설정
	myLight();

	// 텍스처 로드
	loadGLTextures();

	// 투영 행렬 설정
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	// 콜백 함수 등록
	glutDisplayFunc(myDisplay);

	glutReshapeFunc(resizeWindow);
	glutMainLoop();
}