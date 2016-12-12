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

    // constants
    const int WAIT_TIME_POSITION_TAKING = 10000000;
    const int WAIT_TIME_TURNING = 3000000;
    const int WAIT_FOR_PENALTY_POS = 12000000;
    const int WAIT_TIME_POSITION_CORRECTING = 2000000;
    const int TIME_STEP_SIZE_GOALIE = 5000;
    const double ROBOT_ARRIVED_THRESHOLD = 0.2;
    const int NR_ROBOTS = 6;

public:
    Strategy strategy_module;

    Goalie* goalie;
    Striker* striker1;
    Striker* striker2;
    Opponent* opponent1;
    Opponent* opponent2;
    Opponent* opponent3;

    RawBall* datBall;

    Robot* robots[6]; //Should fix this hard coding
    Position robot_positions[6]; //Should fix this hard coding. Should not be static


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

    void update_robot_positions();

    // build string of Position in matlab syntax
    std::string matlsynt(Position pos);
    void set_is_left_side(bool is_left_side_in);
};


#endif // GAME_H
