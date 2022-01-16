//
// Created by Adam HAISSOUBI-VOGIER on 15/01/2022.
//

#include "Fenetre.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/Headers/SDL.h"
#define TITRE "Puissance4"

int creerFenetre()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = NULL;
    SDL_Renderer *renderer = NULL;
    int statut = EXIT_FAILURE;

    if(0 != SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    window = SDL_CreateWindow(TITRE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 700, 0);

    if(NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 239, 237, 237, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    if(NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    bool is_running = true;
    SDL_Event event;
    while (is_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
                goto Quit;
            }
        }
        SDL_Delay(16);
    }

    Quit:
        if(NULL != renderer)
            SDL_DestroyRenderer(renderer);
        if(NULL != window)
            SDL_DestroyWindow(window);
        SDL_Quit();
        return statut;
}