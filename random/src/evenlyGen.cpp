#include "../inc/evenlyGen.h"
#include <cstdlib>
using namespace std;

vector<int> EvenlyGenAlg::generate(int typeNum, int totalNum) {
    vector<int> result;
    int ave = totalNum / typeNum;

    for (int i = 0; i < typeNum; ++i) {
        for (int j = 0; j < ave; ++j) {
            result.push_back(i);
        }
    }

    for (int i = result.size(); i < totalNum; ++i) {
        result.push_back(typeNum - 1);
    }

    return result;
}