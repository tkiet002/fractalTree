#include <SDL2/SDL.h>
#include <cmath>

class Application
{
private:
    int windows_height;
    int windows_width;

    SDL_Window *windows;
    SDL_Renderer *renderer;
    SDL_Event event;

    int branchLength = 125;
    int branchSpreadAngle = 35;
    int minBranchLength = 15;

public:
    void init()
    {
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
        {
            SDL_Init(SDL_INIT_VIDEO);
        }
        this->windows = SDL_CreateWindow("SDL Fractal Tree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->windows_width, this->windows_height,SDL_WINDOW_OPENGL);
        this->renderer = SDL_CreateRenderer(this->windows, -1, SDL_RENDERER_ACCELERATED);
    }
    void RenderTree(int startX, int startY, double brLength, int branchAngle)
    {
        float brAngleInRadian = (branchAngle * M_PI) / 180;
        int endX = startX - brLength * cos(brAngleInRadian); //
        int endY = startY - brLength * sin(brAngleInRadian); //minus because y is the highest height from 0
        SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
        if (brLength >= minBranchLength)
        {
            
            brLength = brLength * 0.8;

            RenderTree(endX, endY, brLength, branchAngle + branchSpreadAngle);
            RenderTree(endX, endY, brLength, branchAngle - branchSpreadAngle);
        }
    }
    void quit()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(windows);
        SDL_Quit();
        exit(0);
    }
    void RenderFrame()
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        RenderTree(windows_width / 2, windows_height, branchLength, 90);
        SDL_RenderPresent(renderer);
    }

    void run()
    {
        init();
        RenderFrame();
        while (true)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    quit();
                    break;
                }
            }
            
        }
        SDL_Delay(100);
        
    }
    Application(int windowHeight, int windowWidth)
    {
        this->windows_height = windowHeight;
        this->windows_width = windowWidth;
    }
};

int main(int argc, char const *argv[])
{
    Application app(900, 900);
    app.run();
    return 0;
}

