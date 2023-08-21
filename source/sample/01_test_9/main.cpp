#include <mahjong/mahjong.hpp>
#include <array>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace xel;
using namespace xmj;


int main(int argc, char * argv[])
{
    cout << "Mahjong Version: " << xmj::VersionString() << endl;

    [[maybe_unused]] xTilePool TestPool = {
        0,
        0, 3, 4, 2, 0, 3, 0, 0, 2
    };

    auto FinalStates = FindPatterns(TestPool);
    for (auto TS : FinalStates) {
        auto DZList = TS.AppliedDaziList;
        for (auto & DZ : DZList) {
            cout << DZ[0] << "," << DZ[1] << "," << DZ[2] << " ";
        }
        cout << endl;
    }

    return 0;
}
