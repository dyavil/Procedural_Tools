#include "glview.h"
#include "iostream"
#define FRUSTUM_SIZE 1.1f

GLView::GLView(QWidget *parent) :
    QGLWidget(parent),
    hg(),
    angle(0.0f)
{
}

void GLView::setHFBase(DrawField df){
    hg = df;
}

void GLView::initializeGL()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CCW);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0, 1.0, 0.0);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    //glScaled(2.0, 2.0, 2.0);

    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    //hg.draw();
    hg.drawInterpol(300, 0);
}

void GLView::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);

    glViewport(0, 0, w, h);

    glOrtho(-FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE,
            -FRUSTUM_SIZE, FRUSTUM_SIZE);

    glMatrixMode(GL_MODELVIEW);
}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    if( event != NULL ) {
        QPoint pos = event->pos();

        if(event->buttons() == Qt::LeftButton) angle += (pos.x() - position.x());

        position = pos;
        updateGL();
    }
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL ) {
    }
}
