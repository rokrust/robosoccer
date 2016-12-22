//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
// Copyright   : (C)
// Description : Main Project File
//============================================================================


#include "game.h"
#include "robot.h"
#include "kogmo_rtdb.hxx"
#include "robo_control.h"
#include "referee.h"
#include "timer.h"

#include <time.h>
#include <iostream>
#include <cmath>

using namespace std;


// Function declaration to call from Scenario
// Code is below main Function
void collision_avoidance_ball_tracking(Game& game_handler_in);
void collision_avoidance_corner_driving(Game& game_handler_in);
void test_controller(Game& game_handler_in);
void test_extrapol_shit(Game& game_handler_in);
void tactic_bring_ball_back_in_opp_field(Game& game_handler_in);
void test_via_pos_shit(Game& game_handler_in);




int main(void) {

    //--------------------------------- Init --------------------------------------------------
    const int client_nr = 222;

    try {
        // Establish connection to the RTDB
        cout << endl << "Connecting to RTDB..." << endl;
        // Create the client name with the unique client number*/
        string client_name = "pololu_client_";
        client_name.push_back((char) (client_nr + '0'));
        RTDBConn DBC(client_name.data(), 0.1, "");

        // Get Robot Colour
        bool is_team_blue = true;
        cout << "Which Team? (0: red - 1: blue) ";
        cin >> is_team_blue;

        Game game_handler(DBC, is_team_blue);

        //-------------------------------------- End Init ---------------------------------


        int SCENARIO;
        cout << "Select scenario..." << endl;
        cout << "Play scenarios:" << endl;
        cout << "1: Run the state machine" << endl;
        cout << "2: Run the goalie doing a penalty save" << endl;
        cout << "3: Run the striker doing a shot the goal" << endl;
        cout << "4: Run the goalie doing the goalkeeper's kick" << endl;
        cout << "5: Run take kick-off positions" << endl;
        cout << "6: Run take penalty positions" << endl;
        cout << "7: Run collision avoidance during ball tracking" << endl;
        cout << "8: Run collision avoidance during corner driving" << endl;


        cout << "SCENARIO = ";
        cin >> SCENARIO;


        // Dem Scenarios
        switch (SCENARIO) {
        case 1:
            game_handler.state_machine(true);
            break;

        case 2:
            game_handler.goalie->do_a_penalty_save(game_handler.datBall,
                                                   game_handler.opponent1,
                                                   game_handler.opponent2,
                                                   game_handler.opponent3);
            break;

        case 3:
            game_handler.striker1->do_a_shot_at_goal(game_handler.datBall,
                                                     game_handler.get_is_left_side());
            break;

        case 4:
            game_handler.goalie->do_the_goalkeepers_kick(game_handler.datBall,
                                                         game_handler.opponent1,
                                                         game_handler.opponent2,
                                                         game_handler.opponent3);
            break;

        case 5:
            game_handler.take_kick_off_position();
            break;

        case 6:
            game_handler.take_penalty_position();
            break;

        case 7:
            collision_avoidance_ball_tracking(game_handler);
            break;

        case 8:
            collision_avoidance_corner_driving(game_handler);
            break;

        case 9:
            test_controller(game_handler);
            break;

        case 10:
            test_extrapol_shit(game_handler);
            break;

        case 11:
            tactic_bring_ball_back_in_opp_field(game_handler);
            break;

        case 12:
            test_via_pos_shit(game_handler);
            break;

        }
    } catch (DBError err) {
        cout << "Client died on Error: " << err.what() << endl;
    }
    cout << "End" << endl;
    return 0;
}

  // ================ //
 // === END MAIN === //
// ================ //



void collision_avoidance_ball_tracking(Game &game_handler_in)
{
    int timer_duration = 100;
    Timer datTimer(timer_duration);
    game_handler_in.striker2->set_sampling_time_s((float) timer_duration/1000);

    // cout << "ball_pos = [" << game_handler.datBall->GetPos().GetX() << ", " << game_handler.datBall->GetPos().GetY() << "]; " << endl;
    // cout << "poses = [";

    // game_handler.striker2->set_target_pos(game_handler.datBall->GetPos());

    while(1) {
        Position pos = game_handler_in.datBall->GetPos();

        game_handler_in.update_robot_positions();
        game_handler_in.striker1->set_pathfinder_target_pos(pos);
        // cout << "Target_pos: " << game_handler.striker1->get_path_finder().get_target_pos() << endl;

        if (datTimer.timeout()) {
            game_handler_in.striker1->set_wheelspeed(timer_duration, game_handler_in.robot_positions);
        }
    }
}

