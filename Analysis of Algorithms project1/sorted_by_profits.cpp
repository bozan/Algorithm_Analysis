#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <sstream>
#include <time.h>

// read from sales.txt and write to sorted_by_profits.txt

using namespace std;

struct myset{
    string country;
    string item_type;
    string order_id;
    string units_sold;
    string total_profit;
    float float_total;
};

vector<struct myset> vec;


bool larger1(myset first, myset second){    // Sort the data according to the total profits
    
    if(first.float_total <= second.float_total)
        return true;
    else if(first.float_total > second.float_total)
        return false;
    return false;
}

void quicksort(int first, int last){ // Quicksort Algoritm Function
    if(last <= first)
        return;
    int i = first + 1; // i start from one plus of the first index, since the first index will be the pivot
    int j = last;
    int answer = first;
    while(i < j){ //until i and j are the same or j>i
        while(larger1(vec[first],vec[i]) and i < last) // as long as the first vector is bigger than vector[i], increase i
            i  = i + 1;
        while(larger1(vec[j],vec[first]) and j > first+1) // as long as the vec[j] is bigger than first vector , decrease j
            j = j - 1;
        if(i < j){ //if i is smaller than j , swap the location of vec[i] and vec[j]
            swap(vec[i],vec[j]);
        }
    }
    answer = last;
    for(int i = first; i < last; i++){
        if(vec[i+1].float_total < vec[first].float_total ){
            answer = i;
            break;
        }
    }
    swap(vec[first],vec[answer]); // swap the location of pivot and vec[answer]
    
    quicksort(first, answer-1);
    quicksort(answer+1, last);

}

void write_to_file(int N,float time){ // writes all the data to the file
    ofstream myfile;
    myfile.open("sorted_by_profits.txt"); // opens the file
    
    if (!myfile){ // checks whether the file has been opened
        cerr << "File cannot be opened!";
        exit(1);
        }
    
    myfile <<"Country    Item Type    Order ID    Units Sold    Total Profit\n"; // writes the header line
    for(int i=0; i < N; i++){ // writes all lines one by one
        myfile << vec[i].country << '\t';
        myfile << vec[i].item_type<< '\t';
        myfile << vec[i].order_id << '\t';
        myfile << vec[i].units_sold << '\t';
        myfile << vec[i].total_profit;
        
        myfile << "\n";
    }
    
    myfile.close(); // closes the file
}
int main(int argc, char *argv[]){
    float y;
    ifstream file;
    file.open("sales.txt"); // opens the file to read.
    
    if (!file){ // checks whether the file has been opened.
        cerr << "File cannot be opened!";
        exit(1);
        }

    long int N = 100; //you should read value of N from command line
    N = stoi(argv[1]); // In order to take N number from the command line.
    
    string line;
    getline(file, line); //this is the header line
    for(int i = 0; i<N; i++){ // read from file and assign data to vector
        struct myset new_set;
        
        getline(file, line, '\t'); //country (string)
        new_set.country = line;
        
        getline(file, line, '\t'); //item type (string)
        new_set.item_type = line;
        
        getline(file, line, '\t'); //order id (string)
        new_set.order_id = line;
        
        file >> line; //units sold (integer)
        new_set.units_sold = line;
        
        file >> line; //total profit (float)
        new_set.total_profit = line;
        stringstream sss(line); // convert string to float for comparing.
        sss >> y;
        new_set.float_total = y;
        getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
        vec.push_back(new_set);
    }
    
    clock_t t; // to calculate the time of running
    t = clock(); // start time
    quicksort(0,N-1);
    t = clock() - t; // finish time
    
    printf ("%f seconds\n",((float)t)/CLOCKS_PER_SEC); // prints the time of executaion of quicksort.
    write_to_file(N ,((float)t)/CLOCKS_PER_SEC); // writes the sorting data to the file.
    
    return 0;
}
