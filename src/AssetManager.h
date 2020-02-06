//
// Created by anuding on 2019/12/29.
//

#ifndef ENGINE_ASSETMANAGER_H
#define ENGINE_ASSETMANAGER_H

#include "Asset.h"
#include <unordered_map>
namespace Engine::RenderCore::Resource {
    class AssetManager {
    public:
        void loadAssets();
    private:

        std::unordered_map<std::string, Asset> assetMap;
    public:
        const std::unordered_map<std::string, Asset> &getAssetMap() const;
    };
}
#endif //ENGINE_ASSETMANAGER_H
