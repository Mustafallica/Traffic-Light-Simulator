// Mustafa AL-Jaburi

#include<iostream>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fstream>
#include<sstream>
#include<map>
#include<sys/wait.h>
using namespace std;


//Car information
struct carinfo {
    string licensePlate;
    int time;
    char direction;
}carsinfo_array[50];

// Global vars. 
int total_elements = 0;
char initial_direction;
int max_cars_perDir;

// Functions Declaration
void fillDataFromFile(); // Function to read data from file
void rearangeCarInfo(); // Function to order direction in clockwise
void displayCarInfo(int); // Function to display car information
void printAllCarsInfo(); // Function to print all cars

int main(){
    fillDataFromFile(); // First step is to read all data from file.
    rearangeCarInfo();  // Here we rearange the order of cars so that cars can move in clockwise fashion.

    for(int i=0;i<total_elements;i++){
        if(fork() == 0) {
            /*//Verify that all childs have same parent. cout<<"Child: "<<getpid()<<" "<<"parent: "<<getppid()<<endl;*/
            displayCarInfo(i);
            exit(0); // Exiting the child process for new child to continue its execution.
        }
        sleep(carsinfo_array[i].time); // other child process waits till the time the current car takes to go.
    }
    for(int i=0;i<total_elements;i++){ // Here the parent waits till its childern complete their execution.
        wait(NULL);}
    return 0;
}


void fillDataFromFile(){
    string line;
    int k=1,j=0;
    ifstream input_file("input1.txt"); // Reading data from file
    if(input_file.is_open()){ //Checking for opening of file
        
        while(getline(input_file,line)){
            if(k>2 && line!= "") { //It is assumed that in file line greater than 2 will hold cars information.
                string current_line;
                stringstream ss(line); //converting string to stream for spliting purposes in our case tokens are based on spaces.
                int i=0;
                
                while(getline(ss,current_line,' ')){ // Tokenization.
                    if(i==0) carsinfo_array[total_elements].licensePlate=current_line; // if i==0 it is assumed that it is license plate.
                    if(i==1) carsinfo_array[total_elements].direction=current_line[0]; // if i==1 it is assumed that it is direction.
                    if(i==2) carsinfo_array[total_elements].time=atoi(current_line.c_str()); // if i==2 it is assumed that it is time.
                    i++;
                }
                total_elements++; //indicates current array size.
            } else{
                if(j==0) initial_direction=line[0]; //Initial direction
                if(j==1) max_cars_perDir=atoi(line.c_str()); //max number of cars that can pass intersection
                j++;
            }
            k++;
        }
        input_file.close(); //closing file
    } else cout<<"Unable to open file\n";
}


void rearangeCarInfo(){
    
    map<char,char>directionMap; //Map that holds clockwise direction information.
    directionMap['N']='E';
    directionMap['E']='S';
    directionMap['S']='W';
    directionMap['W']='N';
    char curr_dir=initial_direction;
    
    /*Scanning all the cars and arranging their moving sequence clockwise
     i-e if we start with 'W' then next car will be from 'N' than 'E' than 'S' and
     this sequence repeats. S->W->N->E....*/
    for(int i=0;i<total_elements;i++){
        bool check=false;
        int j;
        char start=curr_dir;
        do { //Finding next clockwise pattern location
            for(j=i+1;j<total_elements;j++){
                if(curr_dir==carsinfo_array[i].direction){ // Same location will not be swapped.
                    check=true;
                    break;}
                
                /*Only direction that come before some direction i-e if direction is 'E'
                first and we have direction 'N' as initial so we will pick 'N' first than 'E' to follow correct clockwise direction pattern.*/
                if(curr_dir==carsinfo_array[j].direction){
                    struct carinfo temp=carsinfo_array[j];
                    carsinfo_array[j]=carsinfo_array[i];
                    carsinfo_array[i]=temp;
                    check=true;
                    break;}}
            curr_dir=directionMap[curr_dir];
        } while(!check&&start!=curr_dir);} //Just skip the loop when correct ordering is done for a particular car.
}


void displayCarInfo(int index) {
    if(index>-1&&index<total_elements){  //Displays a car information.
        map<char,string>directionDiscription;
        directionDiscription['N']="Northbound";
        directionDiscription['E']="Eastbound";
        directionDiscription['S']="Southbound";
        directionDiscription['W']="Westbound";
        carinfo car=carsinfo_array[index];
        cout<<"Current Direction: "<<directionDiscription[car.direction]<<endl<<"Car "<<car.licensePlate<<" is using the intersection for "<<car.time<<" sec(s)."<<endl;}
}


void printAllCarsInfo() {
    for(int i=0;i<total_elements;i++){  //Display all cars.
        cout<<carsinfo_array[i].licensePlate<<" "<<carsinfo_array[i].direction<<" "<<carsinfo_array[i].time<<endl;}
}

