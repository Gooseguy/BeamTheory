//
//  Vertex.cpp
//  BeamTheory
//
//  Created by Christian on 11/29/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Vertex.h"

Vertex::Vertex(glm::vec3 pos, glm::vec3 col)  : position(pos), color(col) {}
Vertex::Vertex(float x, float y, float z, float r, float g, float b)  : position(x,y,z), color(r,g,b) {}
Vertex::Vertex(float x, float y, float z, glm::vec3 col)  : position(x,y,z), color(col) {}