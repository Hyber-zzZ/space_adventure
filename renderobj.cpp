#include "renderobj.h"



RenderObj::RenderObj()
{
	initializeOpenGLFunctions();
	glGenVertexArrays(1, &VAO);
}


RenderObj::~RenderObj()
{
}

void RenderObj::setShaderProgram(QString name)
{
	program = new  QOpenGLShaderProgram();

	QFile* shader = new QFile("media/shader/" + name + ".vert");
	if(shader->exists()) 
		program->addShaderFromSourceFile(QOpenGLShader::Vertex, "media/shader/" + name + ".vert");

	shader = new QFile("media/shader/" + name + ".frag");
	if (shader->exists())
		program->addShaderFromSourceFile(QOpenGLShader::Fragment, "media/shader/" + name + ".frag");

	shader = new QFile("media/shader/" + name + ".geom");
	if (shader->exists())
		program->addShaderFromSourceFile(QOpenGLShader::Geometry, "media/shader/" + name + ".geom");
	program->link();
}

void RenderObj::loadVertexFromObj(QString objName, QOpenGLBuffer* vertexBuffer)
{
	QVector<QVector3D> _position;
	QVector<QVector2D>_texCoords;
	QVector<QVector3D>_normal;
	QVector<QString>_face;
	QVector<Vertex>vertData;
	QFile file("media/model/" + objName + "/" + objName + ".obj");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!file.atEnd())
		{
			QByteArray line = file.readLine();
			QString str(line);
			str.remove("\n", Qt::CaseSensitive);
			if (str == "")continue;
			QStringList tempStrList = str.split(" ");
			QStringList strList;
			for (int i = 0; i < tempStrList.length(); i++)
				if (tempStrList[i] != "") {
					int k;
					strList.append(tempStrList[i]);
				}
			QString type = strList[0];
			if (type == "v") {
				QVector3D tempPos = { strList[1].toFloat(),  strList[2].toFloat(),  strList[3].toFloat() };
				_position.push_back(tempPos);
			}
			else if (type == "vt") {
				QVector2D tempTexCoords = { strList[1].toFloat(),  strList[2].toFloat() };
				_texCoords.push_back(tempTexCoords);
			}
			else if (type == "vn") {
				QVector3D tempNormal = { strList[1].toFloat(),  strList[2].toFloat(),  strList[3].toFloat() };
				_normal.push_back(tempNormal);
			}
			else if (type == "f") {
				if (strList.length() == 4) {
					for (int j = 1; j < 4; j++)
						_face.push_back(strList[j]);
				}
				else if (strList.length() == 5) {
					_face.push_back(strList[1]);
					_face.push_back(strList[2]);
					_face.push_back(strList[4]);
					_face.push_back(strList[2]);
					_face.push_back(strList[3]);
					_face.push_back(strList[4]);

				}
			}
		}
		file.close();
	}
	for (int i = 0; i < _face.length(); i++) {
		QStringList v_vt_vn = _face[i].split("/");
		int v_index = v_vt_vn[0].toInt() - 1;
		int vt_index = v_vt_vn[1].toInt() - 1;
		int vn_index = v_vt_vn[2].toInt() - 1;
		Vertex tempVertex;
		tempVertex.position = _position[v_index];
		tempVertex.texCoords = _texCoords[vt_index];
		tempVertex.normal = _normal[vn_index];
		vertData.push_back(tempVertex);
	}
	vertexCount = vertData.size();

	if (!vertexBuffer->isCreated())vertexBuffer->create();
	vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vertexBuffer->bind();
	vertexBuffer->allocate(vertData.constData(), sizeof(Vertex)*vertData.size());
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	vertexBuffer->release();

}

void RenderObj::draw()
{
	if (!enabled)return;
	program->bind();

	QMap<QString, QOpenGLTexture*>::Iterator texIterator=textures.begin();
	int texID=0;
	while (texIterator!= textures.end()) {
		QOpenGLTexture* tex = texIterator.value();
		QString name = texIterator.key();
		tex->bind(texID);
		program->setUniformValue(name.toStdString().c_str(), texID);
		texID++;
		texIterator++;
	}

	QMap<QString,QVector3D>::Iterator uniformIterator = uniforms.begin();
	while (uniformIterator!= uniforms.end()) {
		program->setUniformValue(uniformIterator.key().toStdString().c_str(), uniformIterator.value());
		uniformIterator++;
	}
	QMap<QString, QMatrix4x4>::Iterator uniformMatIterator = uniformMats.begin();
	while (uniformMatIterator!=uniformMats.end()) {
		program->setUniformValue(uniformMatIterator.key().toStdString().c_str(), uniformMatIterator.value());
		uniformMatIterator++;
	}
	glBindVertexArray(VAO);
	switch (drawMode)
	{
	case ARRAYS:
		glDrawArrays(beginMode, 0, vertexCount);
		break;
	case ARRAYS_INSTANCED:
		glDrawArraysInstanced(beginMode, 0, vertexCount, instancedCount);
		break;
	case ELEMENTS:
		glDrawElements(beginMode, indexCount, GL_UNSIGNED_SHORT, NULL);
		break;
	case ELEMENTS_INSTANCED:
		glDrawElementsInstanced(beginMode, indexCount,GL_UNSIGNED_SHORT, NULL, instancedCount);
		break;
	default:
		break;
	}
	glBindVertexArray(0);
	program->release();
}
