//
// Created by anuding on 2019/12/29.
//

#include "AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

#include "VKContext.h"
#include <stdexcept>

namespace Engine::RenderCore::Resource {

    void AssetManager::loadAssets() {
        Asset imageAsset;
        AssetMetaInfo metaInfo;

        stbi_uc *pixels = stbi_load("assets/textures/logo500px.png", &metaInfo.width, &metaInfo.height,
                                    &metaInfo.channels,
                                    STBI_rgb_alpha);
        if (!pixels)
            throw std::runtime_error("failed to load image");
        metaInfo.size = metaInfo.width * metaInfo.height * metaInfo.channels;
        imageAsset.setData(pixels);
        imageAsset.setMetaInfo(metaInfo);
        assetMap.insert(std::make_pair<std::string, Asset>("demo", static_cast<Asset &&>(imageAsset)));
        stbi_image_free(pixels);
    }

    const std::unordered_map<std::string, Asset> &AssetManager::getAssetMap() const {
        return assetMap;
    }
}