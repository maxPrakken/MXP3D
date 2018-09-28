#pragma once

#include <iostream>
#include <vector>

#include "texture.hpp"
#include "objloader.hpp"
#include "mxVector.h"

class Entity {
public:
	// default constructor
	Entity();
	// deconstructor
	virtual ~Entity();
	// update function
	virtual void update(float deltaTime);

	// bool that holds loadobj()
	bool model;

	// vector3 position
	Vector3 pos;

	//addchild function. add it to the window and parent
	void addchild(Entity* child);
	//removechild function, removes it from the window and parent
	void removechild(Entity* child);

	// gets all children
	Entity* getChildren();

	// gets parent's vector3 position
	Vector3 getParentPos();

	// gets parent 
	Entity getParent() { return (*_parent); }

	// gets parent pointer
	Entity* getParentPointer() { return _parent; }

private:
	//parent entity
	Entity* _parent;

	// vector with children of entity
	std::vector<Entity*>childrenVector;

	//current and next entity in list
	int _guid;
	static int _nextGuid;
};
