#include "game.h"


#define GOALIE_KICKOFF_POS_L Position(-1.3, 0.0)
#define STRIKER1_NON_KICKOFF_POS_L Position(-0.3, 0.4)
#define STRIKER2_NON_KICKOFF_POS_L Position(-0.3, -0.4)
#define STRIKER1_KICKOFF_POS_L Position(-0.15, -0.15)
#define STRIKER2_KICKOFF_POS_L Position(-0.15, 0.15)

#define GOALIE_KICKOFF_POS_R Position(1.3, 0.0)
#define STRIKER1_NON_KICKOFF_POS_R Position(0.3, 0.4)
#define STRIKER2_NON_KICKOFF_POS_R Position(0.3, -0.4)
#define STRIKER1_KICKOFF_POS_R Position(0.15, -0.15)
#define STRIKER2_KICKOFF_POS_R Position(0.15, 0.15)

void Game::test_init(){
    take_kick_off_positions();
    //strategy_module.set_avoidance_degree(GOALIE, STRIKER1, 0.0);
    set_some_positions();
    //robots[0]->set_target_pos(Position(0,0));
    //robots[1]->set_target_pos(Position(0,0));
    //robots[2]->set_target_pos(Position(0,0));
}


void Game::test_loop(){
    robots[GOALIE]->set_target_pos(datBall->GetPos());
    strategy_module.move_robots();
}

Game::Game()
{
    // Establish connection to the RTDB
    cout << endl << "Connecting to RTDB..." << endl;
    // Create the client name with the unique client number
    string client_name = "pololu_client_";
    client_name.push_back((char) (CLIENT_NBR + '0'));
    static RTDBConn DBC(client_name.data(), 0.1, "");

    // Get Robot Colour
    cout << "Which Team? (0: red - 1: blue) ";
    cin >> is_team_blue;

    // Initialize Referee
    referee_handler = new Referee(DBC);
    referee_handler->Init();

    // Initialize datBall
    datBall = new RawBall(DBC);

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
    robots[GOALIE] = new Goalie(DBC, myGoalieDvNr, 0);
    robots[STRIKER1] = new Striker(DBC, myStriker1DvNr, 1);
    robots[STRIKER2] = new Striker(DBC, myStriker1DvNr+1, 2);
    robots[OPPONENT1] = new Opponent(DBC, theOpponent1DvNr, 3);
    robots[OPPONENT2] = new Opponent(DBC, theOpponent1DvNr+1, 4);
    robots[OPPONENT3] = new Opponent(DBC, theOpponent1DvNr+2, 5);

    // initialize state machine variables
    stay_in_state_machine = true;
    previous_state = REFEREE_INIT;
    current_state = REFEREE_INIT;

    // Initialize Strategy Module
    strategy_module = Strategy(robots, datBall, is_left_side);
}


