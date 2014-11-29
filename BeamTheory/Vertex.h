//
//  Vertex.h
//  BeamTheory
//
//  Created by Christian on 11/29/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "glm/glm.hpp"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    
    Vertex(glm::vec3 pos, glm::vec3 col);
    Vertex(float x, float y, float z, float r, float g, float b);
    Vertex(float x, float y, float z, glm::vec3 col);
    
};