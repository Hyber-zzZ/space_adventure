#include "cubemanager.h"



CubeManager::CubeManager()
{
	createCube(cubeCount);
    timer = new PerformanceTimer();
    connect(timer, &PerformanceTimer::timeout, this, &CubeManager::updateState, Qt::DirectConnection);
    timer->start(10);
}


CubeManager::~CubeManager()
{

}

void CubeManager::createCube(int cubeCount)
{
	for (int i = 0; i < cubeCount; i++) {
		Cube tempCube;
		//if(i)tempCube.size=rand() % 1000 / 200.0f + 0.1f;
		if (i)tempCube.size = rand() % 49 + 1;
		else tempCube.size = 400;
        if (!i)tempCube.health = 200000;
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
		_cubeIsHit.push_back(0);
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
        if (_cubes[i].health<=0) {
            _cubes.remove(i);
            finalModels.remove(i);
            _cubeIsHit.remove(i);
            cubeCount--;
            i--;
            continue;
        }
        _cubeIsHit[i] = 0;
        QVector<Laser> lasers=m_laserMgr->getLasers();
        for(int j=0;j<lasers.length();j++){
            QVector3D laserFrontPos=lasers[j].pos;
            QVector3D cubePos = finalModels[i].column(3).toVector3D();
            float laserPos_cube_dis=cubePos.distanceToPoint(laserFrontPos);
            float laserFront_cube_dis = cubePos.distanceToLine(lasers[j].pos,lasers[j].front);
            if(laserPos_cube_dis <_cubes[i].size+20&&laserFront_cube_dis< _cubes[i].size*0.6){
                m_laserMgr->laserHit(j);
                _cubes[i].health -= lasers[j].damage;
                _cubeIsHit[i] = 1;
            }

        }
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
        if (dis < _cubes[i].size*0.7 + 3)crashed();
    }
}
