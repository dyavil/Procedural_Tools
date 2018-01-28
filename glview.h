#ifndef GLVIEW_H
#define GLVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>
#include "drawfield.h"

class GLView : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLView(QWidget *parent = 0);

    virtual void initializeGL();

    virtual void paintGL();

    virtual void resizeGL(int w, int h);

    virtual void setHFBase(DrawField & hf);

    bool showTree;

    DrawField * getDrawField(){return  &hg;}

protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void wheelEvent ( QWheelEvent * event );

private:
    DrawField hg;
    float angle;
    float anglex;
    float anglez;
    float zoom;
    float progress_zoom;
    QPoint position;

};


#endif // GLVIEW_H
