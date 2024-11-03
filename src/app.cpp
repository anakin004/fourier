#include "app.hpp"
#include "ini.hpp"
#include "math.hpp"
#include <iomanip>
#include <iostream>
#include <SDL.h>
#include <tuple>
#include <random>

const int FPS = 144;


// Function to generate a random double between min and max
double random(double min, double max) {
    static std::random_device rd;  // Obtain a random number from hardware
    static std::mt19937 gen(rd()); // Seed the generator
    std::uniform_real_distribution<> dis(min, max); // Define the range

    return dis(gen);
}

app::app(){
    mRunning = true;
}

void app::start(){

    if(!init() || !loadMedia())
        return;

    SDL_Event e;
    Uint32 StartTicks;
    Uint32 EndTicks;

    int size = 100;
    std::vector<double> b;
    b.reserve(size); // Preallocate memory for size elements
    for(int i = 0; i<size ; i++)
        b.push_back(random(-2,2));
    std::vector<double> a;
    a.reserve(size);
    for(int i = 0; i<size ; i++)
        a.push_back(random(2,3));
    

    arrow.add(a,a.size(), true);
    arrow.add(b,b.size(), false);
    


    while(mRunning){
      
        StartTicks = SDL_GetTicks();

        while(SDL_PollEvent( &e )){
            if( e.type == SDL_QUIT )
                mRunning = false;

        }

        
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
        SDL_RenderClear(gRenderer);
        // render handles angle changes
        arrow.render();
        arrow.updatePositions();
        SDL_RenderPresent(gRenderer);

        Uint32 RanTicks = SDL_GetTicks() - StartTicks;

        // 1 frame / 144 f/s gets you some decimal like .006 seconds
        // .006 seconds 
                                    // time per frame
        if( RanTicks < 1000/144.f ) 
            SDL_Delay( 1000/144.f - RanTicks);

    }

    arrow.free();
    SDL_Quit();
}

bool app::getRunning(){
    return mRunning;
}