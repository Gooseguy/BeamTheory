//
//  MainGame.cpp
//  BeamTheory
//
//  Created by Christian on 11/29/14.
//  Copyright (c) 2014 Christian Cosgrove. All rights reserved.
//

#include "MainGame.h"
#include <stdexcept>
#include <string>
#include "GLManager.h"
#include "ResourcePath.hpp"

MainGame::MainGame() : CurrentGameState(GameState::RUNNING), paused(false)
{
    if (SDL_Init(SDL_INIT_VIDEO)) throw std::logic_error("Failed to initialize SDL!  " + std::string(SDL_GetError()));
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 4);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    windowWidth = 1280, windowHeight = 720;
    
    window = SDL_CreateWindow("Beam Theory", 0, 0, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);
    if (window==nullptr) throw std::logic_error("Failed to initialize window");
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context==nullptr) throw std::logic_error("Failed to initialize SDL_GL");
    
    GLManager glManager(resourcePath() + "fragmentShader.glsl", resourcePath() + "vertexShader.glsl");
    Beam beam(128, 0.1f, 0.0001f, 0.0001f);
    while (CurrentGameState==GameState::RUNNING)
    {
        update(beam);
        draw(beam);
        handleEvents(beam);
        SDL_GL_SwapWindow(window);
    }
}

void MainGame::handleEvents(Beam& beam)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                CurrentGameState = GameState::EXIT;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_ESCAPE:
                    CurrentGameState = GameState::EXIT;
                    break;
                case SDL_SCANCODE_SPACE:
                    beam.Reset();
                    break;
                case SDL_SCANCODE_P:
                    paused=!paused;
                    break;
                default:
                    break;
            }
            default: break;
        }
    }
}

void MainGame::update(Beam &beam)
{
    int x,y;
    if (SDL_GetMouseState(&x,&y) & SDL_BUTTON(SDL_BUTTON_LEFT))
        beam.MouseClick((float)x/windowWidth, (-(float)y/windowHeight+0.5)/beam.SCALE_Y*2);
    if (!paused) for (int i=0;i<10;++i)
        beam.Update();
    
}

void MainGame::draw(Beam &beam)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    beam.Draw();
}