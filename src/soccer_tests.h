//============================================================================
// Name        : soccer_tests.h
// Author      :
// Version     :
// Copyright   : (C)
// Description : Header of the Soccer Test class
//============================================================================

#ifndef SOCCER_TESTS_H
#define SOCCER_TESTS_H

#include "robot.h"
//#include "kogmo_rtdb.hxx"
//#include "robo_control.h"

class Soccer_Tests
{
private:
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
    Soccer_Tests(Robot* blue1_in, Robot* blue2_in, Robot* blue3_in,
                 Robot* red1_in, Robot* red2_in, Robot* red3_in,
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
