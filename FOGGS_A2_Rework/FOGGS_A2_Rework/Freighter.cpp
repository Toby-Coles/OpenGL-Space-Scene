#include "Freighter.h"

Freighter::Freighter(Vector3 startPosition)
{
	mTexture = new Texture2D;
	asteroidRotate = false;
	SceneObject::LoadTexture("Textures/Test.bmp");
	mPosition = startPosition;
}

Freighter::~Freighter()
{
	delete mTexture;
}

void Freighter::Update()
{
	//This jumps the freighter into the scene by making it come in very fast, get to a point, then slow down, imitating a hyperspace jump
	if (mPosition.Z < -120)
	{
		mPosition.Z += 50;
	}
	else
	{
		mPosition.Z += 0.1;
	}


}
