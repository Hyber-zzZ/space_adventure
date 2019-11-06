#pragma once
#include<QObject>
#include"spacecraft.h"
struct Laser {
	QVector3D pos;
	QVector3D front;
	float speed = 10;
	int health = 100;
	int damage = 5;
};
class LaserManager:public QObject
{
	Q_OBJECT
public:
	LaserManager();
	~LaserManager();
	QVector<QMatrix4x4> getModels() { return laserModels; }
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void getSpacecraftInstance(Spacecraft* s) { m_spacecraft = s; }
private slots:
	void emitLaser(QVector3D _pos, QVector3D _front);
	void updateState();
private:
	QTimer* timer;
	QVector <Laser> lasers;
	QVector<QMatrix4x4>laserModels;
	bool shooting = false;
	int freqCount=0;
	Spacecraft* m_spacecraft;
};

