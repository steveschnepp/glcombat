#include <math.h>
#include <complex.h>

#include "usdl.h"

typedef struct {
    float x, y;
} agent;

#define POOL_T agent
#include "pool.h"

char *APP_TITLE = "Agents Tech Demo";

void update(uint32_t delta_ms)
{
}

enum draw_keepscreen draw(unsigned long frame)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    return 1; // should_clear_screen
}
