#pragma once

#include <xel/Common.hpp>
#include <array>
#include <vector>
#include <string>

namespace xmj
{

    using xTilePool = std::array<size_t, 10>;
    using xTileDazi = std::array<size_t, 3>;
    using xTileLian = xTileDazi;
    using xTileKe   = xTileDazi;

    struct xTileSelection
    {
        xTilePool               Pool;
        std::vector<xTileDazi>  AppliedDaziList;
    };

    [[maybe_unused]] static constexpr const xTilePool InitPool = {
        0, 4, 4, 4, 4,  // 0 - 4
        4, 4, 4, 4, 4,  // 5 - 9
    };

    X_EXTERN uint64_t    Version();
    X_EXTERN std::string VersionString();
    X_EXTERN std::vector<xTileSelection> FindPatterns(const xTilePool & StartPool);

}

