#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#define dalam 0
#define transisimasuk 1
#define luar 2
#define transisikeluar 3

using namespace glm;
using namespace std;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void setup_viewport(GLFWwindow* window)
{
    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float)height;
    glViewport(0, 0, width, height);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 500.f, 0, 500.f, 0.0f, 1.f);
    //glOrtho(0, 1366, 0.0f, 768, 0.0f, 1.0f); //pojok kiri , pojok kanan , pojok bawah , pojok atas , near , far (batas nilai)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void circle(float radius, int degree,int line) {
    if (line==1) glBegin(GL_LINE_LOOP);
    else glBegin(GL_POLYGON);    

    int jumlah_titik = 360;
    int x_tengah = 0;
    int y_tengah = 0;
    int sudut, i;

    if (line == 0) glVertex2f(x_tengah, y_tengah);
    for (i = 0; i <= degree+1; i++) {
        float sudut = i * (2 * 3.14 / jumlah_titik);
        float x = x_tengah + radius * cos(sudut);
        float y = y_tengah + radius * sin(sudut);
        glVertex2f(x, y);
    }
    if (line == 0) glVertex2f(x_tengah, y_tengah);
   
    glEnd();
}

void triangle(float alas, float tinggi) {
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.5 * alas, 0);
    glVertex2f(0, tinggi);
    glVertex2f(0.5 * alas, 0);
    glEnd();
}


void rectangle(float horizontal, float vertical)
{
    horizontal /= 2;
    vertical /= 2;
    glBegin(GL_POLYGON);
    glVertex2f(-horizontal, vertical);
    glVertex2f(-horizontal, -vertical);
    glVertex2f(horizontal, -vertical);
    glVertex2f(horizontal, vertical);
    glEnd();
}

void titik(int point) {
    glPushMatrix();
    glTranslatef(-265, 0, 0);

    for (int jumlah = 1; jumlah <= 12; jumlah++) {
        glTranslatef(40, 0, 0);
        if (point == jumlah) {
            glColor3ub(255, 0, 0);
        }
        else  glColor3ub(0, 0, 0);
        
        rectangle(10, 10);
    }

    glPopMatrix(); 
}

float delay;
double time;
double lastTime;
double deltaTime;
int pointMove = 0;

void animasiTitik() {
    
    time = glfwGetTime();
    deltaTime = time - lastTime;

    if (deltaTime >= delay) {

        if (pointMove < 12) {
            pointMove += 1;
        }
        else  pointMove = 1;

        lastTime = time;
    }

    titik(pointMove);
    
}

float xcircle1;
float ycircle1;
float xcircle2;
float ycircle2;
short i = 0;
short j = 0;
uint8_t state = 0; //0 circle di lingkaran 1 , state 2 perpindahan , state 3 di lingkaran luarshor
short change = 0;

void animasiLingkaran() {

    //===== Begin Default =====//
    glColor3ub(255, 255, 0);
    circle(30, 360, 0);

    for (int cr = 0; cr < 360;) {
        glPushMatrix();
        glRotatef(cr, 0, 0, 1);
        glTranslatef(0, 40, 0);
        triangle(40, 60);
        glPopMatrix();
        cr += 90;
    }

    glColor3ub(255, 0, 0);
    circle(170, 360, 1);
    circle(200, 360, 1);

    time = glfwGetTime();
    deltaTime = time - lastTime;

    glPushMatrix();
    if (deltaTime >= delay) {
        if (state == dalam) {
            //lingkaran satu berputar di dalam
            if (i >= 360) {
                state = transisikeluar;
                i = 0;
            }
            xcircle1 = 170 * cos(i * (2 * 3.14 / 360));
            ycircle1 = 170 * sin(i * (2 * 3.14 / 360));
            glTranslatef(xcircle1, ycircle1, 0);
            glColor3ub(255, 0, 0);
            circle(20, 360, 0);
            i++;
        }

        else if (state == transisikeluar) {
            //lingkaran satu berjalan keluar 
            xcircle1 = 170 + change;
            ycircle1 = 0;

            glTranslatef(xcircle1, ycircle1, 0);
            glColor3ub(255, 0, 0);
            circle(20, 360, 0);

            if (change >= 30) {
                state = luar;
            }
            else change++;
        }

        else if (state == luar) {
            //lingkaran satu berputar di luar   
            if (i >= 360) {
                state = transisimasuk;
                i = 0;
            }
            xcircle1 = 200 * cos(i * (2 * 3.14 / 360));
            ycircle1 = 200 * sin(i * (2 * 3.14 / 360));
            glTranslatef(xcircle1, ycircle1, 0);
            glColor3ub(255, 0, 0);
            circle(20, 360, 0);
            i++;
        }

        else if (state == transisimasuk) {
            //lingkaran satu berjalan masuk 
            xcircle1 = 170 + change;
            ycircle1 = 0;

            glTranslatef(xcircle1, ycircle1, 0);
            glColor3ub(255, 0, 0);
            circle(20, 360, 0);

            if (change <= 0) {
                state = dalam;
            }
            else change--;
        }


        //Lingkaran dua tidak terpengaruh state 
        xcircle2 = 40 * cos(j * (2 * 3.14 / 360));
        ycircle2 = 40 * sin(j * (2 * 3.14 / 360));
        if (j >= 360) j -= 360;
        j += 3;

        glTranslatef(xcircle2, ycircle2, 0);
        glColor3ub(255, 0, 255);
        circle(10, 360, 0);


        lastTime = time;
    }
    glPopMatrix();


}


void display()
{
    glTranslatef(250, 250, 0); //Titik 0,0 di pusat layar
    glColor3ub(255, 255, 255);
    rectangle(500, 500);
    delay = 0.001f;
    //animasiTitik();
    animasiLingkaran();
    
  
}

void WindowGL() {
    GLFWwindow* window;
    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(500, 500, "Araaf Ario", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_POINT_SMOOTH);


    while (!glfwWindowShouldClose(window))
    {
        setup_viewport(window);

        display();        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

int main() {

    WindowGL();

    return 0;
}