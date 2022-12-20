#include "../inc/simpleRandomGen.h"
#include <cstdlib>
using namespace std;

vector<int> SimpleRandomGenAlg::randomGen(int typeNum, int totalNum) {
    vector<int> result;

    srand((unsigned)time(NULL));
    for (int i = 0; i < totalNum; ++i) {
        result.push_back(rand() % typeNum);
    }

    return result;
}
