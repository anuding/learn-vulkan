//
// Created by anuding on 2019/12/29.
//

#include "AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"

#include "VKContext.h"
#include <stdexcept>
#include <functional>
namespace Engine::RenderCore::Resource {
	void AssetManager::batchLoadFromFile(const char* path)
	{
	}

	Key AssetManager::load(const char* path) {
		Key key = std::hash<std::string>{}(path);
		auto searchResult = assetMap.find(key);
		if (searchResult != assetMap.end())
			return searchResult->first;
		
		Asset imageAsset;
		AssetMetaInfo metaInfo;

		stbi_uc* pixels = stbi_load(path, &metaInfo.width, &metaInfo.height,
			&metaInfo.channels,
			STBI_rgb_alpha);
		if (!pixels)
			throw std::runtime_error("failed to load image");
		metaInfo.size = metaInfo.width * metaInfo.height * metaInfo.channels;
		imageAsset.setData(pixels);
		imageAsset.setMetaInfo(metaInfo);
		assetMap.insert(std::make_pair<Key, Asset>(std::hash<std::string>{}(path), static_cast<Asset&&>(imageAsset)));
		return key;
	}

	const std::unordered_map<Key, Asset>& AssetManager::getAssetMap() const {
		return assetMap;
	}
	const Asset& AssetManager::getAssetByKey(Key key) const
	{
		auto searchResult = assetMap.find(key);
		if (searchResult != assetMap.end())
			return searchResult->second;
		else
			throw std::exception("no such key in asset map");
	}
}