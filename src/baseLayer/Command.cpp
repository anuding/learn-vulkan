//
// Created by anuding on 2019/12/21.
//

#include <stdexcept>
#include "Command.h"
#include "Queue.h"
#include "VKContext.h"
#include "Mesh.h"

namespace Engine::RenderCore::CommandHelper {
	void createCommandPool() {
		Queue::QueueFamilyIndices queueFamilyIndices = Queue::findQueueFamilies(physicalDevice);
		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool");
		}
	}

	void recordCommandBuffers(Scene& scene) {
		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = nullptr;

			VkRenderPassBeginInfo renderPassBeginInfo = {};
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.renderPass = renderPass;
			renderPassBeginInfo.clearValueCount = 1;
			renderPassBeginInfo.framebuffer = swapChainFrameBuffers[i];
			renderPassBeginInfo.renderArea.offset = { 0, 0 };
			renderPassBeginInfo.renderArea.extent = swapChainExtent;
			VkClearValue clearValue = { 0.2f, 0.2f, 0.2f, 1.0f };
			renderPassBeginInfo.pClearValues = &clearValue;


			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin");
			}
			vkCmdBeginRenderPass(commandBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

			VkBuffer vertexBuffers[] = { vertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			for (auto& gameObj : scene.getGameObjects()) {
				vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers,
					offsets);
				vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);

				vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,
					pipelineLayout, 0, 1,
					&descriptorSets[i], 0,
					nullptr);
				vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(gameObj.getComponent<Mesh>()->indices.size()), 1,
					0, 0, 0);
			}
			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer");
			}
		}

	}

	VkCommandBuffer beginSingleTimeCommands()
	{
		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandPool = commandPool;
		allocateInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer);

		VkCommandBufferBeginInfo commandBeginInfo = {};
		commandBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &commandBeginInfo);
		return commandBuffer;
	}

	void endSingleCommandBuffer(VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
	}

	void allocateCommandBuffers() {
		commandBuffers.resize(swapChainFrameBuffers.size());
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = (uint32_t)commandBuffers.size();
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command buffers");
		}
	}

	void init() {
		createCommandPool();
		allocateCommandBuffers();
	}
}