#pragma once
#ifndef FREIGHTER_H
#define FREIGHTER_H
#include "SceneObject.h"

class Freighter : public SceneObject
{
public:
	Freighter(Vector3 startPosition);
	~Freighter();

	void Update() override;




private:

};


#endif // !SPACE_FIGHTER_H
