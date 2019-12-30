//
// Created by anuding on 2019/12/29.
//

#ifndef ENGINE_ASSET_H
#define ENGINE_ASSET_H

#include <string>
struct AssetMetaInfo{
    int width;
    int height;
    int channels;
    std::string name;
    int size;
};

class Asset {
private:
    unsigned char* data;
    AssetMetaInfo metaInfo;
public:
    unsigned char *getData() const;

    void setData(unsigned char *data);

    const AssetMetaInfo &getMetaInfo() const;

    void setMetaInfo(const AssetMetaInfo &metaInfo);
};


#endif //ENGINE_ASSET_H
