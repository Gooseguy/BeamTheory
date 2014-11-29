//
//  Beam.h
//  BeamTheory
//
//  Created by Christian on 11/29/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include "glm/glm.hpp"
#include <OpenGL/gl3.h>
#include "Vertex.h"
#include <vector>
#include <array>
class Beam
{
public:
    Beam(int length, float spacing, float timeStep, float beamConstant);
    
    const int LENGTH;
    const float TIME_STEP;
    const float SPACING;
    const float BEAM_CONSTANT;
    
    const float SCALE_Y;
    const float SCALE_X;
    const glm::vec2 SHIFT;
    const float BeamThickness;
    
    void Draw();
    void Update();
    
    inline void MouseClick(float x, float y);
    
    void Reset();
    
private:
    void constrainBoundaries();
    
    GLuint vbo, ibo, vao;
    void generateBuffers();
    void updateBuffers();
    std::vector<float> displacement;
    std::vector<float> prevDisplacement;
    std::vector<Vertex> vertices;
    std::vector<unsigned short> indices;
    inline float getStress(int i);
    inline glm::vec3 getColor(int i);
};
void Beam::MouseClick(float x, float y)
{
    int xp = x*LENGTH;
    if (xp<LENGTH && xp>0)
    {
        displacement[xp]=y;
        prevDisplacement[xp]=y;
    }
}

float Beam::getStress(int i)
{
    float stress=0;
    if (i>1) stress-=0.5f * displacement[i-2];
    if (i>0) stress+=displacement[i-1];
    if (i<LENGTH-2) stress+=0.5f * displacement[i+2];
    if (i<LENGTH-1) stress-=displacement[i+1];
    return stress;
}

glm::vec3 Beam::getColor(int i) {
    float stress = getStress(i)*4;
    float variance=0.45;
    return glm::vec3(0.2f) + glm::vec3(exp(-stress*stress*variance), exp(-(stress-1)*(stress-1)*variance), exp(-(stress-2)*(stress-2)*variance));
}