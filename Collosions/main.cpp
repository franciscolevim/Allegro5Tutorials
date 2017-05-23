//This is the example of sprite animation
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace std;
enum Direction {DOWN, LEFT, RIGHT, UP};

// Bounded box collision function

bool isCollision(float tankX, float tankY, int width, int height, float enemyX, float enemyY)
{
    return !(tankX + width < enemyX || tankX > enemyX + width ||
       tankY + height < enemyY || tankY > enemyY + height);
}

// Distance based collision
bool isDistanceCollision(float circle1_x, float circle1_y, float circle2_x, float circle2_y, float r1, float r2)
{
    return (circle1_x - circle2_x) * (circle1_x - circle2_x) +
           (circle1_y - circle2_y) * (circle1_y - circle2_y) <= (r1 + r2) * (r1 + r2);
}

// Pixel perfect collision

bool isDistanceCollision(ALLEGRO_BITMAP *image1, ALLEGRO_BITMAP *image2,
     float &tankX, float &tankY, float fruitX, float fruitY, int width, int height,
     Direction &dir)
{
    if (tankX + width < fruitX || tankX > fruitX + width ||
        tankY + height < fruitY || tankY > fruitY + height)
        return false;
    else
    {
        int top = max(tankY, fruitY);
        int bottom = min(tankY + height, fruitY + height);
        int left = max(tankX, fruitX);
        int right = min(tankX + width, tankY + width);
        const float feedbackDistance = 15.0;
        for(int i = top; i < bottom; ++i)
        {
            for(int j = left; j < right; ++j)
            {
                al_lock_bitmap(image1, al_get_bitmap_format(image1), ALLEGRO_LOCK_READONLY);
                al_lock_bitmap(image2, al_get_bitmap_format(image2), ALLEGRO_LOCK_READONLY);
                ALLEGRO_COLOR color1 = al_get_pixel(image1, j - tankX, i - tankY);
                ALLEGRO_COLOR color2 = al_get_pixel(image2, j - fruitX, i - fruitY);

               // if((color1.a != 0 && color2.a != 0) ||
               //    (color1.g != 0 && color2.g != 0))
                if(color1.a != 0 && color2.a != 0)
                {
                    switch(dir)
                    {
                        case DOWN :
                            //dir = UP;
                            tankY -= feedbackDistance;
                            break;
                        case UP :
                            //dir = DOWN;
                            tankY += feedbackDistance;
                            break;
                        case RIGHT :
                            //dir = LEFT;
                            tankX -= feedbackDistance;
                            break;
                        case LEFT :
                            //dir = RIGHT;
                            tankX += feedbackDistance;
                            break;
                        default:
                            break;
                    }
                    return true;
                }
            }
        }
        return false;

    }
}

