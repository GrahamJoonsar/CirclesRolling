#include "tigr.h"
#include <iostream>
#include <cmath>

const TPixel WHITE = tigrRGB(255, 255, 255);

class Slider{
    private:
        static bool selected;
        bool this_selected;
        int x, y, w;
        double min, max;
        double step;
        double current;
        TPixel BG;
        TPixel KNOB_COLOR;

        double clamp(double c){
            if (c < min) return min;
            if (c > max) return max;
            return c;
        }

        double roundToStep(double c){
            double less = (int)(c/step) * step;
            double more = (int)(c/step + 1) * step;
            return more - c < c - less ? more : less;
        }

    public:
        Slider(int x, int y, int w, double min, double max, double step){
            this->x = x;
            this->y = y;
            this->w = w;

            this->max = max;
            this->min = min;
            this->step = step;

            this->current = roundToStep((min+max)/2);
            this_selected = false;

            BG = tigrRGB(150, 150, 150);
            KNOB_COLOR = tigrRGB(200, 200, 255);
        }

        void draw(Tigr * screen){
            tigrFill(screen, x+2, y+1, w-2, 9, BG);
            tigrFillCircle(screen, x+(w*((current-min)/(max-min))), y+5, 7, KNOB_COLOR);
        }

        void update(Tigr * screen){
            int mx, my, buttons;
            tigrMouse(screen, &mx, &my, &buttons);
            if (buttons & 1){
                if ((x < mx) && (mx < x+w) && (y < my) && (my < y+9) && !selected){
                    selected = this_selected = true;
                }
            } else selected = this_selected = false;

            if (this_selected) current = roundToStep(clamp(((mx-x) / (double)w) * (max-min) + min));
        }

        inline double getVal() const {
            return current;
        }

};

class CheckBox{
    private:
        int x, y, sideLength;
        bool selected;
        TPixel BG;
        TPixel SELECTED_COLOR;
    public:
        CheckBox(int x, int y, int sideLength){
            this->x = x;
            this->y = y;
            this->sideLength = sideLength;
            this->BG = tigrRGB(200, 200, 200);
            this->SELECTED_COLOR = tigrRGB(50, 255, 128);
            selected = false;
        }

        void draw(Tigr * screen){
            tigrFill(screen, x, y, sideLength, sideLength, BG);
            if (selected) tigrFill(screen, x+2, y+2, sideLength-4, sideLength-4, SELECTED_COLOR);
        }

        void update(Tigr * screen){
            static bool selectedLastFrame = false;
            int mx, my, buttons;
            tigrMouse(screen, &mx, &my, &buttons);

            if ((x < mx) && (mx < x+sideLength) && (y < my) && (my < y+sideLength) && buttons&1 && !selectedLastFrame){
                selected = !selected;
            }

            selectedLastFrame = buttons&1;
        }

        inline bool getVal() const{
            return selected;
        }

};

const TPixel RED = tigrRGB(255, 0, 0);
const TPixel BLUE = tigrRGB(0, 0, 255);
const TPixel GREEN = tigrRGB(0, 255, 0);
const TPixel BLACK = tigrRGB(0, 0, 0);

#define PI 3.1415926
#define TAU 6.2831853

bool Slider::selected = false;
int main(void){
    Tigr * screen = tigrWindow(640, 320, "Rolling Circles", 0);

    Slider centerRad(10, 10, 200, 10, 110, 1);
    Slider sideRad(10, 25, 200, 10, 110, 1);
    Slider angleSlider(10, 40, 200, 0, TAU, PI/100);

    CheckBox cb(10, 60, 15);

    double angle = 0;

    while (!tigrClosed(screen)){
        tigrClear(screen, WHITE);

        centerRad.draw(screen);
        centerRad.update(screen);
        sideRad.draw(screen);
        sideRad.update(screen);
        angleSlider.draw(screen);
        angleSlider.update(screen);

        cb.draw(screen);
        cb.update(screen);

        const double crad = centerRad.getVal();
        const double srad = sideRad.getVal();

        double endx;
        double endy;

        angle = angleSlider.getVal();

        tigrCircle(screen, 320, 160, crad, RED);
        tigrCircle(screen, endx=cos(angle)*(crad + srad)+320,
                    endy=sin(angle)*(crad + srad)+160, srad, BLUE);

        tigrLine(screen, endx, endy, endx + cos(angle+angle)*srad, endy + sin(angle+angle)*srad, BLACK);

        /*if (cb.getVal()){
            int px = cos(0)*crad + cos(0)*srad + 320;
            int py = sin(0)*crad + sin(0)*srad + 160;
            for (double i = PI/10; i < TAU; i+=PI/10){
                int cx = cos(i)*crad + cos(i+i)*srad + 320;
                int cy = sin(i)*crad + sin(i+i)*srad + 160;
                tigrLine(screen, px, py, cx, cy, GREEN);
                px = cx;
                py = cy;
            }
        }*/

        tigrUpdate(screen);
    }
}