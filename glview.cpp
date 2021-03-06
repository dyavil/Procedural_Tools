#include "glview.h"
#include "iostream"
#define FRUSTUM_SIZE 1.1f

GLView::GLView(QWidget *parent) :
    QGLWidget(parent),
    showTree(false),
    hg(),
    angle(290.0f),
    anglex(360.0f),
    anglez(45.0f),
    zoom(1.0f),
    progress_zoom(1.0f)
{
}

void GLView::setHFBase(DrawField & df){
    hg = df;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(hg.fields.a.x*zoom, hg.fields.b.x*zoom,
            hg.fields.a.y*zoom, hg.fields.b.y*zoom,
            (hg.fields.a.x)*1.3*zoom, (hg.fields.b.x)*1.3*zoom);
    glMatrixMode(GL_MODELVIEW);
}


void GLView::initializeGL()
{
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CCW);

    glClearColor(0.282353, 0.239216, 0.545098, 1.0f);

    glColor3f(1.0, 1.0, 0.0);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();


    glRotatef(angle, 1.0f, 0.0f, 0.0f);
    glRotatef(anglex, 0.0f, 1.0f, 0.0f);
    glRotatef(anglez, 0.0f, 0.0f, 1.0f);

    hg.draw(showTree);

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

        if(event->buttons() == Qt::LeftButton) {
            angle += (pos.y() - position.y());
            anglex += (pos.x() - position.x());

        }
        if(event->buttons() == Qt::RightButton) {
            anglez += (pos.x() - position.x());
        }

        position = pos;
        updateGL();
    }
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    if( event != NULL ) {
        QPoint pos = event->pos();
        position = pos;
    }
}


void GLView::wheelEvent(QWheelEvent *event)
{
    if( event != NULL ) {
        float tmp = zoom - event->delta()/400.0;
        if(tmp < zoom) progress_zoom += 0.5;
        else progress_zoom = 1.0;
        if(progress_zoom > 12.0) progress_zoom = 12.0;
        zoom = zoom - (event->delta()/400.0/progress_zoom);

        if (zoom < 0.1) zoom =0.1;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glOrtho(hg.fields.a.x*zoom, hg.fields.b.x*zoom,
                hg.fields.a.y*zoom, hg.fields.b.y*zoom,
                (hg.fields.a.x)*1.3, (hg.fields.b.x)*1.3);
        glMatrixMode(GL_MODELVIEW);
        updateGL();
    }
}
