#pragma once

#include "SDL2/SDL.h"
#include "../visulizer.hpp"
#include "../view_port.hpp"
#include <iostream>

template<typename Universe>
class SDLVisulizer : public Visulizer<Universe> {
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    

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
        for(auto it = universe->getParticles().begin(); it != universe->getParticles().end(); it++){
            SDL_RenderDrawPoint(this->renderer, it->getPosition()[0], it->getPosition()[1]);
        }

    }
};