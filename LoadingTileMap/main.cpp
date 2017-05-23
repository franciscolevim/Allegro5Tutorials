//This is the example of loading tile maps
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 640
#define BLOCK_SIZE 40

using namespace std;
typedef vector< vector< int > > Matrix;
typedef vector< vector< char > > Matriz;
typedef vector<string> charMatriz;



/*void loadMap(const string &fileName, Matrix &map)
{
    ifstream fi(fileName);
    if(fi.is_open())
    {
        while(fi)
        {
            string line;
            getline(fi, line);

            if(line.find("[TileSet]") != string::npos)
            {
                state = TILE_SET;
                continue;
            }
            else if(line.find("[Map]") != string::npos)
            {
                state = MAP;
                continue;
            }

            switch(state)
            {
                case TILE_SET :
                    if(line.length() > 0)
                        tileSet = al_load_bitmap(line.c_str());
                    break;
                case MAP :
                    istringstream stream(line);                                 // Создаем строковый поток из исходной строки
                    vector<int> row;
                    while(stream)
                    {
                        string element;
                        getline(stream, element, ' ');
                        if(element.length() > 0)
                            row.push_back(atoi(element.c_str()));
                    }
                    map.push_back(row);
                    break;
            }

                                                  // Загоняем вектор в матрицу
         }
        fi.close();
    }
    else
    {
        al_show_native_message_box(NULL, NULL, NULL, "Could not open map file", NULL,NULL);
    }
}*/

void cargarMapa(const string &fileName, charMatriz &mapa, ALLEGRO_BITMAP *&tileSet);

void printMap(const Matrix &map);

void imprimirMapa(const charMatriz &mapa);

void drawMap(Matrix map); // Рисуеет стенки прямоугольничками

void dibujarMapa(charMatriz mapa, ALLEGRO_BITMAP *tileSet); // Рисует стенки из изображения кирпичиков

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
    float x = 70, y = 80;
    int moveSpeed = 5;
    const float FPS = 60.0;
    float degrees = 0.0f, scale = 1.0f;
    //const float frameFPS = 15.0;
    enum Direction {DOWN, LEFT, RIGHT, UP};
    Direction dir = DOWN;
    vector<ALLEGRO_BITMAP*> tanks;
    tanks.push_back(al_load_bitmap("tank_down.png"));
    tanks.push_back(al_load_bitmap("tank_left.png"));
    tanks.push_back(al_load_bitmap("tank_right.png"));
    tanks.push_back(al_load_bitmap("tank_up.png"));
    ALLEGRO_BITMAP *wall = al_load_bitmap("wall40.png");
    ALLEGRO_BITMAP *muro = al_load_bitmap("muro.png");
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
    ALLEGRO_KEYBOARD_STATE keyState;
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);
    //Загружаем карту
    //Matrix map;
    //loadMap("map.txt", map);
    charMatriz mapa;
    ALLEGRO_BITMAP *tileSet;
    cargarMapa("mapa.txt", mapa, tileSet);
    //tileSet = al_load_bitmap("tiles.png");
    imprimirMapa(mapa);
    //drawMap(map);
    //printMap(map);

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
                    degrees += 0.5f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_S))
                {
                    degrees -= 0.5f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_I))
                {
                    scale += 0.1f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_D))
                {
                    scale -= 0.1f;
                }
                else if(al_key_down(&keyState, ALLEGRO_KEY_L))
                {
                    //Cargamos nuestra mapa en el tiempo real
                    mapa.clear();
                    cargarMapa("mapa.txt", mapa, tileSet);
                }
                else
                {
                    active = false;
                }
            }

            if(degrees == 360 || degrees == -360)
                degrees = 0;
        }
        if(draw)
        {
            //al_draw_bitmap(tileSet, 200, 200, NULL);
            dibujarMapa(mapa, tileSet);
            // Here we draw rotated and scaled bitmap
            al_draw_scaled_rotated_bitmap(tanks[dir], al_get_bitmap_width(tanks[dir]) / 2,
            al_get_bitmap_height(tanks[dir]) / 2, x, y, scale, scale, degrees * M_PI / 180, NULL);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }
    //cout << "Libramos la memoria dianmica";
    al_destroy_display(display);
    al_destroy_timer(timer);
    for(auto & tank : tanks)
        al_destroy_bitmap(tank);
    al_destroy_bitmap(wall);
    al_destroy_bitmap(muro);
    al_destroy_bitmap(tileSet);
    al_destroy_event_queue(event_queue);

    return 0;
}
void cargarMapa(const string &fileName, charMatriz &mapa, ALLEGRO_BITMAP *&tileSet)
{
    enum LoadState { TILE_SET, MAP};
    ifstream fi(fileName);
    int state = NULL;
    if(fi.is_open())
    {
        while(!fi.eof())
        {
            string line;
            getline(fi, line);
            //if(line.find("[TileSet]") != string::npos)
            if(line == "[TileSet]")
            {
                cout << "line = " << line << endl;
                state = TILE_SET;
                continue;
            }
            else if(line == "[Map]")
            {
                cout << "line = " << line << endl;
                state = MAP;
                continue;
            }
            switch(state)
            {
                case TILE_SET :
                    if(line.length() > 0)
                    {
                        tileSet = al_load_bitmap(line.c_str());
                        cout << "line.c_str() = " << line.c_str() << endl;
                    }
                    break;
                case MAP :
                    cout << "line = " << line << endl;
                    mapa.push_back(line);
                    break;
            }
        }
        fi.close();
    }
    else
    {
        al_show_native_message_box(NULL, NULL, NULL, "Could not open map file", NULL,NULL);
    }
}

