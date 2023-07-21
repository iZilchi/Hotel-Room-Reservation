#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <conio.h>

using namespace std;

string username;
const char admin_file[20] = "admin.dat";
const char user_file[20] = "user.dat";
void pause();

class Rooms
{
private:
    struct Node
    {
        int a_rooms, a_beds, a_limit, a_days, a_prices;
        int u_rooms, u_beds, u_limit, u_days, u_prices;
        string username;
        bool available = true;
        Node* next;
    };

    bool isDataLoaded = false;// For checking the data if it already exists or not

public:
    Node* head1 = NULL;
    Node* head2 = NULL;

    //LOGIN FUNCTION
    void login();
    void registerUser();

    //ADMIN FUNCTION
    void adminMenu();
    void addRoom();
    void viewRooms();
    void viewAllReservation();
    void updateAdRoom();
    void deleteRoom();
    void sortAdmin();
    void loadAdRooms();
    void saveAdRooms();

    //USER FUNCTION
    void userMenu();
    void loadUsRooms();
    void saveUsRooms();
    void reserveRooms();
    void deleteReservation();
    void viewUserReservation();
    void sortUser();
    Node * findARoom(int n);
    Node * findURoom(int n);
};

void Rooms::login()
{
    string password;
    cout << "\nPlease enter your username: ";
    cin >> username;

    ifstream userFile(username + ".txt");

    if (userFile.is_open())
    {
        cout << "Please enter your password: ";
        cin >> password;

        string storedPassword;
        userFile >> storedPassword;

        if (password == storedPassword)
        {
            if (username == "admin1")
            {
                cout << "\n\t\t\t\t\t\tWelcome, Admin!\n";
                adminMenu();
            }
            else
            {
                cout << "\n\t\t\t\t\t\tWelcome, " << username << "!\n";
                userMenu();
            }
        }
        else
        {
            cout << "Incorrect password. Login failed.\n";
        }
    }
    else
    {
        cout << "User not found. Please register first.\n";
    }
}

void Rooms::registerUser()
{
    string password;
    cout << "\nEnter a username: ";
    cin >> username;

    ifstream userFile(username + ".txt");
    if (userFile.is_open())
    {
        cout << "Username already exists. Please choose a different one.\n";
        userFile.close();
        return;
    }

    ofstream newUserFile(username + ".txt");
    cout << "Enter a password: ";
    cin >> password;
    newUserFile << password;
    newUserFile.close();

    cout << "Registration successful. You can now log in.\n";
}


void Rooms::adminMenu()
{
    while (true)
    {
        system("cls");
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\t\t\t\t\t\tAdmin Panel\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "1.\tAdd Room\n";
        cout << "2.\tView Rooms\n";
        cout << "3.\tView User Reservation List\n";
        cout << "4.\tUpdate Room\n";
        cout << "5.\tDelete Room\n";
        cout << "6.\tSign Out\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\n\nEnter Your Choice:\n";
        string choice;
        cin >> choice;
        cout << "-------------------------------------------------------------------------------------------------\n";

        switch (stoi(choice))
        {
        case 1:
            addRoom();
            break;
        case 2:
            sortAdmin();
            viewRooms();
            break;
        case 3:
            sortAdmin();
            viewAllReservation();
            break;
        case 4:
            updateAdRoom();
            break;
        case 5:
            deleteRoom();
            break;
        case 6:
            saveAdRooms();
            system("cls");
            cout << "\n\t\t\t\t\t\tSigned out from Admin Panel.\n";
            return;
        default:
            cout << "\t\t\t\t\t\tInvalid Choice\n";
        }
        cout << "-------------------------------------------------------------------------------------------------\n";
        pause();
    }
}


void Rooms::loadAdRooms()
{
    fstream fp;
    fp.open(admin_file, ios::in);

    if (!fp)
    {
        cout << "\n\t\t\t\t\t\tCannot open file\n";
        exit(0);
    }

    while (true)
    {
        Node* new_node = new Node;
        string roomPrefix;
        fp >> roomPrefix;
        if (fp.eof())
        {
            delete new_node;
            break; 
        }

        fp >> new_node->a_rooms;
        fp.ignore();

        fp >> new_node->a_beds;
        fp.ignore();
        fp >> new_node->a_limit;
        fp.ignore();
        fp >> new_node->a_days;
        fp.ignore();
        fp >> new_node->a_prices;
        fp.ignore();
        fp >> new_node->available;
        fp.ignore();

        new_node->next = NULL;

        if (head1 == NULL)
        {
            head1 = new_node;
        }
        else
        {
            Node* ptr1 = head1;
            while (ptr1->next != NULL)
            {
                ptr1 = ptr1->next;
            }
            ptr1->next = new_node;
        }
    }

    fp.close();
    isDataLoaded = true;
}

