// ASU CSE310 Fall 2022 Assignment #1
// Name: Race Musgrave
// ASU ID: 1215151093
// Description: header file for assignment1.cpp which contains all the associated linked list functions

#include <iostream>
#include <iomanip>
#include <string>


using namespace std;

//Car represents a Car information
struct Car
{
    string model, make;
    int vin;            //vehicle identification number
    double price;
    struct Car* next;
};

//class LinkedList will contains a linked list of Cars
class LinkedList
{
    private:
        struct Car* head;

    public:
        LinkedList();
        ~LinkedList();
        bool findCar(int aVin);
        bool addCar(string model, string make, int vin, double price);
        bool removeByVin(int aVin);
        bool removeByModelAndMake(string model, string make);
        bool changeCarInfo(int aVin, string newModelAndMake);
        bool updateCarPrice(int aVin, double newPrice);
        void printCarListByMake(string make);
        void printCarList();
        void addFront(Car * newCar);
        void addMid(Car * curr, Car * prev, Car * newCar);
        void addEnd(Car * newCar);
        double getPrice(double aVin);
};

//Constructor to initialize an empty linked list
LinkedList::LinkedList()
{
    head = NULL;
}

//Destructor
//Before termination, the destructor is called to free the associated memory occupied by the existing linked list.
//It deletes all the nodes including the head and finally prints the number of nodes deleted by it.
//Return value: Prints the number of nodes deleted by it.
LinkedList::~LinkedList()
{
    int carCount = 0;
    Car * curr = head;
    while(curr != NULL){
        head = head->next;
        delete curr;
        curr = head;
        ++carCount;
    }
    head = NULL;
    curr = NULL;
    cout<< "The number of deleted Car is: " << carCount <<"\n";
}

//A function to identify if the parameterized Car is inside the LinkedList or not.
//Return true if it exists and false otherwise.
bool LinkedList::findCar(int aVin)
{
    Car * curr = head;
	while(curr != NULL){
        if(curr->vin == aVin){
            return true;
        }
        curr = curr->next;
    }
    return false;
}

