#include <SDL2/SDL.h>
#include <cmath>
class Application
{
private:
    int window_height;
    int window_width;
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Event event;

    int branchLength = 125;
    int branchMinLength = 15;
    int branchSpreadAngle = 35;
    double branchLengthShorterMul = 0.8; // make branch shorter by multiple t this variable
    void testWindows(int x, int y)
    {
        printf("%f\n", &x);
        printf("%f\n", &y);
    }

public:
    void init()
    {

        if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
            SDL_Init(SDL_INIT_VIDEO);
        this->window = SDL_CreateWindow("Fractal Tree", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
        this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }
    //grow up animation
    void initAnimation()
    {
        /*So for someone wonder it working as
         it's increasing the branch length and start drawing
         when the branch length reach 15(the min branch)
          it start working
        */
        int initBranchLength = this->branchLength;
        for (branchLength = 0; branchLength < initBranchLength; ++branchLength)
        {
            renderFrame();
            SDL_Delay(25);
        }
    }

    void quit()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(0);
    }
    void renderTree(int startX, int startY, double brLength, double brAngle)
    {
        testWindows(startX,startY);
        double angleInRadian = (brAngle * M_PI) / 180.0; // change to radian because cos and sin only working with radian (stoopid comment?!)
        int endX = startX - brLength * cos(angleInRadian);
        int endY = startY - brLength * sin(angleInRadian);

        //startX or startY start from the bottom becasue it slowly decrease from the bottom (here is 900) of the
        // windows to 0 to calculate the height or width of it
        //I know I should read the document (T.T), i know I stoopid.
        ///already depressed                           
        SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
        if (brLength >= branchMinLength)
        {
            brLength = brLength * branchLengthShorterMul;
            renderTree(endX, endY, brLength, brAngle - branchSpreadAngle);
            
            //         root               
            renderTree(endX , endY , brLength, brAngle + branchSpreadAngle);
        }
    }

    void renderFrame()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // actually don't need this pretty much because the background is black already
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);            // draw line
        renderTree(window_width / 2, window_height/1.12, branchLength, 90); // 90 degree
        SDL_RenderPresent(renderer);
    }

    void processInput1()
    {
        switch (this->event.key.keysym.sym)
        {
        case SDLK_UP:
            this->branchLength +=5;
            this->event.key.keysym.sym =false;
            break;
        
        case SDLK_DOWN:
            this->branchLength -=5;
            break;
        case SDLK_RIGHT:
            this->branchSpreadAngle +=1;
            break; 
        case SDLK_LEFT:
            this->branchSpreadAngle -=1;
            break;        
        }
        
        renderFrame();
        
    }
    void eventLoop()
    {
        while (true)
        {
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    quit();
                    break;
                case SDL_KEYDOWN:
                    processInput1();
                    break;
                }
            }

            SDL_Delay(100);
        }
    }
    void run()
    {
        init();
        initAnimation();
        eventLoop();
    }
    Application(int windowHeight, int windowsWidth)
    {
        this->window_height = windowHeight;
        this->window_width = windowsWidth;

    }
};

int main(int argc, char const *argv[])
{
    Application app(900, 900);
    app.run();
    return 0;
}
