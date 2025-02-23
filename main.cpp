/*
 *********************************************************************
 * Ian Leuty
 * inleuty@gmail.com
 * 2/16/2025
 *********************************************************************
 * main
 *********************************************************************
 */

#include "application.h"
using namespace std;

/*
 *********************************************************************
 * main
 * a "Menu" has the following:
 *       void splash();
 *
 *       void load_contestants();
 *       void display_contestants();
 *       void register_contestants();
 *       void find_contestant();
 *       void hydrate_runner();
 *       void unregister();
 *       void estimate_completion();
 *       void check_in();
 *       void start_race();
 *       void disqualify();
 *       void graphical_tree();
 *       void test_copying();
 *       void animate();
 *       void animate_load();
 *       void animate_removal();
 *
 *       const int read_int();
 *       bool again();
 *********************************************************************
 */

int main(void)
{
    int choice{};

    srand(time(0));

    Menu run;

    run.splash();

    do{
        cout << "\n\nPlease select an action."
             << "\n0.  Quit."

             << "\n\n1.  Load contestants from a roster."
             << "\n2.  Manually register contestant(s)."
             << "\n3.  Estimate a contestant's progress."
             << "\n4.  Hydrate a half marathoner."
             << "\n5.  Check in a contestant."
             << "\n6.  Start a race."
             << "\n7.  Unregister a contestant."
             << "\n8.  Find out if a contestant is registered."
             << "\n9.  View registered contestants."
             << "\n10. Disqualify a contestant."
             << "\n11. Display a graphical representation of the underlying Red Black Tree."
             << "\n12. Test assignment operator."
             << "\n13. View animated tree insertion. (WARNING: takes ~2 minutes with 100 items.)"

             << "\n>";

        choice = run.read_int();

        switch (choice){
            case 1:
                run.load_contestants();
                break;
            case 2:
                run.register_contestants();
                break;
            case 3:
                run.estimate_completion();
                break;
            case 4:
                run.hydrate_runner();
                break;
            case 5:
                run.check_in();
                break;
            case 6:
                run.start_race();
                break;
            case 7:
                run.unregister();
                break;
            case 8:
                run.find_contestant();
                break;
            case 9:
                run.display_contestants();
                break;
            case 10:
                run.disqualify();
                break;
            case 11:
                run.graphical_tree();
                break;
            case 12:
                run.test_copying();
                break;
            case 13:
                run.animate();
                break;
            default:
                break;
        }
    } while (choice);


    return 0;
}

