#pragma once
//
// Created by anuding on 2019/11/24.
//

#include "Shader.h"
#include "Application.h"
#include "../utils/FileUtil.h"
#include <iostream>
#include <string>
namespace Engine::RenderCore {
    Shader::~Shader()
    {
        vkDestroyShaderModule(app->device.get(), this->get(), nullptr);
    }
    void Shader::init(Application* app)
    {
        this->app = app;
    }
    void Shader::load(const char* path)
    {
        createShaderModule(Utils::readFile(path));
    }
    void Shader::createShaderModule(const std::vector<char> &code) {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        if (vkCreateShaderModule(app->device.get(), &createInfo, nullptr, &this->get()) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }
}