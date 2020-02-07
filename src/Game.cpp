//
// Created by anuding on 2019/12/24.
//

#include "Game.h"
#include "Command.h"
#include "VKContext.h"
#include "Descriptor.h"
#include "Texture.h"
#include "Sampler.h"
void Engine::Game::update() {
	Application::update();
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

	//create buffer
	RenderCore::BufferHelper::init(scene);

	//create textures
	Texture* texture = new Texture("assets/textures/logo400px.png");
	RenderCore::textureImage = texture->image;
	RenderCore::textureImageMemory = texture->memory;
	RenderCore::textureImageView = texture->imageView;

	//create samplers
	RenderCore::textureSampler = Sampler(RenderCore::device).get();
	
	RenderCore::DescriptorHelper::createDescriptorSets();



	//record commands and begin
	RenderCore::CommandHelper::recordCommandBuffers(
		static_cast<uint32_t>(scene.getGameObjects()[0].getMesh().getVertices().size()),
		static_cast<uint32_t>(scene.getGameObjects()[0].getMesh().getIndices().size()),
		scene
	);
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
