#include <typeinfo> // cout << typeid(...).name()
#include "field"

using namespace std;

void cinclear()
{
    cin.clear(ios::goodbit); // if the latest input was wrong - flushing bufer
    cin.ignore(65535, '\n');
}

int menu()
{
    char choice;
    int key = 1;
    while(key){
        cout << "Would you like to load data from file? (y/n)\n";
        cin >> choice;
        if (choice == 'y') { key = 0; return 1; }
        if(choice == 'n') { key = 0; return 0; }
        else { cout << "ERROR! Incorrect input, try again.\n"; continue; }
    }
}

//-----------------MAIN----------------------------
int main()
{
    filebuf filename;
    auto flagfile = filename.open("input", ios::in);
    if(!flagfile){ cout << " ERROR with file!\n "; exit(1); }
    istream fin(&filename);
   
    int x_load = 0;
    int y_load = 0;
    int army_number = 0;
    int choice = menu();

    if(choice){ // file
        fin >> x_load >> y_load;   
    }  
    else { //console
        cout << "Enter size of field (x y) (unsigned int numbers separated by space): " <<endl;
        cin >> x_load >> y_load;
        int m = 0; 
        while(m++ < 2){
            cout << "_" << m << " army: " << endl;
            cout << "Enter the amount of objects (unsigned int)\n"; 
            cout << "Then - their location (x y) and hit points (unsigned int numbers separated by space): \n";
            cout << "Repeat the same actions for the second army. \n";
        }
    }

    // Field *fd = new Field(x_load, y_load);        
    Field fd(x_load, y_load);

    if(choice){ // file
        fin >> fd;
    }  
    else { //console
        cin >> fd;    
    }

    fd.print_field();
    int flag = 0;
    cout << "See all containers (army: [ object_index x y hp] ):\n"; fd.print_cont();
    while(1){
        int a, x, y, ind;
        while(1){
            cinclear();//setstate(ios::goodbit(true)); 
            cout << "If there is alive object? Enter coordinates of the object (x y - unsigned int numbers separated by space) or q to exit: " << endl;
            flag = 1;
            cin >> x;
            if ( x == 'q') { fd.~Field(); filename.close(); exit(0);}    
            cin >> y;
            a = fd.belonging_army(x,y);
            ind = fd.find_ob_index(x,y);
            if(ind == -1 || a == -1) { cout << "Selected target wasn't found. Try again (maybe inccorect input)." <<endl; 
                    cin.clear(); continue;}
            else if (fd.getconts()[a]->find(ind)->get().is_obj(x,y,fd.getmap())) {
                {cout << "Yes: hp = "<< fd.getconts()[a]->find(ind)->get().gethp() << endl; flag = 1; break;}
            } else {cout << "No: doesn't exist or was killed.\n"; flag = 1;}
            if(flag) break;
        }

    flag = 0;
        while(1){
            cout << "Input coordinates of the target (unsigned int numbers separated by space) to select and get to know its army (q to exit):\n";
            x = 0; cin >> x;
            if ( x == 'q') { fd.~Field(); filename.close(); exit(0);}
            cin >> y;             

            int belong = fd.belonging_army(x,y);
            switch(belong){
                case 0:
                    cout << "Selected target is red\n";
                    flag = 1; break;
                case 1:
                    cout << "Selected target is green\n";
                    flag = 1; break;
                default:
                    cout << "Error! The target wasn't found. Try again (maybe incorrect input).\n";
                    cinclear(); continue;
            }
            if(flag) break;
        }

    int d;
    cout << "Input a damage value (unsigned int): "; 
    cin >> d; 
    ind = fd.find_ob_index(x,y);
    fd.getconts()[a]->find(ind)->get().damage(d);
    cout << "Hit points left in selected target: " << fd.getconts()[a]->find(ind)->get().gethp() <<endl;

    fd.print_field();

    while(1){
        cinclear();
        cout << "Continue or quit? (c/q)\n";
        char switch_on;
        cin >> switch_on; 
        if (switch_on == 'c') continue;
        else if (switch_on == 'q') { fd.~Field(); filename.close(); exit(0);}
        else { cout << "ERROR! Incorrect input, try again.\n";  cinclear(); continue; }
    }
}
    
    fd.~Field();
    filename.close();
    return 0;   
}
//------------------------------------------------
