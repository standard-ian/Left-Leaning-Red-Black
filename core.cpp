/*
 *********************************************************************
 * Ian Leuty
 * inleuty@gmail.com
 * 2/16/2025
 *********************************************************************
 * core hierarchy definition
 *********************************************************************
 *
 */

#include "core.h"

//core.h scope overloaded method to display a contesntant with <<
//by calling overridden:
//      Derived_Contestant.display(ostream&);
std::ostream& operator<<(std::ostream &out, const Contestant &here)
{
    here.display(out);
    return out;
}

/*
 *********************************************************************
 * Contestant class definition
 * Abstract Base Class
 *
 * data members are:
 *      std::string name;
 *      std::string status;
 *      bool disqualified;
 *      int avg_speed;
 *********************************************************************
 */

//default constructor - create a contestant from stdin
Contestant::Contestant(std::string &name_in) : name(name_in), status("REGISTERED"), disqualified(false)
{
    using std::cin, std::cout;

    cout << "Enter " << name << "'s average speed (kilometers/hr).\n>";
    avg_speed = read_int();
}

//file constructor - create a contestant from ifstream
Contestant::Contestant(std::string &name_in, std::ifstream &filein) : name(name_in), status("PRE-REGISTERED"), disqualified(false)
{
    filein >> avg_speed;
    filein.ignore(100, ',');
}

//destructor
Contestant::~Contestant()
{
    name = "";
    status = "";
    disqualified = false;
}

//display the base traits of a contestant
void Contestant::display() const
{
    using std::cout, std::endl, std::setw, std::left;

    if (name == "")
        throw CONTESTANT_ERROR::no_name_exception();

    cout << "| " << left << setw(30) << name << " | " << setw(8) << avg_speed << "km/hr" << " | " << setw(15) << status;
}

//display the base traits of a contestant
//for use with overloaded insertion operator
void Contestant::display(std::ostream &out) const
{
    using std::endl, std::setw, std::left;

    if (name == "")
        throw CONTESTANT_ERROR::no_name_exception();

    out << "| " << left << setw(30) << name << " | " << setw(8) << avg_speed << "km/hr" << " | " << setw(15) << status;
}

//disqualify a contestant
bool Contestant::disqualify()
{
    if (disqualified)
        return false;
    status = "DISQUALIFIED";
    return disqualified = true;
}

//something something getter bad (see structures.cpp line 176 for the only use of this)
std::string Contestant::get_name() const
{
    return name;
}

//compare the name of the contestant with a key passed in
int Contestant::compare_names(const std::string &key)
{
    if (key == "")
        throw CONTESTANT_ERROR::invalid_key_exception();
    return name.compare(key);
}

//compare the name of the contestant with another contestant passed in
int Contestant::compare_names(const Contestant &to_compare)
{
    if (to_compare.name == "")
        throw CONTESTANT_ERROR::invalid_key_exception();
    return name.compare(to_compare.name);
}

//check if the passed in matched the contestants status
bool Contestant::is_status(const std::string &status)
{
    return this -> status.compare(status) == 0;
}

//reads an int in and return it
const int Contestant::read_int()
{
    using std::cout, std::cin, std::endl;

    int the_int{};
    cout << ">";
    cin >> the_int;
    while (cin.fail()){
        cin.clear();
        cin.ignore(100, '\n');
        cout << "\nYou must enter a number." << endl;
        cout << ">";
        cin >> the_int;
    }
    cin.ignore(100, '\n');
    return the_int;
}

/*
 **********************************************************************
 * Walking_Contestant class definition
 *
 * data members are:
 *      int kms_registered;
 *      std::string conversation_topic;
 *      bool tied_shoes;
 **********************************************************************
 */

//default constructor - create a Walking_Contestant from stdin
Walking_Contestant::Walking_Contestant(std::string &name) : Contestant(name), kms_registered(0), tied_shoes(false)
{
    using std::cin, std::cout;

    cout << "Enter " << name << "'s favorite conversation topic.\n>";
    getline(cin, conversation_topic);
}

