#include "game.h"

#define DEBUG 1
#define WAIT_TIME_POSITION_TAKING 10000000
#define WAIT_TIME_TURNING 3000000
#define WAIT_FOR_PENALTY_POS 12000000
#define WAIT_TIME_POSITION_CORRECTING 2000000

#define TIME_STEP_SIZE_GOALIE 5000

#define ROBOT_ARRIVED_THRESHOLD 0.2

#define N_ROBOTS 6


RawBall* Game::datBall = NULL;
Goalie* Game::goalie = NULL;
Striker* Game::striker1 = NULL;
Striker* Game::striker2 = NULL;
Opponent* Game::opponent1 = NULL;
Opponent* Game::opponent2 = NULL;
Opponent* Game::opponent3 = NULL;

Robot* Game::robots[6] = {0};
Position Game::robot_positions[6] = {Position(-1.3, 0.0),  Position(-0.3, 0.4), Position(-0.15, -0.15), Position(0,0), Position(0,0), Position(0,0)};


Game::Game(RTDBConn DBC, bool is_team_blue_in)
{

    // Initialize Referee
    // Referee ref_in(DBC);
    // ref_in.Init();
    referee_handler = new Referee(DBC);

    referee_handler->Init();

    // Initialize datBall
    datBall = new RawBall(DBC);


    // Set Team Colour
    is_team_blue = is_team_blue_in;
    //Position test = robot_positions[0];
    //cout << "x:" << test.GetX() << ", y: " << test.GetY() << endl;

    // Get device Numbers for drobots depending on which colour
    int myGoalieDvNr;
    int myStriker1DvNr;
    int theOpponent1DvNr;
    if (is_team_blue) {
        myGoalieDvNr = 0;
        myStriker1DvNr = 1;
        theOpponent1DvNr = 3;
    }
    else {
        myGoalieDvNr = 3;
        myStriker1DvNr = 4;
        theOpponent1DvNr = 0;
    }


    // Initialize Robot Objects
    cout << "Before goalie" << endl;
    goalie = new Goalie(DBC, myGoalieDvNr, 0, robot_positions[0]);
    cout << "After goalie" << endl;
    striker1 = new Striker(DBC, myStriker1DvNr, 1, robot_positions[1]);
    striker2 = new Striker(DBC, myStriker1DvNr+1, 2, robot_positions[2]);
    opponent1 = new Opponent(DBC, theOpponent1DvNr, 3, robot_positions[3]);
    opponent2 = new Opponent(DBC, theOpponent1DvNr+1, 4, robot_positions[4]);
    opponent3 = new Opponent(DBC, theOpponent1DvNr+2, 5, robot_positions[5]);


    // initialize state machine variables
    stay_in_state_machine = true;
    previous_state = REFEREE_INIT;
    current_state = REFEREE_INIT;

    robots[0] = goalie;
    robots[1] = striker1;
    robots[2] = striker2;
    robots[3] = opponent1;
    robots[4] = opponent2;
    robots[5] = opponent3;

}