void Rooms::saveAdRooms()
{
    fstream fp;
    fp.open(admin_file, ios::out | ios::trunc);

    if (!fp) {
        cout << "\n\t\t\t\t\t\tCannot open file\n";
        exit(0);
    }

    Node* ptr1 = head1;
    while (ptr1 != NULL) {
        fp << "Room " << ptr1->a_rooms << "|"
           << ptr1->a_beds << "|"
           << ptr1->a_limit << "|"
           << ptr1->a_days << "|"
           << ptr1->a_prices << "|" 
           << ptr1->available << endl;

        ptr1 = ptr1->next;
    }

    fp.close();
}

void Rooms::addRoom()
{
    fstream fp;
    fp.open(admin_file, ios::out | ios::app);
    if (!fp) {
        cout << "\n\t\t\t\t\t\tCannot open file\n";
        return;
    }

    sortAdmin();
    viewRooms();
     cout<<"-------------------------------------------------------------------------------------------------\n\n";

    Node* new_node = new Node;
    cout << "Enter Room Number: ";
    cin >> new_node->a_rooms;
    if (findARoom(new_node->a_rooms))
    {
        cout << "Room Number " << new_node->a_rooms << " already exists\n";
        return;
    }
    cout << "Enter Bed Count: ";
    cin >> new_node->a_beds;
    cout << "Enter Room Limit: ";
    cin >> new_node->a_limit;
    new_node->a_days = 1;
    cout << "Enter Room Price: ";
    cin >> new_node->a_prices;
    new_node->next = NULL;

    fp << "Room " << new_node->a_rooms << "|"
       << new_node->a_beds << "|"
       << new_node->a_limit << "|"
       << new_node->a_days << "|"
       << new_node->a_prices << "|"
       << new_node->available << "|";
    fp << endl;
    fp.close();

    if (head1 == NULL) {
        head1 = new_node;
    } else {
        Node* ptr1 = head1;
        while (ptr1->next != NULL)
        {
            ptr1 = ptr1->next;
        }
        ptr1->next = new_node;
    }
    cout << "\t\t\t\t\t\t\nNew Room Added\n";
}

void Rooms::viewRooms()
{
    if (isDataLoaded) loadAdRooms();

    if (head1 == NULL) {
        cout << "\t\t\t\t\t\tThere are currently no rooms\n" << endl;
        return;
    } else {
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "Room Number\t\tBed Count\tRoom Limit\t\tDays\t\tPrices\n";
        Node* ptr1 = head1;
        while (ptr1 != NULL) {
            cout << "Room " << ptr1->a_rooms << "\t\t\t" << ptr1->a_beds << "\t\t" << ptr1->a_limit << "\t\t\t" << ptr1->a_days << "\t\t" << ptr1->a_prices << endl;
            ptr1 = ptr1->next;
        }
    }
}

void Rooms::viewAllReservation(){
    if (isDataLoaded) {
        loadUsRooms();
    }

    if (head2 == NULL) {
        cout << "\t\t\t\t\t\tThere are currently no reservations\n" << endl;
        return;
    } else {
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "User\t\tRoom Number\t\tBed Count\tRoom Limit\t\tDays\t\tPrices\n";
        Node* ptr2 = head2;
        while (ptr2 != NULL)
        {
            cout << ptr2->username << "\t\t" << "Room " << ptr2->u_rooms << "\t\t\t" << ptr2->u_beds << "\t\t" << ptr2->u_limit << "\t\t\t" << ptr2->u_days << "\t\t" << ptr2->u_prices << endl;
            ptr2 = ptr2->next;
        }
        return;
    }
}

