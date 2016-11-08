//============================================================================
// Name        : soccer_tests.h
// Author      :
// Version     :
// Copyright   : (C)
// Description : Header of the Soccer Test class
//============================================================================

#ifndef SOCCER_TESTS_H
#define SOCCER_TESTS_H

#include "goalie.h"
#include "striker.h"
#include "opponent.h"
#include "robot.h"
//#include "kogmo_rtdb.hxx"
//#include "robo_control.h"

class Soccer_Tests
{
private:
    /**
     * @brief Pointer to the first blue robot
     */
    Goalie* goalie;

    /**
     * @brief Pointer to the second blue robot
     */
    Striker* striker1;

    /**
     * @brief Pointer to the third blue robot
     */
    Striker* striker2;

    /**
     * @brief Pointer to the first red robot
     */
    Opponent* opponent1;

    /**
     * @brief Pointer to the second red robot
     */
    Opponent* opponent2;

    /**
     * @brief Pointer to the third red robot
     */
    Opponent* opponent3;

    /**
     * @brief The Ball
     */
    RawBall* datBall;

public:
    /**
     * @brief Soccer_Tests
     * @param blue1_in
     * @param blue2_in
     * @param blue3_in
     * @param red1_in
     * @param red2_in
     * @param red3_in
     */
    Soccer_Tests(Goalie *goalie_in, Striker *striker1_in, Striker *striker2_in,
                 Opponent *opponent1_in, Opponent *opponent2_in, Opponent *opponent3_in,
                 RawBall* datBall_in);

    /**
     * @brief Move all six robots in a loop to inner and outer positions in a rectangle
     */
    void move_in_out();

    /**
     * @brief Drive the first blue robot with constant wheel speed
     */
    void const_wheel_speed();

    /**
     * @brief Drive the first blue robot with a basic P controller on a straight line
     */
    void easy_p_ctrl();

    /**
     * @brief Drive the first blue robot with the custom driving function
     */
    void drive_to_pos();

    void test_command_driving();

    /**
     * @brief Test the custom turning function with two turns
     */
    void test_turns();

    /**
     * @brief Do some stuff with the goalie
     */
    int test_goalie();

    void turn_experiments();

};

#endif // SOCCER_TESTS_H
