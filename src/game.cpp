#include "game.h"

#define DEBUG 1
#define WAIT_TIME_POSITION_TAKING 10000000
#define WAIT_TIME_TURNING 3000000
#define WAIT_FOR_PENALTY_POS 12000000
#define WAIT_TIME_POSITION_CORRECTING 2000000

#define ROBOT_ARRIVED_THRESHOLD 0.2

Game::Game(Referee* ref_in, bool is_team_blue_in, RawBall *datBall_in,
           Goalie* goalie_in, Striker* striker1_in, Striker* striker2_in,
           Opponent* opponent1_in, Opponent* opponent2_in, Opponent* opponent3_in)
{
    referee_handler = ref_in;
    is_team_blue = is_team_blue_in;
    datBall = datBall_in;

    goalie = goalie_in;
    striker1 = striker1_in;
    striker2 = striker2_in;
    opponent1 = opponent1_in;
    opponent2 = opponent2_in;
    opponent3 = opponent3_in;
}

void Game::step(bool verbose)
{
    ePlayMode phase = referee_handler->GetPlayMode();
    set_phase(phase, verbose);

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

    return 0;
}

void Game::perform_kick_off()
{
    striker1->GotoXY(datBall->GetX(), datBall->GetY(), 160, true);
}

int Game::take_penalty_position()
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

    return 0;
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

void Game::set_phase(ePlayMode new_phase, bool verbose=true)
{
    previous_phase = current_phase;
    current_phase = new_phase;

    // catch phase transitions
    if (previous_phase != new_phase) {
        if (verbose) {
            cout << "Game Phase transition" << endl;
        }

        // REFEREE_INIT -> BEFORE_KICK_OFF
        if ((previous_phase == 0) && (current_phase == 1)) {
            if (verbose) {
                cout << "Changed from REFEREE_INIT to BEFORE_KICK_OFF" << endl;
            }
            update_side();
            take_kick_off_position();
            referee_handler->SetReady(!is_team_blue);
        }

        // BEFORE_KICK_OFF -> KICK_OFF
        if ((previous_phase == 1) && (current_phase == 2)) {
            if (verbose) {
                cout << "Changed from BEFORE_KICK_OFF to KICK_OFF" << endl;
            }
            update_kick_off();
            if (has_kick_off) {
                perform_kick_off();
            }
        }

        // KICK_OFF -> PLAY_ON
        if ((previous_phase == 2) && (current_phase == 5)) {
            if (verbose) {
                cout << "Changed from KICK_OFF to PLAY_ON" << endl;
            }
        }

        // PLAY_ON -> BEFORE_KICK_OFF
        if ((previous_phase == 5) && (current_phase == 1)) {
            if (verbose) {
                cout << "Changed from PLAY_ON to KICK_OFF" << endl;
            }
            update_side();
            take_kick_off_position();
            referee_handler->SetReady(!is_team_blue);
        }

        // PLAY_ON -> REFEREE_INIT
        if ((previous_phase == 5) && (current_phase == 0)) {
            if (verbose) {
                cout << "Changed from PLAY_ON to REFEREE_INIT" << endl;
            }
        }

        // PLAY_ON -> BEFORE_PENALTY
        if ((previous_phase == 5) && (current_phase == 3)) {
            if (verbose) {
                cout << "Changed from PLAY_ON to BEFORE_PENALTY" << endl;
            }
            update_side();
            update_kick_off();
            take_penalty_position();
        }

        // BEFORE_PENALTY -> PENALTY
        if ((previous_phase == 3) && (current_phase == 4)) {
            if (verbose) {
                cout << "Changed from BEFORE_PENALTY to PENALTY" << endl;
            }
            update_kick_off();
            update_side();
            if (has_kick_off) {
                striker1->shoot_penalty();
            }
            else {
                goalie->go_to_penalty_save_position();
            }
        }

        // PENALTY -> BEFORE_PENALTY
        if ((previous_phase == 4) && (current_phase == 3)) {
            if (verbose) {
                cout << "Changed from PENALTY to BEFORE_PENALTY" << endl;
            }
            update_side();
            update_kick_off();
            take_penalty_position();
        }

        // PENALTY -> REFEREE_INIT
        if ((previous_phase == 4) && (current_phase == 0)) {
            if (verbose) {
                cout << "Changed from PENALTY to REFEREE_INIT" << endl;
            }
        }

        // existing but senseless transitions
        // BEFORE_KICK_OFF -> PENALTY
        if ((previous_phase == 1) && (current_phase == 4)) {
            if (verbose) {
                cout << "Changed from BEFORE_KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        // REFEREE_INIT -> PENALTY
        if ((previous_phase == 0) && (current_phase == 4)) {
            if (verbose) {
                cout << "Changed from REFEREE_INIT to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        // KICK_OFF -> PENALTY
        if ((previous_phase == 2) && (current_phase == 4)) {
            if (verbose) {
                cout << "Changed from KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        // PLAY_ON -> PENALTY
        if ((previous_phase == 5) && (current_phase == 4)) {
            if (verbose) {
                cout << "Changed from PLAY_ON to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }
    }
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
