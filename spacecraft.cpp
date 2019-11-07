#include "spacecraft.h"



Spacecraft::Spacecraft()
{
	up = { 0,1,0 };
	right = { 1,0,0 };
	front = { 0,0,1 };
	pos = { 0,0,0 };
	timer = new QTimer();
	timer->setInterval(10);
	connect(timer, &QTimer::timeout, this, &Spacecraft::updateState);
	timer->start();
}

Spacecraft::Spacecraft(QVector3D _pos)
{
	up = { 0,1,0 };
	right = { 1,0,0 };
	front = { 0,0,1 };
	pos = _pos;
	timer = new QTimer();
	timer->setInterval(10);
	connect(timer, &QTimer::timeout, this, &Spacecraft::updateState);
	timer->start();
}

Spacecraft::Spacecraft(QVector3D _up, QVector3D _right, QVector3D _front, QVector3D _pos)
{
	up = _up;
	right = _right;
	front = _front;
	pos = _pos;
	timer = new QTimer();
	timer->setInterval(10);
	connect(timer, &QTimer::timeout, this, &Spacecraft::updateState);
	timer->start();
}


Spacecraft::~Spacecraft()
{
}

void Spacecraft::mousePressEvent(QMouseEvent * event)
{
}

void Spacecraft::mouseMoveEvent(QMouseEvent * event)
{

}
void Spacecraft::hoverMoveEvent(QHoverEvent *event){
    QPointF p=event->pos()-event->oldPos();
    if(event->oldPos().x()<500||event->oldPos().x()>1420||event->oldPos().y()<100||event->oldPos().y()>980){
        QCursor::setPos(960,540);
        return;
    }
    yaw(p.x()*(0.0006));
    pitch(p.y()*-0.0006);

}
void Spacecraft::wheelEvent(QWheelEvent * event)
{
}

void Spacecraft::mouseDoubleClickEvent(QMouseEvent * event)
{
}

void Spacecraft::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Up) {
		turningUp = true;
		turningDown = false;
	}
	if (event->key() == Qt::Key_Down) {
		turningUp = false;
		turningDown = true;
	}
	if (event->key() == Qt::Key_Left) {
		turningLeft = true;
		turningRight = false;
	}
	if (event->key() == Qt::Key_Right) {
		turningLeft = false;
		turningRight = true;
	}
	if (event->key() == Qt::Key_A) {
		rollingLeft = true;
		rollingRight = false;
	}
	if (event->key() == Qt::Key_D) {
		rollingLeft = false;
		rollingRight = true;
	}
	if (event->key() == Qt::Key_W) {
		speedUp = true;
		slowDown = false;
	}
	if (event->key() == Qt::Key_S) {
		speedUp = false;
		slowDown = true;
	}
	event->accept();
}

void Spacecraft::keyReleaseEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Up) {
		turningUp = false;
	}
	if (event->key() == Qt::Key_Down) {
		turningDown = false;
	}
	if (event->key() == Qt::Key_Left) {
		turningLeft = false;
	}
	if (event->key() == Qt::Key_Right) {
		turningRight = false;
	}
	if (event->key() == Qt::Key_A) {
		rollingLeft = false;
	}
	if (event->key() == Qt::Key_D) {
		rollingRight = false;
	}
	if (event->key() == Qt::Key_W) {
		speedUp = false;
	} 
	if (event->key() == Qt::Key_S) {
		slowDown = false;
	}
	event->accept();
}

void Spacecraft::updateState()
{
	if (turningLeft) yaw(-0.005);
	if (turningRight) yaw(0.005);
	if (turningUp) pitch(0.005);
	if (turningDown) pitch(-0.005);
	if (rollingLeft) roll(0.005);
	if (rollingRight) roll(-0.005);
	pos = pos + front * speed;
	if (speedUp&&speed<=MaxSpeed)speed +=  0.004;
 	if (slowDown&&speed>=0)speed -=  0.004;
	if (speed > MaxSpeed)speed = MaxSpeed;
	if (speed < 0)speed = 0;
	outputShipSpeed(speed, pos.x(), pos.y(), pos.z());
}

void Spacecraft::roll(float ratio)
{
	rotate(up, right, ratio);
}

void Spacecraft::pitch(float ratio)
{
	rotate(front, up, ratio);
}

void Spacecraft::yaw(float ratio)
{
	rotate(right, front, ratio);
}

void Spacecraft::rotate(QVector3D & a, QVector3D & b, float ratio)
{
	QVector3D tempA = a;
	a = a + b * ratio;
	b = b - tempA * ratio;
	a.normalize();
	b.normalize();
}