//file contstructor - create a Walking_Contestant from ifstream
//format: <CONTESTANT TYPE>,<NAME>,<AVG_SPEED>,<CONVERSATION TOPIC>
Walking_Contestant::Walking_Contestant(std::string &name, std::ifstream &filein) : Contestant(name, filein), kms_registered(0), tied_shoes(false)
{
    getline(filein, conversation_topic);
}

//destructor - reset members to null values
Walking_Contestant::~Walking_Contestant()
{
    kms_registered = 0;
    tied_shoes = false;
    conversation_topic = "";
}

//display - virtual and self-similar for all
void Walking_Contestant::display() const
{
    using std::cout, std::endl, std::setw, std::left;

    try {Contestant::display();}

    catch(CONTESTANT_ERROR::no_name_exception &error){
        throw error;
    }

    cout << " | " << left << setw(21) << "Likes to talk about: " << setw(29) << conversation_topic << " | "
         << setw(14) <<  "Registered for: " << setw(3) << kms_registered << setw(16) << "  kms        |" << endl;
}

//display using ostream argument for overloaded insertion
void Walking_Contestant::display(std::ostream &out) const
{
    using std::endl, std::setw, std::left;

    try {Contestant::display();}

    catch(CONTESTANT_ERROR::no_name_exception &error){
        throw error;
    }

    out << " | " << left << setw(21) << "Likes to talk about: " << setw(29) << conversation_topic << " | "
         << setw(14) <<  "Registered for: " << setw(3) << kms_registered << setw(16) << "  kms        |" << endl;
}

//start a Walking_Contestant - set their status to "WALKING"
bool Walking_Contestant::start()
{
    if (disqualified){
        status = "DISQUALIFIED";
        Contestant::disqualify();
        return false;
    }

    if (status.compare("CHECKED IN") != 0){
        Contestant::disqualify();
        if (!tied_shoes)
            status = "INJURED";
        return false;
    }

    status = "WALKING";
    return true;
}

//check in a contestant
bool Walking_Contestant::check_in()
{
    using std::cin, std::cout, std::endl;

    char tied{};
    cout << "\nConfirm the distance " << name << " will be walking today (kilometers).\n>";
    kms_registered = read_int();
    if (kms_registered > 20 || kms_registered < 1){
        cout << "\nContestant cannot register for more than 20 kms as a walker. Exiting." << endl;
        return false;
    }
    cout << "Are " << name << "'s shoes ties?\n>";
    cin >> tied;
    cin.ignore(100, '\n');
    if (toupper(tied) == 'Y')
        tied_shoes = true;
    else
        cout << "\nWarning, untied shoes are a hazard." << endl;

    status = "CHECKED IN";
    return true;
}

//calculate percentage of race complete
//takes a time (minutes)
float Walking_Contestant::predict_completion(int time)
{
    float hours{static_cast<float>(time) / 60};
    return ((hours * static_cast<float>(avg_speed)) / static_cast<float>(kms_registered)) * 100;
}



/*
 **********************************************************************
 * Bicycle_Contestant class definition
 *
 * data members are:
 *      bool signed_waiver;
 *      int race_stages;
 *      std::string fav_bike;
 *      std::string emergency_contact;
 **********************************************************************
 */

//default constructor - creates a Bicycle_Contestant from stdin
Bicycle_Contestant::Bicycle_Contestant(std::string &name) : Contestant(name), race_stages(0), signed_waiver(false)
{
    using std::cout, std::cin;

    cout << "Enter a description of " << name << "'s bike.\n>";
    getline(cin, fav_bike);
}

//file constructor - creates a Bicycle_Contestant from ifstream
//format: <CONTESTANT TYPE>,<NAME>,<AVG_SPEED>,<FAV BIKE>
Bicycle_Contestant::Bicycle_Contestant(std::string &name, std::ifstream &filein) : Contestant(name, filein), race_stages(0), signed_waiver(false)
{
    getline(filein, fav_bike);
}

