#include <mahjong/mahjong.hpp>
#include <array>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace xel;
using namespace xmj;

using xTilePool = std::array<size_t, 10>;
using xTileDazi = std::array<size_t, 3>;
using xTileLian = xTileDazi;
using xTileKe   = xTileDazi;

[[maybe_unused]] static constexpr const xTilePool InitPool = {
    0, 4, 4, 4, 4,  // 0 - 4
    4, 4, 4, 4, 4,  // 5 - 9
};

std::vector<xTileLian> FindLian(const xTilePool & Pool)
{
    auto List = std::vector<xTileLian>();
    for (auto i = size_t(1); i <= 7; ++i) {
        if (Pool[i] && Pool[i+1] && Pool[i+2]) {
            List.push_back({i, i + 1, i + 2});
        }
    }
    return List;
}

std::vector<xTileKe> FindKe(const xTilePool & Pool)
{
    auto List = std::vector<xTileKe>();
    for (auto i = size_t(1); i <= 9; ++i) {
        if (Pool[i] >= 3) {
            List.push_back({i, i, i});
        }
    }
    return List;
}

void ApplyDazi(xTilePool & Pool, const xTileDazi & Dazi)
{
    --Pool[Dazi[0]];
    --Pool[Dazi[1]];
    --Pool[Dazi[2]];
}

struct xTileSelection
{
    xTilePool          Pool;
    vector<xTileDazi>  AppliedDaziList;
};

bool operator == (const xTileSelection & lhs, const xTileSelection & rhs) {
    return lhs.AppliedDaziList == rhs.AppliedDaziList;
}

bool operator < (const xTileSelection & lhs, const xTileSelection & rhs) {
    return lhs.AppliedDaziList < rhs.AppliedDaziList;
}

int main(int argc, char * argv[])
{
    cout << "Mahjong Version: " << xmj::VersionString() << endl;

    auto KeStates = array<vector<xTileSelection>, 5>{ vector<xTileSelection>{ {InitPool, {}} } };
    for (size_t Ke = 1; Ke <= 4; ++Ke) {
        const auto & PrePools = KeStates[Ke - 1];
        auto & Pools = KeStates[Ke];
        auto TempPool = vector<xTileSelection>();
        for( auto & PrePool : PrePools) {
            auto Choices = FindKe(PrePool.Pool);
            for (auto & Choice: Choices) {
                auto Pool = PrePool.Pool; // make copy;
                ApplyDazi(Pool, Choice);
                auto AppliedDaziList = PrePool.AppliedDaziList; // make copy;
                AppliedDaziList.push_back(Choice);
                sort(AppliedDaziList.begin(), AppliedDaziList.end());
                TempPool.push_back({ Pool, AppliedDaziList });
            }
        }
        sort(TempPool.begin(), TempPool.end());
        size_t Src = 0;
        size_t End = TempPool.size();
        assert(End);
        while(true) {
            Pools.push_back(TempPool[Src]);
            do {
                ++Src;
                if (Src >= End) {
                    goto EndOfUnify;
                }
            } while(Pools.back() == TempPool[Src]);
        }
    EndOfUnify:;


        // step: find Lian
    }

    for (auto & KS : KeStates) {
        for (auto TS : KS) {
            auto DZList = TS.AppliedDaziList;
            for (auto & DZ : DZList) {
                cout << DZ[0] << "," << DZ[1] << "," << DZ[2] << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