void Rooms::updateAdRoom()
{
    if (isDataLoaded) {
        loadAdRooms();
    }

    if (head1 == NULL)
    {
        cout << "\t\t\t\t\t\tThere are currently no rooms\n" << endl;
    }
    else
    {
        viewRooms();
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\nEnter Room Number to Update: ";
        int r_id;
        cin >> r_id;
        Node* ptr1 = head1;
        while (ptr1 != NULL)
        {
            if (r_id == ptr1->a_rooms)
            {
                cout << "Enter New Room Number: ";
                cin >> ptr1->a_rooms;

                cout << "Enter New Bed Count: ";
                cin >> ptr1->a_beds;

                cout << "Enter New Room Limit: ";
                cin >> ptr1->a_limit;

                ptr1->a_days = 1;

                cout << "Enter New Room Price: ";
                cin >> ptr1->a_prices;

                saveAdRooms();

                cout << "\n\t\t\t\t\t\tRoom Updated\n";
                return;
            }
            ptr1 = ptr1->next;
        }
        cout << "\t\t\t\t\t\t\nRoom with Room Number " << r_id << " does not exist\n";
    }
}

void Rooms::deleteRoom()
{
    if (isDataLoaded) {
        loadAdRooms();
    }

    if (head1 == NULL) {
        cout << "\t\t\t\t\t\tThere are currently no rooms" << endl;
    }
    else
    {
        viewRooms();
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\nEnter Room Number to Delete: ";
        int r_id;
        cin >> r_id;

        if (r_id == head1->a_rooms)
        {
            Node* ptr1 = head1;
            head1 = head1->next;
            delete ptr1;
            saveAdRooms();
            cout << "\n\t\t\t\t\t\tRoom Deleted\n";
            return;
        }
        else
        {
            Node* pre1 = head1;
            Node* ptr1 = head1;
            while (ptr1 != NULL)
            {
                if (r_id == ptr1->a_rooms)
                {
                    pre1->next = ptr1->next;
                    delete ptr1;
                    saveAdRooms();
                    cout << "\n\t\t\t\t\t\tRoom Deleted\n";
                    return;
                }
                pre1 = ptr1;
                ptr1 = ptr1->next;
            }
        }
        cout << "\n\t\t\t\t\t\tRoom with Room Number " << r_id << " does not exist\n";
    }
}

void Rooms::sortAdmin()
{
    if (head1 == NULL) return;
    int count = 0, r_rooms, r_beds, r_limits, r_days, r_prices;
    Node* ptr1 = head1;
    while (ptr1 != NULL)
    {
        count++;
        ptr1 = ptr1->next;
    }
    for (int i = 0; i < count; i++)
    {
        Node* ptr1 = head1;
        for (int j = 1; j < count; j++)
        {
            if (ptr1->a_rooms > ptr1->next->a_rooms)
            {
                r_rooms = ptr1->a_rooms;
                r_beds = ptr1->a_beds;
                r_limits = ptr1->a_limit;
                r_days = ptr1->a_days;
                r_prices = ptr1->a_prices;

                ptr1->a_rooms = ptr1->next->a_rooms;
                ptr1->a_beds = ptr1->next->a_beds;
                ptr1->a_limit = ptr1->next->a_limit;
                ptr1->a_days = ptr1->next->a_days;
                ptr1->a_prices = ptr1->next->a_prices;

                ptr1->next->a_rooms = r_rooms;
                ptr1->next->a_beds = r_beds;
                ptr1->next->a_limit = r_limits;
                ptr1->next->a_days = r_days;
                ptr1->next->a_prices = r_prices;
            }
            ptr1 = ptr1->next;
        }
    }
}

void Rooms::userMenu()
{
    while (true)
    {
        system("cls");
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\t\t\t\t\t\tUser Panel\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "1.\tReserve Rooms.\n";
        cout << "2.\tDelete Reservation.\n";
        cout << "3.\tView Reservation\n";
        cout << "4.\tSign Out\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\n\nEnter Your Choice:\n";
        string choice;
        cin >> choice;
        cout << "-------------------------------------------------------------------------------------------------\n";

        switch (stoi(choice))
        {
        case 1:
            reserveRooms();
            break;
        case 2:
            deleteReservation();
            break;
        case 3:
            sortUser();
            viewUserReservation();
            break;
        case 4:
            saveUsRooms();
            system("cls");
            cout << "\n\t\t\t\t\t\tSigned out from User Panel.\n";
            pause;
            return;
        default:
            cout << "\t\t\t\t\t\tInvalid Choice\n";
        }
        cout << "-------------------------------------------------------------------------------------------------\n";
        pause();
    }
}

