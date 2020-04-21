#pragma once
#include <vulkan/vulkan.h>
#include "VKComp.h"

namespace Engine::RenderCore {
	class Buffer : public VkComp<VkBuffer>{
	public:
		Buffer() = default;
		~Buffer();

		virtual void init(Application* app) override;
		void createBuffer(const VkBufferCreateInfo &createInfo, const VkMemoryPropertyFlags& properties);
		void updateBufferMemory(unsigned char* data, uint32_t size);
	private:
		VkBufferView bufferView;
		VkDeviceMemory bufferMemory;

		uint32_t findBufferMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	};
}