//Creates a new Car and inserts it into the list at the right place.
//It also maintains an alphabetical ordering of Cars by their make and model, i.e.
//Return value: true if it is successfully inserted and false in case of failures.
bool LinkedList::addCar(string model, string make, int vin, double price)
{
    //if the car doesnt already exit, then we continue into the fucntion
    if(findCar(vin) == false){
        Car * prev = NULL;
        Car * curr = head;
        Car * newCar = new Car;
        newCar->model = model;
        newCar->make = make;
        newCar->vin = vin;
        newCar->price = price;
        newCar->next = NULL;
        
        if(head == NULL){
            addFront(newCar);
            return true;
        }
        while(curr != NULL){
            //first if checks for the most specific scenario, make, model, and vin.
            if(curr->make.compare(newCar->make) == 0 && curr->model.compare(newCar->model) == 0 && curr->vin > newCar->vin){

                if(prev == NULL){
                    addFront(newCar);                               
                    return true;
                }
                addMid(curr, prev, newCar);
                return true;
            }
            //second if rules out vin and only checks make and model
            else if(curr->make.compare(newCar->make) == 0 && curr->model.compare(newCar->model) > 0){

                if(prev == NULL){
                    addFront(newCar);                               
                    return true;
                }
                addMid(curr, prev, newCar);
                return true;
            }
            //third if rules out model and only checks make
            else if(curr->make.compare(newCar->make) > 0){

                if(prev == NULL){
                    addFront(newCar);                               
                    return true;
                }
                addMid(curr, prev, newCar);
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        //if it doesnt fit anywhere in the list, it will be added at the end
        addEnd(newCar);
        return true;
    }
    cout<< "Duplicated Car. Not added.\n";
    return false;
}

//helper function to add a Car to the front of the linked list
void LinkedList::addFront(Car * newCar){
    newCar->next = head;
    head = newCar;
}

//helper function to add a Car somwhere in the middle of the linked list
void LinkedList::addMid(Car * curr, Car * prev, Car * newCar){
    newCar->next = curr;
    prev->next = newCar;
}

//helper function to add a Car to the end of the linked list
void LinkedList::addEnd(Car * newCar){
    Car* current = head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = newCar;
}

//----------------------------------end of addCar------------------------------------------------------

//Removes the specified Car from the list, releases the memory and updates pointers.
//Return true if it is successfully removed, false otherwise.
bool LinkedList::removeByVin(int aVin)
 {
    Car * curr = head;
    Car * prev = NULL;

    //first checks if the car exists in the list, if not, exits function.
    if(findCar(aVin) == false){
        return false;
    }

    //in the case that there is only one car in the list
    if(curr->next == NULL ){
        delete curr;
        head = NULL;
        return true;
    }
    
    //in the case there are multiple cars
	while(curr != NULL){
        //removes from the front
        if(prev == NULL && curr->vin == aVin){
            head = head->next;
            delete curr;
            curr = head;
            return true;
        }
        //removes from the middle or end
        if(curr->vin == aVin){
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
 }
//----------------------------------end of removeByVin------------------------------------------------------

//Removes the given Car from the list, releases the memory and updates pointers.
//Return true if it is successfully removed, false otherwise. Note: all Cars with
//the same model and make should be removed from the list.
bool LinkedList::removeByModelAndMake(string model, string make)
{   
    //in the case the list is empty
    if (head == NULL){
        cout << "No such Car found.\n";
        return false;
    }

    Car * curr = head;
    Car * prev = NULL;
    bool check = false;
    //changes check to true if the vehcile exists in the list
    while(curr != NULL){
        if(curr->make == make && curr->model == model){
            check = true;
        }
        curr = curr->next;
    }
    curr = head;


    //this while loop 
	while(curr != NULL){
        if(prev == NULL && curr->make == make && curr->model == model){
            head = head->next;
            delete curr;
            curr = head;
        }
        else if(prev != NULL && curr->make == make && curr->model == model){
            prev->next = curr->next;
            delete curr;
            curr = prev->next;
        }
        else{
            prev = curr;
            curr = curr->next;
        }
    }

    //to check if there are still existing duplicates
    //if there are, it changes check to false
    curr = head;
    prev = NULL;
    while(curr != NULL){
        if(curr->make == make && curr->model == model){
            check = false;
        }
        prev = curr;
        curr = curr->next;
    }

    //what is the final result of check?
    if(check == true){
        return true;
    }
    cout << "No such Car found.\n";
    return false;
}
//----------------------------------end of removeByModelAndMake------------------------------------------------------

//Modifies the data of the given Car. Return true if it modifies successfully and
//false otherwise. Note: after changing a Car model and make, the linked list must still
//maintain the alphabetical order.
bool LinkedList::changeCarInfo(int aVin, string newModelAndMake)
{
    if(findCar(aVin) == false){
        return false;
    }

    string model, make;
    int delim, len;
    delim = newModelAndMake.find(' ');                            //this section finds the position of the space then creates
    len = newModelAndMake.length();                               //two variables for the make and model seperatelyq
    model = newModelAndMake.substr(0,delim);
    make = newModelAndMake.substr(delim+1,len-1);

    double price = getPrice(aVin);
    Car * curr = head;
    while(curr != NULL){
        if(curr->vin == aVin){
            removeByVin(aVin);
            addCar(model, make, aVin, price);                       //it can be re-added to the list from the gathered info
            return true;
        }
        curr = curr->next;
    }
    return false;
}

//helper function for changeCarInfo to get the price of certain car using vin
double LinkedList::getPrice(double aVin){
    Car* curr = head;
    double price = 0;
    while(curr != NULL){
        if(curr->vin == aVin){
            price = curr->price;
        }
        curr = curr->next;
    }
    return price;
}
//----------------------------------end of changeCarInfo------------------------------------------------------

//this function takes in a vin and price and will find the given car in the
//linked list and change its price accordingly
bool LinkedList::updateCarPrice(int aVin, double newPrice)
{
    //first check if the car exists
	if(findCar(aVin) == false){
        return false;
    }

    //now find the car and changes price
    Car * curr = head;
    while(curr != NULL){
        if(curr->vin == aVin){
            curr->price = newPrice;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

//Prints all Cars in the list with the same make.
void LinkedList::printCarListByMake(string make)
{
    //in the case the list is empty
    if (head == NULL){
        cout << "Car List Empty\n";
    }
    
    Car * curr = head;
    bool check = false;
    //this section is used to make sure there exists any vehicles of the given make
    while(curr != NULL){
        if(curr->make == make){
           check = true;
        }
        curr = curr->next;
    }
    //if none of this make exists
    if (check == false){
        cout << "No Cars with the specified make found.\n";
    }

    //otherwise, loop through the linked list and find each car of the given make
    curr = head;
    while(curr != NULL){
        if(curr->make.compare(make) == 0){
            cout  << left    << setw(12) << curr->model
                  << left    << setw(12) << curr->make
                  << right   << setw(8) << curr->vin
                  << setw(10) << fixed << setprecision(2) << curr->price << "\n";
        }
        curr = curr->next;
    }

}

//Prints all Cars in the linked list starting from the head node.
void LinkedList::printCarList()
{
    Car * curr = head;
    while(curr != NULL){
        cout << left    << setw(12) << curr->model
             << left    << setw(12) << curr->make
             << right   << setw(8) << curr->vin
             << setw(10) << fixed << setprecision(2) << curr->price << "\n";
        curr = curr->next;
    }
}