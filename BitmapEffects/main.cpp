//This is the example of bitmap effects
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <cmath>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
//#define DELAY 0.1

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

    bool done = false, draw = true, active = false;
    float x = 10, y = 10;
    int moveSpeed = 5;
    float sourceX = 0, sourceY = 0;
    const float FPS = 60.0;
    float degrees = 0.0f, scale = 1.0f;
    const float frameFPS = 15.0;
    enum Direction {DOWN, LEFT, RIGHT, UP};
    Direction dir = DOWN;
    ALLEGRO_BITMAP *player = al_load_bitmap("player1.png");
    ALLEGRO_BITMAP *player2 = al_clone_bitmap(player); // Bitmap clone
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
                if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
                {
                    y += moveSpeed;
                    dir = DOWN;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_UP))
                {
                    y -= moveSpeed;
                    dir = UP;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                {
                    x += moveSpeed;
                    dir = RIGHT;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                {
                    x -= moveSpeed;
                    dir = LEFT;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_W))
                {
                    ++degrees;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_S))
                {
                    --degrees;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_I))
                {
                    scale += 0.1f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_D))
                {
                    scale -= 0.1f;
                }
                else
                {
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
            if(degrees == 360 || degrees == -360)
                degrees = 0;
        }
        if(draw)
        {
            ALLEGRO_BITMAP *subBitmap = al_create_sub_bitmap(player, sourceX * al_get_bitmap_width(player) / 3,
            sourceY * al_get_bitmap_height(player) / 4, al_get_bitmap_width(player) / 3, al_get_bitmap_height(player) / 4);
            // So we can draw roated bitmaps
            al_draw_rotated_bitmap(subBitmap, al_get_bitmap_width(subBitmap) / 2,
            al_get_bitmap_height(subBitmap) / 2, x, y, degrees * M_PI / 180, NULL);
            //Draw tinted bitmap
            //al_draw_bitmap_region(player2, 0,0,al_get_bitmap_width(player2) / 3, al_get_bitmap_height(player2) / 4, 400,400, NULL);
            al_draw_tinted_bitmap(subBitmap, al_map_rgba(255,255,0,100), 200,200, NULL); // Transparen image
            al_draw_tinted_bitmap(subBitmap, al_map_rgba(0,255,0,255), 300,300, NULL); //Draw tinted bitmap without transparency
            al_draw_tinted_bitmap(subBitmap, al_map_rgba(255,0,0,255), 400,400, NULL); //Draw tinted bitmap without transparency
            // Here we draw scaled bitmap
            al_draw_scaled_bitmap(subBitmap, 0, 0,  al_get_bitmap_width(subBitmap),
            al_get_bitmap_height(subBitmap), 500, 200, al_get_bitmap_width(subBitmap) * scale,
            al_get_bitmap_height(subBitmap) * scale, NULL);

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
            al_destroy_bitmap(subBitmap);
        }
    }
    //cout << "Libramos la memoria dianmica";
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_timer(frameTimer);
    al_destroy_bitmap(player);
    al_destroy_bitmap(player2);
    al_destroy_event_queue(event_queue);

    return 0;
}
