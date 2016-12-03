#ifndef GAME_H
#define GAME_H

//class Strategy;

#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "robot.h"
#include "goalie.h"
#include "striker.h"
#include "opponent.h"
#include "referee.h"
//#include "strategy.h"

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

    // constants
    const int WAIT_TIME_POSITION_TAKING = 10000000;
    const int WAIT_TIME_TURNING = 3000000;
    const int WAIT_FOR_PENALTY_POS = 12000000;
    const int WAIT_TIME_POSITION_CORRECTING = 2000000;
    const int TIME_STEP_SIZE_GOALIE = 5000;
    const double ROBOT_ARRIVED_THRESHOLD = 0.2;
    const int NR_ROBOTS = 6;

public:
    static Goalie* goalie;
    static Striker* striker1;
    static Striker* striker2;
    static Opponent* opponent1;
    static Opponent* opponent2;
    static Opponent* opponent3;

    static RawBall* datBall;

    static Robot* robots[6]; //Should fix this hard coding
    static Position robot_positions[6]; //Should fix this hard coding

//    Strategy* strategy_modul;

    Game(RTDBConn DBC, bool is_team_blue_in);

    // state machine
    void print_state(ePlayMode state=PAUSE);
    void update_state();
    void state_machine(bool verbose=false);

    // Kick off functions
    int take_kick_off_position();
    void update_kick_off();

    // Penalty functions
    void take_penalty_position();

    void update_side();
	
    // Boolean get functions
    bool get_is_team_blue();
    bool get_is_left_side();
    bool get_has_kick_off();

    void update_robot_positions();

    // build string of Position in matlab syntax
    std::string matlsynt(Position pos);

};


#endif // GAME_H
