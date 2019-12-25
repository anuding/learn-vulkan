//
// Created by anuding on 2019/12/24.
//

#include "Game.h"
#include "CommandBuffer.h"


void Engine::Game::update() {
    Application::update();
}

Engine::Game::Game() {
    init();
}

void Engine::Game::init() {
    std::vector<Vertex> vertices = {{{0.0f,  -0.5f, 0.5f},
                                            {1.0f, 0.4f, 0.0f}},
                                    {{0.5f,  0.5f,  1.1f},
                                            {0.0f, 1.0f, 0.2f}},
                                    {{-0.5f, 0.5f,  1.1f},
                                            {0.0f, 0.2f, 1.0f}}};
    bufferManager.createVertexBuffer(_physicalDevice, _device, _vertexBuffer, _vertexBufferMemory,
                                     vertices);
    RenderCore::CommandHelper::createCommandBuffers(_device, _commandBuffers, _swapChainFrameBuffers, _commandPool,
                                                    _renderPass, _swapChainExtent, _graphicsPipeline, _vertexBuffer,
                                                    static_cast<uint32_t >(vertices.size()));


}
