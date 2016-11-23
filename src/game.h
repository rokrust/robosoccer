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
    static Goalie* goalie;
    static Striker* striker1;
    static Striker* striker2;
    static Opponent* opponent1;
    static Opponent* opponent2;
    static Opponent* opponent3;

    static RawBall* datBall;

    static Robot* robots[6];

    /* Game(Referee* ref_in, bool is_team_blue_in, RawBall *datBall_in,
         Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
         Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in); */
    Game(RTDBConn DBC, bool is_team_blue_in);

    void step(bool verbose=false);

    int take_kick_off_position();
    void perform_kick_off();

    int take_penalty_position();

    void update_side();
    void update_kick_off();

    void set_phase(ePlayMode new_phase, bool verbose);
    /* void set_ball(RawBall* ball);
    void set_own_team(Goalie* myGoalie, Striker* myStriker1, Striker* myStriker2);
    void set_opponent_team(Opponent* anOpponent1, Opponent* anOpponent2, Opponent* anOpponent3); */
	
    bool get_is_team_blue();
    bool get_is_left_side();
    bool get_has_kick_off();
    // RawBall* get_ball(); //Probably not needed

    // state machine
    void print_state(ePlayMode state=PAUSE);
    void update_state();
    void state_machine(bool verbose=false);

    // build string of Position in matlab syntax
    std::string matlsynt(Position pos);

};
#endif // GAME_H
