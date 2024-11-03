#include "texture.hpp"
#include "math.hpp"
#include <tuple>

#define c 10
#define c2 1



texture::texture() : mPositions(), fourierY() {
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

texture::~texture() {
    free();
}

void texture::free()
{
    //Free texture if it exists
    if( mTexture != nullptr )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = nullptr;

    }
}

int texture::getWidth() {
    return mWidth;
}

int texture::getHeight() {
    return mHeight;
}



auto computeDimensions(float radius, float mHeight, float mWidth) {
    float width = (radius / mHeight) * mWidth;
    float height = static_cast<float>(radius);
    float centerX = width / 2.f;
    return std::make_tuple(width, height, centerX);
}

void texture::add( std::vector<double>&vals, const int N, bool isFirst  ){
    std::vector< std::tuple<double,double,double,double,double> > fourierY = dft(vals, N);
    static float time = 0;
    float x = 0;
    float y = 0;

        

    const float dt = 2*M_PI/fourierY.size();
    time += dt;

    for(int i = 0; i<N ; i++){
        double freq = std::get<2>(fourierY[i]);
        double radius = std::get<3>(fourierY[i])/c2;
        double phase = std::get<4>(fourierY[i]);

        // adding positions
        if (i > 0) {
            auto dims = computeDimensions(radius, mHeight, mWidth);
            std::pair<float, float> pos = getXY(i - 1, isFirst );
            SDL_FRect r = {pos.first, pos.second, std::get<0>(dims), std::get<1>(dims)};
            SDL_FPoint p = {std::get<2>(dims), std::get<1>(dims)};
            if(isFirst)
                mPositions.first.push_back(std::make_tuple(r, p, phase, freq/c ));
            else
                mPositions.second.push_back(std::make_tuple(r, p, phase, freq/c ));
        } else {
            
            auto dims = computeDimensions(radius, mHeight, mWidth);
            SDL_FRect r = {WIDTH / 2.f - 400, HEIGHT / 2.f + 100, std::get<0>(dims), std::get<1>(dims)};

            SDL_FPoint p = {std::get<2>(dims), std::get<1>(dims)};
            if(isFirst)
                mPositions.first.push_back(std::make_tuple(r, p, phase, freq/c ));
            else{
                r.x = WIDTH/2+40;
                r.y = HEIGHT/2 - HEIGHT/4 - HEIGHT/6;
                mPositions.second.push_back(std::make_tuple(r, p, 90.f, freq/c ));
            }
        }
    }
}

std::pair< float , float > texture::getXY( int idx, bool isFirst ){
    if(isFirst){
        double rad = (std::get<2>(mPositions.first[idx])+ 90.f)*M_PI/180.f;
        float radius = std::get<0>(mPositions.first[idx]).h;

        return{ (-1 * radius * cos(rad)) + std::get<1>(mPositions.first[idx]).x + std::get<0>(mPositions.first[idx]).x,
                (-1 * radius * sin(rad)) + std::get<1>(mPositions.first[idx]).y + std::get<0>(mPositions.first[idx]).y
            };
    }
    else{
        double rad = (std::get<2>(mPositions.second[idx])+ 90.f)*M_PI/180.f;
        float radius = std::get<0>(mPositions.second[idx]).h;

        return{ (-1 * radius * cos(rad)) + std::get<1>(mPositions.second[idx]).x + std::get<0>(mPositions.second[idx]).x,
                (-1 * radius * sin(rad)) + std::get<1>(mPositions.second[idx]).y + std::get<0>(mPositions.second[idx]).y
            };
    }

}