//destrutor
Bicycle_Contestant::~Bicycle_Contestant()
{
    fav_bike = "";
    emergency_contact = "";
    race_stages = 0;
    signed_waiver = false;
}

//displays a base contestant plus attributes of a Bicycle_Contestant
void Bicycle_Contestant::display() const
{
    using std::cout, std::endl, std::setw, std::left;

    try {Contestant::display();}

    catch(CONTESTANT_ERROR::no_name_exception &error){
        throw error;
    }

    cout << " | " << left << setw(18) << "Bike description: " << setw(32) << fav_bike << " | "
         << setw(14) << "Registered for: " << setw(3) << race_stages << setw(9) << " 3km stages  |" << endl;
}

//displays a base contestant plus attributes of a Bicycle_Contestant with ostream arument
void Bicycle_Contestant::display(std::ostream &out) const
{
    using std::endl, std::setw, std::left;

    try {Contestant::display();}

    catch(CONTESTANT_ERROR::no_name_exception &error){
        throw error;
    }

    out << " | " << left << setw(18) << "Bike description: " << setw(32) << fav_bike << " | "
         << setw(14) << "Registered for: " << setw(3) << race_stages << setw(9) << " 3km stages  |" << endl;
}

//start a Bicycle_Contestant - set status to "RIDING"
bool Bicycle_Contestant::start()
{
    if (disqualified || status.compare("CHECKED IN") != 0 || !signed_waiver){
        status = "DISQUALIFIED";
        Contestant::disqualify();
        return false;
    }

    status = "CYCLING";
    return true;
}

//check in a bicycle contestant
bool Bicycle_Contestant::check_in()
{
    using std::cin, std::cout;

    char check{};
    cout << "\nConfirm the number of 3km stages " << name << " will be completing today\n>";
    race_stages = read_int();
    while (race_stages > 10 || race_stages < 2){
        cout << "Contestant must sign up for 2 - 10 3km race stages. Enter a value in this range.\n";
        race_stages = read_int();
    }

    cout << "Has " << name << " signed the waiver? (y/n)\n>";
    cin >> check;
    cin.ignore(100, '\n');
    if (toupper(check) == 'Y')
        signed_waiver = true;
    else{
        status = "DISQUALIFIED";
        Contestant::disqualify();
        return false;
    }

    cout << "Enter an emergency contact for " << name << " (optional)\n>";
    getline(cin, emergency_contact);

    status = "CHECKED IN";
    return true;

}

//return a percent completion based on 3km stages inteded to complete and avg_speed
float Bicycle_Contestant::predict_completion(int time)
{
    float hours{static_cast<float>(time) / 60};
    return ((hours * static_cast<float>(avg_speed)) / (3 * race_stages)) * 100;
}



/*
 *********************************************************************
 * Half_Marathon_Contestant class definition
 *
 * data members are:
 *      int racer_number;
 *      int hydration_level;
 *      bool record_holder;
 *      int previous_best;
 *********************************************************************
 */

//default constructor, creates a Half_Marathon_Contestant from stdin
Half_Marathon_Contestant::Half_Marathon_Contestant(std::string &name) : Contestant(name), racer_number(rand() % 999), hydration_level(50), record_holder(false)
{
    using std::cout, std::cin;

    cout << "Enter " << name << "'s previous best time for the half marathon (21k) in minutes.\n>";
    previous_best = read_int();
}

//file constructor, creates a Half_Marathon_Contestant from ifstream
//format: <CONTESTANT TYPE>,<NAME>,<AVG_SPEED>,<PREVIOUS BEST>,<RACER NUMBER>
Half_Marathon_Contestant::Half_Marathon_Contestant(std::string &name, std::ifstream &filein) : Contestant(name, filein), hydration_level(50), record_holder(false)
{
    filein >> previous_best;
    filein.ignore(100, ',');
    filein >> racer_number;
    filein.ignore(100, '\n');
}

//destructor
Half_Marathon_Contestant::~Half_Marathon_Contestant()
{
    racer_number = 0;
    hydration_level = 0;
    record_holder = false;
    previous_best = 0;
}

