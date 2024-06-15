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

GLfloat rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
GLint mouseX = 0, mouseY = 0;
GLfloat zoomFactor = 1.0f;
GLuint textures[4];
GLUquadricObj* obj;

void initializeGL();
void initializeTextures();
void setupLighting();
void checkGLError(const char* context);
AUX_RGBImageRec* loadBMP(const char* filename);
bool loadTextures();
void drawHuman();
void drawHead();
void drawBody();
void drawArms();
void drawLegs();
void handleReshape(int width, int height);
void handleDisplay();
void handleKeyboard(unsigned char key, int x, int y);
void handleMouseMotion(int x, int y);
void handleMouse(int button, int state, int x, int y);

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Virtual Human");

    initializeGL();
    initializeTextures();
    setupLighting();

    glutDisplayFunc(handleDisplay);
    glutReshapeFunc(handleReshape);
    glutKeyboardFunc(handleKeyboard);
    glutMouseFunc(handleMouse);
    glutMotionFunc(handleMouseMotion);

    glutMainLoop();
    return 0;
}

void initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
}

void initializeTextures() {
    glGenTextures(4, textures);
    loadTextures();
}

void setupLighting() {
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightPosition[] = { 100.0f, 100.0f, 400.0f, 1.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientLight);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseLight);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void checkGLError(const char* context) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error in " << context << ": " << err << std::endl;
    }
}

AUX_RGBImageRec* loadBMP(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return nullptr;
    fclose(file);
    return auxDIBImageLoad(filename);
}

bool loadTextures() {
    AUX_RGBImageRec* textureImage[4];
    bool status = false;

    textureImage[0] = loadBMP("head.bmp");
    textureImage[1] = loadBMP("body.bmp");
    textureImage[2] = loadBMP("red.bmp");
    textureImage[3] = loadBMP("yellow.bmp");

    for (int i = 0; i < 4; ++i) {
        if (textureImage[i]) {
            status = true;
            glGenTextures(1, &textures[i]);
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, textureImage[i]->sizeX, textureImage[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage[i]->data);
            checkGLError("loadTextures");
            if (textureImage[i]->data) free(textureImage[i]->data);
            free(textureImage[i]);
        }
    }
    return status;
}

void drawHuman() {
    obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricTexture(obj, GL_TRUE);

    glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    drawHead();
    drawBody();
    drawArms();
    drawLegs();
    glPopMatrix();
}

void drawHead() {
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    gluSphere(obj, 0.27, 50, 30);
    glRotatef(90.0, 1.0, 0.0, 0.0);
}

void drawBody() {
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTranslatef(0.0, -0.60, 0.0);
    glRotatef(270.0, 1.0, 0.0, 0.0);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0, 0.65, 1.0);
    glMatrixMode(GL_MODELVIEW);

    gluCylinder(obj, 0.174, 0.167, 0.34, 30, 30);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glRotatef(90.0, 1.0, 0.0, 0.0);

    // 아랫면 추가
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, 0.0);
    gluDisk(obj, 0.0, 0.174, 30, 1);
    glPopMatrix();

    GLfloat d2[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, d2);
    glTranslatef(0.0, 0.37, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutSolidTorus(0.025, 0.161, 30, 30);

    glRotatef(-90, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -0.17, 0.0);
}

void drawArms() {
    GLfloat d2[] = { 1.0, 1.0, 1.0, 1.0 };
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, d2);

    glPushMatrix();
    glTranslatef(0.13, 0.095, 0.0);
    glRotatef(90, 0.1, 0.18, 0.0);
    gluCylinder(obj, 0.065, 0.05, 0.18, 3, 3);
    glTranslatef(0.0, 0.007, 0.2);
    glutSolidSphere(0.04, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.33, 0.0, 0.0);
    glRotatef(-90, -75, 90, 0.0);
    gluCylinder(obj, 0.05, 0.05, 0.2, 3, 3);
    glTranslatef(0.0, 0.007, 0.2);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glutSolidSphere(0.06, 20, 20);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glPushMatrix();
    glTranslatef(-0.13, 0.095, 0);
    glRotatef(90, 0.1, -0.18, 0.0);
    gluCylinder(obj, 0.065, 0.05, 0.18, 3, 3);
    glTranslatef(0.0, -0.008, 0.2);
    glutSolidSphere(0.04, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.33, 0.0, 0.0);
    glRotatef(90, 75, 90, 0.0);
    gluCylinder(obj, 0.05, 0.05, 0.2, 3, 3);
    glTranslatef(0.0, 0.007, 0.2);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glutSolidSphere(0.06, 20, 20);
    glPopMatrix();
}

void drawLegs() {
    GLfloat d2[] = { 1.0, 1.0, 1.0, 1.0 };
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, d2);

    glPushMatrix();
    glTranslatef(0.09, -0.2, 0.0);
    glRotatef(90, 0.1, 0.0, 0.0);
    glRotatef(8, 0.0, 0.1, 0.0);
    gluCylinder(obj, 0.05, 0.07, 0.25, 30, 30);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15, -0.45, 0.0);
    glRotatef(90, 0.1, 0.0, 0.02);
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glutSolidTorus(0.055, 0.04, 30, 10);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, textures[2]);
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
    glBindTexture(GL_TEXTURE_2D, textures[3]);
    glutSolidTorus(0.055, 0.04, 30, 30);
    glPopMatrix();
}

void handleReshape(int width, int height) {
    if (height == 0) height = 1;
    float aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspectRatio, 1.0f, 500.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handleDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f * zoomFactor); // Apply zoom factor
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
    drawHuman();
    glutSwapBuffers();
}

void handleKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a':
        zoomFactor *= 0.9f; // Zoom in
        break;
    case 'z':
        zoomFactor *= 1.1f; // Zoom out
        break;
    case 27: // ESC key
        exit(0);
        break;
    }
    glutPostRedisplay();
}

void handleMouseMotion(int x, int y) {
    rotationX += (y - mouseY);
    rotationY += (x - mouseX);
    mouseX = x;
    mouseY = y;
    glutPostRedisplay();
}

void handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseX = x;
        mouseY = y;
    }
}