void Game::state_machine(bool verbose)
{
    // include possibility to leave the state machine by changing the flag stay_in_state_machine
    // so-far no use-case (Simon, 16.11.2016)
    while (stay_in_state_machine) {

        // reset the flag to stay in a state
        stay_in_state = true;

        // update side and kick_off information from referee
        // update_kick_off internally calls update_side
        // given that the referee can change the kick_off later, this has to be polled also in the loops
        update_kick_off();

        if (verbose) {
            cout << "----------------State transition----------------" << endl;
            cout << "is_team_blue: " << is_team_blue << endl;
            cout << "is_left_side: " << is_left_side << endl;
            cout << "has_kick_off: " << has_kick_off << endl;
            cout << "Previous ";
            print_state(previous_state);
            cout << "Current ";
            print_state(current_state);
        }

        // INITIALIZATION CASE
        if ((previous_state == 0) && (current_state == 0) && stay_in_state) {
            if (verbose) {
                cout << "State machine initialized. Waiting for state change..." << endl;
            }
            while (stay_in_state) {
                update_state();
            }
        }


        // REFEREE_INIT -> BEFORE_KICK_OFF
        if ((previous_state == 0) && (current_state == 1) && stay_in_state) {
            if (verbose) {
                cout << "Changed from REFEREE_INIT to BEFORE_KICK_OFF" << endl;
            }

            // side and kick_off are updated within the take_kick_off_positions
            take_kick_off_position();
            referee_handler->SetReady(!is_team_blue);

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // BEFORE_KICK_OFF -> KICK_OFF
        if ((previous_state == 1) && (current_state == 2) && stay_in_state) {
            if (verbose) {
                cout << "Changed from BEFORE_KICK_OFF to KICK_OFF" << endl;
            }

            // perform Game state initializations like creating timers

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // KICK_OFF -> PLAY_ON
        if ((previous_state == 2) && (current_state == 5) && stay_in_state) {
            if (verbose) {
                cout << "Changed from KICK_OFF to PLAY_ON" << endl;
            }

            // perform Game state initializations like creating timers
            Timer goalie_timer(TIME_STEP_SIZE_GOALIE);
            goalie_timer.enable_periodically();

            while (stay_in_state) {
                // perform regular state tasks like timers
                if (goalie_timer.timeout()) {
                    cout << "Goalie timer has timed out" << endl;
                    goalie_timer.enable_periodically();
                }

                // detect state changes
                update_state();
            }

            goalie_timer.~Timer();
        }

        // PLAY_ON -> BEFORE_KICK_OFF
        if ((previous_state == 5) && (current_state == 1) && stay_in_state) {
            if (verbose) {
                cout << "Changed from PLAY_ON to KICK_OFF" << endl;
            }

            // side and kick_off are updated within the take_kick_off_positions
            take_kick_off_position();
            referee_handler->SetReady(!is_team_blue);

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // PLAY_ON -> REFEREE_INIT
        if ((previous_state == 5) && (current_state == 0) && stay_in_state) {
            if (verbose) {
                cout << "Changed from PLAY_ON to REFEREE_INIT" << endl;
            }

            // perform Game state initializations like creating timers

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // PLAY_ON -> BEFORE_PENALTY
        if ((previous_state == 5) && (current_state == 3) && stay_in_state) {
            if (verbose) {
                cout << "Changed from PLAY_ON to BEFORE_PENALTY" << endl;
            }

            // side and kick_off are updated within the function
            take_penalty_position();

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // BEFORE_PENALTY -> PENALTY
        if ((previous_state == 3) && (current_state == 4) && stay_in_state) {
            if (verbose) {
                cout << "Changed from BEFORE_PENALTY to PENALTY" << endl;
            }

            // perform Game state initializations like creating timers

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // PENALTY -> BEFORE_PENALTY
        if ((previous_state == 4) && (current_state == 3) && stay_in_state) {
            if (verbose) {
                cout << "Changed from PENALTY to BEFORE_PENALTY" << endl;
            }

            // side and kick_off are updated within the function
            take_penalty_position();

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // PENALTY -> REFEREE_INIT
        if ((previous_state == 4) && (current_state == 0) && stay_in_state) {
            if (verbose) {
                cout << "Changed from PENALTY to REFEREE_INIT" << endl;
            }

            // perform Game state initializations like creating timers

            while (stay_in_state) {
                // perform regular state tasks like timers

                // detect state changes
                update_state();
            }
        }

        // existing but senseless transitions
        // BEFORE_KICK_OFF -> PENALTY
        if ((previous_state == 1) && (current_state == 4) && stay_in_state) {
            if (verbose) {
                cout << "Changed from BEFORE_KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
            while (stay_in_state) {
                update_state();
            }
        }

        // REFEREE_INIT -> PENALTY
        if ((previous_state == 0) && (current_state == 4) && stay_in_state) {
            if (verbose) {
                cout << "Changed from REFEREE_INIT to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
            while (stay_in_state) {
                update_state();
            }
        }

        // KICK_OFF -> PENALTY
        if ((previous_state == 2) && (current_state == 4) && stay_in_state) {
            if (verbose) {
                cout << "Changed from KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
            while (stay_in_state) {
                update_state();
            }
        }

        // PLAY_ON -> PENALTY
        if ((previous_state == 5) && (current_state == 4) && stay_in_state) {
            if (verbose) {
                cout << "Changed from PLAY_ON to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
            while (stay_in_state) {
                update_state();
            }
        }
    }
}

void Game::update_state()
{
    ePlayMode polled_current_state = referee_handler->GetPlayMode();

    // invoke state transition if the state changed
    if (polled_current_state != current_state) {
        previous_state = current_state;
        current_state = polled_current_state;
        stay_in_state = false;
    }
}

void Game::update_side()
{
    int blue_has_right = referee_handler->GetBlueSide();
    if ((is_team_blue) && (!blue_has_right)) {
        is_left_side = true;
    }
    if ((is_team_blue) && (blue_has_right)) {
        is_left_side = false;
    }
    if ((!is_team_blue) && (!blue_has_right)) {
        is_left_side = false;
    }
    if ((!is_team_blue) && (blue_has_right)) {
        is_left_side = true;
    }
}

void Game::update_kick_off()
{
    update_side();
    int kicking_side_right = referee_handler->GetSide();
    if ((kicking_side_right) && (is_left_side)) {
        has_kick_off = false;
    }
    if ((kicking_side_right) && (!is_left_side)) {
        has_kick_off = true;
    }
    if ((!kicking_side_right) && (is_left_side)) {
        has_kick_off = true;
    }
    if ((!kicking_side_right) && (!is_left_side)) {
        has_kick_off = false;
    }
}

void Game::print_state(ePlayMode state)
{
    ePlayMode state_to_print;

    if (state == PAUSE) {
        state_to_print = current_state;
    } else {
        state_to_print = state;
    }

    switch (state_to_print) {
    case 0:
        cout << "State is REFEREE_INIT: " << state_to_print << endl;
        break;
    case 1:
        cout << "State is BEFORE_KICK_OFF: " << state_to_print << endl;
        break;
    case 2:
        cout << "State is KICK_OFF: " << state_to_print << endl;
        break;
    case 3:
        cout << "State is BEFORE_PENALTY: " << state_to_print << endl;
        break;
    case 4:
        cout << "State is PENALTY: " << state_to_print << endl;
        break;
    case 5:
        cout << "State is PLAY_ON: " << state_to_print << endl;
        break;
    case 6:
        cout << "State is PAUSE: " << state_to_print << endl;
        break;
    case 7:
        cout << "State is TIME_OVER: " << state_to_print << endl;
        break;
    default:
        cout << "Unknown State: " << state_to_print << endl;
        break;
    }
}


void Game::take_kick_off_position()
{
    update_side();
    update_kick_off();
    bool kicking_team = has_kick_off;
    bool left_side = is_left_side;

    // define position pointers
    // they are needed to check the positions without determining the side and role again
    Position* goalie_pos;
    Position* striker1_pos;
    Position* striker2_pos;

    // define kick-off positions
    Position pos_goalie_left(-1.3, 0.0);
    Position pos_goalie_right(1.3, 0.0);

    // define striker positions if not kicking off
    Position pos_defender1_left(-0.3, 0.4);
    Position pos_defender2_left(-0.3, -0.4);
    Position pos_defender1_right(0.3, 0.4);
    Position pos_defender2_right(0.3, -0.4);

    // define striker positions if kicking off
    Position pos_striker1_left(-0.15, -0.15);
    Position pos_striker2_left(-0.15, 0.15);
    Position pos_striker1_right(0.15, 0.15);
    Position pos_striker2_right(0.15, -0.15);

    // define orientations
    Angle* orientation_striker1;
    Angle* orientation_striker2;
    Angle* orientation_goalie;
    Angle left_forward(0);
    Angle right_forward(180);
    Angle goalie_left(90);
    Angle goalie_right(90);
    Angle left_side_striker1(45);
    Angle left_side_striker2(-45);
    Angle right_side_striker1(-135);
    Angle right_side_striker2(135);

    if (left_side) {
        orientation_goalie = &goalie_left;
        goalie_pos = &pos_goalie_left;
        if (kicking_team) {
            orientation_striker1 = &left_side_striker1;
            orientation_striker2 = &left_side_striker2;
            striker1_pos = &pos_striker1_left;
            striker2_pos = &pos_striker2_left;
        } else {
            orientation_striker1 = &left_forward;
            orientation_striker2 = &left_forward;
            striker1_pos = &pos_defender1_left;
            striker2_pos = &pos_defender2_left;
        }
    } else {
        orientation_goalie = &goalie_right;
        goalie_pos = &pos_goalie_right;
        if (kicking_team) {
            orientation_striker1 = &right_side_striker1;
            orientation_striker2 = &right_side_striker2;
            striker1_pos = &pos_striker1_right;
            striker2_pos = &pos_striker2_right;
        } else {
            orientation_striker1 = &right_forward;
            orientation_striker2 = &right_forward;
            striker1_pos = &pos_defender1_right;
            striker2_pos = &pos_defender2_right;
        }
    }

    goalie->GotoPos(*goalie_pos);
    striker1->GotoPos(*striker1_pos);
    striker2->GotoPos(*striker2_pos);

    usleep(WAIT_TIME_POSITION_TAKING);

    // check if robots arrived savely
    bool goalie_unfinished = true;
    bool striker1_unfinished = true;
    bool striker2_unfinished = true;
    while (goalie_unfinished || striker1_unfinished || striker2_unfinished) {
        if (goalie->GetPos().DistanceTo(*goalie_pos) > ROBOT_ARRIVED_THRESHOLD) {
            cout << "Goalie did not arrive so far" << endl;
            goalie->Turn(90);
            goalie->MoveDist(0.2, 30, false);
        } else {
            goalie_unfinished = false;
        }
        if (striker1->GetPos().DistanceTo(*striker1_pos) > ROBOT_ARRIVED_THRESHOLD) {
            cout << "Striker1 did not arrive so far" << endl;
            striker1->Turn(90);
            striker1->MoveDist(0.2, 30, false);
        } else {
            striker1_unfinished = false;
        }
        if (striker2->GetPos().DistanceTo(*striker2_pos) > ROBOT_ARRIVED_THRESHOLD) {
            cout << "Striker2 did not arrive so far" << endl;
            striker2->Turn(90);
            striker2->MoveDist(0.2, 30, false);
        } else {
            striker2_unfinished = false;
        }

        usleep(WAIT_TIME_POSITION_CORRECTING);

        if (goalie_unfinished) {
            goalie->GotoPos(*goalie_pos);
        }
        if (striker1_unfinished) {
            striker1->GotoPos(*striker1_pos);
        }
        if (striker2_unfinished) {
            striker2->GotoPos(*striker2_pos);
        }
        usleep(WAIT_TIME_POSITION_TAKING);
    }

    // turn robots into their needed orientation
    goalie->spot_turn(*orientation_goalie);
    striker1->spot_turn(*orientation_striker1);
    striker2->spot_turn(*orientation_striker2);

    usleep(WAIT_TIME_TURNING);
}

void Game::take_penalty_position()
{
    update_kick_off();
    bool kicking_team = has_kick_off;

    cout << "Penalty Position: is_left side: " << is_left_side;
    cout << " as kicking team " << kicking_team << endl;

    Position posGoalie;
    Position posStriker1;
    Position posStriker2;
    Angle angle4allRobots(180);

    if (is_left_side && kicking_team) {
        cout << "In take_penalty_position - Case 1" << endl;
        posStriker1.SetX(0.0); // Striking from Middle Point
        posStriker1.SetY(0.0);

        posGoalie.SetX(1.0); // Somewhere on the right side
        posGoalie.SetY(-0.3);

        posStriker2.SetX(1.0); // Somewhere on the right side
        posStriker2.SetY(0.3);

        angle4allRobots.Set(3.1415); // all orientate to left side
    }
    else if (is_left_side && !kicking_team) {
        cout << "In take_penalty_position - Case 2" << endl;

        posGoalie.SetX(-1.3); // Defend
        posGoalie.SetY(0.0);

        posStriker1.SetX(1.0); // Somewhere on the right side
        posStriker1.SetY(-0.3);

        posStriker2.SetX(1.0); // Somewhere on the right side
        posStriker2.SetY(0.3);

        angle4allRobots.Set(0); // all orientate to left side
    }
    else if (!is_left_side && kicking_team) {
        cout << "In take_penalty_position - Case 3" << endl;

        posStriker1.SetX(0.0); // Striking from Middle Point
        posStriker1.SetY(0.0);

        posGoalie.SetX(-1.0); // Somewhere on the right side
        posGoalie.SetY(-0.3);

        posStriker2.SetX(-1.0); // Somewhere on the right side
        posStriker2.SetY(0.3);

        angle4allRobots.Set(0); // all orientate to left side
    }
    else if (!is_left_side && !kicking_team) {
        cout << "In take_penalty_position - Case 4" << endl;

        posGoalie.SetX(1.3); // Defend
        posGoalie.SetY(0.0);

        posStriker1.SetX(-1.0); // Somewhere on the right side
        posStriker1.SetY(-0.3);

        posStriker2.SetX(-1.0); // Somewhere on the right side
        posStriker2.SetY(0.3);

        angle4allRobots.Set(3.1415); // all orientate to left side
    }

    goalie->GotoPos(posGoalie);
    striker1->GotoPos(posStriker1);
    striker2->GotoPos(posStriker2);

    usleep(WAIT_FOR_PENALTY_POS);

    goalie->spot_turn(angle4allRobots);
    striker1->spot_turn(angle4allRobots);
    striker2->spot_turn(angle4allRobots);
}


bool Game::get_is_team_blue()
{
    return is_team_blue;
}

bool Game::get_is_left_side()
{
    return is_left_side;
}

bool Game::get_has_kick_off()
{
    return has_kick_off;
}


void Game::update_robot_positions(){
    for(int i = 0; i < N_ROBOTS; i++){
        robot_positions[i] = robots[i]->GetPos();
        robots[i]->get_path_finder().update_vector_field_positions();
    }
}


std::string Game::matlsynt(Position pos)
{
    int precision = 3; // after decimal point

    std::ostringstream stream_for_matlab_syntax;
    stream_for_matlab_syntax << "[" << setprecision(precision) << fixed << pos.GetX() << ", " << (double) pos.GetY() << "]; ";
    return stream_for_matlab_syntax.str();

    // Usage
    // Position pos2print(1, 0);
    // cout << "pos2print = " << game_handler.matlsynt(pos2print) << endl;
}
