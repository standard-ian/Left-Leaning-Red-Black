/*
 * Ian Leuty
 * ileuty@pdx.edu
 * 2/16/2025
 * CS302 Winter 2025
 * Program #3
 * ******************************************************************** * * * application definition
 *
 *********************************************************************
 */

/*
 * menu class implementation
 * data menmbers are:
 ***************************************************************************
 */
#include "application.h"
using namespace std;

//default constructor
Menu::Menu() : filename("") {}
//splash screen and menu entry
void Menu::prompt()
{
    int choice{};

    //push up to top of term window and print welcome
cout << "\033[2J\033[1;1H" << "\033[38;3;39m" << endl
         << R"(

            ███████╗██╗  ██╗ █████╗ ███╗   ███╗██████╗  ██████╗  ██████╗██╗  ██╗    ██████╗ ██╗   ██╗███╗   ██╗
            ██╔════╝██║  ██║██╔══██╗████╗ ████║██╔══██╗██╔═══██╗██╔════╝██║ ██╔╝    ██╔══██╗██║   ██║████╗  ██║
            ███████╗███████║███████║██╔████╔██║██████╔╝██║   ██║██║     █████╔╝     ██████╔╝██║   ██║██╔██╗ ██║
            ╚════██║██╔══██║██╔══██║██║╚██╔╝██║██╔══██╗██║   ██║██║     ██╔═██╗     ██╔══██╗██║   ██║██║╚██╗██║
            ███████║██║  ██║██║  ██║██║ ╚═╝ ██║██║  ██║╚██████╔╝╚██████╗██║  ██╗    ██║  ██║╚██████╔╝██║ ╚████║
            ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝  ╚═╝ ╚═════╝  ╚═════╝╚═╝  ╚═╝    ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═══╝)"

         << "\033[0;0m" << endl;

    cout << "\nRace Day! Please be aware of the following registration rules:"
         << "\nA. Racers must be checked in before a race starts, or they will be disqualified or potentially injured."
         << "\nB. Hydrating a marathoner will help them race faster!"
         << "\nC. Races may be started only once." << endl;
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
        choice = read_int();
        switch (choice){
            case 1:
                load_contestants();
                break;
            case 2:
                register_contestants();
                break;
            case 3:
                estimate_completion();
                break;
            case 4:
                hydrate_runner();
                break;
            case 5:
                check_in();
                break;
            case 6:
                start_race();
                break;
            case 7:
                unregister();
                break;
            case 8:
                find_contestant();
                break;
            case 9:
                display_contestants();
                break;
            case 10:
                disqualify();
                break;
            case 11:
                graphical_tree();
                break;
            case 12:
                test_copying();
                break;
            case 13:
                animate();
                break;
            default:
                break;
        }
    } while (choice);

}

//clean application interface for loading contestants
void Menu::load_contestants()
{
    cout << "\nLoad Contestants." << endl;

    bool is_loaded{false};
    int loaded{};
    while (!is_loaded){
        try{
            loaded = load();
            is_loaded = true;
        }
        catch (APPLICATION_ERROR::no_file_exception &error){
            cout << error.msg << "Try ";
            is_loaded = !again();
        }
    }
    cout << "\n" << loaded << " contestants were loaded from the roster." << endl;
}

//load a roster of contestants from a file
int Menu::load()
{
    int num_loaded{};
    cout << "\nEnter the name of the contestant roster file.\n>";
    getline(cin, filename);

    //throw exception if file not opened
    filein.open(filename);
    if (!filein)
        throw APPLICATION_ERROR::no_file_exception();

    filein.peek();

    while (!filein.eof()){
        string name{};
        int type{};

        //first check the file for the type so correct initial object can be contstructed
        filein >> type;
        filein.ignore(100, ',');

        getline(filein, name, ',');
        try{
            switch (type){
                case 1: {
                            auto walk_ptr{make_shared<Walking_Contestant>(name, filein)};
                            num_loaded += tree.insert(name, move(walk_ptr));
                        }
                    break;

                case 2: {
                            auto b_ptr{make_shared<Bicycle_Contestant>(name, filein)};
                            num_loaded += tree.insert(name, move(b_ptr));
                        }
                    break;

                case 3: {
                            auto hm_ptr{make_shared<Half_Marathon_Contestant>(name, filein)};
                            num_loaded += tree.insert(name, move(hm_ptr));
                        }
                    break;

                default:
                    break;
            }
            filein.peek();
        }
        catch (TREE_ERROR::duplicate_name_exception &error){
            cout << error.msg << "Skipping...." << endl;
        }
    }
    filein.close();
    return num_loaded;
}

