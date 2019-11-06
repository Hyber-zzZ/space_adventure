#include "cubemanager.h"



CubeManager::CubeManager()
{
	createCube(cubeCount);
	timer = new QTimer();
	timer->setInterval(10);
	connect(timer, &QTimer::timeout, this, &CubeManager::updateState, Qt::DirectConnection);
	timer->start();
}


CubeManager::~CubeManager()
{

}

void CubeManager::createCube(int cubeCount)
{
	for (int i = 0; i < cubeCount; i++) {
		Cube tempCube;
		if(i)tempCube.size=rand() % 1000 / 200.0f + 0.1f;
		else tempCube.size = 200;
		tempCube.initAng = { rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f };
		tempCube.rotateAng_spd={ rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f };

		QVector3D V = getVerticalV(axis);
		int dis= rand() % (MaxRadius-MinRadius) +MinRadius;
		V *= dis;
		V += axis.normalized()*MaxRadius / 100*(rand()%4-2);
		QMatrix4x4 tempRelPosMat;
		if (i) {
			tempRelPosMat.rotate(rand() % 360, axis);
			tempRelPosMat.translate(V);
		}
		tempRelPosMat.rotate(rand() % 360, { rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f ,rand() % 1000 / 1000.0f });
		tempRelPosMat.scale(tempCube.size);
		tempCube.relativePos = tempRelPosMat;
		_cubes.push_back(tempCube);

		QMatrix4x4 tempMat;
		tempMat.translate(centerPos);
		tempMat = tempMat * tempRelPosMat;
		finalModels.push_back(tempMat);

	}
}
QVector3D CubeManager::getVerticalV(QVector3D v)
{
	if (v.x() == 0)
		return { 1,0,0 };
	else if (v.y() == 0)
		return { 0,1,0 };
	else if (v.z() == 0)
		return { 0,0,1 };
	else {
		float tempZ = -(v.y() / v.z());
		QVector3D tempVec3 = { 0,1,tempZ };
		tempVec3.normalize();
		return tempVec3;
	}
}
void CubeManager::updateState() {
	for (int i = 0; i < _cubes.length(); i++) {
		QMatrix4x4 tempMat1;
		tempMat1.rotate(0.05, axis);
		_cubes[i].relativePos = tempMat1 * _cubes[i].relativePos;
		QVector4D rotateAng_spd = _cubes[i].rotateAng_spd;
		if (i)_cubes[i].relativePos.rotate(rotateAng_spd.w(), { rotateAng_spd.x(),rotateAng_spd.y(),rotateAng_spd.z() });
		QMatrix4x4 tempMat2;
		tempMat2.translate(centerPos);
		finalModels[i] = tempMat2* _cubes[i].relativePos;
		QVector3D pos = finalModels[i].column(3).toVector3D();
		float dis = pos.distanceToPoint(m_spacecraft->getPos());
		if (dis < _cubes[i].size * 0.5 + 2)crashed();
	}
}
