#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <iostream>
#include "renderer.h"

void computeMatricesFromInputs(float deltaTime);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif