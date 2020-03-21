#pragma once

#include <vector>
#include <string>
#include <fstream>

namespace Engine::Utils {
    std::vector<char> readFile(const std::string& filename);
}