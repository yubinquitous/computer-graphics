#pragma comment(lib, "glaux")
#pragma comment(lib, "legacy_stdio_definitions")
#pragma warning(disable:4996)

#include <windows.h>
#include <glut.h>
#include <iostream>
#include <stdio.h>
#include <glaux.h>
#include <GL.h>
#include <GLU.h>

int viewport;
GLfloat gYAngle, gXAngle, gZAngle;
GLint moveX, moveY;
GLuint texture[4];
GLUquadricObj* body;
GLUquadricObj* head;

void drawHead(GLuint texture, GLUquadricObj* obj, GLUquadricObj* head);
void drawBody(GLuint texture, GLUquadricObj* obj);
void drawArms(GLuint texture1, GLuint texture2, GLUquadricObj* obj, GLfloat* d2);
void drawLegs(GLuint texture1, GLuint texture2, GLUquadricObj* obj, GLfloat* d2);

/*
#define COLOR_TEXTURE_NUM 16
#define IMAGE_TEXTURE_NUM 16
#define ZERO 0
#define INT_ONE 1
#define FlOAT_ONE 1.0
#define ESC 27
#define GL_PI 3.1415f

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
    checkGLError("resizeWindow");
}

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
*/

void checkGLError(const char* context) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error in %s: %d\n", context, err);
    }
}

void myLight(void) {
    float AmbientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float DiffuseColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    float SpecularColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float Position[] = { 100.0f, 100.0f, 400.0f, 1.0f };

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, SpecularColor);
    glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientColor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseColor);
    glLightfv(GL_LIGHT0, GL_POSITION, Position);
    checkGLError("myLight");
}

void myDisplay(void) {
    GLfloat d1[] = { 0.0, 0.5, 0.83, 1.0 };
    GLfloat d2[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat d3[] = { 0.7, 0.0, 0.0, 1.0 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0 + viewport, 0 + viewport, 600 - 2 * viewport, 600 - 2 * viewport);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(gYAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(gXAngle, 1.0f, 0.0f, 1.0f);
    glRotatef(gZAngle, 0.0f, 0.0f, 0.0f);

    GLUquadricObj* obj;
    obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricNormals(obj, GL_SMOOTH);
    gluQuadricTexture(obj, GL_TRUE);  // Enable texturing for this quadric

    glEnable(GL_TEXTURE_2D);
    // 머리
    drawHead(texture[0], obj, head);

    // 몸통
    drawBody(texture[1], obj);

    // 팔
    drawArms(texture[2], texture[3], obj, d2);

    // 다리
    drawLegs(texture[2], texture[3], obj, d2);

    glutSwapBuffers();
    gluDeleteQuadric(obj);
    checkGLError("myDisplay");
}

void drawHead(GLuint texture, GLUquadricObj* obj, GLUquadricObj* head) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    gluSphere(head, 0.29, 50, 30);
    glRotatef(90.0, 1.0, 0.0, 0.0);
}

void drawBody(GLuint texture, GLUquadricObj* obj) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTranslatef(0.0, -0.60, 0.0);
    glRotatef(270.0, 1.0, 0.0, 0.0);

// 텍스처 좌표 조정을 위한 코드를 추가합니다.
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0, 0.65, 1.0); // 텍스처의 Y축을 조정하여 압축을 줄입니다.
    glMatrixMode(GL_MODELVIEW);

    // 구체 대신 원통으로 바꾸고 높이를 줄입니다. (높이를 0.4에서 0.2로 줄임)
    gluCylinder(obj, 0.174, 0.167, 0.34, 30, 30); // 원통의 바닥 반지름, 윗부분 반지름은 유지하고 높이만 줄임.

    // 텍스처 좌표 원래대로 복원
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glRotatef(90.0, 1.0, 0.0, 0.0);

    // 목
    GLfloat d2[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, d2);
    glTranslatef(0.0, 0.37, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutSolidTorus(0.025, 0.161, 30, 30);

    glRotatef(-90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.17, 0.0);
}

void drawArms(GLuint texture1, GLuint texture2, GLUquadricObj* obj, GLfloat* d2) {
    glBindTexture(GL_TEXTURE_2D, texture1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, d2);

    // 왼팔
    glPushMatrix();
    glTranslatef(0.13, 0.095, 0.0);
    glRotatef(90, 0.1, 0.18, 0.0);
    gluCylinder(obj, 0.065, 0.05, 0.18, 3, 3);
    glTranslatef(0.0, 0.007, 0.2);
    glutSolidSphere(0.04, 20, 20); //팔꿈치
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33, 0.0, 0.0);
    glRotatef(-90, -75, 90, 0.0);
    gluCylinder(obj, 0.05, 0.05, 0.2, 3, 3);
    glTranslatef(0.0, 0.007, 0.2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glutSolidSphere(0.06, 20, 20); //왼손
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture1);
    // 오른팔
    glPushMatrix();
    glTranslatef(-0.13, 0.095, 0);
    glRotatef(90, 0.1, -0.18, 0.0);
    gluCylinder(obj, 0.065, 0.05, 0.18, 3, 3);
    glTranslatef(0.0, -0.008, 0.2);
    glutSolidSphere(0.04, 20, 20); //팔꿈치
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.33, 0.0, 0.0);
    glRotatef(90, 75, 90, 0.0);
    gluCylinder(obj, 0.05, 0.05, 0.2, 3, 3);
    glTranslatef(0.0, 0.007, 0.2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glutSolidSphere(0.06, 20, 20); //오른손
    glPopMatrix();
}

