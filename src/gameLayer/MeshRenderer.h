#include "Component.h"
#include "Material.h"
#include <vector>
class MeshRenderer : public Componenet
{
public:
	MeshRenderer();
	~MeshRenderer();

	std::vector<Material> materials;
};
