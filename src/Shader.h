//
// Created by anuding on 2019/11/24.
//
#include <fstream>
#include <vector>
#include <vulkan/vulkan.h>
#include <glm/detail/type_mat2x2.hpp>

namespace Engine::RenderCore::ShaderHelper {
        struct UniformBufferObject{
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };
        std::vector<char> readFile(const std::string & filename);
        VkShaderModule createShaderModule(const std::vector<char>& code);

}