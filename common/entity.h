#pragma once

#include <iostream>
#include <vector>

#include "texture.hpp"
#include "objloader.hpp"

class Entity {
public:
	// default constructor
	Entity();
	//deconstructor
	virtual ~Entity();
	//update function
	virtual void update(float deltaTime);

	//bool that holds loadobj()
	bool model;

private:

};
