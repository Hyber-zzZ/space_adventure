#include "renderer.h"
#include<QDebug>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include<QCursor>
const float Pi = 3.14159265359;
const int cubeCount = 500;

Renderer::Renderer(){
    initializeOpenGLFunctions();
    spacecraft = new Spacecraft({ 0,0,-50 });
	connect(spacecraft, &Spacecraft::outputShipSpeed, this, &Renderer::outputShipSpeed, Qt::DirectConnection);
	laserMgr = new LaserManager();
	laserMgr->getSpacecraftInstance(spacecraft);
	cubeMgr = new CubeManager();
	cubeMgr->setCenterPos({ 0,0,10000 });
    cubeMgr->getSpacecraft(spacecraft);
    cubeMgr->getLaserMgr(laserMgr);
	cubeMgr->useMMTimmer();
	tempmgr = new CubeManager();
	tempmgr->setCenterPos({ 0,0,10000 });
	tempmgr->getSpacecraft(spacecraft);
	tempmgr->getLaserMgr(laserMgr);
	connect(cubeMgr, &CubeManager::crashed, this, &Renderer::crashed, Qt::DirectConnection);
}
Renderer::~Renderer(){
}

void Renderer::setViewportSize(const QSize &size) {
    if (size != m_viewportSize)
    {
        _isResized = false;
        m_viewportSize = size;
    }
}
void Renderer::setWindow(QQuickWindow *window) {
    m_window = window;
}

void Renderer::initScene()
{
    qDebug() << "init";
    glEnable(GL_DEPTH_TEST);
   
	makeCube();
	makeLaser();
    _isInited = true;
}

void Renderer::resizeScene()
{
    //glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    releaseSFBO();
    makeSFBO();
    _isResized = true;
}

void Renderer::paint()
{
    if (!_isInited)
    {
        initScene();
    }
    if (!_isResized)
    {
        resizeScene();
    }
	camFront = spacecraft->getFront();
	camUp = spacecraft->getUp();
	camRight = spacecraft->getRight();
	camPos = spacecraft->getPos();
    glBindFramebuffer(GL_FRAMEBUFFER, _msaaFbo);
    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());
    glClearColor(0.0, 0.0, 0.0, 1.0f);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view;
    view.lookAt(camPos, camPos + camFront, camUp);
    //view.translate(0.0f,0.0f,-50.0f);  
    QMatrix4x4 projection;
   projection.perspective(45, 1.8, 2.0f, 50000.0f);
    
    QVector3D lightPos = { 0.0f,300000.0f,0.0f }; 
    QVector3D lightClr = { 0.8f,0.8f,0.8f };
   
	_renderObjs["lasers"]->setUniform("view", view);
	_renderObjs["lasers"]->setUniform("projection", projection);
	_renderObjs["lasers"]->setUniform("lightPos", lightPos);
	_renderObjs["lasers"]->setUniform("lightClr", lightClr);
	_buffers["laserInstanced"]->bind();
	_renderObjs["lasers"]->setInstancedCount(laserMgr->getModels().count());
	_buffers["laserInstanced"]->allocate(laserMgr->getModels().constData(), sizeof(QMatrix4x4)*laserMgr->getModels().count());
	_renderObjs["lasers"]->draw();


	_renderObjs["cubes"]->setUniform("view", view);
	_renderObjs["cubes"]->setUniform("projection", projection);
	_renderObjs["cubes"]->setUniform("lightPos", lightPos);
	_renderObjs["cubes"]->setUniform("lightClr", lightClr);

	_buffers["cubeInstanced"]->bind();
	_renderObjs["cubes"]->setInstancedCount(cubeMgr->getCubeCount()*2);
	_buffers["cubeInstanced"]->allocate(cubeMgr->getCubeModels().constData(), cubeMgr->getCubeCount() * 2*sizeof(QMatrix4x4));
	_buffers["cubeInstanced"]->write(tempmgr->getCubeCount() * sizeof(QMatrix4x4), tempmgr->getCubeModels().constData(), tempmgr->getCubeCount() * sizeof(QMatrix4x4));
	_buffers["cubeHit"]->bind();
	_buffers["cubeHit"]->allocate(cubeMgr->getCubeHit().constData(), cubeMgr->getCubeCount() *2* sizeof(float));
	_buffers["cubeInstanced"]->write(tempmgr->getCubeCount()  * sizeof(float), tempmgr->getCubeHit().constData(), tempmgr->getCubeCount() * sizeof(float));
	_renderObjs["cubes"]->draw();



    glBindFramebuffer(GL_READ_FRAMEBUFFER, _msaaFbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_window->renderTargetId());
    glBlitFramebuffer(0, 0, m_viewportSize.width(), m_viewportSize.height(),  // src rect
        0, 0, m_viewportSize.width(), m_viewportSize.height(),  // dst rect
        GL_COLOR_BUFFER_BIT, // buffer mask
        GL_LINEAR);
    m_window->resetOpenGLState();
    m_window->update();

}


