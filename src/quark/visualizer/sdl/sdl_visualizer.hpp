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
    // window size
    Vector<int, 2> window_size;
    // view port
    ViewPort view;
    
    
    public:
    SDLVisualizer() {
        // init members

        // default values
        int size[2] = {800, 450};
        this->window_size = Vector<int, 2>(size);

        if(SDL_VideoInit(NULL) != 0) {
            // error while initializing the video system, destroy self
            exit(1); // better error management ?
        }
        // create the window with title, x, y, width, height, flags.
        // here we set the flags so the window grab the focus on creation, and is resizable.
        this->window = SDL_CreateWindow("SDL universe visulizer", 100, 100, window_size[0], window_size[1], SDL_WINDOW_RESIZABLE);
        this->renderer = SDL_CreateRenderer(this->window, -1, 0);


    }

    ~SDLVisualizer() {
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
        SDL_Quit();
    }

    // some setters
    void setViewportCorner(double corner[2]) {
        this->view.corner = Vector<double, 2>(corner);
    }

    void setViewportSize(double size[2]) {
        this->view.size = Vector<double, 2>(size);
    }

    void setViewportDimensions(unsigned int dim[2]) {
        this->view.dimensions = Vector<unsigned int, 2>(dim);
    }

    private:
    void handleEvents() {
        while (SDL_PollEvent(&this->event)) {
            switch(this->event.type) {
                case SDL_QUIT:
                    exit(EXIT_SUCCESS);
                    break;
                case SDL_WINDOWEVENT:
                    switch(this->event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            this->setSize(this->event.window.data1, this->event.window.data2);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    void setSize(int x, int y) {
        int new_size[2] = {x, y};
        this->window_size = Vector<int, 2>(new_size);
    }

    public: 
    void draw(Universe* universe) override {
        SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 1);
        SDL_RenderClear(this->renderer);
        SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
        for(auto particle : universe->getParticles()){
            SDL_RenderDrawPoint(
                this->renderer,
                (particle.getPosition()[view.dimensions[0]] - view.corner[0]) * window_size[0] / view.size[0],
                (particle.getPosition()[view.dimensions[1]] - view.corner[1]) * window_size[1] / view.size[1]
            );
        }
        SDL_RenderPresent(this->renderer);

        // events handling so the window is responding
        this->handleEvents();
    }
};