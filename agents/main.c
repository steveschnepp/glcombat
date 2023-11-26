#include <math.h>
#include <complex.h>

#include "usdl.h"

char *APP_TITLE = "Agents Tech Demo";

int should_update = 0;
float power = 1.0;


void update(uint32_t delta_ms)
{
}

enum draw_keepscreen draw(unsigned long frame)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    return 1; // should_clear_screen
}
