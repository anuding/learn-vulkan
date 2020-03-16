//
// Created by anuding on 2019/12/24.
//

#include "Game.h"
#include "Command.h"
#include "VKContext.h"
#include "Descriptor.h"
#include "Texture.h"
#include "Sampler.h"
#include "Mesh.h"
void Engine::Game::update() {
	Application::update();
}
Engine::Game::~Game()
{
	//destroy gameObjs

}
Engine::Game::Game() {
	init();
}

void Engine::Game::init() {

}

Engine::Game::Game(Scene& scene) {
	scenes.push_back(scene);
	//loading assets
	RenderCore::assetManager.load("assets/textures/logo400px.png");

	//create vertex buffer and index buffer
	RenderCore::BufferHelper::createVertexBuffer(scene.getGameObjects()[0].getComponent<Mesh>()->vertices,
		RenderCore::vertexBuffer, RenderCore::vertexBufferMemory);
	RenderCore::BufferHelper::createIndexBuffer(scene.getGameObjects()[0].getComponent<Mesh>()->indices,
		RenderCore::indexBuffer, RenderCore::indexBufferMemory);
	RenderCore::BufferHelper::createUniformBuffer();

	//create textures
	Texture* texture = new Texture("assets/textures/logo400px.png");
	RenderCore::textureImageView = texture->imageView;

	//create samplers
	RenderCore::textureSampler = Sampler(RenderCore::device).get();
	
	//update descriptorsSets
	RenderCore::DescriptorHelper::updateDescriptorSets();

	//record commands and begin
	RenderCore::CommandHelper::recordCommandBuffers(scene);
}

const std::vector<Scene>& Engine::Game::getScenes() const {
	return scenes;
}

void Engine::Game::setScenes(const std::vector<Scene>& scenes) {
	Game::scenes = scenes;
}

const Scene& Engine::Game::getCurrentScene() const {
	return scenes[0];
}
