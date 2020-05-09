#pragma once

#include "Common/Entity/Entity.h"

#include <vector>
#include <memory>

struct World
{
    std::vector<std::unique_ptr<Entity>> entities;
};