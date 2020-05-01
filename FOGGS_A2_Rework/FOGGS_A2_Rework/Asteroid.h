#pragma once	
#ifndef ASTEROID_H
#define ASTEROID_H



#include "SceneObject.h"

class Asteroid : public SceneObject
{
public:
	Asteroid();
	Asteroid(Asteroid* asteroidModel);
	//Asteroid(Mesh asteroidMesh);
	~Asteroid();

	int GenerateRandomNumber();
	void SetRandomPosition();

private:

};




#endif