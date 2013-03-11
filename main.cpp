#include <cstdio>
#include <vector>
//#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define DEBUG 0             //mudar para 1 ativa as mensagens de debug

#define screenHeight 600
#define screenWidth 900
#define windowName "Projeto 1 - Processamento Gráfico"

#define fps 30
#define cpoint_radius 4.0

#define white al_map_rgb(255,255,255)
#define black al_map_rgb(0,0,0)
#define red al_map_rgb(255,0,0)
#define teal al_map_rgb(70,225,255)
#define liteblue al_map_rgb(20,125,235)
#define litegreen al_map_rgb(0,145,65)

using namespace std;

struct Point {
    float x,y;
    Point(float nx=0.0,float ny=0.0):x(nx),y(ny) {};
};

std::vector<Point> control = vector<Point>();
bool modified = true;

ALLEGRO_FONT *font;

int point_n = 0;
int ctrl_index = -1;
float derivative_t = 0.5;

float dvx = 0;
float dvy = 0;
float dc1x = 0;
float dc1y = 0;
float dc2x = 0;
float dc2y = 0;

int curve_precision = 50;

bool increase_derivative_t = false;
bool decrease_derivative_t = false;
bool increase_curve_precision = false;
bool decrease_curve_precision = false;

bool polygon_visible = true;
bool cpoints_visible = true;
bool curve_visible = true;

Point l_interp(Point p1, Point p2, float t) {
    float xr = (1-t)*p1.x + t*p2.x;
    float yr = (1-t)*p1.y + t*p2.y;
    if(DEBUG) printf("interpolation of (%f,%f) and (%f,%f)\n",p1.x,p1.y,p2.x,p2.y);
    return Point(xr,yr);
}

void dc_iteration(Point *control,float t, int npoints, std::vector<Point> *curve) {
    Point next;

    for( int i = 0; i < npoints-1; i++ ) {
        next = l_interp(control[i],control[i+1],t);
        if(DEBUG) printf("result = (%f,%f)\n",next.x,next.y);
        if(npoints == 2) {
            curve->push_back(next);
        }
        control[i] = next;
    }
}

Point get_derivative(Point* control, float t, int npoints, int order)
{
    std::vector<Point> *dv = new std::vector<Point>;
    Point p;

    if(order < npoints) {
        for( unsigned int j = 0; j < order; j++ ) {
            for( int i = 0; i < npoints-1; i++ ) {
                control[i] = Point(control[i+1].x-control[i].x,control[i+1].y-control[i].y);
            }
            npoints--;
        }

        if(npoints > 1) {
            while(npoints > 1) {
                dc_iteration(control,t,npoints,dv);
                npoints--;
            }
            p = dv->at(0);
        } else {
            delete dv;
            return control[0];
        }

        delete dv;
        return p;
    } else {
        delete dv;
        return Point(0,0);
    }
}

