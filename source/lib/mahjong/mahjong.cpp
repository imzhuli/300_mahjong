#include <mahjong/mahjong.hpp>
#include <xel/Util/Command.hpp>
#include <cstring>

namespace xmj {

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

}
