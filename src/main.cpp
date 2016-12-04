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
        case 1 :
            game_handler.state_machine(true);
            break;

        case 2:
            Game::goalie->do_a_penalty_save();
            break;

        case 3:
            Game::striker1->do_a_shot_at_goal(game_handler.get_is_left_side());
            break;

        case 4:
            Game::goalie->do_the_goalkeepers_kick();
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
    Game::striker2->set_sampling_time((float) timer_duration/1000);

    // cout << "ball_pos = [" << Game::datBall->GetPos().GetX() << ", " << Game::datBall->GetPos().GetY() << "]; " << endl;
    // cout << "poses = [";

    // game_handler.striker2->set_target_pos(Game::datBall->GetPos());

    while(1) {
        Position pos = Game::datBall->GetPos();

        game_handler_in.update_robot_positions();
        Game::striker1->set_target_pos(pos);
        // cout << "Target_pos: " << Game::striker1->get_path_finder().get_target_pos() << endl;

        if (datTimer.timeout()) {
            Game::striker1->set_wheelspeed(timer_duration);
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

    Game::striker1->set_target_pos(left_window);
    Game::striker2->set_target_pos(right_hall);
    Game::goalie->set_target_pos(left_hall);

    Position rand_pos(0.0, 0.0);
    Game::opponent1->set_target_pos(rand_pos);
    Game::opponent1->GotoPos(rand_pos);
    Timer robot_timer(robot_duration);

    while(1) {
        game_handler_in.update_robot_positions();
        if (robot_timer.timeout()) {
            float rand1 = ((rand() % 100) / 100.0);
            float rand2 = ((rand() % 100) / 100.00);
            rand_pos.SetX(rand1 - 0.5);
            rand_pos.SetY(rand2 - 0.5);

            // Set target positions
            if (fabs(Game::goalie->get_target_pos().DistanceTo(Game::goalie->GetPos())) < 0.1) {
                Position target = Game::goalie->get_target_pos();
                target.SetX(-target.GetX());
                target.SetY(-target.GetY());
                Game::goalie->set_target_pos(target);
                cout << "Goalie goal changed to x: " << target.GetX() << " y: " << target.GetY() << endl;
            }
            if (fabs(Game::striker1->get_target_pos().DistanceTo(Game::striker1->GetPos())) < 0.1) {
                Position target = Game::striker1->get_target_pos();
                target.SetX(-target.GetX());
                target.SetY(-target.GetY());
                Game::striker1->set_target_pos(target);
                cout << "Striker1 goal changed to x: " << target.GetX() << " y: " << target.GetY() << endl;
            }
            if (fabs(Game::striker2->get_target_pos().DistanceTo(Game::striker2->GetPos())) < 0.1) {
                Position target = Game::striker2->get_target_pos();
                target.SetX(-target.GetX());
                target.SetY(-target.GetY());
                Game::striker2->set_target_pos(target);
                cout << "Striker2 goal changed to x: " << target.GetX() << " y: " << target.GetY() << endl;
            }
            if (fabs(Game::opponent1->get_target_pos().DistanceTo(Game::opponent1->GetPos())) < 0.1) {
                Game::opponent1->set_target_pos(rand_pos);
                Game::opponent1->GotoPos(rand_pos);
            }

            Game::goalie->set_wheelspeed(robot_duration);
            Game::striker1->set_wheelspeed(robot_duration);
            Game::striker2->set_wheelspeed(robot_duration);
        }
    }
}

void test_controller(Game& game_handler_in)
{
    double KP_h, KI_h, KD_h;
    double KP_d, KI_d, KD_d;
    cout << "KP_h: ";
    cin >> KP_h;
    cout << "KI_h: ";
    cin >> KI_h;
    cout << "KD_h: ";
    cin >> KD_h;
    cout << "KP_d: ";
    cin >> KP_d;
    cout << "KI_d: ";
    cin >> KI_d;
    cout << "KD_d: ";
    cin >> KD_d;
    int ctrlDuration = 100;
    Timer robot_timer(ctrlDuration);
    Position target_pos(0.0, 0.0);
    Game::goalie->set_target_pos(target_pos);

    while(1) {
        if (robot_timer.timeout()) {
            Game::goalie->update_movement(ctrlDuration, KP_h, KI_h, KD_h, KP_d, KI_d, KD_d);
        }
    }
}
