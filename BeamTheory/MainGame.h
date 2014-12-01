//
//  MainGame.h
//  BeamTheory
//
//  Created by Christian on 11/29/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//
#pragma once
#include <SDL2/SDL.h>
#include "Beam.h"

class MainGame
{
public:
    enum class GameState
    {
        RUNNING, EXIT
    } CurrentGameState;
    
    MainGame();
private:
    glm::vec2 mousePosPrev;
    SDL_Window* window;
    int windowWidth, windowHeight;
    void handleEvents(Beam& beam);
    void update(Beam& beam);
    void draw(Beam& beam);
    bool paused;
};