void Game::state_machine(bool verbose)
{
    // include possibility to leave the state machine by changing the flag stay_in_state_machine
    // so-far no use-case (Simon, 16.11.2016)
    std::cout << "Stay: " << stay_in_state_machine << std::endl;
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
            take_kick_off_positions();
            referee_handler->SetReady(!is_team_blue);

            while (stay_in_state) {
                // perform regular state tasks like timers
                cout << "Moving robots" << endl;
                strategy_module.move_robots();

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
            Timer goalie_timer(100);
            goalie_timer.enable();

            while (stay_in_state) {
                // perform regular state tasks like timers
                if (goalie_timer.timeout()) {
                    cout << "Goalie timer has timed out" << endl;
                    goalie_timer.enable();
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
            take_kick_off_positions();
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
            take_penalty_positions();

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
            take_penalty_positions();

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
    // ANDI DID MODIFY THIS WITH THE SET FUNCTION IT SHOULD STILL DO THE SAME BUT ALSO SET THE SIDE IN STRATEGY
    bool blue_has_right = referee_handler->GetBlueSide();

    if ((is_team_blue) && (!blue_has_right)) {
        // is_left_side = true;
        this->set_is_left_side(true);
    }
    if ((is_team_blue) && (blue_has_right)) {
        // is_left_side = false;
        this->set_is_left_side(false);
    }
    if ((!is_team_blue) && (!blue_has_right)) {
        // is_left_side = false;
        this->set_is_left_side(false);
    }
    if ((!is_team_blue) && (blue_has_right)) {
        // is_left_side = true;
        this->set_is_left_side(true);
    }
}

void Game::update_kick_off()
{
    update_side();

    cout << "Side updated" << endl;
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

//A bit ugly and unwieldy but should get the job done
void Game::take_kick_off_positions() {
	update_side();
	update_kick_off();
    cout << "Kick off updated" << endl;

	if (is_left_side) {
        robots[GOALIE]->set_target_pos(GOALIE_KICKOFF_POS_L);
		robots[OPPONENT1]->set_target_pos(GOALIE_KICKOFF_POS_R);

		if (has_kick_off) {
			robots[STRIKER1]->set_target_pos(STRIKER1_KICKOFF_POS_L);
			robots[STRIKER2]->set_target_pos(STRIKER2_KICKOFF_POS_L);
			robots[OPPONENT2]->set_target_pos(STRIKER1_NON_KICKOFF_POS_R);
			robots[OPPONENT3]->set_target_pos(STRIKER2_NON_KICKOFF_POS_R);
		}
		
		else {
			robots[STRIKER1]->set_target_pos(STRIKER1_NON_KICKOFF_POS_L);
			robots[STRIKER2]->set_target_pos(STRIKER2_NON_KICKOFF_POS_L);
			robots[OPPONENT2]->set_target_pos(STRIKER1_KICKOFF_POS_R);
			robots[OPPONENT3]->set_target_pos(STRIKER2_KICKOFF_POS_R);
		}
	}

	else {
		robots[GOALIE]->set_target_pos(GOALIE_KICKOFF_POS_R);
		robots[OPPONENT1]->set_target_pos(GOALIE_KICKOFF_POS_L);

		if (has_kick_off) {
			robots[STRIKER1]->set_target_pos(STRIKER1_KICKOFF_POS_R);
			robots[STRIKER2]->set_target_pos(STRIKER2_KICKOFF_POS_R);
			robots[OPPONENT2]->set_target_pos(STRIKER1_NON_KICKOFF_POS_L);
			robots[OPPONENT3]->set_target_pos(STRIKER2_NON_KICKOFF_POS_L);
		}

		else {
			robots[STRIKER1]->set_target_pos(STRIKER1_NON_KICKOFF_POS_R);
			robots[STRIKER2]->set_target_pos(STRIKER2_NON_KICKOFF_POS_R);
			robots[OPPONENT2]->set_target_pos(STRIKER1_KICKOFF_POS_L);
			robots[OPPONENT3]->set_target_pos(STRIKER2_KICKOFF_POS_L);
		}

	}
}

void Game::take_penalty_positions(){


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

void Game::set_is_left_side(bool is_left_side_in)
{
    is_left_side = is_left_side_in;
    strategy_module.set_is_left_side(is_left_side_in);
}


void Game::set_some_positions(){
    robots[GOALIE]->set_target_pos(STRIKER1_NON_KICKOFF_POS_R);
    robots[STRIKER1]->set_target_pos(GOALIE_KICKOFF_POS_R);
    robots[STRIKER2]->set_target_pos(STRIKER2_NON_KICKOFF_POS_R);
}

/*
//Fix this
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
        // cout << "In take_penalty_position - Case 1" << endl;
        posStriker1.SetX(0.0); // Striking from Middle Point
        posStriker1.SetY(0.0);

        posGoalie.SetX(1.0); // Somewhere on the right side
        posGoalie.SetY(-0.3);

        posStriker2.SetX(1.0); // Somewhere on the right side
        posStriker2.SetY(0.3);

        angle4allRobots.Set(3.1415); // all orientate to left side
    }
    else if (is_left_side && !kicking_team) {
        // cout << "In take_penalty_position - Case 2" << endl;

        posGoalie.SetX(-1.3); // Defend
        posGoalie.SetY(0.0);

        posStriker1.SetX(1.0); // Somewhere on the right side
        posStriker1.SetY(-0.3);

        posStriker2.SetX(1.0); // Somewhere on the right side
        posStriker2.SetY(0.3);

        angle4allRobots.Set(0); // all orientate to left side
    }
    else if (!is_left_side && kicking_team) {
        // cout << "In take_penalty_position - Case 3" << endl;

        posStriker1.SetX(0.0); // Striking from Middle Point
        posStriker1.SetY(0.0);

        posGoalie.SetX(-1.0); // Somewhere on the right side
        posGoalie.SetY(-0.3);

        posStriker2.SetX(-1.0); // Somewhere on the right side
        posStriker2.SetY(0.3);

        angle4allRobots.Set(0); // all orientate to left side
    }
    else if (!is_left_side && !kicking_team) {
        // cout << "In take_penalty_position - Case 4" << endl;

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
*/
