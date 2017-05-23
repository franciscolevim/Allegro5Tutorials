#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define DELAY 20

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
   // int value = al_show_native_message_box(display, "MessageBox Title", "Question", "Did you visit youtube.com?",NULL,ALLEGRO_MESSAGEBOX_QUESTION | ALLEGRO_MESSAGEBOX_YES_NO);
    //al_rest(1.0);
    // cout << value << endl;

    al_set_window_position(display, 200, 100);
    al_set_window_title(display, "CodingMadeEasy");
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

    //al_init_font_addon();
    //al_init_ttf_addon();
    al_init_image_addon();
    al_init_primitives_addon();
    al_install_keyboard();
    al_install_mouse();

    // Draw text

    //ALLEGRO_FONT *font = al_load_font("DroidSansMono.ttf", 48, NULL);
    //al_draw_text(font, al_map_rgb(44,117,225),SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "THIS IS TEXT");

    ALLEGRO_COLOR electricBlue = al_map_rgb(44,117,255);
    ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
    ALLEGRO_COLOR playerColor = electricBlue;
    //float points[8] = {0,0,400,100,50,200, SCREEN_WIDTH, SCREEN_HEIGHT};

    //Draw primitives

    //al_draw_spline(points, al_map_rgb(255,0,0), 5);
    //al_draw_line(0,0,200,100, al_map_rgb(0,255,0),10);
    //al_draw_circle(150,150, 50, electricBlue,15);
    //al_draw_filled_ellipse(500,100,60,40, al_map_rgb(255,255,255));
    //al_draw_triangle(600, 100, 700,100,650,500, al_map_rgb(255,0,0),10);
    //al_draw_arc(100,500,70, 0, 3.14, al_map_rgb(0,255,0),10);
    //al_draw_rounded_rectangle(250,100,400,200,5,5,al_map_rgb(255,255,0),5);
    //al_flip_display();
    bool done = false;
    bool draw = true;
    int x = 10, y = 10;
    int moveSpeed = 5;
    const float FPS = 60.0;
    enum Direction {UP, DOWN, LEFT, RIGHT};
    ALLEGRO_BITMAP *player = al_load_bitmap("player.png");

    //ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    //al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    //al_start_timer(timer);
    //KeyBoard events
    Direction dir = DOWN;
    /*while(!done)
    {
        //cout << "We have entered to the main loop" << endl;
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        if(events.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(events.keyboard.keycode)
            {
                case ALLEGRO_KEY_DOWN :
                    dir = DOWN;
                    //cout << "Move down" << endl;
                    break;
                case ALLEGRO_KEY_UP :
                    dir = UP;
                    //cout << "Move up" << endl;
                    break;
                case ALLEGRO_KEY_LEFT :
                    dir = LEFT;
                    //cout << "Move left" << endl;
                    break;
                case ALLEGRO_KEY_RIGHT :
                    dir = RIGHT;
                    //cout << "Move right" << endl;
                    break;
                case ALLEGRO_KEY_ESCAPE :
                    done = true;
                    break;
            }
        }
        if(events.type = ALLEGRO_EVENT_TIMER)
        {
            switch(dir)
            {
            case DOWN :
                y += moveSpeed;
                break;
            case UP :
                y -= moveSpeed;
                break;
            case LEFT :
                x -= moveSpeed;
                break;
            case RIGHT :
                x += moveSpeed;
                break;
            }
            draw = true;
        }
        if(draw)
        {
            draw = false;
            al_draw_rectangle(x,y, x + 20, y + 20, electricBlue, 2);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }*/

    //KeyBoard state
    while(!done)
    {
        //cout << "We have entered to the main loop" << endl;
        ALLEGRO_EVENT events;
        al_wait_for_event(event_queue, &events);
        if(events.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(events.keyboard.keycode)
            {
               case ALLEGRO_KEY_ESCAPE :
                    done = true;
                    break;
            }
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if(events.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            x = events.mouse.x;
            y = events.mouse.y;
        }
        else if(events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if(events.mouse.button & 1)
                playerColor = electricBlue;
            else if(events.mouse.button & 2)
                playerColor = yellow;
        }
            //Timer event
        /*if(events.type = ALLEGRO_EVENT_TIMER)
        {
            //KeyBoard state events
            al_get_keyboard_state(&keyState);
            if(al_key_down(&keyState, ALLEGRO_KEY_DOWN))
                y += moveSpeed;
            else if(al_key_down(&keyState, ALLEGRO_KEY_UP))
                y -= moveSpeed;
            else if(al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
                x += moveSpeed;
            else if(al_key_down(&keyState, ALLEGRO_KEY_LEFT))
                x -= moveSpeed;
            draw = true;
        }*/
        /*if(draw)
        {
            draw = false;
            al_draw_rectangle(x,y, x + 20, y + 20, playerColor, 2);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }*/
        al_draw_bitmap(player, x, y, ALLEGRO_FLIP_HORIZONTAL);
        al_draw_rectangle(x,y, x + 20, y + 20, playerColor, 2);
        al_flip_display();
        al_clear_to_color(al_map_rgb(0,0,0));
    }
    //al_rest(DELAY);

    //al_destroy_font(font);
    //al_destroy_timer(timer);
    al_destroy_bitmap(player);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);    

    return 0;
}
