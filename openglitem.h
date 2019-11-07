#ifndef OPENGLITEM_H
#define OPENGLITEM_H

#include<QTime>
#include<QQuickItem>
#include<QBasicTimer>
#include<QQuickWindow>
#include "renderer.h"
#include "spacecraft.h"
class OpenGLItem :public QQuickItem
{
    Q_OBJECT
public:
    OpenGLItem();
    ~OpenGLItem();
signals:
    void outputShipSpeed(float speed, float shipx, float shipy, float shipz);
	void crashed();
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    //virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void hoverMoveEvent(QHoverEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

public slots:
    void sync();
    void cleanup();
private slots:
    void handleWindowChanged(QQuickWindow *win);
private:
    Renderer *m_renderer;

};


#endif // OPENGLITEM_H
