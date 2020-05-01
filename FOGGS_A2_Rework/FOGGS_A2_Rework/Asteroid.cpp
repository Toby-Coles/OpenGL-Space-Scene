#include "Asteroid.h"
#include <random>

Asteroid::Asteroid()
{
	std::cout << "Asteroid Created" << std::endl;
	
	asteroidRotate = true;
	mTexture = new Texture2D;
	
	SceneObject::LoadTexture("Textures/Asteroid.bmp");	// Load the texture for the asteroid
	//mPosition = startPosition;

	SetRandomPosition();
	GenerateRandomNumber();
}

Asteroid::Asteroid(Asteroid* asteroidModel)
{
	std::cout << "Asteroid Created" << std::endl;
	//objLoad.mLoadedMeshes = asteroidModel;
	asteroidRotate = true;
	mTexture = new Texture2D;

	SceneObject::LoadTexture("Textures/Asteroid.bmp");
	//mPosition = startPosition;

	SetRandomPosition();
	GenerateRandomNumber();
}

Asteroid::~Asteroid()

{
}

int Asteroid::GenerateRandomNumber()
{
	std::random_device rd;
	std::mt19937 e{ rd() };
	std::uniform_int_distribution<int> dist{ -120, 120 };

	return dist(e);
}

void Asteroid::SetRandomPosition()
{
	int randomXnumber = GenerateRandomNumber();
	//std::cout << "X: " << randomXnumber << std::endl;
	int randomYnumber = GenerateRandomNumber();
	//std::cout << "Y: " << randomYnumber << std::endl;
	int randomZnumber = GenerateRandomNumber();
	//std::cout << "Z: " << randomZnumber << std::endl;

	mPosition.X = randomXnumber;
	

	mPosition.Y = randomYnumber;

	mPosition.Z = randomZnumber;

	
}
