#pragma once

#include <SDL.h>
#include "Object.hpp"
#include "ini.hpp"

class app {

    public:

        app();
        void start();
        bool getRunning();

    private:

        bool mRunning;

};


extern SDL_Renderer *gRenderer;
extern const int WIDTH;
extern const int HEIGHT;
extern Object arrow;
