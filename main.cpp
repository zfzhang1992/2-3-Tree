#include "tree23.h"
#include <vector>
using namespace std;

int main() {
    vector<int> vec{3, 9, 8, 6, 2, 1, 7, 10, 11, 11, 0, 17, 15, 22, 29, 31, 17};

    tree23 t;

    for(int i = 0, n = vec.size(); i < n; i++) {
        cout << "**************** " << i << " : " << vec[i] << endl;
        t.insert(vec[i]);
        t.treePrint();
    }
}