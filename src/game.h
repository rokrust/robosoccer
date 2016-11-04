#ifndef GAME_H
#define GAME_H

#include "share.h"
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "robot.h"
#include "referee.h"

class Game
{
private:
    ePlayMode previous_phase;
    ePlayMode current_phase;
    Referee* referee_handler;

    /**
     * @brief Pointer to the first blue robot
     */
    Robot* blue1;

    /**
     * @brief Pointer to the second blue robot
     */
    Robot* blue2;

    /**
     * @brief Pointer to the third blue robot
     */
    Robot* blue3;

    /**
     * @brief Pointer to the first red robot
     */
    Robot* red1;

    /**
     * @brief Pointer to the second red robot
     */
    Robot* red2;

    /**
     * @brief Pointer to the third red robot
     */
    Robot* red3;

public:
    Game(Referee* ref_in,
         Robot* blue1_in, Robot* blue2_in, Robot* blue3_in,
         Robot* red1_in, Robot* red2_in, Robot* red3_in);
    void step();



    void set_phase(ePlayMode new_phase);
    ePlayMode get_phase(bool display=false);
};

#endif // GAME_H
