/*
 *********************************************************************
 * Ian Leuty
 * inleuty@gmail.com
 * 2/16/2025
 *********************************************************************
 * application declaration
 *********************************************************************
 * These are available to be called from any "Menu" object.
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
 *********************************************************************
 */
#include <unistd.h>
#include "structures.h"
#include "core.h"

//exceptions related to the application
struct APPLICATION_ERROR
{
    struct no_file_exception{
        std::string msg{"\nFailed to open the file.\n"};
    };
};

//main program interface
//all void returning becase this is the highest (current) level of abstraction
//and these are just cleaner wrappers for the Red_Black tree template
//easy enough to modify later if used in a larger application
class Menu
{
    public:
        Menu();
        void splash();

        void load_contestants();
        void display_contestants();
        void register_contestants();
        void find_contestant();
        void hydrate_runner();
        void unregister();
        void estimate_completion();
        void check_in();
        void start_race();
        void disqualify();
        void graphical_tree();
        void test_copying();
        void animate();
        void animate_load();
        void animate_removal();
        const int read_int();
        bool again();

    protected:
        //instantiation of the Red_Black tree template using
        //a string key (name) and a Contestant smart pointer.
        //use dynamic_pointer_cast on shared_ptr when downcasting
        Red_Black<std::string, std::shared_ptr<Contestant>> tree;

        //markers for if these races have started aready
        bool cycling{}, walking{}, running {};

        //for reading in from a file
        std::ifstream filein;
        std::string filename;

        //utility functions used by the application interface
        int load();
        bool reg();
        void check(const std::string &name);
};