void collision_avoidance_corner_driving(Game &game_handler_in)
{
    int robot_duration = 100;
    double x_val = 1.0;
    double y_val = 0.65;
    Position left_window(-x_val, -y_val);
    Position left_hall(-x_val, y_val);
    // Position right_window(x_val,-y_val);
    Position right_hall(x_val, y_val);

    game_handler_in.striker1->set_pathfinder_target_pos(left_window);
    game_handler_in.striker2->set_pathfinder_target_pos(right_hall);
    game_handler_in.goalie->set_pathfinder_target_pos(left_hall);

    Position rand_pos(0.0, 0.0);
    game_handler_in.opponent1->set_pathfinder_target_pos(rand_pos);
    game_handler_in.opponent1->GotoPos(rand_pos);
    Timer robot_timer(robot_duration);

    while(1) {
        game_handler_in.update_robot_positions();
        if (robot_timer.timeout()) {
            float rand1 = ((rand() % 100) / 100.0);
            float rand2 = ((rand() % 100) / 100.00);
            rand_pos.SetX(rand1 - 0.5);
            rand_pos.SetY(rand2 - 0.5);

            // Set target positions
            if (fabs(game_handler_in.goalie->get_pathfinder_target_pos().DistanceTo(game_handler_in.goalie->GetPos())) < 0.1) {
                Position target = game_handler_in.goalie->get_pathfinder_target_pos();
                target.SetX(-target.GetX());
                target.SetY(-target.GetY());
                game_handler_in.goalie->set_pathfinder_target_pos(target);
                cout << "Goalie goal changed to x: " << target.GetX() << " y: " << target.GetY() << endl;
            }
            if (fabs(game_handler_in.striker1->get_pathfinder_target_pos().DistanceTo(game_handler_in.striker1->GetPos())) < 0.1) {
                Position target = game_handler_in.striker1->get_pathfinder_target_pos();
                target.SetX(-target.GetX());
                target.SetY(-target.GetY());
                game_handler_in.striker1->set_pathfinder_target_pos(target);
                cout << "Striker1 goal changed to x: " << target.GetX() << " y: " << target.GetY() << endl;
            }
            if (fabs(game_handler_in.striker2->get_pathfinder_target_pos().DistanceTo(game_handler_in.striker2->GetPos())) < 0.1) {
                Position target = game_handler_in.striker2->get_pathfinder_target_pos();
                target.SetX(-target.GetX());
                target.SetY(-target.GetY());
                game_handler_in.striker2->set_pathfinder_target_pos(target);
                cout << "Striker2 goal changed to x: " << target.GetX() << " y: " << target.GetY() << endl;
            }
            if (fabs(game_handler_in.opponent1->get_pathfinder_target_pos().DistanceTo(game_handler_in.opponent1->GetPos())) < 0.1) {
                game_handler_in.opponent1->set_pathfinder_target_pos(rand_pos);
                game_handler_in.opponent1->GotoPos(rand_pos);
            }

            game_handler_in.goalie->set_wheelspeed(robot_duration, game_handler_in.robot_positions);
            game_handler_in.striker1->set_wheelspeed(robot_duration, game_handler_in.robot_positions);
            game_handler_in.striker2->set_wheelspeed(robot_duration, game_handler_in.robot_positions);
        }
    }
}

void test_controller(Game& game_handler_in)
{
    int ctrlDuration = 100;
    Timer robot_timer(ctrlDuration);
}

void test_extrapol_shit(Game& game_handler_in)
{
    int timer_duration = 150;
    Timer datTimer(timer_duration);
    game_handler_in.goalie->set_sampling_time_s((float) timer_duration/1000);
    int sign = 1;
    Position go_here(sign * 1.0, sign * 0.3);

    int waitabit = game_handler_in.goalie->spot_turn(game_handler_in.goalie->GetPos().AngleOfLineToPos(go_here));
    datTimer.enable_manually(waitabit);

    game_handler_in.goalie->set_robot_target_pos(go_here);

    while(1) {
        if (datTimer.timeout()) {
            game_handler_in.goalie->set_wheelspeed(timer_duration, game_handler_in.robot_positions);

            cout << "Dist to target = " << game_handler_in.goalie->get_robot_target_pos().DistanceTo(game_handler_in.goalie->GetPos()) << endl;
        }
    }
}

void test_via_pos_shit(Game& game_handler_in)
{
    int timer_duration = 150;
    Timer datTimer(timer_duration);
    game_handler_in.goalie->set_sampling_time_s((float) timer_duration/1000);

    int sign = +1;
    Position go_here(sign*1.1, +0.4);
    Position go_via_here(sign*1.0, -0.4);
    game_handler_in.goalie->set_robot_via_path(go_via_here, go_here);

    game_handler_in.goalie->update_temporary_target_pos(true);

    int waitabit = game_handler_in.goalie->spot_turn_on_target_if_necessary();
    datTimer.enable_manually(waitabit);

    while(1) {
        if (datTimer.timeout()) {
            game_handler_in.goalie->set_wheelspeed(timer_duration, game_handler_in.robot_positions);

            bool changed_target_pos = game_handler_in.goalie->update_temporary_target_pos(true);
            if (changed_target_pos) { // Do a Spot Turn if necessary
                int waitabit = game_handler_in.goalie->spot_turn_on_target_if_necessary();
                datTimer.enable_manually(waitabit);
            }

            cout << "Dist to target = " << game_handler_in.goalie->get_temporary_target_pos().DistanceTo(game_handler_in.goalie->GetPos()) << endl;
        }
    }
}