//register one or many contestants
void Menu::register_contestants()
{
    cout << "\nRegister contestant(s)" << endl;
    int num_registered{};

    do{
        num_registered += reg();

        cout << "\nRegister ";
    } while (again());
    cout << "\n" << num_registered << " contestants were registered in person." << endl;

}

//manually register 1 contestant from stdin
bool Menu::reg()
{
    cout << "\nWhat type of contestant is registering?" << endl;
    cout << "1. Walking\n2. Bicycling\n3. Half Marathon\n>";
    bool success{};
    int choice{read_int()};
    string name{};

    cout << "\nEnter the contestant's name.\n>";
    getline(cin, name);
    try{
        switch (choice){
                case 1: {
                            auto walk_ptr{make_shared<Walking_Contestant>(name)};
                            success = tree.insert(name, move(walk_ptr));
                        }
                    break;

                case 2: {
                            auto cycle_ptr{make_shared<Bicycle_Contestant>(name)};
                            success = tree.insert(name, move(cycle_ptr));
                            auto test = tree.retrieve(name);
                            cout << *test;
                        }
                    break;

                case 3: {
                            auto hm_ptr{make_shared<Half_Marathon_Contestant>(name)};
                            success = tree.insert(name, move(hm_ptr));
                        }
                    break;

                default:
                    break;
        }
        if (success){
            cout << "\nContestant registered." << endl;
            return 1;
        }
        else{
            cout << "\nContestant could not be registered." << endl;
            return 0;
        }
    }

    catch (TREE_ERROR::duplicate_name_exception &error) {
        cout << error.msg;
        return false;
    }
}

//clean interface for client to display
void Menu::display_contestants()
{
    int displayed{};
    cout << "\nDisplay Contestants." << endl;
    vector<shared_ptr<Contestant>> contestants;
    tree.fetch_data(contestants);
    do{
        int choice{};
        cout << "\nWhich registration to view?"
             << "\n0. Return to the main menu."
             << "\n1. Walking contestants."
             << "\n2. Bicycle contestants."
             << "\n3. Half marathon contestants."
             << "\n4. All contestants."
             << "\n>";
        choice = read_int();
        switch (choice){
            case 1:
                for (const auto &item : contestants){
                    if (auto w_ptr{dynamic_pointer_cast<Walking_Contestant>(item)}){
                        ++displayed;
                        cout << *w_ptr;
                    }
                }
                break;
            case 2:
                for (const auto &item : contestants){
                    if (auto b_ptr{dynamic_pointer_cast<Bicycle_Contestant>(item)}){
                        ++displayed;
                        cout << *b_ptr;
                    }
                }
                break;
            case 3:
                for (const auto &item : contestants){
                    if (auto hm_ptr{dynamic_pointer_cast<Half_Marathon_Contestant>(item)}){
                        ++displayed;
                        cout << *hm_ptr;
                    }
                }
                break;
            case 4:
                displayed = tree.display();
                break;

            default:
                break;
        }
        cout << "\n" << displayed << " contestants are registered for that category." << endl;

        cout << "\nDisplay ";
    } while(again());
}

//just searches for a contestant to see if they've registered
void Menu::find_contestant()
{
    cout << "\nFind contestant(s)" << endl;
    do{
        string name;
        cout << "\nEnter a contstant's name to check if they're registered.\n>";
        getline(cin, name);
        if (tree.find(name))
            cout << "\n" << name << " is registered." << endl;
        else
            cout << "\n" << name << " is not registered." << endl;

        cout << "\nSearch ";
    } while (again());

}

//retrieve a runner and use RTTI to hydrate them if they are a runner
void Menu::hydrate_runner()
{
    do{
        string name;
        cout << "\nEnter a runner's name to hydrate them.\n>";
        getline(cin, name);
        try{
            //use the Red_Black retrieve function which returns a unique_ptr reference to the player at a node
            //dynamic cast to ID the player type and allow them to hydrate properly if a runner....or..... :)
            auto w_ptr{dynamic_pointer_cast<Walking_Contestant>(tree.retrieve(name))};
            auto hm_ptr{dynamic_pointer_cast<Half_Marathon_Contestant>(tree.retrieve(name))};

            if (hm_ptr)
                hm_ptr -> hydrate();
            else if (w_ptr)
                cout << "\nSorry, the water is only for runners. Try the Benson Bubblers?" << endl;
            else
                cout << "\nSorry, cyclists cannot be hydrated with normal water, only beer." << endl;
        }
        //the search terminates in a sentinel node meaning the player was not present
        catch (TREE_ERROR::not_found_exception &error){
            cout << error.msg;
        }
        cout << "\nHydrate ";
    } while (again());
}

