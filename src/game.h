#ifndef GAME_H
#define GAME_H

#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "robot.h"
#include "goalie.h"
#include "striker.h"
#include "opponent.h"
#include "referee.h"
#include "strategy.h"

#define WAIT_TIME_POSITION_TAKING 10000000
#define WAIT_TIME_TURNING 3000000
#define WAIT_FOR_PENALTY_POS 12000000
#define WAIT_TIME_POSITION_CORRECTING 2000000
#define TIME_STEP_SIZE_GOALIE 5000
#define ROBOT_ARRIVED_THRESHOLD 0.2

//array indexes
#define N_ROBOTS 6

#define GOALIE 0
#define STRIKER1 1
#define STRIKER2 2
#define OPPONENT1 3
#define OPPONENT2 4
#define OPPONENT3 5


class Game
{
private:
    ePlayMode previous_state;
    ePlayMode current_state;
    Referee* referee_handler;

    bool is_team_blue;
    bool is_left_side; // IF YOU CHANGE THIS: USE THE SET_FUNCTION SO IT ALSO GETS CHANGED IN THE STRATEGY MODULE
    bool has_kick_off;

    bool stay_in_state_machine;
    bool stay_in_state;

public:
    Strategy strategy_module;

    RawBall* datBall;
    Robot* robots[N_ROBOTS]; //Should fix this hard coding

    Game(RTDBConn DBC, bool is_team_blue_in);

    // state machine
    void state_machine(bool verbose=false);
    void update_state();
    void update_side();
    void update_kick_off();
    void print_state(ePlayMode state=PAUSE);

    // Take position functions
    void take_kick_off_position();
    void take_penalty_position();
	
    // Boolean get functions
    bool get_is_team_blue();
    bool get_is_left_side();
    bool get_has_kick_off();

    // build string of Position in matlab syntax
    std::string matlsynt(Position pos);
    void set_is_left_side(bool is_left_side_in);
};


#endif // GAME_H
