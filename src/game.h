#pragma once
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

#define GOAL_MAX_YPOS 0.13
#define GOAL_MIN_YPOS -0.13
#define GOAL_LEFT_XPOS -1.48
#define GOAL_RIGHT_XPOS 1.48


class Game
{
private:
    ePlayMode previous_state;
    ePlayMode current_state;
    Referee* referee_handler;

    bool is_team_blue;
    bool is_left_side;
    bool has_kick_off;

    bool stay_in_state_machine;
    bool stay_in_state;

public:
    Goalie* goalie;
    Striker* striker1;
    Striker* striker2;
    Opponent* opponent1;
    Opponent* opponent2;
    Opponent* opponent3;
    static RawBall* datBall;

    Game(Referee* ref_in, bool is_team_blue_in, RawBall *datBall_in,
         Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
         Opponent* opponent1_in = 0, Opponent* opponent2_in = 0, Opponent* opponent3_in = 0);

    void step(bool verbose=false);

    int take_kick_off_position();
    void perform_kick_off();

    int take_penalty_position();

    void update_side();
    void update_kick_off();

    void set_phase(ePlayMode new_phase, bool verbose);
    void set_ball(RawBall* ball);
	
    bool get_is_team_blue();
    bool get_is_left_side();
    bool get_has_kick_off();
    RawBall* get_ball(); //Probably not needed

    // state machine
    void print_state(ePlayMode state=PAUSE);
    void update_state();
    void state_machine(bool verbose=false);


};
#endif // GAME_H
