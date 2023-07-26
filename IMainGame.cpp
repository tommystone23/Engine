#include "IMainGame.h"
#include "Engine.h"
#include "Timing.h"
#include "IGameScreen.h"
#include "ScreenList.h"

IMainGame::IMainGame()
    : _screen_list(NULL),
    _current_screen(NULL),
    _is_running(false),
    _fps(0.0f)
{
    _screen_list = new ScreenList(this);
}

IMainGame::~IMainGame()
{
}

void IMainGame::run()
{
    if(!init())
        return;
    
    FPSLimiter limiter;
    limiter.init(60.0f);
    _is_running = true;
    while(_is_running)
    {
        limiter.begin();
        update();
        draw();
        _fps = limiter.end();
        _window.swap_buffers();
    }
}

void IMainGame::exit()
{
    _current_screen->on_exit();
    if(_screen_list)
        delete _screen_list;
    _is_running = false;
}

bool IMainGame::init()
{
    Engine::init();

    if(!init_systems())
        return false;
    // This function will be defined in child class
    on_init();

    add_screens();
    _current_screen = _screen_list->get_current_screen();
    _current_screen->set_running();
    _current_screen->on_entry();

    return true;
}

bool IMainGame::init_systems()
{
    _window.create("Default Window", 1920, 1080, 0);
    return true;
}

void IMainGame::update()
{
    _input_manager.update();
    if(_current_screen) {
        switch(_current_screen->get_screen_state()) {
            case(ScreenState::RUNNING):
                _current_screen->update();
                break;
            case(ScreenState::CHANGE_NEXT):
                _current_screen->on_exit();
                _current_screen = _screen_list->move_next();
                if(_current_screen) {
                    _current_screen->set_running();
                    _current_screen->on_entry();
                }
                break;
            case(ScreenState::CHANGE_PREVIOUS):
                _current_screen->on_exit();
                _current_screen = _screen_list->move_previous();
                if(_current_screen) {
                    _current_screen->set_running();
                    _current_screen->on_entry();
                }
                break;
            case(ScreenState::EXIT_APPLICATION):
                exit();
                break;
            default:
                break;
            
            
        }
    }
}

void IMainGame::draw()
{
    _current_screen->draw();
}

void IMainGame::on_SDL_event(SDL_Event &event)
{
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case(SDL_QUIT):
                _is_running = false;
                break;
            case(SDL_MOUSEMOTION):
                _input_manager.set_mouse_coords(event.motion.x, event.motion.y);
                break;
            case(SDL_KEYDOWN):
                _input_manager.press_key(event.key.keysym.sym);
                break;
            case(SDL_KEYUP):
                _input_manager.release_key(event.key.keysym.sym);
                break;
            case(SDL_MOUSEBUTTONDOWN):
                _input_manager.press_key(event.button.button);
                break;
            case(SDL_MOUSEBUTTONUP):
                _input_manager.release_key(event.button.button);
                break;
        }
    }
}