//bool isPixelCollision(ALLEGRO_BITMAP *image1, ALLEGRO_BITMAP)
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
    float circleX = 100, circleY = 50, circleR = 20, circleThickness = 2;
    ALLEGRO_COLOR circleColor = al_map_rgb(255,0,0);
    float enemyCircleX = 400, enemyCircleY = 300, enemyCircleR = 30, enemyCircleThickness = 2;
    ALLEGRO_COLOR enemyCircleColor = al_map_rgb(0,255,0);
    float circleMoveSpeed = 18;
    float fruitX = 300, fruitY = 300;
    float appleX = 500, appleY = 500;
    //float sourceX = 0, sourceY = 0;
    float moveSpeed = 5.0;
    const float FPS = 30.0;
    //Rotating image
    float degrees = 0.0f;
    //const float frameFPS = 15.0;

    Direction dir = DOWN, circleDir = DOWN;

    ALLEGRO_BITMAP *tank = al_load_bitmap("tank_left.png");

    vector<ALLEGRO_BITMAP*> tanks;
    tanks.push_back(al_load_bitmap("tank_down.png"));
    tanks.push_back(al_load_bitmap("tank_left.png"));
    tanks.push_back(al_load_bitmap("tank_right.png"));
    tanks.push_back(al_load_bitmap("tank_up.png"));

    ALLEGRO_BITMAP *fruit = al_load_bitmap("strawberry.png");
    ALLEGRO_BITMAP *apple = al_load_bitmap("apple.png");
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    //ALLEGRO_TIMER *frameTimer = al_create_timer(1.0 / frameFPS);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    //al_register_event_source(event_queue, al_get_timer_event_source(frameTimer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);
    //al_start_timer(frameTimer);

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
                   //al_stop_timer(frameTimer);
                   done = true;
                   break;
               }
        }
        else if(events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            al_stop_timer(timer);
            //al_stop_timer(frameTimer);
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
                if(al_key_down(&keyState, ALLEGRO_KEY_S))
                {
                    circleY += moveSpeed;
                    circleDir = DOWN;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_W))
                {
                    circleY -= moveSpeed;
                    circleDir = UP;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_D))
                {
                    circleX += moveSpeed;
                    circleDir = RIGHT;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_A))
                {
                    circleX -= moveSpeed;
                    circleDir = LEFT;
                }
                else
                {
                    active = false;
                }
            }
            // Checking fo tanks collision
            if(isCollision(x,y, al_get_bitmap_width(tank), al_get_bitmap_height(tank), 200,200))
            {
                //cout << "Есть столкновение!";
                switch(dir)
                {
                    case DOWN :
                        dir = UP;
                        y -= moveSpeed;
                        break;
                    case UP :
                        dir = DOWN;
                        y += moveSpeed;
                        break;
                    case RIGHT :
                        dir = LEFT;
                        x -= moveSpeed;
                        break;
                    case LEFT :
                        dir = RIGHT;
                        x += moveSpeed;
                        break;
                    default:
                        break;
                }
            }

            // Insert here pixel-based collision

            for(auto tank : tanks)
            {
                if(isDistanceCollision(tank, fruit, x, y, fruitX, fruitY, al_get_bitmap_width(tank),
                   al_get_bitmap_height(tank), dir) ||
                   isDistanceCollision(tank, apple, x, y, appleX, appleY, al_get_bitmap_width(tank),
                   al_get_bitmap_height(tank), dir))
                   break;
            }

            // Checking for circles collision

            if(isDistanceCollision(circleX, circleY, enemyCircleX, enemyCircleY, circleR, enemyCircleR))
            {
                //cout << "Есть столкновение!";
                switch(circleDir)
                {
                    case DOWN :
                        circleDir = UP;
                        circleY -= circleMoveSpeed;
                        break;
                    case UP :
                        circleDir = DOWN;
                        circleY += circleMoveSpeed;
                        break;
                    case RIGHT :
                        circleDir = LEFT;
                        circleX -= circleMoveSpeed;
                        break;
                    case LEFT :
                        circleDir = RIGHT;
                        circleX += circleMoveSpeed;
                        break;
                    default:
                        break;
                }
            }
           /* else if(events.timer.source == frameTimer)
            {
                if(active)
                    ++sourceX;
                else
                    sourceX = 0;
                if(sourceX >= 2)
                    sourceX = 0;
                sourceY = dir;

            }*/
            draw = true;
        }
        if(draw)
        {
            al_draw_bitmap(tanks[dir], x, y, NULL);
            al_draw_bitmap(tank, 200, 200, NULL);
            al_draw_bitmap(fruit, fruitX, fruitY, NULL);
            al_draw_bitmap(apple, appleX, appleY, NULL);
            al_draw_circle(circleX, circleY, circleR, circleColor, circleThickness);
            al_draw_circle(enemyCircleX, enemyCircleY, enemyCircleR, enemyCircleColor, enemyCircleThickness);
            //al_draw_bitmap_region(player, sourceX * al_get_bitmap_width(player) / 3, sourceY * al_get_bitmap_height(player) / 4,
            //                      al_get_bitmap_width(player) / 3, al_get_bitmap_height(player) / 4, x, y, NULL);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }
    //cout << "Libramos la memoria dianmica";
    al_destroy_display(display);
    al_destroy_timer(timer);
    //al_destroy_timer(frameTimer);
    for(auto &tank : tanks)
        al_destroy_bitmap(tank);
    al_destroy_bitmap(tank);
    al_destroy_bitmap(fruit);
    al_destroy_bitmap(apple);
    al_destroy_event_queue(event_queue);

    return 0;
}
