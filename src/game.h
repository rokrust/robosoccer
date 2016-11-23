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

#define POSITION_HISTORY_LENGTH 3


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

    // first index is goalie, striker1, striker2, opponent1, opponent2, opponent3
    // second index is how many time steps we go backward
    Position robot_position_history[6][POSITION_HISTORY_LENGTH];
    Position robot_velocity_estimation[6];
    Position robot_position_prediction[6];

    static Robot* robots[6];

    Game(Referee* ref_in, bool is_team_blue_in, RawBall *datBall_in,
         Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
         Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in);

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

    // movement prediction and velocity estimation
    void update_position_history();
    void update_estimation_and_prediction(double ms_between_positions);

    // state machine
    void print_state(ePlayMode state=PAUSE);
    void update_state();
    void state_machine(bool verbose=false);


};
#endif // GAME_H