void Rooms::loadUsRooms(){
        fstream fp;
    fp.open(user_file, ios::in);

    if (!fp)
    {
        cout << "\n\t\t\t\t\t\tCannot open file\n";
        return;
    }

    while (true)
    {
        Node* new_node = new Node;
        string roomPrefix;
        fp >> roomPrefix;
        if (fp.eof())
        {
            delete new_node;
            break; 
        }

        fp >> new_node->username;
        fp.ignore();
        fp >> new_node->u_rooms;
        fp.ignore();
        fp >> new_node->u_beds;
        fp.ignore();
        fp >> new_node->u_limit;
        fp.ignore();
        fp >> new_node->u_days;
        fp.ignore();
        fp >> new_node->u_prices;
        fp.ignore();

        new_node->next = NULL;

        if (head2 == NULL)
        {
            head2 = new_node;
        }
        else
        {
            Node* ptr2 = head2;
            while (ptr2->next != NULL)
            {
                ptr2 = ptr2->next;
            }
            ptr2->next = new_node;
        }
    }

    fp.close();
    isDataLoaded = true;
}

void Rooms::saveUsRooms(){
    fstream fp;
    fp.open(user_file, ios::out | ios::trunc);

    if (!fp) {
        cout << "\n\t\t\t\t\t\tCannot open file\n";
        return;
    }

    Node* ptr2 = head2;
    while (ptr2 != NULL)
    {
        fp << ptr2->username << "|" << "Room " << ptr2->u_rooms << "|"
           << ptr2->u_beds << "|"
           << ptr2->u_limit << "|"
           << ptr2->u_days << "|"
           << ptr2->u_prices << "|" << endl;

        ptr2 = ptr2->next;
    }

    fp.close();
}

void Rooms::reserveRooms() {
    fstream fp;
    fp.open(user_file, ios::out | ios::app);
    if (!fp)
    {
        cout << "\n\t\t\t\t\t\tCannot open file\n";
        return;
    }

    if (head1 == NULL) {
        cout << "\t\t\t\t\t\tThere are currently no rooms\n" << endl;
        return;
    }

    viewRooms();
    cout << "-------------------------------------------------------------------------------------------------\n\n";

    Node* new_node = new Node;

    cout << "Enter Room Number: ";
    cin >> new_node->u_rooms;
    Node *room = findARoom(new_node->u_rooms);
    if (room) {
        if (!room->available) {
            cout << "Room Number " << new_node->u_rooms << " is not available!\n";
            return;
        } else room->available = false;
    } else {
        cout << "Room Number " << new_node->u_rooms << " is not available!\n";
        return;
    }
    new_node->username = username;
    new_node->u_beds = room->a_beds;
    new_node->u_limit = room->a_limit;
    cout << "Enter Days of Reservation: ";
    cin >> new_node->u_days;
    new_node->next = NULL;

    int u_prices = new_node->u_days * room->a_prices;
    new_node->u_prices = u_prices;

    fp << new_node->username << "|" << "Room " << new_node->u_rooms << "|"
       << new_node->u_beds << "|"
       << new_node->u_limit << "|"
       << new_node->u_days << "|"
       << new_node->u_prices << "|";
    fp << endl;
    fp.close();

    if (head2 == NULL)
    {
        head2 = new_node;
    }
    else
    {
        Node* ptr2 = head2;
        while (ptr2->next != NULL)
        {
            ptr2 = ptr2->next;
        }
        ptr2->next = new_node;
    }
    cout << "\t\t\t\t\t\t\nNew Room Reserved\n";
}

