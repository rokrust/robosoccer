#include "soccer_tests.h"

Soccer_Tests::Soccer_Tests(RoboControl* blue1_in, RoboControl* blue2_in, RoboControl* blue3_in,
                           RoboControl* red1_in, RoboControl* red2_in, RoboControl* red3_in)
{
    blue1 = blue1_in;
    blue2 = blue2_in;
    blue3 = blue3_in;
    red1 = red1_in;
    red2 = red2_in;
    red3 = red3_in;
}

void Soccer_Tests::move_in_out()
{
    /** Define outer positions for all six robots */
    Position posb0_out(-0.5, -0.6);
    Position posb1_out(0.0, -0.6);
    Position posb2_out(0.5, -0.6);
    Position posr0_out(-0.5, 0.6);
    Position posr1_out(0.0, 0.6);
    Position posr2_out(0.5, 0.6);

    /** Define inner positions for all six robots */
    Position posb0_in(-0.2, -0.1);
    Position posb1_in(0.0, -0.1);
    Position posb2_in(0.2, -0.1);
    Position posr0_in(-0.2, 0.1);
    Position posr1_in(0.0, 0.1);
    Position posr2_in(0.2, 0.1);

    int loop = 1;

    while (loop) {
            cout << "Moving all six robots to the outer positions" << endl;

            blue1->GotoXY(posb0_out.GetX(), posb0_out.GetY(), 40, true);
            blue2->GotoXY(posb1_out.GetX(), posb1_out.GetY(), 40, true);
            blue3->GotoXY(posb2_out.GetX(), posb2_out.GetY(), 40, true);
            red1->GotoXY(posb0_out.GetX(), posr0_out.GetY(), 40, true);
            red2->GotoXY(posb1_out.GetX(), posr1_out.GetY(), 40, true);
            red3->GotoXY(posb2_out.GetX(), posr2_out.GetY(), 40, true);

            usleep(6000000);
            cout << "Moving all six robots to the inner positions" << endl;

            blue1->GotoXY(posb0_out.GetX(), posb0_in.GetY(), 40, true);
            blue2->GotoXY(posb1_out.GetX(), posb1_in.GetY(), 40, true);
            blue3->GotoXY(posb2_out.GetX(), posb2_in.GetY(), 40, true);
            red1->GotoXY(posb0_out.GetX(), posr0_in.GetY(), 40, true);
            red2->GotoXY(posb1_out.GetX(), posr1_in.GetY(), 40, true);
            red3->GotoXY(posb2_out.GetX(), posr2_in.GetY(), 40, true);

            //loop = 0;
            usleep(6000000);
    }
}
