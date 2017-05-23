//This is the example of sprite animation
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace std;

int main()
{
    ALLEGRO_DISPLAY *display;

    if(!al_init())
    {
        al_show_native_message_box(NULL,NULL,NULL, "Could not initialize Allegro 5", NULL, NULL);
        return -1;
    }

    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    if(!display)
    {
         al_show_native_message_box(NULL, NULL, NULL, "Could not create Allegro Window", NULL,NULL);
    }

    al_set_window_position(display, 200, 100);
    al_set_window_title(display, "CodingMadeEasy");
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();
    ALLEGRO_BITMAP *player = al_load_bitmap("player1.png");
    bool done = false, draw = true, active = false;
    float x = 10, y = SCREEN_HEIGHT - al_get_bitmap_height(player);
    float sourceX = 0, sourceY = 0;
    int moveSpeed = 5;
    float velX = 0.0f, velY = 0.0f;
    const float gravity = 1.0f;
    bool isOnTheGround = false;
    float jumpSpeedY = 100.0f;
    float jumpSpeedX = 20.0f;
    const float FPS = 60.0f;
    const float frameFPS = 15.0f;
    enum Direction {UP, LEFT, RIGHT};
    Direction dir = UP;
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TIMER *frameTimer = al_create_timer(1.0 / frameFPS);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(frameTimer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);
    al_start_timer(frameTimer);

    while(!done)
    {
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        al_get_keyboard_state(&keyState);

        if(events.type == ALLEGRO_EVENT_KEY_UP)
        {
               if(events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
               {
                   al_stop_timer(timer);
                   al_stop_timer(frameTimer);
                   done = true;
                   break;
               }
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_stop_timer(timer);
            al_stop_timer(frameTimer);
            done = true;
            break;
        }
        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            if(events.timer.source == timer)
            {
                active = true;
                if(al_key_down(&keyState, ALLEGRO_KEY_UP) && isOnTheGround)
                {
                    y -= jumpSpeedY;
                    x += jumpSpeedX;
                    dir = UP;
                    isOnTheGround = false;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                {
                    velX = moveSpeed;
                    dir = RIGHT;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                {
                    velX = -moveSpeed;
                    dir = LEFT;
                }
                else
                {
                    velX = 0;
                    active = false;
                }
            }
            else if(events.timer.source == frameTimer)
            {
                if(active)
                    ++sourceX;
                else
                    sourceX = 0;
                if(sourceX >= 2)
                    sourceX = 0;
                sourceY = dir;
                draw = true;
            }

            x += velX;
            if(!isOnTheGround)
                velY += gravity;
            else
                velY = 0;
            y += velY;
            isOnTheGround = (y >= SCREEN_HEIGHT - al_get_bitmap_height(player)) ; // Являемся ли игрок на земле
            if(isOnTheGround)
                y = SCREEN_HEIGHT - al_get_bitmap_height(player);
        }
        if(draw)
        {
            // draws bitmap region
            al_draw_bitmap_region(player, sourceX * al_get_bitmap_width(player) / 3, sourceY * al_get_bitmap_height(player) / 4,
                                  al_get_bitmap_width(player) / 3, al_get_bitmap_height(player) / 4, x, y, NULL);
            al_flip_display();
            al_clear_to_color(al_map_rgb(255,255,255));
        }
    }
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_timer(frameTimer);
    al_destroy_bitmap(player);
    al_destroy_event_queue(event_queue);

    return 0;
}
