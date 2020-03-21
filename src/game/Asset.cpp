//
// Created by anuding on 2019/12/29.
//

#include "Asset.h"

unsigned char *Asset::getData() const {
    return data;
}

void Asset::setData(unsigned char *data) {
    Asset::data = data;
}

const AssetMetaInfo &Asset::getMetaInfo() const {
    return metaInfo;
}

void Asset::setMetaInfo(const AssetMetaInfo &metaInfo) {
    Asset::metaInfo = metaInfo;
}
