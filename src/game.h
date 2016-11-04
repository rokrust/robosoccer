#ifndef GAME_H
#define GAME_H

#include "share.h"
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "robot.h"
#include "goalie.h"
#include "striker.h"
#include "opponent.h"
#include "referee.h"

class Game
{
private:
    ePlayMode previous_phase;
    ePlayMode current_phase;
    Referee* referee_handler;

    bool is_team_blue;
    bool is_left_side;
    bool has_kick_off;

public:
    Goalie* goalie;
    Striker* striker1;
    Striker* striker2;
    Opponent* opponent1;
    Opponent* opponent2;
    Opponent* opponent3;

    Game(Referee* ref_in, bool is_team_blue_in,
         Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
         Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in);

    void step(bool verbose=false);


    int take_kick_off_position();

    void set_team(bool is_blue);

    void set_phase(ePlayMode new_phase, bool verbose);
    ePlayMode get_phase(bool display=false);

    void set_is_left_side(bool is_left_side_in);

    bool check_is_team_blue();

    bool check_is_left_side();

    void update_side();

    void update_kick_off();
};

#endif // GAME_H
