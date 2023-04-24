#pragma once

#include "SDL2/SDL.h"
#include "../visualizer.hpp"
#include "../view_port.hpp"
#include <iostream>

template<typename Universe>
class SDLVisulizer : public Visualizer<Universe> {
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    const bool show_vel = false;
    const bool show_force = false;
    

    public:
    SDLVisulizer() {
        // init members

        if(SDL_VideoInit(NULL) != 0) {
            // error while initializing the video system, destroy self
            exit(1); // better error management ?
        }
        // create the window with title, x, y, width, height, flags.
        // here we set the flags so the window grab the focus on creation, and is resizable.
        this->window = SDL_CreateWindow("SDL universe visulizer", 100, 100, 800, 450, SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_RESIZABLE);
        this->renderer = SDL_CreateRenderer(this->window, -1, 0);

        // create another thread that handle events

    }

    ~SDLVisulizer() {
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
            SDL_RenderDrawPoint(this->renderer, particle.getPosition()[0] * 800, particle.getPosition()[1] * 450);
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