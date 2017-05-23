//This is the example of sprite animation and putting sound effects to our program
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <iostream>
#include <sstream>
#include <vector>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define DELAY 0.1

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
    al_install_audio();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_mouse();

    bool done = false, draw = true, active = false;
    float x = 10, y = 10;
    int moveSpeed = 5;
    enum Direction {DOWN = 0, LEFT = 3, RIGHT = 6, UP = 9};
    int index = 0;
    Direction dir = DOWN, prevDir;
    vector<ALLEGRO_BITMAP*> playerWalk;
    for(int i = 1; i <= 12; ++i)
    {
        stringstream str;
        str << "Sprites/" << i << ".png";
        playerWalk.push_back(al_load_bitmap(str.str().c_str()));
    }
    /*for(auto &bitmap : playerWalk)
        al_convert_mask_to_alpha(bitmap, al_map_rgb(255,255,255));*/
    ALLEGRO_SAMPLE *soundEffect = al_load_sample("Sounds/sound.wav"); // loads sound effect
    al_reserve_samples(2);
    ALLEGRO_SAMPLE *song = al_load_sample("Sounds/song.ogg"); // Loads background music
    ALLEGRO_SAMPLE_INSTANCE *songInstance = al_create_sample_instance(song);
    al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());
    ALLEGRO_TIMER *timer = al_create_timer(DELAY);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_play_sample_instance(songInstance);
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
            //break;
        }
        else if(events.type == ALLEGRO_EVENT_TIMER)
        {
            al_get_keyboard_state(&keyState);
            active = true;
            prevDir = dir;
            if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
            {
                y += moveSpeed;
                dir = DOWN;
                al_play_sample(soundEffect, 1.0f, 0.0f, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
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
            else
            {
                active = false;
            }
            if(active)
            {
                ++index;
            }
            else
            {
                index = dir + 1;
            }
            if(index >= dir + 3)
                index = dir;
            if(dir != prevDir)
                index = dir;
            draw = true;
        }
        if(draw)
        {
            al_draw_bitmap(playerWalk[index],x,y, NULL);
            al_flip_display();
            al_clear_to_color(al_map_rgb(255,255,255));
        }
    }
    for(int i = 0; i < int(playerWalk.size()); ++i)
    {
        al_destroy_bitmap(playerWalk[i]);
    }
    al_destroy_sample(soundEffect);
    al_destroy_sample(song);
    al_destroy_sample_instance(songInstance);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
