#pragma once
#include "PLYModel.h"

#include <string>

struct PLYMgr
{
    std::vector<PLYModel> models;
    std::vector<PLYModel> favorites;

    PLYMgr() = default;
    ~PLYMgr() = default;

    bool LoadPLY(const std::string &filepath);
};
