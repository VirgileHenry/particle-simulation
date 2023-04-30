#pragma once

#include "SDL2/SDL.h"
#include "../visualizer.hpp"
#include "../view_port.hpp"
#include <iostream>

template<typename Universe>
class SDLVisualizer : public Visualizer<Universe> {
    private:
    SDL_Window* window;
    SDL_Event event;
    SDL_Renderer* renderer;

    public:
    SDLVisualizer() {
        // init members

        if(SDL_VideoInit(NULL) != 0) {
            // error while initializing the video system, destroy self
            exit(1); // better error management ?
        }
        // create the window with title, x, y, width, height, flags.
        // here we set the flags so the window grab the focus on creation, and is resizable.
        this->window = SDL_CreateWindow("SDL universe visulizer", 100, 100, 800, 450, SDL_WINDOW_RESIZABLE);
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
        while (SDL_PollEvent(&this->event)) {
            switch(this->event.type) {
                case SDL_QUIT:
                    exit(EXIT_SUCCESS);
                    break;
                default:
                    break;
            }
        }
    }

    public: 
    void draw(Universe* universe) override {
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 1);
        SDL_RenderClear(this->renderer);
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        for(auto particle : universe->getParticles()){
            SDL_RenderDrawPoint(this->renderer, particle.getPosition()[0] * 800 / 250, particle.getPosition()[1] * 450 / 250);
        }
        SDL_RenderPresent(this->renderer);

        // events handling so the window is responding
        this->handleEvents();
    }
};