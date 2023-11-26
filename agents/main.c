#include <math.h>
#include <complex.h>

#include "usdl.h"

typedef struct {
    float x, y;
} agent_t;

#define POOL_T agent_t
#include "pool.h"

char *APP_TITLE = "Agents Tech Demo";

static POOL_agent_t agents;
static float x = 0;

void update(uint32_t delta_ms)
{
    if (POOL_agent_t_len(&agents) < 1000) {
        agent_t agent = {0};
        agent.x = x;
        agent.y = x + 1.0;
        POOL_agent_t_add(&agents, agent);
    }
    POOL_agent_t_dump(&agents);
}

void update_event(SDL_Event e)
{
    x += 0.1;
}

enum draw_keepscreen draw(unsigned long frame)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    return 1;			// should_clear_screen
}
