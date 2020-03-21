GameObject:
- Components[]:
    - Transform (ubo)
    - Mesh (vbo vao)
    - MeshRenderer:
        - materials[]:
            - texPairs[] 
                1. texPath
                2. texPtr (shared_ptr<Texture>)
            - shaderSets[]
                1. vertexShader
                    1. shaderPath
                    2. shaderPtr (shared_ptr<Shader>)
                2. fragmentShader
                    1. shaderPath
                    2. shaderPtr

Asset.h
``` cpp
unordered_map<uint64_t, shared_ptr<Asset>>AssetManager 

class Asset{
public:
    const char* path;
    uint64_t id;
}

enum SHADER_TYPE{
    VERTEX, FRAGMENT
}

class Shader{
    SHADER_TYPE shaderType;
    VkShaderModule shaderModule;
}

class Texture{
    unsigned char* fileData;
	VkImage image;
	VkDeviceMemory memory;
	VkImageView imageView;
}
```



loop over gameObjects:
``` c++
//loading
for(auto o : scene->gameObjects)
{
    //load materials
    for(auto mat : o.MeshRenderer->materials)
    {
        //load textures
        for(auto texPair : mat->texPairs)
        {
            Asset<Texture> asset_t(tex.first);
            tex.second = make_shared<Asset>(asset_t);
            assetManager.add(tex.second);
        } 
        //load vertex shader                                
    } 

}
```
