#pragma once
#include<QtGui>
#include<QObject>
#include<QTimer>
const float MaxSpeed = 5;
class Spacecraft :public QObject
{
	Q_OBJECT
signals:
	void outputShipSpeed(float speed, float shipx, float shipy, float shipz);
public:
	Spacecraft();
	Spacecraft(QVector3D _pos);
	Spacecraft(QVector3D _up, QVector3D _right, QVector3D _front, QVector3D _pos);
	~Spacecraft();
	QVector3D getUp() { return up; }
	QVector3D getRight() { return right; }
	QVector3D getFront() { return front; }
	QVector3D getPos() { return pos; }
	float getSpeed() { return speed; }
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
    void hoverMoveEvent(QHoverEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void changeDirection(QPointF p);

private:
	QVector3D up, right, front,pos;
	float speed=0;
	bool turningUp = false, turningDown = false, turningLeft = false, turningRight = false, rollingLeft = false, rollingRight = false, speedUp = false,slowDown = false;
	QTimer *timer;
	void roll(float ratio);
	void pitch(float ratio);
	void yaw(float ratio);
	void rotate(QVector3D &a, QVector3D &b, float ratio);

private slots:
	void updateState();

};