//unrgister a number of contestants
void Menu::unregister()
{
    int unregistered{};
    cout << "\nUnregister contestant(s)." << endl;
    do{
        string name;
        cout << "\nEnter a contstant's name to remove them from the registration.\n>";
        getline(cin, name);
        if (tree.remove(name)){
            cout << "\n" << name << " was removed." << endl;
            ++unregistered;
        }
        else
            cout << "\n" << name << " was not found." << endl;

        cout << "\nUnregister ";
    } while (again());
    cout << "\n" << unregistered << " contestants were unregistered." << endl;
}

//retrieve (a) contestant(s) and use their average speed as well as race specifics to estimate completion
void Menu::estimate_completion()
{
    cout << "\nEstimate a contestant's progress in their race." << endl;
    do{
        char choice{};
        string name;
        cout << "\nEnter a contstant's name\n>";
        getline(cin, name);

        if (tree.find(name)){
            cout << "\n" << name << " is registered." << endl;
            if (!tree.retrieve(name) -> is_status("CHECKED IN")){
                cout << "\nNot enough details to estimate " << name << "'s completion.\n"
                     << "This contestant must first check in.\n"
                     << "Check in now? (y/n)\n>";
                cin >> choice;
                cin.ignore(100, '\n');
                if (toupper(choice) == 'Y')
                    check(name);
            }

            else{
                cout << "\nEnter the elapsed time since the start of their race (minutes)." << endl;
                int time_from_start{read_int()};
                float completion{tree.retrieve(name) -> predict_completion(time_from_start)};
                if (completion < 100 && completion > 0)
                    cout << "\n" << name << "'s completion percentage will be "
                         << completion << "% after " << time_from_start << " minutes." << endl;
                else if (completion >= 100)
                    cout << "\n" << name << " will have finished the race after "
                         << time_from_start << " minutes." << endl;
            }
        }

        else{
            cout << "\n" << name << " is not registered. Register them now?\n>";
            cin >> choice;
            cin.ignore(100, '\n');
            if (toupper(choice) == 'Y')
                reg();
        }

        cout << "\nEstimate ";
    } while (again());
}

//check in multiple contestants
void Menu::check_in()
{
    cout << "\nCheck in contestant(s)." << endl;
    do{
        string name;
        cout << "\nEnter a contstant's name\n>";
        getline(cin, name);
        check(name);

        cout << "\nCheck in ";
    } while (again());
}

//check in a single contestant
void Menu::check(const string &name)
{
    char choice{};
    if (tree.find(name)){
        cout << "\n" << name << " is registered." << endl;
        if (tree.retrieve(name) -> is_status("CHECKED IN"))
            cout << "\n" << name << " was already checked in." << "\n" << endl;
        else if (tree.retrieve(name) -> check_in()){
            cout << "\n" << name << " has been checked in." << "\n" << endl;
            cout << *tree.retrieve(name);
        }
    }
    else{
        cout << "\n" << name << " is not registered. Would you like to register them? (y/n)\n>";
        cin >> choice;
        cin.ignore(100, '\n');
        if (toupper(choice) == 'Y')
            reg();
    }
}

//start a particular race
//use RTTI to find correct contestants and mark them as started.
void Menu::start_race()
{
    vector<shared_ptr<Contestant>> contestants;
    tree.fetch_data(contestants);
    cout << "\nStart race(s)." << endl;
    do{
        int choice{};
        cout << "\nWhich race is starting?" << endl;
        cout << "1. Walking\n2. Bicycling\n3. Half Marathon\n>";
        cin >> choice;
        cin.ignore(100, '\n');

        switch (choice){
            case 1:
                    if (!walking){
                        walking = true;
                        for (auto &item : contestants){
                            if (auto w_ptr{dynamic_pointer_cast<Walking_Contestant>(item)})
                                w_ptr -> start();
                        }
                    }
                    else
                        cout << "\nThe walking race has already started." << endl;
                break;
            case 2:
                    if (!cycling){
                        cycling = true;
                        for (auto &item : contestants){
                            if (auto b_ptr{dynamic_pointer_cast<Bicycle_Contestant>(item)})
                                b_ptr -> start();
                        }
                    }
                    else
                        cout << "\nThe cycling race has already started." << endl;
                break;

            case 3:
                    if (!running){
                        running = true;
                        for (auto &item : contestants){
                            if (auto hm_ptr{dynamic_pointer_cast<Half_Marathon_Contestant>(item)})
                                hm_ptr -> start();
                        }
                    }
                    else
                        cout << "\nThe half marathon has already started." << endl;
                break;

            default:
                break;
        }
        cout << "\nSelect ";
    } while (again() && (!cycling || !walking || !running));
}

