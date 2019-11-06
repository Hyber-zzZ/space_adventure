#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include<QMap>
#include<QOpenGLBuffer>
#include<QFile>

struct  Vertex
{
	QVector3D position;
	QVector2D texCoords;
	QVector3D normal;
};

class RenderObj :public QOpenGLFunctions_4_5_Core
{

public:
	enum DrawMode
	{
		ARRAYS,
		ARRAYS_INSTANCED,
		ELEMENTS,
		ELEMENTS_INSTANCED
	};
	RenderObj();
	~RenderObj();
	void setEnabled(bool _enabled) { enabled = _enabled; }
	void setShaderProgram(QOpenGLShaderProgram *_program) { program = _program; }
	void setShaderProgram(QString name);
	void setMode(DrawMode _drawMode, GLenum _beginMode){ beginMode = _beginMode; drawMode = _drawMode;}
	void setVertexCount(int count) { vertexCount = count; }
	void setInstancedCount(int count) { instancedCount = count; }
	void setIndexCount(int count) { indexCount = count; }
	void setTexture(QString name, QOpenGLTexture* _texture) { textures[name] = _texture; }
	void setUniform(QString name, QVector3D value) { uniforms[name] = value; }
	void setUniform(QString name, QMatrix4x4 value) { uniformMats[name] = value; }
	void loadVertexFromObj(QString objPath,QOpenGLBuffer* vertexBuffer);
	void bindVAO() { glBindVertexArray(VAO); }
	void unbindVAO(){ glBindVertexArray(0); }
	QOpenGLShaderProgram* getProg() { return program; }
	void draw();
private:
	bool enabled;
	QOpenGLShaderProgram* program;
	QOpenGLBuffer* mainBuffer;
	QOpenGLBuffer* indexBuffer;
	QOpenGLBuffer* instancedBuffer;
	int vertexCount,indexCount, instancedCount;
	GLuint VAO;
	GLenum beginMode;
	DrawMode drawMode;
	QMap<QString, QVector3D>uniforms;
	QMap<QString, QMatrix4x4>uniformMats;
	QMap<QString,QOpenGLTexture*> textures;
	
};