void tactic_bring_ball_back_in_opp_field(Game& game_handler_in)
{
    int timer_duration = 150;
    Timer striker1Timer(timer_duration);
    Timer striker2Timer(timer_duration);

    game_handler_in.goalie->set_sampling_time_s((float) timer_duration/1000);
    game_handler_in.striker1->set_sampling_time_s((float) timer_duration/1000);
    game_handler_in.striker2->set_sampling_time_s((float) timer_duration/1000);

    // Determine the shit out of what strategic move to do
    game_handler_in.strategy_module.strat_move();

    // Take care of striker 1
    int wait_time1 = game_handler_in.striker1->spot_turn_on_target_if_necessary();
    if (wait_time1 > 0)
        striker1Timer.enable_manually(wait_time1);
    bool striker1_at_ball = false;
    bool striker1_shot = false;

    // Take care of striker 2
    int wait_time2 = game_handler_in.striker2->spot_turn_on_target_if_necessary();
    if (wait_time2 > 0)
        striker2Timer.enable_manually(wait_time2);
    bool striker2_at_ball = false;
    bool striker2_shot = false;

    while(1) {
        // Striker 1 Stuff
        if (striker1Timer.timeout() && !striker1_shot) {
            game_handler_in.striker1->set_wheelspeed(timer_duration, game_handler_in.robot_positions);

            bool changed_target_pos1 = game_handler_in.striker1->update_temporary_target_pos(true);
            if (changed_target_pos1) { // Do a Spot Turn if necessary
                wait_time1 = game_handler_in.striker1->spot_turn_on_target_if_necessary();
                striker1Timer.enable_manually(wait_time1);
            }

            if (game_handler_in.striker1->GetPos().DistanceTo(game_handler_in.striker1->get_robot_target_pos()) <= 0.03 &&
                    game_handler_in.striker1->GetPos().DistanceTo(game_handler_in.datBall->GetPos()) <= 0.1) {
                cout << "Striker 1 is close to its target pos and ball" << endl;

                if (striker1_at_ball) {
                    game_handler_in.striker1->MoveMs(120, 120, 500, 0);
                    striker1Timer.enable_manually(500 + 30);
                    striker1_shot = true;
                    cout << "Striker 1 done" << endl;
                } else {
                    wait_time1 = game_handler_in.striker1->spot_turn(game_handler_in.striker1->GetPos().AngleOfLineToPos(game_handler_in.datBall->GetPos()));
                    striker1Timer.enable_manually(wait_time1);
                    striker1_at_ball = true;
                    cout << "Striker 1 at ball" << endl;
                }
            }
        }

        // Striker 2 Stuff
        if (striker2Timer.timeout() && !striker2_shot) {
            game_handler_in.striker2->set_wheelspeed(timer_duration, game_handler_in.robot_positions);

            bool changed_target_pos2 = game_handler_in.striker2->update_temporary_target_pos(true);
            if (changed_target_pos2) { // Do a Spot Turn if necessary
                wait_time2 = game_handler_in.striker2->spot_turn_on_target_if_necessary();
                striker2Timer.enable_manually(wait_time2);
            }

            if (game_handler_in.striker2->GetPos().DistanceTo(game_handler_in.striker2->get_robot_target_pos()) <= 0.03 &&
                    game_handler_in.striker2->GetPos().DistanceTo(game_handler_in.datBall->GetPos()) <= 0.1) {
                cout << "Striker 2 is close to its target pos and ball" << endl;

                if (striker2_at_ball) {
                    game_handler_in.striker2->MoveMs(160, 160, 500, 0);
                    striker2_shot = true;
                    cout << "Striker 2 done" << endl;
                } else {
                    wait_time2 = game_handler_in.striker2->spot_turn(game_handler_in.striker2->GetPos().AngleOfLineToPos(game_handler_in.datBall->GetPos()));
                    striker2Timer.enable_manually(wait_time2);
                    striker2_at_ball = true;
                    cout << "Striker 2 at ball" << endl;
                }
            }
        }

        if (striker1_shot && striker2_shot) {
            cout << "Both Strikers are done, ending loop" << endl;
            break;
        }
    }

    cout << "ENDED LOOP" << endl;
}
