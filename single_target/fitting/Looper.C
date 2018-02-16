#include <vector>
#include <iostream>

class Looper {
  public:
    Looper();
    
    void AddLoop(int);
    vector<int> GetNext();
    vector<int> GetSizes();
    int GetDimension();
    bool Finished();

  private:
    vector<int> sizes;
    vector<int> indices;
    int n;
    bool finished;
    void Increment();
};

Looper::Looper(){
    n = 0;
    finished = kFALSE;
}

void Looper::AddLoop(int size){
    if (size<1)
    {
    	cout <<"Cannot add loop with size less than 1. Resetting to 1." <<endl;
    	size = 1;
    }
    sizes.push_back(size);
    indices.push_back(0);
    n++;
}

vector <int> Looper::GetNext(){
    vector<int> current  = indices;
    Increment();
    return current;
}

vector <int> Looper::GetSizes(){
    return sizes;
}

int Looper::GetDimension(){
	return n;
}

void Looper::Increment(){
    for (int i=0; i<n; i++){
        indices[i]++;
	if (indices[i] == sizes[i]){
		indices[i] = 0;
        }
	else return;
    }
    finished = true;
}

bool Looper::Finished(){
	return finished;
}