void drawLegs(GLuint texture1, GLuint texture2, GLUquadricObj* obj, GLfloat* d2) {
    glBindTexture(GL_TEXTURE_2D, texture1);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, d2);

    // 오른다리
    glPushMatrix();
    glTranslatef(0.09, -0.2, 0.0);
    glRotatef(90, 0.1, 0.0, 0.0);
    glRotatef(8, 0.0, 0.1, 0.0);
    gluCylinder(obj, 0.05, 0.07, 0.25, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15, -0.45, 0.0);
    glRotatef(90, 0.1, 0.0, 0.02);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glutSolidTorus(0.055, 0.04, 30, 10);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture1);
    // 왼다리
    glPushMatrix();
    glTranslatef(-0.09, -0.2, 0.0);
    glRotatef(90, 0.1, 0.0, 0.0);
    glRotatef(350.0, 0.0, 1.0, 0.0);
    gluCylinder(obj, 0.05, 0.07, 0.25, 30, 10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.15, -0.45, 0.0);
    glRotatef(270, 0.1, 0.0, 0.0);
    glRotatef(5.0, 0.0, 1.0, -0.03);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glutSolidTorus(0.055, 0.04, 30, 30);
    glPopMatrix();
}



void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'z':
        viewport += 5;
        glutPostRedisplay();
        break;
    case 'a':
        viewport -= 5;
        glutPostRedisplay();
        break;
    case 'q':
        exit(0);
        break;
    }
}

void myMouseMotion(GLint x, GLint y) {
    if (x - moveX > 0)
        gYAngle += 5.0f;
    else if (x - moveX < 0)
        gYAngle -= 5.0f;

    if (y - moveY > 0)
        gXAngle += 5.0f;
    else if (y - moveY < 0)
        gXAngle -= 5.0f;

    moveX = x;
    moveY = y;
    glutPostRedisplay();
}

class Initialization {
public:
    void InitGLUT(int& argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(600, 600);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("anpanman");
    }

    void InitOpenGL() {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glFrontFace(GL_CCW);
        myLight();
        if (!loadGLTextures()) {
            printf("Failed to load textures\n");
            exit(-1);
        }
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    }

    void RegisterCallbacks() {
        glutDisplayFunc(myDisplay);
        glutKeyboardFunc(myKeyboard);
        glutMotionFunc(myMouseMotion);
    }

private:
    AUX_RGBImageRec* loadBMP(const char* filename)
    {
        if (!filename)
        {
            printf("filename error\n");
            return NULL;
        }

        FILE* fp = fopen(filename, "rb");
        if (fp)
        {
            fclose(fp);
            return auxDIBImageLoad(filename);
        }
        else
        {
            printf("file open failed: %s\n", filename);
            return NULL;
        }
    }

    int loadGLTextures(void) {
        int Status = FALSE;
        head = gluNewQuadric();
        gluQuadricDrawStyle(head, GLU_FILL);
        gluQuadricTexture(head, GL_TRUE);
        body = gluNewQuadric();
        gluQuadricDrawStyle(body, GLU_FILL);
        gluQuadricTexture(body, GL_TRUE);
        AUX_RGBImageRec* TextureImage[4] = { NULL, NULL, NULL, NULL };

        // Load head texture
        if ((TextureImage[0] = loadBMP("head.bmp"))) {
            Status = TRUE;
            glGenTextures(1, &texture[0]);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
            checkGLError("loadGLTextures - head.bmp");
        }
        else {
            printf("loadBMP fail for head.bmp\n");
        }

        // Load body texture
        if ((TextureImage[1] = loadBMP("body.bmp"))) {
            Status = TRUE;
            glGenTextures(1, &texture[1]);
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
            checkGLError("loadGLTextures - body.bmp");
        }
        else {
            printf("loadBMP fail for body.bmp\n");
        }

        // Load red texture
        if ((TextureImage[2] = loadBMP("red.bmp"))) {
            Status = TRUE;
            glGenTextures(1, &texture[2]);
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[2]->sizeX, TextureImage[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
            checkGLError("loadGLTextures - red.bmp");
        }
        else {
            printf("loadBMP fail for red.bmp\n");
        }

        // Load yellow texture
        if ((TextureImage[3] = loadBMP("yellow.bmp"))) {
            Status = TRUE;
            glGenTextures(1, &texture[3]);
            glBindTexture(GL_TEXTURE_2D, texture[3]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TextureImage[3]->sizeX, TextureImage[3]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);
            checkGLError("loadGLTextures - yellow.bmp");
        }
        else {
            printf("loadBMP fail for yellow.bmp\n");
        }

        // Free texture data
        for (int i = 0; i < 4; i++) {
            if (TextureImage[i]) {
                if (TextureImage[i]->data) {
                    free(TextureImage[i]->data);
                }
                free(TextureImage[i]);
            }
        }

        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        checkGLError("loadGLTextures");

        return Status;
    }
};

int main(int argc, char** argv) {
    Initialization init;
    init.InitGLUT(argc, argv);
    init.InitOpenGL();
    init.RegisterCallbacks();

    glutMainLoop();
    return 0;
}
