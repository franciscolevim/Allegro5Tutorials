//This example contnues the sprite animation theme
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define DELAY 0.1

using namespace std;

int main()
{
    // Here we initialize allegro5 library
    if(!al_init())
    {
        al_show_native_message_box(NULL,NULL,NULL, "Could not initialize Allegro 5", NULL, NULL);
        return -1;
    }

    //Here we create window
    ALLEGRO_DISPLAY *display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

    if(!display)
    {
         al_show_native_message_box(NULL, NULL, NULL, "Could not create Allegro Window", NULL,NULL);
    }

    al_set_window_position(display, 200, 100);
    al_set_window_title(display, "CodingMadeEasy");

    al_init_image_addon();
    al_install_keyboard();
    al_install_mouse();

    bool done = false, draw = true, active = false;
    float x = 10, y = 10;
    int moveSpeed = 5;
    int imageNumber = -1;
    bool startAnimation = false;

    //Here we create our player

    ALLEGRO_BITMAP* player = al_load_bitmap("Sprites/superMan1.png");
    ALLEGRO_BITMAP* tank = al_load_bitmap("Sprites/tank2.png");

    //Here we declare two vectors
    vector<int> source;
    vector<int> width;

    source.push_back(0);

    ALLEGRO_COLOR pixel, color;
    ALLEGRO_COLOR lastPixel = al_get_pixel(player, 0, 0);

    for(int i = 0; i < al_get_bitmap_width(player); ++i)
    {
        // Here we identify a red color using alpha-channel
        color = al_map_rgba(255,0,0,255);
        pixel = al_get_pixel(player, i, 0);

        if(memcmp(&pixel,&lastPixel, sizeof(ALLEGRO_COLOR)))
        {
            if(!memcmp(&pixel, &color, sizeof(ALLEGRO_COLOR)))
            {
                //If we detect red pixel we will push current index i to the source vector
                source.push_back(i);
                if(source.size() == 2)
                    width.push_back(i);
                else
                    width.push_back(i - width[width.size() - 1]);
            }
        }
        else if(i == al_get_bitmap_width(player) - 1)
        {
            cout << "Достигли конца картинки";
            width.push_back(i - width[width.size() - 1]);
        }

        lastPixel = pixel;
    }
    al_convert_mask_to_alpha(player, al_map_rgba(255,0,0,255));
    //al_convert_mask_to_alpha(player, al_map_rgba(255,255,255,255));
    ALLEGRO_TIMER *timer = al_create_timer(DELAY);
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
        if(events.type == ALLEGRO_EVENT_KEY_UP)
        {
               if(events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
               {
                   done = true;
                   break;
               }
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
           //al_stop_timer(timer);
            done = true;
            break;
        }
        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            draw = true;
            active = true;
            al_get_keyboard_state(&keyState);
            if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
            {
                y += moveSpeed;
            }
            else if(al_key_down(&keyState, ALLEGRO_KEY_UP))
            {
                y -= moveSpeed;
            }
            else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
            {
                x += moveSpeed;
            }
            else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
            {
                x -= moveSpeed;
            }
            else if(al_key_down(&keyState, ALLEGRO_KEY_A) && !startAnimation)
            {
                startAnimation = true;
            }
            else
                active = false;
        }

        if(startAnimation)
        {
            ++imageNumber;
            if(imageNumber >= source.size())
            {
                imageNumber = -1;
                startAnimation = false;
            }

        }

        draw = true;

        if(draw)
        {
            if(!startAnimation)
                al_draw_bitmap(player,x,y, NULL);
            else
                al_draw_bitmap_region(player, source[imageNumber], 0, width[imageNumber], al_get_bitmap_height(player), x, y, NULL);
            al_draw_bitmap(tank, 200,200,NULL);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }
    al_draw_bitmap(player,x,y, NULL);
    al_flip_display();
    al_destroy_bitmap(player);
    al_destroy_bitmap(tank);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
