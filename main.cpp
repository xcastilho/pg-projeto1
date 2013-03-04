#include <cstdio>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#define ScreenHeight 360
#define ScreenWidth 480
#define WindowName "teste 1"

#define point_n 6

#define white al_map_rgb(255,255,255)
#define black al_map_rgb(0,0,0)
#define red al_map_rgb(255,0,0)

using namespace std;

struct Point {
    float x,y;
    Point(float nx=0.0,float ny=0.0):x(nx),y(ny) {};
};

Point l_interp(Point p1, Point p2, float t) {
    float xr = (1-t)*p1.x + t*p2.x;
    float yr = (1-t)*p1.y + t*p2.y;
    return Point(xr,yr);
}

void dc_iteration(Point* control,float t, int npoints) {
    Point next;
    for( int i = 0; i < npoints-1; i++ ) {
        //al_draw_filled_circle(control[i].x,control[i].y,2.0,white);
        al_draw_line(control[i].x,control[i].y,
                     control[i+1].x,control[i+1].y,
                     white,1.0F);
        next = l_interp(control[i],control[i+1],t);
        //printf("interpolação %d-%d: x=%.2f, y=%.2f\n",i,i+1,next.x,next.y);
        if(npoints > 2) al_draw_filled_circle(next.x,next.y,2.0,white);
        else al_draw_filled_circle(next.x,next.y,2.0,red);
        control[i] = next;
    }
}

int main()
{
    int npoints = point_n;
    Point control[] = {Point(100,100),Point(150,170),Point(80,200),Point(70,250),Point(120,230),Point(170,200)};
    Point ccont[point_n];
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *events;


    if(!al_init()) {
        al_show_native_message_box(NULL, NULL, "Error", "Could not initialize Allegro 5", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    events = al_create_event_queue();
    if(!events) {
       al_show_native_message_box(NULL, NULL, "Error", "Failed to create event_queue.\n", NULL, ALLEGRO_MESSAGEBOX_ERROR);
       return -1;
    }

    display = al_create_display(ScreenWidth, ScreenHeight);
    if(!display) {
        al_show_native_message_box(NULL, NULL,  "Error", "Failed to create display.\n", NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
    al_init_primitives_addon();


    al_set_new_display_flags(ALLEGRO_NOFRAME);
    al_set_window_position(display, 200, 100);
    al_set_window_title(display, WindowName);

    for( int i = 0; i < npoints; i++ )
        al_draw_filled_circle(control[i].x,control[i].y,4.0,white);

    /**start gambiarra***/
    for(int i = 0; i < npoints; i++ ) {
        ccont[i] = control[i];
    }
    /**end gambiarra****/
    for( float t = 0.0; t <= 1; t+=0.01) {
        for( int i = 0; npoints > 1; i++, npoints--) {
            dc_iteration(control,t,npoints);
        }
        al_flip_display();
        al_rest(0.1);
        al_clear_to_color(black);
        npoints = point_n;
        /**start gambiarra***/
        for(int i = 0; i < npoints; i++ ) {
            control[i] = ccont[i];
        }
        /**end gambiarra****/
    }

    al_rest(10.0);
    al_destroy_event_queue(events);
    al_destroy_display(display);

    return 0;
}