void Rooms::deleteReservation()
{
    if (isDataLoaded)
    {
        loadUsRooms();
    }

    if (head2 == NULL)
    {
        cout << "\t\t\t\t\t\tThere are currently no rooms\n" << endl;
    }
    else
    {
        viewUserReservation();
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\nEnter Room Number to Delete: ";
        int r_id;
        cin >> r_id;

        Node *ptr2 = findURoom(r_id);
        if (ptr2 != NULL)
        {
            // Mark the room as available again in admin's data
            Node *room = findARoom(ptr2->u_rooms);
            if (room)
            {
                room->available = true;
                saveAdRooms();
            }

            if (r_id == head2->u_rooms)
            {
                head2 = head2->next;
                delete ptr2;
                saveUsRooms();
                cout << "\n\t\t\t\t\t\tRoom Deleted\n";
                return;
            }
            else
            {
                Node *pre2 = head2;
                while (pre2->next != NULL && pre2->next != ptr2)
                {
                    pre2 = pre2->next;
                }
                pre2->next = ptr2->next;
                delete ptr2;
                saveUsRooms();
                cout << "\n\t\t\t\t\t\tRoom Deleted\n";
                return;
            }
        }
        else
        {
            cout << "\n\t\t\t\t\t\tReservation of Room No. " << r_id << " is not found.\n";
        }
    }
}

void Rooms::viewUserReservation(){
    if (isDataLoaded) {
        loadUsRooms();
    }

    if (head2 == NULL) {
        cout << "\t\t\t\t\t\tThere are currently no rooms\n" << endl;
        return;
    } else {
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "Room Number\t\tBed Count\tRoom Limit\t\tDays\t\tPrices\n";
        Node* ptr2 = head2;
        while (ptr2 != NULL)
        {
            if(username == ptr2->username){
            cout << "Room " << ptr2->u_rooms << "\t\t\t" << ptr2->u_beds << "\t\t" << ptr2->u_limit << "\t\t\t" << ptr2->u_days << "\t\t" << ptr2->u_prices << endl;
            }
            ptr2 = ptr2->next;
        }
        return;
    }
}

void Rooms::sortUser()
{
    if (head2 == NULL) {
        return;
    }
    int count = 0, r_rooms, r_beds, r_limits, r_days, r_prices;
    Node* ptr2 = head2;
    while (ptr2 != NULL)
    {
        count++;
        ptr2 = ptr2->next;
    }
    for (int i = 0; i < count; i++)
    {
        Node* ptr2 = head2;
        for (int j = 1; j < count; j++)
        {
            if (ptr2->u_rooms > ptr2->next->u_rooms)
            {
                r_rooms = ptr2->u_rooms;
                r_beds = ptr2->u_beds;
                r_limits = ptr2->u_limit;
                r_days = ptr2->u_days;
                r_prices = ptr2->u_prices;

                ptr2->u_rooms = ptr2->next->u_rooms;
                ptr2->u_beds = ptr2->next->u_beds;
                ptr2->u_limit = ptr2->next->u_limit;
                ptr2->u_days = ptr2->next->u_days;
                ptr2->u_prices = ptr2->next->u_prices;

                ptr2->next->u_rooms = r_rooms;
                ptr2->next->u_beds = r_beds;
                ptr2->next->u_limit = r_limits;
                ptr2->next->u_days = r_days;
                ptr2->next->u_prices = r_prices;
            }
            ptr2 = ptr2->next;
        }
    }
}

Rooms::Node * Rooms::findARoom(int n) {
    Node *ptr1 = head1;
    while (ptr1 != NULL) {
        if (ptr1->a_rooms == n) return ptr1;
        ptr1 = ptr1->next;
    }
    return NULL;
}

Rooms::Node * Rooms::findURoom(int n) {
    for (Node *ptr2 = head2; ptr2 != NULL; ptr2 = ptr2->next) {
        if (ptr2->u_rooms == n) return ptr2;
    }
    return NULL;
}

void pause() {
    cout<<"Press ANY KEY to continue...\n";
    getch();
}

int main()
{
    fstream ufile(user_file, ios::out);
    ufile.close();
    fstream afile(admin_file, ios::out);
    ufile.close();

    Rooms r;

    while (true)
    {
        system("cls");
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\t\t\t\tWelcome to Room Reservation System!\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "1.\tLogin\n";
        cout << "2.\tRegister\n";
        cout << "3.\tExit\n";
        cout << "-------------------------------------------------------------------------------------------------\n";
        cout << "\n\nEnter Your Choice (or 'exit' to go back to the menu):\n";

        int choice;
        cin >> choice;
        cout << "-------------------------------------------------------------------------------------------------\n";

        switch (choice)
        {
        case 1:
            r.login();
            break;
        case 2:
            r.registerUser();
            break;
        case 3:
            cout << "Thank you for using our Hotel Reservation System!\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
        pause();
    }

    return 0;
}