#ifndef CUBEMANAGER_H
#define CUBEMANAGER_H

#include<QObject>
#include<QtGui>
#include<QTimer>
#include"spacecraft.h"
struct Cube {
	QVector3D pos;
	QVector4D initAng;//��ʼ�Ƕ�
	QVector4D rotateAng_spd;//��ת�ǶȺ��ٶ�
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
	void getSpacecraftInstance(Spacecraft* s) { m_spacecraft = s; }
private:
	int cubeCount = 600;
	int MaxRadius =400;
	int MinRadius = 250;
	QVector3D centerPos = {0,0,1000};
	QVector3D axis = { 0.2,1,-0.2 };
	QTimer* timer;
	QVector<Cube>_cubes;
	QVector<QMatrix4x4> finalModels;
	
	QVector<float>_cubeIsHit;
	void createCube(int cubeCount);
	QVector3D getVerticalV(QVector3D v);
	Spacecraft* m_spacecraft;
private slots:
	void updateState();
};

#endif