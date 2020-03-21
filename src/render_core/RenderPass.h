#pragma once
//
// Created by anuding on 2019/12/21.
//

#include <vulkan/vulkan.h>
#include "VkComp.h"
namespace Engine::RenderCore {

	class RenderPass : public VkComp<VkRenderPass>
	{
	public:
		RenderPass() =default;
		~RenderPass();

		// Inherited via VkComp
		virtual void init(Application* app) override;
	private:
		void createRenderPass();
	};
}