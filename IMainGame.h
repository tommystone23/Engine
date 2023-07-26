#ifndef IMAINGAME_H
#define IMAINGAME_H
#include "Window.h"
#include "InputManager.h"

class ScreenList;
class IGameScreen;

class IMainGame
{
public:
    IMainGame();
    virtual ~IMainGame();

    void run();
    void exit();

    virtual void on_init() = 0;
    virtual void add_screens() = 0;
    virtual void on_exit() = 0;

    const float fps() const { return _fps; }

    void on_SDL_event(SDL_Event &event);

    bool is_running() { return _is_running; }

protected:
    ScreenList *_screen_list;
    IGameScreen *_current_screen;
    Window _window;
    InputManager _input_manager;

    bool _is_running;
    float _fps;

    bool init();
    bool init_systems();

    virtual void update();
    virtual void draw();
};

#endif // IMAINGAME_H