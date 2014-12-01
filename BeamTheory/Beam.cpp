//
//  Beam.cpp
//  BeamTheory
//
//  Created by Christian on 11/29/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "Beam.h"

Beam::Beam(int length, float spacing, float timeStep, float beamConstant) : LENGTH(length), SPACING(spacing), TIME_STEP(timeStep), BEAM_CONSTANT(beamConstant), displacement(LENGTH), prevDisplacement(LENGTH), SCALE_Y(0.001), SCALE_X(2.0f/LENGTH/SPACING), SHIFT(-1,0), BeamThickness(0.01f), Boundary1(BoundaryType::FREE), Boundary2(BoundaryType::CLAMP)
{
    generateBuffers();
    Reset();
    Update();
}

void Beam::Draw()
{
    updateBuffers();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
    glBindVertexArray(0);
}

void Beam::generateBuffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glBindVertexArray(0);
    
    
    vertices.reserve(LENGTH);
    indices.reserve(2*LENGTH-1);
    for (unsigned short i = 0; i<LENGTH;++i)
    {
        vertices.push_back(Vertex(i*SPACING*SCALE_X + SHIFT.x, SCALE_Y * displacement[i]+ SHIFT.y - BeamThickness/2, 0.0, getColor(i)));
        vertices.push_back(Vertex(i*SPACING*SCALE_X + SHIFT.x, SCALE_Y * displacement[i]+ SHIFT.y + BeamThickness/2, 0.0, getColor(i)));
        if (i<LENGTH-1)
        {
            indices.push_back(2*i+0);
            indices.push_back(2*i+1);
            indices.push_back(2*i+2);
            indices.push_back(2*i+1);
            indices.push_back(2*i+3);
            indices.push_back(2*i+2);
        }
    }
}

void Beam::Update()
{
    //Explicit finite difference method
    //This is too unstable for practical use.
    const int steps = 100;
//    for (int k = 0; k<steps;++k)
//    {
//        constrainBoundaries();
//        for (int i = 2; i<LENGTH-2;++i)
//        {
//            nextDisplacement[i] = BEAM_CONSTANT * TIME_STEP * TIME_STEP / (steps * steps * SPACING*SPACING*SPACING*SPACING) * (displacement[i+2] - 4*displacement[i+1] + 6*displacement[i] - 4*displacement[i-1] + displacement[i+2]) + 2*displacement[i] - prevDisplacement[i];
//        }
//        
//        prevDisplacement = displacement;
//        displacement = nextDisplacement;
//    }
    
    std::vector<float> nextDisplacement = displacement;
    const float omega = 1.2;
    const float SPACING4=1;//SPACING*SPACING*SPACING*SPACING;
    const float SPACING3=SPACING*SPACING*SPACING;
    
    //Uses the SOR algorithm, http://en.wikipedia.org/wiki/Successive_over-relaxation#Algorithm
    
    for (int k = 0; k<steps;++k)
    {
        for (int i = 0;i<LENGTH;++i)
        {
            float sig=0;
            constrainBoundaries(nextDisplacement, sig, i);
//            else sig+=2*nextDisplacement[i]-nextDisplacement[i-1];
//            float sig = displacement[i+2] - 4*displacement[i+1] - 4*displacement[i-1] + displacement[i-2];
            nextDisplacement[i]+=omega * (((-0.1f*(nextDisplacement[i] - 2*displacement[i] + prevDisplacement[i])-sig)/(6.f)-nextDisplacement[i]));
            
        }
    }
//    prevPrevDisplacement = prevDisplacement;
    prevDisplacement = displacement;
    displacement = nextDisplacement;
    
}

void Beam::updateBuffers()
{
    for (size_t i = 0; i<LENGTH;++i)
    {
        vertices[2*i].position.y = SCALE_Y * displacement[i] + SHIFT.y - BeamThickness/2;
        vertices[2*i].color = getColor(i);
        vertices[2*i+1].position.y = SCALE_Y * displacement[i] + SHIFT.y + BeamThickness/2;
        vertices[2*i+1].color = getColor(i);
    }
    
    glBindVertexArray(vao);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*indices.size(), &indices[0], GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void Beam::constrainBoundaries(std::vector<float>& nextDisplacement, float &sigma, int i)
{
    if (i>1) sigma+=nextDisplacement[i-2]-4*nextDisplacement[i-1];
    else
    {
        switch (Boundary1)
        {
            case BoundaryType::CLAMP:
                nextDisplacement[i]=3*nextDisplacement[i+2];
                break;
            case BoundaryType::FREE:
//                sigma+=-6*nextDisplacement[i]+3*nextDisplacement[i+1];
                sigma+=-4*nextDisplacement[i]+nextDisplacement[i+2];
                break;
            case BoundaryType::SIMPLE_SUPPORT:
                break;
        }
    }
    
    
    if (i<LENGTH-2) sigma+=nextDisplacement[i+2]-4*nextDisplacement[i+1];
    else
    {
        switch (Boundary2)
        {
            case BoundaryType::CLAMP:
                nextDisplacement[i]=0;
                nextDisplacement[i-1]=0;
//                sigma+=2*nextDisplacement[i]-nextDisplacement[i-1];
                break;
            case BoundaryType::FREE:
//                sigma+=-6*nextDisplacement[i-1]+3*nextDisplacement[i-2];
                break;
            case BoundaryType::SIMPLE_SUPPORT:
                break;
        }
    }
}

void Beam::Reset() {
    for (int i = 0; i<LENGTH;++i)
    {
        displacement[i] = 0.0f;
        prevDisplacement[i] = 0.0f;
//        displacement[i] = sin(M_PI * i / LENGTH)*100;
//        prevDisplacement[i] = sin(M_PI * (i) / LENGTH)*100;
    }
    
}

void Beam::MouseClick(float x, float y, float px, float py)
{
    int xp = x * LENGTH;
    xp = std::max(std::min(xp,LENGTH-1), 0);
    int xpp = x * LENGTH;
    xpp = std::max(std::min(xpp,LENGTH-1), 0);
    displacement[xp]=y;
    prevDisplacement[xpp]=py;
}