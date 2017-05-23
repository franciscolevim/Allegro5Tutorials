//This is the example of camera effects

#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <cmath>
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace std;

void cameraUpdate(float *cameraPosition, float x, float y, float w, float h)
{
    cameraPosition[0] = -(SCREEN_WIDTH / 2) + x + w / 2;
    cameraPosition[1] = -(SCREEN_HEIGHT / 2) + y + h / 2;

    if(cameraPosition[0] < 0)
        cameraPosition[0] = 0;
    if(cameraPosition[1] < 0)
        cameraPosition[1] = 0;
}

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
    //float sourceX = 0, sourceY = 0;
    const float FPS = 60.0;
    float degrees = 0.0f;
    float scale = 1.0f;
    enum Direction {DOWN, LEFT, RIGHT, UP};
    Direction dir = DOWN;
    //Создаем вектор для танка в разных направлениях
    vector<ALLEGRO_BITMAP*> tank;
    tank.push_back(al_load_bitmap("tank_down.png"));
    tank.push_back(al_load_bitmap("tank_left.png"));
    tank.push_back(al_load_bitmap("tank_right.png"));
    tank.push_back(al_load_bitmap("tank_up.png"));
    //Создаем фон
    ALLEGRO_BITMAP *background = al_load_bitmap("wood.png");

    ALLEGRO_TRANSFORM camera; // Создаем камеру
    float cameraPosition[2] = {0,0}; // Массив из двух чисел - x и y камеры

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);

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
                   done = true;
                   break;
               }
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_stop_timer(timer);
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
                    // поворот карты по часовой
                    degrees += 0.5f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_S))
                {
                    // поворот карты против часовой
                    degrees -= 0.5f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_I))
                {
                    //zoom in
                    scale += 0.05f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_D))
                {
                    //zoom out
                    scale -= 0.05f;
                }
                else
                {
                    active = false;
                }

                cameraUpdate(cameraPosition, x, y, al_get_bitmap_width(tank[0]), al_get_bitmap_height(tank[0]));
                al_identity_transform(&camera);
                //Это все для вращения/////////////////////////
                //Camera roating around window center
                //Here we translate the backgroung image to the window center
                al_translate_transform(&camera, -(x + al_get_bitmap_width(tank[0]) / 2) ,
                        -(y + al_get_bitmap_height(tank[0]) / 2));
                //Here we rotate and scale camera
                al_rotate_transform(&camera, degrees * M_PI / 180);
                al_scale_transform(&camera, scale, scale);
                // Here we translate the backgroung image back
                al_translate_transform(&camera, -cameraPosition[0] + x + al_get_bitmap_width(tank[0]) / 2, -cameraPosition[1] + y + al_get_bitmap_height(tank[0]) / 2);
                //////////////////////////////////
                //А это для перемещения камерв
                // Вместо перемещения всех объектов вправо мы перемещаем камеру влево
                //al_translate_transform(&camera, -cameraPosition[0] + cameraPosition[0], -cameraPosition[1] + cameraPosition[1]);
                al_use_transform(&camera);
            }
        }
        if(draw)
        {
            al_draw_bitmap(background, 0, 0, NULL);
            al_draw_bitmap(tank[dir], x, y, NULL);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }
    al_destroy_display(display);
    al_destroy_timer(timer);
    for(auto &t: tank)
        al_destroy_bitmap(t);
    al_destroy_bitmap(background);
    al_destroy_event_queue(event_queue);

    return 0;
}
