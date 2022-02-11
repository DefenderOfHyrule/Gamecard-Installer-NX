#include <stdint.h>
#include <stdbool.h>
#include <switch.h>

#include "nx/input.h"


bool g_touch_lock = false;
PadState padState = { 0 };
HidTouchScreenState hidState = { 0 };

void init_input(void)
{
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&padState);
}

input_t get_input(void)
{
    padUpdate(&padState);
    hidGetTouchScreenStates(&hidState, 1);

    input_t input = { 0 };
    input.down = padGetButtonsDown(&padState);
    input.held = padGetButtons(&padState);
    input.t_count = hidState.count;
    if (input.t_count) {
        input.t_pos = hidState.touches[0];
    }
    return input;
}

bool is_touch(void)
{
    input_t input = get_input();
    return input.t_count > 0;
}

int check_if_touch_yesno(const input_t *input)
{
    if (input->t_count)
    {
        for (uint16_t i = 0, x = 255; !g_touch_lock && i < 2; i++, x += 770 / 2)
        {
            if (input->t_pos.x > x && input->t_pos.x < x + (770 / 2) && input->t_pos.y > 210 + 295 - 72 && input->t_pos.y < 210 + 295)
            {
                g_touch_lock = true;
                return i;
            }
        }
        g_touch_lock = true;
    }
    else
    {
        g_touch_lock = false;
    }
    return -1;
}

bool check_if_touch_error(void)
{
    input_t input = get_input();
    if (input.t_count)
    {
        if (!g_touch_lock && input.t_pos.x > 455 && input.t_pos.x < 455 + 365 && input.t_pos.y > 470 && input.t_pos.y < 470 + 65)
            return true;
        g_touch_lock = true;
    }
    else
    {
        g_touch_lock = false;
    }

    if ((input.down & HidNpadButton_A || input.down & HidNpadButton_B))
        return true;
    return false;
}

int check_if_option(const input_t *input)
{
    if (input->t_count)
    {
        for (uint16_t i = 0, y = 300; !g_touch_lock && i < 3; i++, y += 125)
        {
            if (input->t_pos.x > 475 && input->t_pos.x < 475 + 720 && input->t_pos.y > y && input->t_pos.y < y + 70)
            {
                g_touch_lock = true;
                return i;
            }
        }
        g_touch_lock = true;
    }
    else
    {
        g_touch_lock = false;
    }
    return -1;
}

uint32_t move_cursor_up(uint32_t cursor, uint32_t cursor_max)
{
    return cursor == 0 ? cursor_max - 1 : --cursor;
}

uint32_t move_cursor_down(uint32_t cursor, uint32_t cursor_max)
{
    return cursor == cursor_max - 1 ? 0 : ++cursor;
}