#include <stdio.h>
#include<iostream>

using namespace std;

void bigSum(vector vect1){
    int sum = 0;
    if(vect1.is_empty())

        return 0;

    for (int i = 0; i < vect1.length(); i++){
        if(vec[i]>0)
            sum +=vect[i];
    }
    return sum;


}
int main(){
    vector<int> vect1; 
    vector<string> vect2; 
    bool stop = false;
    string x;
    while (!stop){
        cout >> x >> endl;
        if(x =="STOP")
            return;
    
        for (int i = 0; i < x.length(); i++)
            if (isdigit(x[i]) == false)
                vect2.push_back(x);
            else 
                vect1.push_back(x);
    }
    int result = bigSum(vect1);
    if (result == 0)
        cout << "String vector is empty"<<endl;
    else
        cout << result << endl;


    return 0;
}