//disqualify contestant(s)
void Menu::disqualify()
{
    cout << "\nDisqualify contestant(s)" << endl;
    do{
        string name;
        cout << "\nEnter a contstant's name\n>";
        getline(cin, name);
        if (tree.find(name)){
            cout << "\n" << name << " is registered." << endl;
            if (tree.retrieve(name) -> is_status("DISQUALIFIED"))
                cout << "\n" << name << " was already disqualified." << "\n" << endl;
            else if (tree.retrieve(name) -> disqualify()){
                cout << "\n" << name << " has been disqualified." << "\n" << endl;
                cout << *tree.retrieve(name);
            }
        }
        else
            cout << "\n" << name << " is not registered." << endl;

    cout << "\nDisqualify ";
    } while (again());
}


//call the display tree graphical method of the BST
//use to verify the tree is balanced
void Menu::graphical_tree()
{
    cout << tree;
}

void Menu::test_copying()
{
    Red_Black<string, shared_ptr<Contestant>> new_copy;
    new_copy = tree;
    cout << "\nThis is the new after copying:\n" << new_copy;
    sleep(2);

    cout << "\nDeleting the copy..." << endl;
    cout << new_copy.remove_all() << " contestants removed from the copy." << endl;
    sleep(2);

    cout << "\nThis is the original tree:\n" << tree;
    sleep(2);

    cout << "\nThis is the copy after deleting:\n" << new_copy;
    sleep(2);
}

//animated graphical display of insertion and removal
void Menu::animate()
{
    int choice{};

    cout << "\nAnimated graphical display." << endl;
    do{
        cout << "\nInsertion or removal?"
             << "\n0. Return to Main Menu."
             << "\n1. Insertion."
             << "\n2. Removal (Not perfect yet)."
             << "\n>";
        choice = read_int();
        switch (choice){
            case 1:
                animate_load();
                break;
            case 2:
                animate_removal();
                break;
            default:
                break;
        }
    } while (choice);
}


//load a file and create an "animation" of the insertion process.
void Menu::animate_load()
{
    int items{tree.size()};
    int num_loaded{};
    tree.remove_all();
    cout << "\nBeginning animation of tree insertion. There are " << items << " items in the tree.\n"
         << "This \"animation\" should take roughly " << items * 2 << " seconds to complete." << endl;
    //throw exception if file not opened
    filein.open(filename);
    if (!filein)
        throw APPLICATION_ERROR::no_file_exception();

    filein.peek();

    while (!filein.eof()){
        string name{};
        int type{};

        //first check the file for the type so correct initial object can be contstructed
        filein >> type;
        filein.ignore(100, ',');

        getline(filein, name, ',');
        try{
            switch (type){
                case 1: {
                            auto walk_ptr{make_shared<Walking_Contestant>(name, filein)};
                            num_loaded += tree.insert(name, move(walk_ptr));
                        }
                    break;

                case 2: {
                            auto b_ptr{make_shared<Bicycle_Contestant>(name, filein)};
                            num_loaded += tree.insert(name, move(b_ptr));
                        }
                    break;

                case 3: {
                            auto hm_ptr{make_shared<Half_Marathon_Contestant>(name, filein)};
                            num_loaded += tree.insert(name, move(hm_ptr));
                        }
                    break;

                default:
                    break;
            }
            filein.peek();
        }
        catch (TREE_ERROR::duplicate_name_exception &error){
            cout << error.msg << "Skipping...." << endl;
        }

        cout << "\033[2J\033[1;1H" << endl;
        cout << "Put #" << num_loaded << "/" << items << "\n" << tree;
        sleep(1);
    }
    filein.close();
}

void Menu::animate_removal()
{
    if (tree.size() == 0)
        cout << "\nNo contestants in the tree yet. Load a registration or manually register some." << endl;
    else{
        int original_size{tree.size()};
        int removed{1};
        vector<string> keys{};
        tree.fetch_keys(keys);
        while (removed <= original_size){
            string to_remove{keys[rand() % keys.size()]};
            cout << "\033[2J\033[1;1H" << endl;
            cout << "Removal #" << removed << "/" << original_size << "\n"
                 << "Removing: " << to_remove << "\n" << tree;
            ++removed;
            sleep(1);
            tree.remove(to_remove);
            keys.erase(find(keys.begin(), keys.end(), to_remove));
        }
    }

}

//reads an int in and return it
const int Menu::read_int()
{
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

//utility function to prompt repeat
bool Menu::again()
{
    char again{};
    cout << "again? (y/n)" << endl;
    cout << ">";
    cin >> again;
    cin.ignore(100, '\n');
    if (toupper(again) == 'Y')
        return true;
    return false;
}

