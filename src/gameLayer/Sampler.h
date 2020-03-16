#include <vulkan/vulkan.h>

class Sampler
{
public:
	Sampler(VkDevice device);
	~Sampler();
	VkSampler get();
private:
	VkSampler sampler;
};