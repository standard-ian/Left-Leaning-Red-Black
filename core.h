/*
 *********************************************************************
 * Ian Leuty
 * inleuty@gmail.com
 * 2/16/2025
 *********************************************************************
 * core hierarchy declaration
 *********************************************************************
 * Shamrock Run
 * Half marathon, 8K, 6K, 5K, "stride" walk, Leprechaun Lap (1K)
 * Track contestants during the race
 * All race options treated the same (use dynamic binding)
 *
 * Need to support:
 *      - 3 "Contenstants"
 *      - 3 "Races" with self similar functionality
 *          -register
 *          -start
 *          -stop
 *          -quit
 *          -take break
 *          -win
 *
 * Benchmarks:
 *      - Progress Submission 1:
 *          -Select 3 race type contestants (bicycle race, costume race, half marathon)
 *          -2 (min) common characteristics is base class
 *          -Base class must be ABC with at least 1 pure virtual function
 *          -2 (min) unique characteristics for each derived races as a private data member.
 *          -3 self similar functions in the core hierarchy - same headers, different implentation
 *          -1 (min) different function in just one of the derived classes to experience RTTI and downcasting
 *
 *      - Progress Submission 2:
 *          -Data structure for players across all races in a single data structure
 *          -Red black tree with nodes - UNIQUE key value for each node
 *          -Node has a smart pointer to the base class (Contestant) to experience dynamic binding and upcasting
 *
 *********************************************************************
 */


#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <vector>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <ctime>

//exceptions related to the core hierarchy
struct CONTESTANT_ERROR
{
    struct invalid_key_exception{
        std::string msg{"\nInvalid key.\n"};
    };
    struct no_name_exception{
        std::string msg{"\nContestant has no name set.\n"};
    };
};

//abstract contestant base class
class Contestant
{
    public:
        Contestant();
        Contestant(std::string &name);
        Contestant(std::string &name, std::ifstream &filein);

        friend std::ostream& operator<<(std::ostream &out, const Contestant &here);
        virtual ~Contestant();
        virtual void display() const;
        virtual void display(std::ostream &out) const;
        virtual bool start() = 0;
        virtual bool check_in() = 0;
        virtual float predict_completion(int time) = 0;

        bool set_winner();
        bool disqualify();
        std::string get_name() const;
        int compare_names(const std::string &key);
        int compare_names(const Contestant &to_compare);
        bool is_status(const std::string &status);

    protected:
        std::string name;
        std::string status;
        const int read_int();
        int avg_speed;
        bool disqualified;
};

//derived contestant - walking
class Walking_Contestant: public Contestant
{
    public:
        Walking_Contestant();
        Walking_Contestant(std::string &name);
        Walking_Contestant(std::string &name, std::ifstream &filein);
        ~Walking_Contestant();
        void display() const;
        void display(std::ostream &out) const;
        bool start();
        bool check_in();
        float predict_completion(int time);

    protected:
        int kms_registered;
        std::string conversation_topic;
        bool tied_shoes;
};

//derived contestant - bicycle
class Bicycle_Contestant : public Contestant
{
    public:
        Bicycle_Contestant();
        Bicycle_Contestant(std::string &name);
        Bicycle_Contestant(std::string &name, std::ifstream &filein);
        ~Bicycle_Contestant();
        void display() const;
        void display(std::ostream &out) const;
        bool start();
        bool check_in();
        float predict_completion(int time);

    protected:
        int race_stages;
        bool signed_waiver;
        std::string fav_bike;
        std::string emergency_contact;
};

//derived contestant marathon runner
class Half_Marathon_Contestant : public Contestant
{
    public:
        Half_Marathon_Contestant();
        Half_Marathon_Contestant(std::string &name);
        Half_Marathon_Contestant(std::string &name, std::ifstream &filein);
        ~Half_Marathon_Contestant();
        void display() const;
        void display(std::ostream &out) const;
        bool start();
        bool hydrate();
        bool check_in();
        float predict_completion(int time);

    protected:
        int racer_number;
        int hydration_level;
        bool record_holder;
        int previous_best;
};

