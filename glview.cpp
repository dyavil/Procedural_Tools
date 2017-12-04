#include "glview.h"
#include "iostream"
#include <GL/gl.h>
#define FRUSTUM_SIZE 1.1f

GLView::GLView(QWidget *parent) :
    QGLWidget(parent),
    hg(Vector2(1, 0), Vector2(0, 1),1, 1, 0),
    angle(0.0f)
{
}

void GLView::setHFBase(HeightField hf){
    hg = hf;
}

void GLView::drawHFBase(){


    /*for (unsigned int i = 0; i < hg.field.size(); ++i) {

        int id1 = i%hg.w;
        int id2 = i/hg.h;

            Vector2 pos = Vector2(((float)id1/(float)hg.w), ((float)id2/(float)hg.h));
            pos = pos.x*hg.getU()+pos.y*hg.getV();
            pos = pos + Vector2(-0.5, -0.5);
            glColor3f(hg.field[i], hg.field[i], hg.field[i]);
            glBegin(GL_POINTS);
            glVertex3f(pos.x, pos.y, hg.field[i]/10.0);
            //std::cout << pos << std::endl;


        glEnd();
    }*/

    for (int i = 0; i < hg.h-1; i++) {
        for (int j = 0; j < hg.w-1; j++) {
            int v1 = i*hg.w+j;
            int v2 = (i+1)*hg.w+j;
            int v3 = i*hg.w+j+1;

            int id1 = v1%hg.w;
            int id2 = v1/hg.h;


            Vector2 pos = Vector2(((float)id1/(float)hg.w), ((float)id2/(float)hg.h));
            pos = pos.x*hg.getU()+pos.y*hg.getV();
            pos = pos + Vector2(-0.5, -0.5);
            glColor3f(hg.field[v1], hg.field[v1], hg.field[v1]);
            glBegin(GL_TRIANGLES);
            glVertex3f(pos.x, pos.y, hg.field[v1]/10.0);


            id1 = v3%hg.w;
            id2 = v3/hg.h;
            pos = Vector2(((float)id1/(float)hg.w), ((float)id2/(float)hg.h));
            pos = pos.x*hg.getU()+pos.y*hg.getV();
            pos = pos + Vector2(-0.5, -0.5);
            glColor3f(hg.field[v3], hg.field[v3], hg.field[v3]);
            glVertex3f(pos.x, pos.y, hg.field[v3]/10.0);
            id1 = v2%hg.w;
            id2 = v2/hg.h;
            pos = Vector2(((float)id1/(float)hg.w), ((float)id2/(float)hg.h));
            pos = pos.x*hg.getU()+pos.y*hg.getV();
            pos = pos + Vector2(-0.5, -0.5);
            glColor3f(hg.field[v2], hg.field[v2], hg.field[v2]);
            glVertex3f(pos.x, pos.y, hg.field[v2]/10.0);


            v1 = i*hg.w+j+1;
            v2 = (i+1)*hg.w+j;
            v3 = (i+1)*hg.w+j+1;


            id1 = v2%hg.w;
            id2 = v2/hg.h;
            pos = Vector2(((float)id1/(float)hg.w), ((float)id2/(float)hg.h));
            pos = pos.x*hg.getU()+pos.y*hg.getV();
            pos = pos + Vector2(-0.5, -0.5);
            glColor3f(hg.field[v2], hg.field[v2], hg.field[v2]);
            glVertex3f(pos.x, pos.y, hg.field[v2]/10.0);


            id1 = v1%hg.w;
            id2 = v1/hg.h;
            pos = Vector2(((float)id1/(float)hg.w), ((float)id2/(float)hg.h));
            pos = pos.x*hg.getU()+pos.y*hg.getV();
            pos = pos + Vector2(-0.5, -0.5);
            glColor3f(hg.field[v1], hg.field[v1], hg.field[v1]);
            glBegin(GL_TRIANGLES);
            glVertex3f(pos.x, pos.y, hg.field[v1]/10.0);

            id1 = v3%hg.w;
            id2 = v3/hg.h;
            pos = Vector2(((float)id1/(float)hg.w), ((float)id2/(float)hg.h));
            pos = pos.x*hg.getU()+pos.y*hg.getV();
            pos = pos + Vector2(-0.5, -0.5);
            glColor3f(hg.field[v3], hg.field[v3], hg.field[v3]);
            glVertex3f(pos.x, pos.y, hg.field[v3]/10.0);
            glEnd();
        }
    }

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


    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawHFBase();

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
