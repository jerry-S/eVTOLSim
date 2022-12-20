#include <vector>
using namespace std;

/**
 * @brief Class or strategy to generate random EVs from different vendors. Strategy pattern
 */
class SimpleRandomGenAlg {
public:
    /**
     * @brief Simple way to generate EVs from different vendors
     *
     * @param vendorNum how many vendors we have
     * @param totalNum  how many EVs
     *
     * @return list of EVs
     */
    static vector<int> randomGen(int vendorNum, int totalNum);
    SimpleRandomGenAlg() = delete;
};
