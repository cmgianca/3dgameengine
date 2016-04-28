#ifndef GAME
#define GAME

#include "engine/common/application.h"

class Game : public Application {
public:
    Game(View* v);
    ~Game();

    void initialize();
private:
    GLuint m_gameShader;
};

#endif // GAME

