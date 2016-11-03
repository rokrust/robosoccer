#include "game.h"

#define DEBUG 1

Game::Game(Referee* ref_in,
           Robot* blue1_in, Robot* blue2_in, Robot* blue3_in,
           Robot* red1_in, Robot* red2_in, Robot* red3_in)
{
    referee_handler = ref_in;
    blue1 = blue1_in;
    blue2 = blue2_in;
    blue3 = blue3_in;
    red1 = red1_in;
    red2 = red2_in;
    red3 = red3_in;
}

void Game::set_phase(ePlayMode new_phase)
{
    phase = new_phase;
}

ePlayMode Game::get_phase()
{
    return phase;
}

void Game::step()
{
    phase = referee_handler->GetPlayMode();
    if (DEBUG) {
        cout << "Game state is " << phase << endl;
        cin.get();
    }
}

