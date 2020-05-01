#pragma once
#include "GLUTCallbacks.h"
#define REFRESHRATE 16
#define ASTEROIDCOUNT

#include <Windows.h> // Required for windows compatibility
#include <gl/GL.h> //OpenGL
#include <gl/GLU.h> // OpenGL Utilities
#include "GL\freeglut.h"// freeglut library

#include "OBJLoader.h" 

#include "SceneObject.h"
#include "SpaceFighter.h"
#include "Asteroid.h"
#include "Freighter.h"


class HelloGL
{
public:
	HelloGL(int argc, char* argv[]);
	~HelloGL();

	void Keyboard(unsigned char key, int x, int y);
	void Update();
	void Display();


	void InitObjects();
	void InitGL(int argc, char* argv[]);
	void InitLighting();
	void InitFog();

	void GenerateAsteroidField();

private:

	Vector3 rotation; 

	//LoadOBJ objLoad;
	Texture2D* mTexture; 
	Camera* camera;
	Vector3 cameraPosition;
	Vector3 cameraTarget;


	
	Vector3* lightPosition;
	Lighting* lightData;
	Lighting* mLight;

	SpaceFigher* spaceFighter; 
	Freighter* freighter;
	Asteroid* asteroid;
	Asteroid** asteroidArray;

	//aAsteroid* asteroidMeshObject;
	int asteroidCount;

	//SceneObject* asteroidMesh;

	//Fog Variables
	GLuint filter;
	GLuint fogMode[3] = { GL_EXP, GL_EXP2, GL_LINEAR };
	GLuint fogFilter;
	GLfloat fogColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };


};