void Renderer::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->localPos();
}

void Renderer::mouseMoveEvent(QMouseEvent *event)
{
    if (spacecraft)
    {
        spacecraft->mouseMoveEvent(event);
    }
}
void Renderer::hoverMoveEvent(QHoverEvent *event){
    if (spacecraft)
    {
        spacecraft->hoverMoveEvent(event);
    }
}
void Renderer::keyPressEvent(QKeyEvent * event)
{
	if (laserMgr) {
		laserMgr->keyPressEvent(event);
	}
	if (spacecraft) 
	{
		spacecraft->keyPressEvent(event);
	}
}

void Renderer::keyReleaseEvent(QKeyEvent * event)
{
	if (laserMgr)
	{
		laserMgr->keyReleaseEvent(event);
	}
	if (spacecraft) 
	{
		spacecraft->keyReleaseEvent(event);
	}
}

void Renderer::wheelEvent(QWheelEvent *ev)
{
}

void Renderer::mouseDoubleClickEvent(QMouseEvent *event)
{
}

void Renderer::makeSFBO()
{
    glGenFramebuffers(1, &_msaaFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _msaaFbo);

    glGenRenderbuffers(1, &_msaaColor);
    glBindRenderbuffer(GL_RENDERBUFFER, _msaaColor);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, m_viewportSize.width(), m_viewportSize.height());

    glGenRenderbuffers(1, &_msaaDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, _msaaDepth);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT32, m_viewportSize.width(), m_viewportSize.height());

    glGenRenderbuffers(1, &_msaaId);
    glBindRenderbuffer(GL_RENDERBUFFER, _msaaId);
    //glRenderbufferStorage(GL_RENDERBUFFER, GL_R32I, m_viewportSize.width(), m_viewportSize.height());
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_R32F, m_viewportSize.width(), m_viewportSize.height());

    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _msaaColor);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _msaaDepth);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, _msaaId);
    GLenum drawbuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawbuffers);

    qWarning() << glGetError();
}

void Renderer::releaseSFBO()
{
    if (_msaaFbo != 0)
    {
        glDeleteFramebuffers(1, &_msaaFbo);
        _msaaFbo = 0;
        glDeleteRenderbuffers(1, &_msaaColor);
        glDeleteRenderbuffers(1, &_msaaDepth);
        glDeleteRenderbuffers(1, &_msaaId);
        _msaaColor = _msaaDepth = _msaaId = 0;
    }
}

void Renderer:: makeCube()
{
	RenderObj* tempObj = new RenderObj();
	tempObj->setMode(RenderObj::ARRAYS_INSTANCED, GL_TRIANGLES);
	tempObj->setShaderProgram("cube");

	QOpenGLTexture* tempTex = new QOpenGLTexture(QImage("media/image/container.jpg"));
	tempObj->setTexture("ourTexture1", tempTex);
	tempTex = new QOpenGLTexture(QImage("media/image/awesomeface.png").mirrored());
	tempObj->setTexture("ourTexture2", tempTex);

	tempObj->bindVAO();
	QOpenGLBuffer* vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vertexBuffer->create();
	vertexBuffer->bind();
	vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
	QVector <GLfloat> _vertices = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	vertexBuffer->allocate(_vertices.constData(), _vertices.count() * sizeof(float));
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	tempObj->setVertexCount(36);
	_buffers["cubeVertex"] = vertexBuffer;

	QOpenGLBuffer* instancedBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	instancedBuffer->create();
	instancedBuffer ->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	instancedBuffer->bind();
	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (const GLvoid*)(i * sizeof(QVector4D)));
		glEnableVertexAttribArray(3 + i);
		glVertexAttribDivisor(3 + i, 1);
	}
	tempObj->setInstancedCount(cubeCount);
	_buffers["cubeInstanced"] = instancedBuffer;
	instancedBuffer= new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	instancedBuffer->create();
	instancedBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	instancedBuffer->bind();
	glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(7,1);
	_buffers["cubeHit"] = instancedBuffer;
	instancedBuffer->release();
	tempObj->unbindVAO();
	_renderObjs["cubes"] = tempObj;

}

