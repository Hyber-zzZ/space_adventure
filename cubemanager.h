#ifndef CUBEMANAGER_H
#define CUBEMANAGER_H

#include<QObject>
#include<QtGui>
#include<QTimer>
#include"spacecraft.h"
#include"lasermanager.h"
struct Cube {
	QVector3D pos;
	QVector4D initAng;//初始角度
	QVector4D rotateAng_spd;//自转角度和速度
	QMatrix4x4 relativePos;
	float size;
	int health=100;

};
class CubeManager :public QObject
{
	Q_OBJECT
signals:
	void crashed();
public:
	CubeManager();
	~CubeManager();
	QVector<QMatrix4x4> getCubeModels() { return finalModels; }
	int getCubeCount() { return cubeCount; }
    void getSpacecraft(Spacecraft* spacecraft) { m_spacecraft = spacecraft; }
    void getLaserMgr(LaserManager* laserMgr){m_laserMgr=laserMgr;}
private:
	int cubeCount = 600;
	int MaxRadius =400;
	int MinRadius = 250;
	QVector3D centerPos = {0,0,500};
	QVector3D axis = { 0.2,1,-0.2 };
	QTimer* timer;
	QVector<Cube>_cubes;
	QVector<QMatrix4x4> finalModels;
	
	QVector<float>_cubeIsHit;
	void createCube(int cubeCount);
	QVector3D getVerticalV(QVector3D v);
	Spacecraft* m_spacecraft;
	LaserManager* m_laserMgr;
private slots:
	void updateState();
};

#endif
