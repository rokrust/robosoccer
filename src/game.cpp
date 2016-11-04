#include "game.h"

#define DEBUG 1
#define WAIT_TIME_POSITION_TAKING 10000000
#define WAIT_TIME_TURNING 3000000

Game::Game(Referee* ref_in, bool is_team_blue_in,
           Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
           Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in)
{
    referee_handler = ref_in;
    is_team_blue = is_team_blue_in;

    goalie = goalie_in;
    striker1 = striker1_in;
    striker2 = striker2_in;
    opponent1 = opponent1_in;
    opponent2 = opponent2_in;
    opponent3 = opponent3_in;

    // get side

}

void Game::set_phase(ePlayMode new_phase, bool verbose=true)
{
    previous_phase = current_phase;
    current_phase = new_phase;

    // catch phase transitions
    if (previous_phase != new_phase) {
        if (verbose) {
            cout << "Game Phase transition" << endl;
        }

        if ((previous_phase == 0) && (current_phase == 1)) {
            // REFEREE_INIT -> BEFORE_KICK_OFF
            if (verbose) {
                cout << "Changed from REFEREE_INIT to BEFORE_KICK_OFF" << endl;
            }
            update_side();
            take_kick_off_position();
        }

        if ((previous_phase == 1) && (current_phase == 2)) {
            // BEFORE_KICK_OFF -> KICK_OFF
            if (verbose) {
                cout << "Changed from BEFORE_KICK_OFF to KICK_OFF" << endl;
            }
        }

        if ((previous_phase == 2) && (current_phase == 5)) {
            // KICK_OFF -> PLAY_ON
            if (verbose) {
                cout << "Changed from KICK_OFF to PLAY_ON" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 1)) {
            // PLAY_ON -> BEFORE_KICK_OFF
            if (verbose) {
                cout << "Changed from PLAY_ON to KICK_OFF" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 0)) {
            // PLAY_ON -> REFEREE_INIT
            if (verbose) {
                cout << "Changed from PLAY_ON to REFEREE_INIT" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 3)) {
            // PLAY_ON -> BEFORE_PENALTY
            if (verbose) {
                cout << "Changed from PLAY_ON to BEFORE_PENALTY" << endl;
            }
        }

        if ((previous_phase == 3) && (current_phase == 4)) {
            // BEFORE_PENALTY -> PENALTY
            if (verbose) {
                cout << "Changed from BEFORE_PENALTY to PENALTY" << endl;
            }
        }

        if ((previous_phase == 4) && (current_phase == 3)) {
            // PENALTY -> BEFORE_PENALTY
            if (verbose) {
                cout << "Changed from PENALTY to BEFORE_PENALTY" << endl;
            }
        }

        if ((previous_phase == 4) && (current_phase == 0)) {
            // PENALTY -> REFEREE_INIT
            if (verbose) {
                cout << "Changed from PENALTY to REFEREE_INIT" << endl;
            }
        }

        // existing but senseless transitions
        if ((previous_phase == 1) && (current_phase == 4)) {
            // BEFORE_KICK_OFF -> PENALTY
            if (verbose) {
                cout << "Changed from BEFORE_KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        if ((previous_phase == 0) && (current_phase == 4)) {
            // REFEREE_INIT -> PENALTY
            if (verbose) {
                cout << "Changed from REFEREE_INIT to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        if ((previous_phase == 2) && (current_phase == 4)) {
            // KICK_OFF -> PENALTY
            if (verbose) {
                cout << "Changed from KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 4)) {
            // PLAY_ON -> PENALTY
            if (verbose) {
                cout << "Changed from PLAY_ON to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }
    }
}

ePlayMode Game::get_phase(bool display)
{
    if (display) {
        switch (current_phase) {
        case 0:
            cout << "Game Phase is REFEREE_INIT: " << current_phase << endl;
            break;
        case 1:
            cout << "Game Phase is BEFORE_KICK_OFF: " << current_phase << endl;
            break;
        case 2:
            cout << "Game Phase is KICK_OFF: " << current_phase << endl;
            break;
        case 3:
            cout << "Game Phase is BEFORE_PENALTY: " << current_phase << endl;
            break;
        case 4:
            cout << "Game Phase is PENALTY: " << current_phase << endl;
            break;
        case 5:
            cout << "Game Phase is PLAY_ON: " << current_phase << endl;
            break;
        case 6:
            cout << "Game Phase is PAUSE: " << current_phase << endl;
            break;
        case 7:
            cout << "Game Phase is TIME_OVER: " << current_phase << endl;
            break;
        default:
            cout << "Unknown Game Phase: " << current_phase << endl;
            break;
        }
    }

    return current_phase;
}

void Game::step(bool verbose=false)
{
    ePlayMode phase = referee_handler->GetPlayMode();
    set_phase(phase);

    if (verbose) {
        bool display = true;
        get_phase(display);
        cin.get();
    }
}

int Game::take_kick_off_position()
{
    update_side();
    update_kick_off();
    bool kicking_team = has_kick_off;
    bool left_side = is_left_side;

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
    Angle goalie_left(-90);
    Angle goalie_right(90);
    Angle left_side_striker1(45);
    Angle left_side_striker2(-45);
    Angle right_side_striker1(-135);
    Angle right_side_striker2(135);

    if (left_side) {
        orientation_goalie = &goalie_left;
        goalie->GotoPos(pos_goalie_left);
        if (kicking_team) {
            orientation_striker1 = &left_side_striker1;
            orientation_striker2 = &left_side_striker2;
            striker1->GotoPos(pos_striker1_left);
            striker2->GotoPos(pos_striker2_left);
        } else {
            orientation_striker1 = &left_forward;
            orientation_striker2 = &left_forward;
            striker1->GotoPos(pos_defender1_left);
            striker2->GotoPos(pos_defender2_left);
        }
    } else {
        orientation_goalie = &goalie_right;
        goalie->GotoPos(pos_goalie_right);
        if (kicking_team) {
            orientation_striker1 = &right_side_striker1;
            orientation_striker2 = &right_side_striker2;
            striker1->GotoPos(pos_striker1_right);
            striker2->GotoPos(pos_striker2_right);
        } else {
            orientation_striker1 = &right_forward;
            orientation_striker2 = &right_forward;
            striker1->GotoPos(pos_defender1_right);
            striker2->GotoPos(pos_defender2_right);
        }
    }

    usleep(WAIT_TIME_POSITION_TAKING);

    // TODO Check if robots arrived savely


    // turn robots into their needed orientation
    goalie->spot_turn(*orientation_goalie);
    striker1->spot_turn(*orientation_striker1);
    striker2->spot_turn(*orientation_striker2);

    usleep(WAIT_TIME_TURNING);

    return 0;
}

void Game::set_is_left_side(bool is_left_side_in)
{
    is_left_side = is_left_side_in;
}

bool Game::check_is_team_blue()
{
    return is_team_blue;
}

bool Game::check_is_left_side()
{
    return is_left_side;
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
        is_left_side = true;
    }
    if ((!is_team_blue) && (blue_has_right)) {
        is_left_side = false;
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
