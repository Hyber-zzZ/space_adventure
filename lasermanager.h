#pragma once
#include<QObject>
#include"spacecraft.h"
struct Laser {
	QVector3D pos;
	QVector3D front;
    float speed = 100;
    int health = 50;
	int damage = 5;
};
class LaserManager:public QObject
{
	Q_OBJECT
public:
	LaserManager();
	~LaserManager();
	QVector<QMatrix4x4> getModels() { return laserModels; }
	void setShoot(bool _isShooting) { shooting = _isShooting; }
	void getSpacecraftInstance(Spacecraft* s) { m_spacecraft = s; }
    void laserHit(int index){lasers[index].health=0;}
    QVector<Laser> getLasers(){return lasers;}
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