void Renderer::makeLaser()
{
	RenderObj* tempObj = new RenderObj();
	tempObj->setMode(RenderObj::ARRAYS_INSTANCED, GL_TRIANGLES);
	tempObj->setShaderProgram("laser");

	QVector<QVector4D> laserVertex; //x,y,z,colorTier
	for (int i = 0; i < 10; i++) {
		int k = i + 1;
		laserVertex.push_back({ 0,0,4,1 });
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.06, cos(i*Pi / 5.0)*0.06, 3.8, 0.9));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.06, cos(k*Pi / 5.0)*0.06, 3.8, 0.9));
	}
	for (int i = 0; i < 10; i++) {
		int k = i + 1;
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.06, cos(i*Pi / 5.0)*0.06, 3.8, 0.9));
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.07, cos(i*Pi / 5.0)*0.07, 3.5, 0.8));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.07, cos(k*Pi / 5.0)*0.07, 3.5, 0.8));

		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.06, cos(i*Pi / 5.0)*0.06, 3.8, 0.9));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.06, cos(k*Pi / 5.0)*0.06, 3.8, 0.9));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.07, cos(k*Pi / 5.0)*0.07, 3.5, 0.8));
	}
	for (int i = 0; i < 10; i++) {
		int k = i + 1;
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.07, cos(i*Pi / 5.0)*0.07, -3.5, 0.2));
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.07, cos(i*Pi / 5.0)*0.07, 3.5, 0.8));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.07, cos(k*Pi / 5.0)*0.07, 3.5, 0.8));

		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.07, cos(i*Pi / 5.0)*0.07, -3.5, 0.2));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.07, cos(k*Pi / 5.0)*0.07, -3.5, 0.2));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.07, cos(k*Pi / 5.0)*0.07, 3.5, 0.8));
	}
	for (int i = 0; i < 10; i++) {
		int k = i + 1;
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.06, cos(i*Pi / 5.0)*0.06, -3.8, 0.1));
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.06, cos(i*Pi / 5.0)*0.07, -3.5, 0.2));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.07, cos(k*Pi / 5.0)*0.07, -3.5, 0.2));

		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.06, cos(i*Pi / 5.0)*0.06, -3.8, 0.1));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.06, cos(k*Pi / 5.0)*0.06, -3.8, 0.1));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.07, cos(k*Pi / 5.0)*0.07, -3.5, 0.2));
	}
	for (int i = 0; i < 10; i++) {
		int k = i + 1;
		laserVertex.push_back({ 0,0,-4,0 });
		laserVertex.push_back(QVector4D(sin(i*Pi / 5.0)*0.06, cos(i*Pi / 5.0)*0.06, -3.8, 0.1));
		laserVertex.push_back(QVector4D(sin(k*Pi / 5.0)*0.06, cos(k*Pi / 5.0)*0.06, -3.8, 0.1));
	}
	tempObj->bindVAO();
	QOpenGLBuffer* vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vertexBuffer->create();
	vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vertexBuffer->bind();
	vertexBuffer->allocate(laserVertex.constData(), laserVertex.count() * sizeof(QVector4D));
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	tempObj->setVertexCount(laserVertex.count());
	vertexBuffer->release();

	QOpenGLBuffer* instancedBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	instancedBuffer->create();
	instancedBuffer->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	instancedBuffer->bind();
	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(QMatrix4x4), (const GLvoid*)(i * sizeof(QVector4D)));
		glEnableVertexAttribArray(1 + i);
		glVertexAttribDivisor(1+ i, 1);
	}
	instancedBuffer->release();
	_buffers["laserInstanced"] = instancedBuffer;
	tempObj->unbindVAO();
	_renderObjs["lasers"] = tempObj;
}
