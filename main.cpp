#include <cstdio>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#define ScreenHeight 600
#define ScreenWidth 900
#define WindowName "teste"

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

void dc_iteration(Point* control,float t, int npoints, std::vector<Point> *curve) {
    Point next;
    for( int i = 0; i < npoints-1; i++ ) {
        al_draw_line(control[i].x,control[i].y,
                     control[i+1].x,control[i+1].y,
                     white,1.0F);
        next = l_interp(control[i],control[i+1],t);
        //printf("interpolação %d-%d: x=%.2f, y=%.2f\n",i,i+1,next.x,next.y);
        if(npoints > 2) al_draw_filled_circle(next.x,next.y,2.0,white);
        else {
            al_draw_filled_circle(next.x,next.y,2.0,red);
            curve->push_back(next);
        }
        control[i] = next;
    }
}

inline void error_msg(const char *s)
{
    al_show_native_message_box(NULL, NULL, "Error", s, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void draw_curve(float step, Point *control, int points)
{
    int npoints = points;
    int curvepoints = 0;
    Point *ccont = new Point[points];
    std::vector<Point> *curve = new vector<Point>;

    for( float t = 0.0; t <= 1; t+=0.01) {
        for( int i = 0; i < points; i++ )
            al_draw_filled_circle(control[i].x,control[i].y,4.0,white);
        for(int i = 0; i < points; i++ )
            ccont[i] = control[i];
        for( int i = 0; npoints > 1; i++, npoints--) {
            dc_iteration(ccont,t,npoints,curve);
        }
        curvepoints++;
        for( int i = 0; i < curvepoints-1; i++ ) {
            al_draw_line(curve->at(i).x,curve->at(i).y,
                         curve->at(i+1).x,curve->at(i+1).y,
                         red,2.0F);
        }
        al_flip_display();
        al_rest(0.1);
        al_clear_to_color(black);
        npoints = points;
    }

    delete[] ccont;
    delete curve;
}

int initialize(ALLEGRO_DISPLAY *&d, ALLEGRO_EVENT_QUEUE *&e)
{
    if(!al_init()) {
        error_msg("Could not initialize Allegro 5.\n");
        return 1;
    }

    e = al_create_event_queue();
    if(!e) {
       error_msg("Failed to create event queue.\n");
       return 2;
    }

    d = al_create_display(ScreenWidth, ScreenHeight);
    if(!d) {
        error_msg("Failed to create display.\n");
        return 3;
    }

    if(!al_install_mouse()) {
        error_msg("Unable to initialize mouse driver.\n");
        return 4;
    }
    al_register_event_source(e,al_get_display_event_source(d));   //registra o display para leitura de eventos
    al_init_primitives_addon();

    return 0;
}

void display_config(ALLEGRO_DISPLAY *d, const char *name, int x, int y)
{
    al_set_new_display_flags(ALLEGRO_NOFRAME);
    al_set_window_position(d, x, y);
    al_set_window_title(d, name);
}

void deinit(ALLEGRO_DISPLAY *&d, ALLEGRO_EVENT_QUEUE *&e)
{
    al_destroy_event_queue(e);
    al_destroy_display(d);
}

int main()
{
    int init_status;

    bool done = false;
    Point control[] = {Point(500,25),Point(600,100),Point(350,200),Point(500,225),Point(200,245),Point(400,250)};
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *events;
    ALLEGRO_EVENT *next_event = new ALLEGRO_EVENT;

    if(init_status = initialize(display,events)) return init_status;
    display_config(display,"Teste",200,100);

    draw_curve(0.01,control,point_n);

    while(!done) {
        if(!al_is_event_queue_empty(events)) {
            al_get_next_event(events,next_event);
            if(next_event->type == ALLEGRO_EVENT_DISPLAY_CLOSE) done = true;
        }
    }
    delete next_event;

    deinit(display,events);
    return 0;
}
