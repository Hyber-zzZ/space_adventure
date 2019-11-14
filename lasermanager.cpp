#include "lasermanager.h"



LaserManager::LaserManager()
{
	timer = new QTimer();
	timer->setInterval(10);
	connect(timer, &QTimer::timeout, this, &LaserManager::updateState, Qt::DirectConnection);
	timer->start();
}


LaserManager::~LaserManager()
{
}

void LaserManager::emitLaser(QVector3D _pos, QVector3D _front){
	Laser tempLaser;
	tempLaser.front = _front;
	tempLaser.pos = _pos;
	lasers.push_back(tempLaser);
	QVector3D axis = QVector3D::crossProduct(_front, { 0,0,1 });
	float dot = QVector3D::dotProduct(_front, { 0,0,1 });
	float angle = acosf( dot)/3.14159265359*180;
	QMatrix4x4 tempModel;
	tempModel.translate(_pos);
	tempModel.rotate(-angle, axis);
	laserModels.push_back(tempModel);
}
void LaserManager::updateState()
{
	if (shooting) {
		freqCount++;
		if (freqCount == 6) {
            emitLaser(m_spacecraft->getPos() -m_spacecraft->getFront()*1+ m_spacecraft->getRight()*1-m_spacecraft->getUp()*1, m_spacecraft->getFront());
			emitLaser(m_spacecraft->getPos() - m_spacecraft->getFront()*1 + m_spacecraft->getRight()*-1-m_spacecraft->getUp()*1, m_spacecraft->getFront());
			//emitLaser(m_spacecraft->getPos() + m_spacecraft->getFront() * 1 , m_spacecraft->getFront());
			freqCount = 0;
		}
	}
	for (int i = 0; i < lasers.length(); i++) {
		lasers[i].health--;
        if (lasers[i].health<=0) {
			lasers.remove(i);
			laserModels.remove(i);
			i--;
			continue;
		}
		QVector3D x= lasers[i].front*lasers[i].speed;
		lasers[i].pos += x;
		QMatrix4x4 tempModel;
		tempModel.translate(x);
		laserModels[i] = tempModel * laserModels[i];
	}
}
