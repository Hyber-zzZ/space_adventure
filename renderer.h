#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>
#include <QQuickWindow>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLBuffer>
#include <QOpenGLFramebufferObject>
#include <QVector3D>
#include<QElapsedTimer>
#include <QVector>
#include <QMap>
#include <QScopedPointer>
#include <QMouseEvent>
#include<QVariantList>
#include "spacecraft.h"
#include"lasermanager.h"
#include"cubemanager.h"
#include "renderobj.h"
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)
QT_FORWARD_DECLARE_CLASS(QOpenGLShader)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class Renderer : public QObject, protected QOpenGLFunctions_4_5_Core
{
Q_OBJECT
signals:
    void outputShipSpeed(float speed,float shipx, float shipy, float shipz);
	void emitLaser(QVector3D _pos, QVector3D _front);
	void crashed();
public:
    Renderer();
    ~Renderer();
    void setViewportSize(const QSize &size);
    void setWindow(QQuickWindow *window);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
	Spacecraft* getSpacecraft(){ return spacecraft; }

public slots:
    void paint();
private:
    QSize m_viewportSize;
    QQuickWindow *m_window;
    bool _isInited = false;
    void initScene();
    bool _isResized = false;
    void resizeScene();

    void makeSFBO();
    void releaseSFBO();
    GLuint _msaaFbo = 0;
    GLuint _msaaColor, _msaaDepth, _msaaId;

    QOpenGLTexture *texture1;
    QOpenGLTexture *texture2;
    QVector<float>scale;
    QVector<QVector3D>pos;
    QVector<QVector4D>rotateAng_Spd;
    QVector<QMatrix4x4>models;


	QVector3D camPos;
	QVector3D camUp;
	QVector3D camRight;
	QVector3D camFront;
    QPointF m_lastPos;

	Spacecraft *spacecraft;
	LaserManager *laserMgr;
	CubeManager *cubeMgr;

	QMap<QString,RenderObj*>_renderObjs;
	QMap<QString, QOpenGLBuffer*>_buffers;

	void makeCube();
	void makeLaser();

};
#endif // RENDERER_H
