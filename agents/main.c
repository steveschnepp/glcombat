#include <math.h>
#include <complex.h>

#include "usdl.h"

typedef struct {
    float x, y;
} agent_t;

#define POOL_T agent_t
#include "pool.h"

char *APP_TITLE = "Agents Tech Demo";

void update(uint32_t delta_ms)
{
    if (POOL_agent_t_len < 1000) {

    }

}

void update_event(SDL_Event e)
{

}

enum draw_keepscreen draw(unsigned long frame)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    return 1;			// should_clear_screen
}
