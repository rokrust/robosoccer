#include "game.h"

#define DEBUG 1

Game::Game(Referee* ref_in,
           Robot* blue1_in, Robot* blue2_in, Robot* blue3_in,
           Robot* red1_in, Robot* red2_in, Robot* red3_in)
{
    referee_handler = ref_in;
    blue1 = blue1_in;
    blue2 = blue2_in;
    blue3 = blue3_in;
    red1 = red1_in;
    red2 = red2_in;
    red3 = red3_in;
}

void Game::set_phase(ePlayMode new_phase)
{
    previous_phase = current_phase;
    current_phase = new_phase;

    // catch phase transitions
    if (previous_phase != new_phase) {
        if (DEBUG) {
            cout << "Game Phase transition" << endl;
        }

        if ((previous_phase == 0) && (current_phase == 1)) {
            // REFEREE_INIT -> BEFORE_KICK_OFF
            if (DEBUG) {
                cout << "Changed from REFEREE_INIT to BEFORE_KICK_OFF" << endl;
            }
        }

        if ((previous_phase == 1) && (current_phase == 2)) {
            // BEFORE_KICK_OFF -> KICK_OFF
            if (DEBUG) {
                cout << "Changed from BEFORE_KICK_OFF to KICK_OFF" << endl;
            }
        }

        if ((previous_phase == 2) && (current_phase == 5)) {
            // KICK_OFF -> PLAY_ON
            if (DEBUG) {
                cout << "Changed from KICK_OFF to PLAY_ON" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 1)) {
            // PLAY_ON -> BEFORE_KICK_OFF
            if (DEBUG) {
                cout << "Changed from PLAY_ON to KICK_OFF" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 0)) {
            // PLAY_ON -> REFEREE_INIT
            if (DEBUG) {
                cout << "Changed from PLAY_ON to REFEREE_INIT" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 3)) {
            // PLAY_ON -> BEFORE_PENALTY
            if (DEBUG) {
                cout << "Changed from PLAY_ON to BEFORE_PENALTY" << endl;
            }
        }

        if ((previous_phase == 3) && (current_phase == 4)) {
            // BEFORE_PENALTY -> PENALTY
            if (DEBUG) {
                cout << "Changed from BEFORE_PENALTY to PENALTY" << endl;
            }
        }

        if ((previous_phase == 4) && (current_phase == 3)) {
            // PENALTY -> BEFORE_PENALTY
            if (DEBUG) {
                cout << "Changed from PENALTY to BEFORE_PENALTY" << endl;
            }
        }

        if ((previous_phase == 4) && (current_phase == 0)) {
            // PENALTY -> REFEREE_INIT
            if (DEBUG) {
                cout << "Changed from PENALTY to REFEREE_INIT" << endl;
            }
        }

        // existing but senseless transitions
        if ((previous_phase == 1) && (current_phase == 4)) {
            // BEFORE_KICK_OFF -> PENALTY
            if (DEBUG) {
                cout << "Changed from BEFORE_KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        if ((previous_phase == 0) && (current_phase == 4)) {
            // REFEREE_INIT -> PENALTY
            if (DEBUG) {
                cout << "Changed from REFEREE_INIT to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        if ((previous_phase == 2) && (current_phase == 4)) {
            // KICK_OFF -> PENALTY
            if (DEBUG) {
                cout << "Changed from KICK_OFF to PENALTY" << endl;
                cout << "--Exceptional Transition--" << endl;
            }
        }

        if ((previous_phase == 5) && (current_phase == 4)) {
            // PLAY_ON -> PENALTY
            if (DEBUG) {
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

void Game::step()
{
    ePlayMode phase = referee_handler->GetPlayMode();
    set_phase(phase);

    if (DEBUG) {
        bool display = true;
        get_phase(display);
        cin.get();
    }
}
