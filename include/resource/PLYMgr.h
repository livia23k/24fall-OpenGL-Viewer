#pragma once
#include "PLYModel.h"

#include <string>

struct PLYMgr
{
    std::vector<std::shared_ptr<PLYModel>> models;

    PLYMgr() = default;
    ~PLYMgr() = default;

    bool LoadPLY(const std::string &filepath);
};
