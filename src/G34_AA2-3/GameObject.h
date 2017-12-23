#pragma once
#include "Renderer.h"

class GameObject //les classes que tinguin Update i Draw hereden d'aquesta classe interfa�
{
public:
	GameObject();
	~GameObject();
	virtual void Update() = 0;
	virtual void Draw() = 0;

};

