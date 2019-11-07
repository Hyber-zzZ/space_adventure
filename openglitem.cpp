#include "openglitem.h"
#include<QDebug>
OpenGLItem::OpenGLItem(): m_renderer(nullptr)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    //setAcceptTouchEvents();
    setFocus(true);
    setFlag(ItemAcceptsInputMethod);
    keepMouseGrab();
    connect(this, &QQuickItem::windowChanged, this, &OpenGLItem::handleWindowChanged);
}
OpenGLItem::~OpenGLItem(){

}

void OpenGLItem::mousePressEvent(QMouseEvent *event)
{
    if (m_renderer)
    {
        m_renderer->mousePressEvent(event);
    }
}
/*
void OpenGLItem::mouseMoveEvent(QMouseEvent *event)
{
    if (m_renderer)
    {
        m_renderer->mouseMoveEvent(event);
    }
}*/

void OpenGLItem::wheelEvent(QWheelEvent *event)
{
    if (m_renderer)
    {
        m_renderer->wheelEvent(event);
    }
}

void OpenGLItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_renderer)
    {
        m_renderer->mouseDoubleClickEvent(event);
    }
}

void OpenGLItem::keyPressEvent(QKeyEvent * event)
{
    if (m_renderer)
    {
        m_renderer->keyPressEvent(event);
    }
}

void OpenGLItem::keyReleaseEvent(QKeyEvent * event)
{
    if (m_renderer)
    {
        m_renderer->keyReleaseEvent(event);
    }
}

void OpenGLItem::hoverMoveEvent(QHoverEvent *event)
{
    if (m_renderer)
    {
        m_renderer->hoverMoveEvent(event);
    }
}

void OpenGLItem::handleWindowChanged(QQuickWindow *win){
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &OpenGLItem::sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &OpenGLItem::cleanup, Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}
void OpenGLItem::sync()
{
    if (!m_renderer)
    {
        m_renderer = new Renderer();
        connect(window(), &QQuickWindow::beforeRendering, m_renderer, &Renderer::paint, Qt::DirectConnection);
        connect(m_renderer, &Renderer::outputShipSpeed, this, &OpenGLItem::outputShipSpeed, Qt::DirectConnection);
		connect(m_renderer, &Renderer::crashed, this, &OpenGLItem::crashed, Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setWindow(window());
}

void OpenGLItem::cleanup()
{
    qDebug()<<"cleanup";
    if (m_renderer)
    {
        delete m_renderer;
        m_renderer = nullptr;
    }
}
