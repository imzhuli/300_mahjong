#include <mahjong/mahjong.hpp>
#include <xel/Util/Command.hpp>
#include <cstring>

namespace xmj {

    using namespace std;
    using namespace xel;

    X_INLINE uint64_t MakeVersion(uint16_t Major, uint16_t Minor, uint16_t Sub1 = 0, uint16_t Sub2 = 0)
    {
        return (uint64_t(Major) << 48) | (uint64_t(Minor) << 32) | (uint64_t(Sub1) << 16) | (uint64_t)Sub2;
    }

    uint64_t Version() {
        return MakeVersion(0x00, 0x01, 0x00, 0x00);
    }

    std::string VersionString()
    {
        auto V = Version();
        auto Major = (unsigned)(V >> 48);
        auto Minor = (unsigned)((V & 0x0000'FFFF'0000'0000) >> 32);
        auto Sub1  = (unsigned)((V & 0x0000'0000'FFFF'0000) >> 16);
        auto Sub2  = (unsigned)((V & 0x0000'0000'0000'FFFF) >> 0);

        char Buffer[64];
        sprintf(Buffer, "%u.%u.%u.%u", Major, Minor, Sub1, Sub2);
        return Buffer;
    }

    static std::vector<xTileLian> FindLian(const xTilePool & Pool, size_t Start = 1)
    {
        auto List = std::vector<xTileLian>();
        for (auto i = Start; i <= 7; ++i) {
            if (Pool[i] && Pool[i+1] && Pool[i+2]) {
                List.push_back({i, i + 1, i + 2});
            }
        }
        return List;
    }

    static std::vector<xTileKe> FindKe(const xTilePool & Pool, size_t Start = 1)
    {
        auto List = std::vector<xTileKe>();
        for (auto i = Start; i <= 9; ++i) {
            if (Pool[i] >= 3) {
                List.push_back({i, i, i});
            }
        }
        return List;
    }

    X_STATIC_INLINE void ApplyDazi(xTilePool & Pool, const xTileDazi & Dazi)
    {
        --Pool[Dazi[0]];
        --Pool[Dazi[1]];
        --Pool[Dazi[2]];
    }

    X_STATIC_INLINE bool operator == (const xTileSelection & lhs, const xTileSelection & rhs) {
        return lhs.AppliedDaziList == rhs.AppliedDaziList;
    }

    X_STATIC_INLINE bool operator < (const xTileSelection & lhs, const xTileSelection & rhs) {
        return lhs.AppliedDaziList < rhs.AppliedDaziList;
    }

    vector<xTileSelection> FindPatterns(const xTilePool & StartPool)
    {
        assert(!StartPool[0]);
        auto KeStates = array<vector<xTileSelection>, 5>{ vector<xTileSelection>{ {StartPool, {}} } };
        for (size_t Ke = 1; Ke <= 4; ++Ke) {
            const auto & PrioStateList = KeStates[Ke - 1];
            auto & StateList = KeStates[Ke];
            auto TempStateList = vector<xTileSelection>();
            for( auto & PrioState : PrioStateList) {
                auto Choices = FindKe(PrioState.Pool);
                for (auto & Choice: Choices) {
                    auto Pool = PrioState.Pool; // make copy;
                    ApplyDazi(Pool, Choice);
                    auto AppliedDaziList = PrioState.AppliedDaziList; // make copy;
                    AppliedDaziList.push_back(Choice);
                    sort(AppliedDaziList.begin(), AppliedDaziList.end());
                    TempStateList.push_back({ Pool, AppliedDaziList });
                }
            }
            sort(TempStateList.begin(), TempStateList.end());
            size_t Src = 0;
            size_t End = TempStateList.size();
            if (End) {
                while(true) {
                    StateList.push_back(TempStateList[Src]);
                    do {
                        ++Src;
                        if (Src >= End) {
                            goto EndOfUnify;
                        }
                    } while(StateList.back() == TempStateList[Src]);
                }
                EndOfUnify:Pass();
            }
        }

        auto TempFinalStates = vector<xTileSelection>();
        auto FinalStates = vector<xTileSelection>();
        for (size_t Ke = 0; Ke <= 4; ++Ke) {
            auto & KeStateList = KeStates[Ke];
            for(auto & KeState : KeStateList) {
                // apply all
                TempFinalStates.push_back(KeState);
                auto LianStates = array<vector<xTileSelection>, 5>{ vector<xTileSelection>{ KeState }};
                for (size_t Lian = 1; Lian <= 4 - Ke; ++Lian) {
                    const auto & PrioStateList = LianStates[Lian - 1];
                    auto & StateList = LianStates[Lian];
                    for (auto & PrioState : PrioStateList) {
                        auto Choices = FindLian(PrioState.Pool);
                        for (auto & Choice: Choices) {
                            auto Pool = PrioState.Pool; // make a copy;
                            ApplyDazi(Pool, Choice);
                            auto AppliedDaziList = PrioState.AppliedDaziList; // make copy;
                            AppliedDaziList.push_back(Choice);
                            sort(AppliedDaziList.begin(), AppliedDaziList.end());
                            StateList.push_back({ Pool, AppliedDaziList });
                        }
                    }
                    for (auto & LianState : StateList) {
                        TempFinalStates.push_back(LianState);
                    }
                }
            }
        }
        sort(TempFinalStates.begin(), TempFinalStates.end());
        size_t Src = 0;
        size_t End = TempFinalStates.size();
        if (End) {
            while(true) {
                FinalStates.push_back(TempFinalStates[Src]);
                do {
                    ++Src;
                    if (Src >= End) {
                        goto EndOfFinalUnify;
                    }
                } while(FinalStates.back() == TempFinalStates[Src]);
            }
            EndOfFinalUnify:Pass();
        }

        return FinalStates;
    }

}
