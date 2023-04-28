#pragma once

#include "SDL2/SDL.h"
#include "../visualizer.hpp"
#include "../view_port.hpp"
#include <iostream>

template<typename Universe>
class SDLVisualizer : public Visualizer<Universe> {
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    const bool show_vel = false;
    const bool show_force = false;
    int windowSize[2] = {800, 450};
    double windowView[2] = {0.01, 0.01};
    double windowCorner[2] = {-200, -200};
    unsigned int windowDimensions[2] = {0, 1};
    Vector<double, 2> corner = windowCorner;
    Vector<double, 2> size = windowView;
    Vector<unsigned int, 2> dimensions = windowDimensions;
    ViewPort view = ViewPort(corner, size, dimensions);
    

    public:
    SDLVisualizer() {
        // init members

        if(SDL_VideoInit(NULL) != 0) {
            // error while initializing the video system, destroy self
            exit(1); // better error management ?
        }
        // create the window with title, x, y, width, height, flags.
        // here we set the flags so the window grab the focus on creation, and is resizable.
        this->window = SDL_CreateWindow("SDL universe visulizer", 100, 100, windowSize[0], windowSize[1], SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_RESIZABLE);
        this->renderer = SDL_CreateRenderer(this->window, -1, 0);

        // create another thread that handle events

    }

    ~SDLVisualizer() {
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
        SDL_Quit();
    }

    private:
    void handleEvents() {
        SDL_Event event;
        if(SDL_PollEvent(&event)) {
            // match on event.type
        }
    }

    public: 
    void draw(Universe* universe) const override {
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 1);
        SDL_RenderClear(this->renderer);
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        for(auto particle : universe->getParticles()){
            SDL_RenderDrawPoint(this->renderer, particle.getPosition()[view.dimensions[0]] * windowSize[0]/view.size[0] - view.corner[0], particle.getPosition()[view.dimensions[1]] * windowSize[1]/view.size[1] - view.corner[1]);
        }
        // debug display
        if(this->show_force) {
            SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
            // draw all particles velocity
            for(auto particle : universe->getParticles()){
                SDL_RenderDrawLine(this->renderer,
                    particle.getPosition()[0] * 800, particle.getPosition()[1] * 450,
                    particle.getPosition()[0] * 800 + particle.getForce()[0], particle.getPosition()[1] * 450 + particle.getForce()[1]
                );
            }
        }
        if(this->show_vel) {
            SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
            // draw all particles velocity
            for(auto particle : universe->getParticles()){
                SDL_RenderDrawLine(this->renderer,
                    particle.getPosition()[0] * 800, particle.getPosition()[1] * 450,
                    particle.getPosition()[0] * 800 + particle.getVelocity()[0], particle.getPosition()[1] * 450 + particle.getVelocity()[1]
                );
            }
        }
        SDL_RenderPresent(this->renderer);

    }
};