void texture::updatePositions(){
    //assumes minimum of 1 element in mPositions
    std::get<2>(mPositions.first[0]) += std::get<3>(mPositions.first[0]);
    std::get<2>(mPositions.second[0]) += std::get<3>(mPositions.second[0]);

    



    SDL_SetRenderDrawColor(gRenderer, 255, 0 , 0, 0);
    
    // the position of the arrows will be based on the previous arrow, we handle the first arrow seperately
    for( int i = 1; i<mPositions.first.size() ; i++ ){

            std::pair<float ,float> prevPos = getXY(i-1, true );

            float x = prevPos.first - std::get<0>(mPositions.first[i]).w/2;
            float y = prevPos.second - std::get<0>(mPositions.first[i]).h;
            std::get<0>(mPositions.first[i]).x = x;
            std::get<0>(mPositions.first[i]).y = y;
            std::get<2>(mPositions.first[i]) += std::get<3>(mPositions.first[i]);
            //std::get<2>(mPositions[i])+=std::get<3>(mPositions[i]);

    }

    for( int i = 1; i<mPositions.second.size() ; i++ ){

            std::pair<float ,float> prevPos = getXY(i-1, false);

            float x = prevPos.first - std::get<0>(mPositions.second[i]).w/2;
            float y = prevPos.second - std::get<0>(mPositions.second[i]).h;
            std::get<0>(mPositions.second[i]).x = x;
            std::get<0>(mPositions.second[i]).y = y;
            std::get<2>(mPositions.second[i]) += std::get<3>(mPositions.second[i]);
            //std::get<2>(mPositions[i])+=std::get<3>(mPositions[i]);

    }

}


void texture::render(){
    
    
    static std::vector<float> draw;

    // redering first fourier
    for ( int iter = 0; iter<mPositions.first.size(); iter++) {


        SDL_FRect thisArrowPos = std::get<0>(mPositions.first[iter]);
        SDL_FPoint thisCenter = std::get<1>(mPositions.first[iter]);
        double angle = std::get<2>(mPositions.first[iter]);

        

        SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 0);
        SDL_RenderCopyExF(gRenderer, mTexture, NULL, &thisArrowPos, angle , &thisCenter , SDL_FLIP_NONE);


        if(iter==mPositions.first.size()-1){
            std::pair<float,float> pos = getXY( iter, true );
            float temp = pos.first;
            draw.push_back(pos.second);
        }
        
    
    } 

    //second fourier
    for ( int iter = 0; iter<mPositions.second.size(); iter++) {


        SDL_FRect thisArrowPos = std::get<0>(mPositions.second[iter]);
        SDL_FPoint thisCenter = std::get<1>(mPositions.second[iter]);
        double angle = std::get<2>(mPositions.second[iter]);

        

        SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 0);
        SDL_RenderCopyExF(gRenderer, mTexture, NULL, &thisArrowPos, angle , &thisCenter , SDL_FLIP_NONE);


        if(iter==mPositions.second.size()-1){
            std::pair<float,float> pos = getXY(iter, false);
            draw.push_back(pos.first);
        }
        
    }

    //            SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 30);
    //        SDL_RenderDrawLineF(gRenderer, temp, pos.second, WIDTH/2.f+250, pos.second );

    SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 0);
    for(int j = 1 ; j<draw.size(); j+=2){
        SDL_RenderDrawPointF(gRenderer, draw[j] ,draw[j-1]);
    }

}



bool texture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

    SDL_Surface* src = SDL_GetWindowSurface( gWindow );

    SDL_Surface* optimized = nullptr;

    if( loadedSurface == nullptr )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        optimized = SDL_ConvertSurface ( loadedSurface, src->format, 0 );
        
        if( optimized == nullptr ){
            printf("Cannot optimize");
        }
        else{

            Uint32 pixelColor = SDL_MapRGBA(optimized->format, 0, 0, 0, 255);
            SDL_SetColorKey( optimized, SDL_TRUE, pixelColor);

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface( gRenderer, optimized );

            

            if( newTexture == nullptr )
            {
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }

            else
            {
                //Get image dimensions
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
            SDL_FreeSurface( optimized );

        }
    }

    //Return success
    mTexture = newTexture;
    return mTexture != nullptr;
}


