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
        void batchLoadFromFile(const char* path);
        Key load(const char* path);
        const std::unordered_map<Key, Asset>& getAssetMap() const;
        const Asset& getAssetByKey(Key key) const;
    private:
        std::unordered_map<Key, Asset> assetMap;
    public:
       
    };
}
#endif //ENGINE_ASSETMANAGER_H
