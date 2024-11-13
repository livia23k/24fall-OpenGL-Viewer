#pragma once
#include "PLYModel.h"

#include <string>

struct PLYMgr
{
    std::vector<PLYModel> models;

    PLYMgr() = default;
    ~PLYMgr() = default;

    bool LoadPLY(const std::string &filepath);
};
