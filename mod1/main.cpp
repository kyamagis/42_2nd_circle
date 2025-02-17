
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>

void show(void);
void onWindowClose(void);
bool firstRender = false;

int window; // ウィンドウIDをグローバルに宣言

int main(int argc, char** argv)
{
    glutInit(&argc, argv);  // 必ず最初に呼び出す
    glutInitDisplayMode(GLUT_SINGLE);
    glutIdleFunc(NULL);
    glutInitWindowSize(400, 300);
    glutInitWindowPosition(200, 200);

    window = glutCreateWindow("mod1");
    std::cout << "Window ID: " << window << std::endl;

    // ウィンドウが閉じられた際に onWindowClose を呼び出す
    glutWMCloseFunc(onWindowClose);  // ウィンドウの閉じる処理にコールバックを設定

    glutInitDisplayMode(GLUT_RGBA);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    glutDisplayFunc(show);
    glutMainLoop(); // イベントループに入る

    return 0;
}

void show(void)
{
    std::cout << "show: " << window << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);

    glVertex2d(-0.5, 0.5);
    glVertex2d( 0.5, 0.5);
    glVertex2d( 0.5,-0.5);

    glVertex2d(0.0, 0.0);
    glVertex2d(1.0, 0.0);
    glVertex2d(0.0, 0.1);

    glEnd();
    glFlush();
    if (firstRender) {
        firstRender = false;
        
        glutPostRedisplay(); // 明示的に1回だけ再描画を要求
    }
}

void onWindowClose(void)
{
    // ウィンドウが閉じられたときにリソース解放などの処理を実行
    std::cout << "Window closed, performing cleanup." << window << std::endl;
    // 正しいウィンドウIDを渡して、ウィンドウを破棄する
    glutDestroyWindow(window);
    std::exit(0);
}

//////////////////////////////////////////

// #include <GL/glut.h>
// #include <GL/freeglut.h>
// #include <iostream>

// void show(void);

// int main(int argc, char** argv)
// {
//   glutInit(&argc, argv);  // 必ず最初に呼び出す

//   glutInitDisplayMode(GLUT_SINGLE);
//   glutInitWindowSize(400, 300);
//   glutInitWindowPosition(200, 200);

//   int window = glutCreateWindow("mod1");
//   std::cout << window << std::endl;

//   // ウィンドウが閉じられた際に mainLoop が戻るように設定
//   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  
//   glutInitDisplayMode(GLUT_RGBA);
//   glClearColor(0.0, 0.0, 0.0, 1.0);

//   glutDisplayFunc(show);
//   glutMainLoop(); // イベントループに入る

//   // glutMainLoop が戻った後にリソース解放
//   // glutDestroyWindow(window);

//   return 0;
// }

// void show(void)
// {
//   glClear(GL_COLOR_BUFFER_BIT);
//   glColor3d(1.0, 0.0, 0.0);
//   glBegin(GL_LINE_LOOP);

//   glVertex2d(-0.5, 0.5);
//   glVertex2d( 0.5, 0.5);
//   glVertex2d( 0.5,-0.5);

//   glEnd();
//   glFlush();
// }
////////////////////////////////////////////////////////
// #include <GL/glut.h>
// #include <GL/freeglut.h>
// #include <iostream>

// void show(void);

// int main(int argc, char** argv)
// {
//   glutInit(&argc, argv);

//   glutInitDisplayMode(GLUT_SINGLE);
//   glutInitWindowSize(400, 300);
//   glutInitWindowPosition(200, 200);

//   int window = glutCreateWindow("mod1");
//   std::cout << window << std::endl;
//   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
//   glutInitDisplayMode(GLUT_RGBA);
//   glClearColor(0.0, 0.0, 0.0, 1.0);
    
//   glutDisplayFunc(show);
//   glutMainLoop();

//   glutDestroyWindow(window);
//   glutExit();

//   return 0;
// }

// void show(void)
// {
//   glClear(GL_COLOR_BUFFER_BIT);
//   glColor3d(1.0, 0.0, 0.0);
//   glBegin(GL_LINE_LOOP);

// //   glVertex2d(-0.5,-0.5);
//   glVertex2d(-0.5, 0.5);
//   glVertex2d( 0.5, 0.5);
//   glVertex2d( 0.5,-0.5);

//   glEnd();
//   glFlush();
// }
