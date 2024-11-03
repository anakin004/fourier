#pragma once

#include "ini.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

class texture {
    public:

        texture();
        ~texture();
        bool loadFromFile(std::string path);
        void free();
        void add( std::vector<double>&vals, const int N, bool isFirst  );
        void updatePositions();
        std::pair< float , float > getXY( int idx, bool isFirst );
        void render();
        int getWidth();
        int getHeight();

        // scale width from circle size 
        // void scale( float rad );
    
    private:
    
        std::pair< std::vector< std::tuple<SDL_FRect , SDL_FPoint , double , double> >  , std::vector< std::tuple< SDL_FRect , SDL_FPoint , double , double > > > mPositions;


        std::vector< std::tuple<double,double,double,double,double> > fourierY;
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};


extern SDL_Window *gWindow;
extern SDL_Renderer *gRenderer;
extern const int WIDTH;
extern const int HEIGHT;