//display a base Contestant plus unique attributes of a Half_Marathon_Contestant
void Half_Marathon_Contestant::display() const
{
    using std::cout, std::endl, std::setw, std::left;

    try {Contestant::display();}

    catch(CONTESTANT_ERROR::no_name_exception &error){
        throw error;
    }

    cout << " | " << left << setw(10) << "Number: " << setw(5) << racer_number << " | "
         << setw(20) << "Previous best time: " << setw(4) << previous_best << setw(10) << " minutes | ";
    cout << "\033[38;5;39m" << setw(3) << hydration_level << '%' << " hydrated" << "\033[0;0m" << "  | ";
    if (record_holder)
        cout << left << "RECORD HOLDER!" << " | " << endl;
    else
        cout << "               | " << endl;
}

//display a base Contestant plus unique attributes of a Half_Marathon_Contestant with ostream argument
void Half_Marathon_Contestant::display(std::ostream &out) const
{
    using std::endl, std::setw, std::left;

    try {Contestant::display();}

    catch(CONTESTANT_ERROR::no_name_exception &error){
        throw error;
    }

    out << " | " << left << setw(10) << "Number: " << setw(5) << racer_number << " | "
         << setw(20) << "Previous best time: " << setw(4) << previous_best << setw(10) << " minutes | ";
    out << "\033[38;5;39m" << setw(3) << hydration_level << '%' << " hydrated" << "\033[0;0m" << "  | ";
    if (record_holder)
        out << left << "RECORD HOLDER!" << " | " << endl;
    else
        out << "               | " << endl;
}

//unique member function for Half_Marathon_Contestant
bool Half_Marathon_Contestant::hydrate()
{
    using std::cout, std::cin, std::endl, std::setw;

    if (hydration_level < 100){
        char drink{};
        do{
            hydration_level += 10;
            cout << "\nGULP!" << endl;
            cout << "\n\033[38;5;39m" << setw(3) << hydration_level << '%' << " hydrated" << "\033[0;0m" << endl;
            cout << "\nDrink more? (y/n)\n>";
            drink = read_int();
        } while (toupper(drink) == 'Y' && hydration_level < 100);
        if (hydration_level == 100)
            cout << "\n" << name << " is fully hydrated." << endl;
        else
            cout << "\n" << name << " is " << hydration_level << "% hydrated." << endl;
        return true;
    }
    cout << "\n" << name << " is already full hydrated!" << endl;
    return false;
}

bool Half_Marathon_Contestant::start()
{
    if (disqualified || status.compare("CHECKED IN") != 0 || racer_number == 0){
        status = "DISQUALIFIED";
        Contestant::disqualify();
        return false;
    }

    status = "RUNNING";
    return true;
}

//check in a Half_Marathon_Contestant
bool Half_Marathon_Contestant::check_in()
{
    using std::cin, std::cout, std::endl;

    char confirm{};
    cout << "\nIs " << name << " a record holder?\n>";
    cin >> confirm;
    cin.ignore(100, '\n');
    if (toupper(confirm) == 'Y')
        record_holder = true;
    cout << "Estimate " << name << "'s current hydration level.\n>";
    cin >> hydration_level;
    cin.ignore(100, '\n');
    if (hydration_level < 20){
        cout << name << " is too dehydrated to check in now." << endl;
        return false;
    }
    if (hydration_level > 100 || hydration_level < 0)
        hydration_level = 50;
    status = "CHECKED IN";
    return true;

}

//predict completion percentage of the Half_Marathon_Contestant based on hydration level, average of previous best, and avg_speed
float Half_Marathon_Contestant::predict_completion(int time)
{
    float hours{static_cast<float>(time) / 60};
    float adjusted_speed{(static_cast<float>(avg_speed) + (static_cast<float>(previous_best) / 21) / 2)};
    adjusted_speed *= (100 / hydration_level);
    return ((hours * adjusted_speed) / 21) * 100;
}