void printMap(const Matrix &map)
{
    ofstream fo("testMapReading.txt");
    fo << "rowCount = " << map.size() << endl;
    for(int i = 0; i < int(map.size()); ++i)
    {
        fo << "row number " << i << " has " << map[i].size() << " elements\n";
        for(int j = 0; j < int(map[i].size()); ++j)
            fo << map[i][j] << " ";
        fo << endl;
    }
}

void imprimirMapa(const charMatriz &mapa)
{
    ofstream fo("testMapa.txt");
    for(const auto &str : mapa)
            fo << str << endl;
        fo << endl;
}

void drawMap(Matrix map)
{
    for(int i = 0; i < int(map.size()); ++i)
        for(int j = 0; j < int(map[i].size()); ++j)
        {
            if(map[i][j] == 0)
                al_draw_filled_rectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE,
                i * BLOCK_SIZE + BLOCK_SIZE, al_map_rgb(0,80,200));
            else
                al_draw_filled_rectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, j * BLOCK_SIZE + BLOCK_SIZE,
                i * BLOCK_SIZE + BLOCK_SIZE, al_map_rgb(200,50,100));

        }
}

void dibujarMapa(charMatriz mapa, ALLEGRO_BITMAP *tileSet)
{
    for(int i = 0; i < int(mapa.size()); ++i)
        for(int j = 0; j < int(mapa[i].size()); ++j)
        {
            if(mapa[i][j] == 'X')
                al_draw_bitmap_region(tileSet, 0, 0, BLOCK_SIZE, BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE, NULL);
            if(mapa[i][j] == 'B')
                al_draw_bitmap_region(tileSet, BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE, j * BLOCK_SIZE, i * BLOCK_SIZE, NULL);
        }
}

//////Версия функции для считывания карты из файла
/// В основу взят урок автора курса по Allegro5
/// Но она не учитывает тот факт, что строки могут быть разного размера
/// т.е. матрица может быть не квадратной
/// void loadMap(const string &fileName, Matrix &map)
/*void loadMap(const string &fileName, Matrix &map)
{
    ifstream fi(fileName);
    if(fi.is_open())
    {
        string line;
        getline(fi, line);
        cout << line << endl;
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        int mapSizeX = line.size();
        fi.seekg(0, std::ios::beg);
        while(!fi.eof())
        {
            vector<int> row;
            for(int i = 0; i < mapSizeX; ++i)
            {
                int value;
                fi >> value;
                row.push_back(value);
            }
            map.push_back(row);
        }
        fi.close();
    }
    else
    {
        al_show_native_message_box(NULL, NULL, NULL, "Could not open map file", NULL,NULL);
    }
}*/
