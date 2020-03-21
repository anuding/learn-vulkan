#include <vulkan/vulkan.h>
#include "Asset.h"
class Texture
{
public:
	Texture(const char* path);
	~Texture();


	VkImage image;
	VkDeviceMemory memory;
	VkImageView imageView;

private:
	void createTextureBuffer(Key key);

	VkImageView createImageView();
};
