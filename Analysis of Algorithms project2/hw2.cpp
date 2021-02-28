//
//  hw2.cpp
//  
//
//  Created by Beyza Ozan on 14.12.2020.
//

#include <stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include <vector>
#include <time.h>
#include <math.h>

using namespace std;


vector<double> vec; // keeps all inserted distances
vector<double> nearest; // keeps only calling nearest taxi's disctances


void insert_taxi(double x) // adding new taxi (its distance x)
{
    vec.push_back(0); // vector size increased by 1 for new distance x (starts from 1st index, 0. is ignored)
    vec[vec.size()-1] = x; // assign new distance 'x' to the last index
    int i = vec.size()-1;
    while(vec[i] < vec[i/2] and i > 1){  // swap as long as new one(x) smaller then parent
        swap(vec[i],vec[i/2]);
        i = i/2;
    }
}
void update_distance(int i) // decreasing taxi's distance as 0.01
{
    vec[i] = vec[i] - 0.01; // decrease by 0.01
    
    while(vec[i] < vec[i/2] and i > 1){ // swap as long as 'decreased distance' smaller then parent
        swap(vec[i],vec[i/2]);
        i = i/2; // go to the parent
    }
}

void min_heapify(int i)
{
    int left = 2*i; // index of left child
    int right = 2*i + 1; // index of right child
    int smallest = i;
    
    if(left <= vec.size()-1 and vec[left] < vec[i]) // if left child is smaller than parent
        smallest = left;
    else  // if parent is smaller than child
        smallest = i;
    
    if(right <= vec.size()-1 and vec[right] < vec[smallest]) // if right child smaller then 'smallest' variable
        smallest = right;
    
    if(smallest != i) // unless 'smallest' is equal to our i index,
    {
        swap(vec[i], vec[smallest]); // replace the old smallest with new smallest
        min_heapify(smallest); // the same is done 4+for children downstairs until reach the leaf
    }
}

double call_nearest_taxi() // remove called taxi from the heap
{
    if(vec.size()-1 < 1) // if there is no taxi location in the heap
        cout << "error : heap underflow" << endl;

    double min = vec[1]; // keep the nearest location at 'min'
    vec[1] = vec[vec.size()-1]; // assign the last position in the heap to the first position
    vec.resize(vec.size()-1);
    min_heapify(1); // rearrange first (top) element's location
    return min;
}

int main(int argc, char *argv[]){
    
    int m = stoi(argv[1]); // taken number of operation m
    double p = stod(argv[2]); // taken probobility p
    
    double x,y,distance;
    int num_of_add = 0, num_of_updates = 0;
    string line;
    vec.push_back(0);
    
    ifstream file;
    file.open("locations.txt"); // open input file
    if (!file){ // checks whether the file has been opened.
        cerr << "File cannot be opened!";
        exit(1);
    }
    
    getline(file, line); // for getting the header line
    
    srand (time(NULL));
    
   
    clock_t t; // to calculate the time of running
    t = clock(); // start time
    
    
    for(int i = 1; i <= m; i++){ // from 1. to m. operation
        double r = (double)rand() / RAND_MAX;
        if(i % 100 == 0) // the closest taxi is removed after every 100 operations
            nearest.push_back(call_nearest_taxi());
        
        else if(p > r){ // each operation is an update with a probability of p
            if(vec.size()-1 > 0){
                update_distance(rand() % (vec.size()-1));
                num_of_updates++; // increase number of updating taxi in every updating process
            }
            else // if vector size smaller than 1
                cout << "error : heap underflow" << endl;
        }
        else{ // an addition of a new taxi with a probability of (1 - p)
            num_of_add++; // increase number of adding taxi in every adding process
            file >> x;  // get the longitude from file
            file >> y; // get the latitude from file
            
            distance = sqrt( pow((x -  33.40819), 2) + pow((y - 39.19001), 2) ); // calculate distance with the Euclidian distance
            insert_taxi(distance); // add new taxi' distance to the heap
        }
    }
    t = (clock() - t); // finish time
    t *= 1000; // second to millisecond
    file.close();

    ofstream wfile; // write to file
    wfile.open("result.txt"); // open the file. If there is no file, create first then open it.
    
    if (!wfile){ // checks whether the file has been opened
        cerr << "File cannot be opened!";
        exit(1);
        }
//    whole writing process
    wfile << "The distance of the called taxis : \n";
    for(int i = 0; i < nearest.size(); i++)
        wfile << i+1 <<". : "<< nearest[i]<<"\n";
    wfile << "The number of taxi addition : "<< num_of_add <<"\n";
    wfile << "The number of distance updates : "<< num_of_updates <<"\n";
    wfile << "Total running time in milliseconds :"<< ((double)t)/CLOCKS_PER_SEC <<"\n";
    wfile.close(); // close the file
    
    cout << ((double)t)/CLOCKS_PER_SEC; // to see the running time results in terminal
        
    return 0;
}

