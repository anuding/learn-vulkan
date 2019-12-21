//
// Created by anuding on 2019/11/24.
//
#include <fstream>
#include <vector>
#include <vulkan/vulkan.h>

namespace Engine::RenderCore {
    namespace ShaderHelper {
        std::vector<char> readFile(const std::string & filename);
        VkShaderModule createShaderModule(const VkDevice& device,const std::vector<char>& code);
    }
}