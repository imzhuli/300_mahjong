#include <mahjong/mahjong.hpp>
#include <xel/Util/Command.hpp>
#include <cstring>
#include <iostream>

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

    vector<xTileSelection> FindPatterns(const xTilePool & StartPool)
    {
        assert(!StartPool[0]);
        size_t Total = 0;
        for (auto Count : StartPool) {
            assert(Count <= 4);
            Total += Count;
        }
        Total /= 3;
        auto KeStates = vector<vector<xTileSelection>>{ vector<xTileSelection>{ {StartPool, {}} } };
        KeStates.resize(Total + 1);
        for (size_t Ke = 1; Ke < KeStates.size(); ++Ke) {
            const auto & PrioStateList = KeStates[Ke - 1];
            auto & StateList = KeStates[Ke];
            auto TempStateList = vector<xTileSelection>();

            for( auto & PrioState : PrioStateList) {
                size_t Start = PrioState.AppliedDaziList.size() ? (PrioState.AppliedDaziList.back()[0] + 1) : 1;
                auto Choices = FindKe(PrioState.Pool, Start);
                for (auto & Choice: Choices) {
                    auto Pool = PrioState.Pool; // make copy;
                    ApplyDazi(Pool, Choice);
                    auto AppliedDaziList = PrioState.AppliedDaziList; // make copy;
                    AppliedDaziList.push_back(Choice);
                    StateList.push_back({ Pool, std::move(AppliedDaziList) });
                }
            }
        }

        auto TempFinalStates = vector<xTileSelection>();
        // auto FinalStates = vector<xTileSelection>();
        for (size_t Ke = 0; Ke <= Total; ++Ke) {
            auto & KeStateList = KeStates[Ke];
            for(auto & KeState : KeStateList) {
                // apply all
                TempFinalStates.push_back(KeState);
                auto LianStates = vector<vector<xTileSelection>>{ vector<xTileSelection>{ KeState }};
                LianStates.resize(Total - Ke + 1);
                for (size_t Lian = 1; Lian < LianStates.size(); ++Lian) {
                    const auto & PrioStateList = LianStates[Lian - 1];
                    auto & StateList = LianStates[Lian];
                    for (auto & PrioState : PrioStateList) {
                        size_t Start = PrioState.AppliedDaziList.size() ? (
                            PrioState.AppliedDaziList.back()[0] == PrioState.AppliedDaziList.back()[1] ? 1 : PrioState.AppliedDaziList.back()[0]) : 1;
                        auto Choices = FindLian(PrioState.Pool, Start);
                        for (auto & Choice: Choices) {
                            auto Pool = PrioState.Pool; // make a copy;
                            ApplyDazi(Pool, Choice);
                            auto AppliedDaziList = PrioState.AppliedDaziList; // make copy;
                            AppliedDaziList.push_back(Choice);
                            StateList.push_back({ Pool, std::move(AppliedDaziList) });
                        }
                    }
                    for (auto & LianState : StateList) {
                        TempFinalStates.push_back(LianState);
                    }
                }
            }
        }
        return TempFinalStates;
    }

}