inline void error_msg(const char *s)
{
    al_show_native_message_box(NULL, NULL, "Error", s, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

void draw_control_points(std::vector<Point> control, int points)
{
    for( int i = 0; i < points; i++ )
        al_draw_filled_circle(control[i].x,control[i].y,4.0,white);
}

void draw_control_polygon(std::vector<Point> control, int points)
{
    for( int i = 0; i < points-1; i++ )
        al_draw_line(control[i].x, control[i].y,
                     control[i+1].x, control[i+1].y,
                     teal,1.0F);
}

void draw_curve(float step, std::vector<Point> control, int points)
{
    Point dc1, dc2;
//    float unitx1, unity1, unitx2, unity2;
    int npoints = points;
    int curvepoints = 0;
    Point *ccont = new Point[points];
    Point *vcont0 = new Point[points];
    Point *vcont = new Point[points];
    Point *vcont2 = new Point[points];
    std::vector<Point> *curve = new vector<Point>;
    std::vector<Point> *dvpoint = new vector<Point>;


    for( float t = 0.0; t <= 1; t+=step) {
        for(int i = 0; i < points; i++ ) {
            ccont[i] = control[i];
            vcont[i] = control[i];
            vcont2[i] = control[i];
            vcont0[i] = control[i];
        }
        for( int i = 0; npoints > 1; i++, npoints--) {
            dc_iteration(ccont,t,npoints,curve);
        }
        curvepoints++;
        npoints = points;
    }
    for( int i = 0; i < curvepoints-1; i++ ) {
        al_draw_line(curve->at(i).x,curve->at(i).y,
                     curve->at(i+1).x,curve->at(i+1).y,
                     red,2.0F);
    }
    al_draw_line(curve->at(curvepoints-1).x,curve->at(curvepoints-1).y,
                 control[point_n-1].x,control[point_n-1].y,
                 red,2.0F);


    for( int i = 0; npoints > 1; i++, npoints--) {
        dc_iteration(vcont0,derivative_t,npoints,dvpoint);
    }
    dc1 = get_derivative(vcont, derivative_t, points, 1);
    dc2 = get_derivative(vcont2, derivative_t, points, 2);
    //magia negra
/*    unitx1 = 2*sqrt(2)*(dc1.x/(dc1.x*dc1.x+dc1.y*dc1.y)) - 2*sqrt(2)*dc1.y/(dc1.x*dc1.x+dc1.y*dc1.y);
    unity1 = 2*sqrt(2)*(dc1.x/(dc1.x*dc1.x+dc1.y*dc1.y)) + 2*sqrt(2)*dc1.y/(dc1.x*dc1.x+dc1.y*dc1.y);
    unitx2 = 2*sqrt(2)*(dc2.x/(dc2.x*dc2.x+dc2.y*dc2.y)) - 2*sqrt(2)*dc2.y/(dc2.x*dc2.x+dc2.y*dc2.y);
    unity2 = 2*sqrt(2)*(dc2.x/(dc2.x*dc2.x+dc2.y*dc2.y)) + 2*sqrt(2)*dc2.y/(dc2.x*dc2.x+dc2.y*dc2.y);*/

    al_draw_line(dvpoint->at(0).x,dvpoint->at(0).y,
                 dvpoint->at(0).x+dc1.x,dvpoint->at(0).y+dc1.y,
                 liteblue,1.0F);
/*    al_draw_line(dvpoint->at(0).x+dc1.x,dvpoint->at(0).y+dc1.y,
                 dvpoint->at(0).x+dc1.x-500*unity1,dvpoint->at(0).y+dc1.y-500*unity1,
                 liteblue,1.0F);
    al_draw_line(dvpoint->at(0).x+dc1.x,dvpoint->at(0).y+dc1.y,
                 dvpoint->at(0).x+dc1.x+500*unity1,dvpoint->at(0).y+dc1.y+500*unity1,
                 liteblue,1.0F);*/
    al_draw_line(dvpoint->at(0).x,dvpoint->at(0).y,
                 dvpoint->at(0).x+dc2.x,dvpoint->at(0).y+dc2.y,
                 litegreen,1.0F);
/*    al_draw_line(dvpoint->at(0).x+dc1.x,dvpoint->at(0).y+dc1.y,
                 dvpoint->at(0).x+dc1.x-500*unity2,dvpoint->at(0).y+dc1.y-500*unity2,
                 liteblue,1.0F);
    al_draw_line(dvpoint->at(0).x+dc1.x,dvpoint->at(0).y+dc1.y,
                 dvpoint->at(0).x+dc1.x+500*unity2,dvpoint->at(0).y+dc1.y+500*unity2,
                 liteblue,1.0F);*/
    dvx = dvpoint->at(0).x;
    dvy = dvpoint->at(0).y;
    dc1x = dc1.x;
    dc1y = dc1.y;
    dc2x = dc2.x;
    dc2y = dc2.y;

    delete[] ccont;
    delete curve;
}

void update()
{
    al_clear_to_color(black);
    if(polygon_visible) draw_control_polygon(control,point_n);
    if(cpoints_visible) draw_control_points(control,point_n);
    if(point_n >= 2 && curve_visible) draw_curve(1.0/curve_precision,control,point_n);
    al_draw_textf(font,white,10,10,0,"t = %.2f",derivative_t);
    al_draw_textf(font,white,10,20,0,"p = (%.2f,%.2f)",dvx,dvy);
    al_draw_textf(font,white,10,30,0,"d1 = (%.2f,%.2f)",dc1x,dc1y);
    al_draw_textf(font,white,10,40,0,"d2 = (%.2f,%.2f)",dc2x,dc2y);
    al_draw_textf(font,white,10,50,0,"av = %d",curve_precision);
    al_flip_display();
}

int initialize(ALLEGRO_DISPLAY *&d, ALLEGRO_TIMER *&t, ALLEGRO_EVENT_QUEUE *&e)
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

    t = al_create_timer(1.0/fps);
    if(!t) {
        error_msg("Failed to create timer.\n");
        return 7;
    }

    d = al_create_display(screenWidth, screenHeight);
    if(!d) {
        error_msg("Failed to create display.\n");
        return 3;
    }

    if(!al_install_keyboard()) {
        error_msg("Unable to initialize keyboard driver.\n");
        return 8;
    }

    if(!al_install_mouse()) {
        error_msg("Unable to initialize mouse driver.\n");
        return 4;
    }

    if(!al_set_system_mouse_cursor(d, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT)) {
        error_msg("Unable to access system mouse cursor\n");
        return 5;
    }

    if(!al_init_primitives_addon()) {
        error_msg("Error initializing primitive generator\n");
        return 6;
    }

    al_init_font_addon();
    al_register_event_source(e,al_get_display_event_source(d));   //registra o display para leitura de eventos
    al_register_event_source(e,al_get_timer_event_source(t));     //registra o timer para leitura de eventos
    al_register_event_source(e,al_get_keyboard_event_source());   //registra o teclado para leitura de eventos
    al_register_event_source(e,al_get_mouse_event_source());      //registra o mouse para leitura de eventos

    al_start_timer(t);

    return 0;
}

void mouse_press(int x, int y, unsigned button )
{
    bool point_selected = false;
    for(int i = 0; i < point_n; i++) {
        if(x > (control[i].x - cpoint_radius) && x < (control[i].x + cpoint_radius)
        && y > (control[i].y - cpoint_radius) && y < (control[i].y + cpoint_radius))
        {
            point_selected = true;
            if(DEBUG) printf("selected control point is %d at (%d,%d)\n",i,x,y);
            if(button == 1) {
                ctrl_index = i;
            } else if (button == 2) {
                if(DEBUG) printf("deletion request for %d\n",i);
                control.erase(control.begin()+i);
                point_n--;
                modified = true;
            }
            break;
        }
    }
    if(!point_selected) {
        if(button == 1) {
            control.push_back(Point(x,y));
            ctrl_index = point_n;
            if(DEBUG) printf("point %d created at (%d,%d)\n",point_n,x,y);
            point_n++;
            modified = true;
        }
    }
}

void mouse_release(int x, int y, unsigned button )
{
    if(ctrl_index >= 0) {
        ctrl_index = -1;
        //modified = true;
    }
    if(DEBUG) printf("no control point selected.\n");
}

void mouse_movement(int dx, int dy)
{
    if( ctrl_index >= 0 ) {
        control[ctrl_index] = Point(control[ctrl_index].x + dx, control[ctrl_index].y + dy);
        modified = true;
    }
}

void key_press(int keycode)
{
    switch(keycode) {
        case ALLEGRO_KEY_TAB:
            cpoints_visible = !cpoints_visible;
            break;
        case ALLEGRO_KEY_P:
            polygon_visible = !polygon_visible;
            break;
        case ALLEGRO_KEY_C:
            curve_visible = !curve_visible;
            break;
        case ALLEGRO_KEY_A:
            increase_derivative_t = true;
            break;
        case ALLEGRO_KEY_S:
            decrease_derivative_t = true;
            break;
        case ALLEGRO_KEY_Q:
            increase_curve_precision = true;
            break;
        case ALLEGRO_KEY_W:
            decrease_curve_precision = true;
            break;
        }
    modified = true;
}

void key_release(int keycode)
{
    switch(keycode) {
        case ALLEGRO_KEY_A:
            increase_derivative_t = false;
            break;
        case ALLEGRO_KEY_S:
            decrease_derivative_t = false;
            break;
        case ALLEGRO_KEY_Q:
            increase_curve_precision = false;
            break;
        case ALLEGRO_KEY_W:
            decrease_curve_precision = false;
    }
}

void display_config(ALLEGRO_DISPLAY *d, const char *name, int x, int y)
{
    al_set_window_position(d, x, y);
    al_set_window_title(d, name);
}

void deinit(ALLEGRO_DISPLAY *&d, ALLEGRO_TIMER *&t, ALLEGRO_EVENT_QUEUE *&e)
{
    al_destroy_event_queue(e);
    al_destroy_timer(t);
    al_destroy_display(d);
}

int main()
{
    int init_status;
    bool done = false;
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *events;
    ALLEGRO_KEYBOARD_STATE *state = new ALLEGRO_KEYBOARD_STATE;
    ALLEGRO_EVENT *next_event = new ALLEGRO_EVENT;

    if(init_status = initialize(display,timer,events)) return init_status;
    display_config(display,windowName,0,0);

    font = al_create_builtin_font();

    update();
    while(!done) {
        al_wait_for_event(events,next_event);
        switch(next_event->type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
            case ALLEGRO_EVENT_TIMER:
                if(increase_derivative_t && derivative_t < 0.99) derivative_t += 0.01;
                if(decrease_derivative_t && derivative_t > 0.01) derivative_t -= 0.01;
                if(increase_curve_precision && (curve_precision + 1 > curve_precision)) curve_precision += 1;
                if(decrease_curve_precision && (curve_precision > 1)) curve_precision -= 1;
                if(modified) {
                    al_get_keyboard_state(state);
                    if((!al_key_down(state,ALLEGRO_KEY_A)) && (!al_key_down(state,ALLEGRO_KEY_S))
                    && (!al_key_down(state,ALLEGRO_KEY_W)) && (!al_key_down(state,ALLEGRO_KEY_Q))) modified = false;
                    update();
                }
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                mouse_press(next_event->mouse.x, next_event->mouse.y,next_event->mouse.button);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                mouse_release(next_event->mouse.x, next_event->mouse.y, next_event->mouse.button);
                break;
            case ALLEGRO_EVENT_MOUSE_AXES:
                mouse_movement(next_event->mouse.dx, next_event->mouse.dy);
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                key_press(next_event->keyboard.keycode);
                break;
            case ALLEGRO_EVENT_KEY_UP:
                key_release(next_event->keyboard.keycode);
                break;
        }
    }


    delete state;
    delete next_event;
    deinit(display,timer,events);
    return 0;
}
