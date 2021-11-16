#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <cstdlib>
#include <iostream>

#define WIDTH 200
#define HEIGHT 200
#define PIXEL_SIZE 2
#define WRAP_SIZE 256

using namespace std;


struct Vec2
{
    float x, y;
};

float DotProduct(const Vec2 &v1, const Vec2 &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}


class PerlinNoise2D
{
    private:
        vector<Vec2> _gradients;
    public:
        void init(int seed = 0)
        {
            // set random seed
            srand(seed);

            Vec2 grads[4] = {
                {1.0, 0.0},
                {-1.0, 0.0},
                {0.0, 1.0},
                {0.0, -1.0},
            };


            //generate gradient grid
            _gradients.reserve(WRAP_SIZE * WRAP_SIZE);
            for(int i = 0; i < WRAP_SIZE * WRAP_SIZE; i++)
            {
                _gradients[i] = grads[rand() % 4];
            }
        }

        float value(float x, float y)
        {
            //integer part
            int ix = (int)x;
            int iy = (int)y;
            //fraction part
            float fx = x - ix;
            float fy = y - iy;
            
            //wrap arround
            ix = ix % WRAP_SIZE;
            iy = iy % WRAP_SIZE;


            //get nearest gradients
            Vec2 pTL = _gradients[iy * WRAP_SIZE + ix];
            Vec2 pTR = _gradients[iy * WRAP_SIZE + ix + 1];
            Vec2 pBL = _gradients[(iy + 1) * WRAP_SIZE + ix];
            Vec2 pBR = _gradients[(iy + 1) * WRAP_SIZE + ix + 1];

            //find distance from the gradients
            Vec2 dTL = {fx, fy};
            Vec2 dTR = {fx - 1, fy};
            Vec2 dBL = {fx, fy - 1};
            Vec2 dBR  = {fx - 1, fy - 1};

            //calculate dot prodect for each corner 
            float fTL = DotProduct(pTL, dTL);
            float fTR = DotProduct(pTR, dTR);
            float fBL = DotProduct(pBL, dBL);
            float fBR = DotProduct(pBR, dBR);

            
            auto lerp = [](float &a, float &b, float f){ return b * f + a * (1 - f); };
            //6t^5 -15t^4 +10t^3
            auto smooth = [](float &t) { return t * t * t * ( t *(t * 6 - 15)  + 10); }; 

            fx = smooth(fx);
            fy = smooth(fy);
            //interpotate
            float fL = lerp(fTL, fBL, fy);
            float fR = lerp(fTR, fBR, fy);

            float val = lerp(fL, fR, fx); // range (-1 -> 1)
            
            //shift range 0 -> 1
            return (val + 1.0) /2.0;

        }

};

class Demo : public olc::PixelGameEngine
{
    public:
        PerlinNoise2D noise;

        float offsetX, offsetY;
        float scale;
        Demo()
        {
        }

        bool OnUserCreate()
        {
            noise.init(2);
            offsetX = 0.0f;
            offsetY = 0.0f;
            scale = 3.0;
            return true;
        }

        bool OnUserUpdate(float dt)
        {
            float delta = 1;

            if(GetKey(olc::Key::LEFT).bHeld) offsetX -= delta;
            if(GetKey(olc::Key::RIGHT).bHeld) offsetX += delta;
            if(GetKey(olc::Key::UP).bHeld) offsetY -= delta;
            if(GetKey(olc::Key::DOWN).bHeld) offsetY += delta;

            if(GetMouseWheel() > 0) scale -= delta;
            if(GetMouseWheel() < 0) scale += delta;



            for(int y = 0; y < HEIGHT; y++)
                for(int x = 0; x < WIDTH; x++)
                {
                    float x1 = (x * scale + offsetX)/ WIDTH;
                    float y1 = (y * scale  + offsetY)/ HEIGHT; 
                    int v = noise.value(x1, y1) * 255;
                    Draw(x,y, olc::Pixel(v, 0,0));

                }
            return true;
        }

};


int main(int argc, char *argv[])
{
    Demo demo;
    if(demo.Construct(WIDTH, HEIGHT, PIXEL_SIZE, PIXEL_SIZE))
    {
        demo.Start();
    }
    return 0;
}


