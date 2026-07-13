#include "graphboard/authoring.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <map>
#include <stdexcept>
#include <vector>

namespace graphboard::authoring {

namespace {

struct ColorSample {
    std::uint8_t r=0,g=0,b=0;
    std::uint32_t count=0;
};

struct ColorBox {
    std::vector<ColorSample> colors;
    std::uint64_t population=0;
    std::uint8_t minR=0,maxR=0,minG=0,maxG=0,minB=0,maxB=0;
};

void measure(ColorBox& box) {
    box.population=0;
    if(box.colors.empty())return;
    box.minR=box.maxR=box.colors[0].r;box.minG=box.maxG=box.colors[0].g;box.minB=box.maxB=box.colors[0].b;
    for(const auto& c:box.colors){box.population+=c.count;box.minR=std::min(box.minR,c.r);box.maxR=std::max(box.maxR,c.r);
        box.minG=std::min(box.minG,c.g);box.maxG=std::max(box.maxG,c.g);box.minB=std::min(box.minB,c.b);box.maxB=std::max(box.maxB,c.b);}
}

std::pair<ColorBox,ColorBox> split(ColorBox box) {
    const int rr=box.maxR-box.minR,gr=box.maxG-box.minG,br=box.maxB-box.minB;
    int channel=0;if(gr>rr&&gr>=br)channel=1;else if(br>rr&&br>gr)channel=2;
    std::stable_sort(box.colors.begin(),box.colors.end(),[&](const auto& a,const auto& b){
        return channel==0?a.r<b.r:channel==1?a.g<b.g:a.b<b.b;});
    const auto half=box.population/2;std::uint64_t sum=0;std::size_t cut=1;
    for(;cut<box.colors.size();++cut){sum+=box.colors[cut-1].count;if(sum>=half)break;}
    ColorBox left,right;left.colors.assign(box.colors.begin(),box.colors.begin()+static_cast<std::ptrdiff_t>(cut));
    right.colors.assign(box.colors.begin()+static_cast<std::ptrdiff_t>(cut),box.colors.end());measure(left);measure(right);return {std::move(left),std::move(right)};
}

PaletteColor average(const ColorBox& box) {
    if(box.population==0)return {};
    std::uint64_t r=0,g=0,b=0;for(const auto& c:box.colors){r+=static_cast<std::uint64_t>(c.r)*c.count;g+=static_cast<std::uint64_t>(c.g)*c.count;b+=static_cast<std::uint64_t>(c.b)*c.count;}
    return {static_cast<std::uint8_t>(r/box.population),static_cast<std::uint8_t>(g/box.population),static_cast<std::uint8_t>(b/box.population),0};
}

} // namespace

Palette generatePalette(const std::vector<RgbaImage>& images,std::uint8_t transparentIndex,std::uint8_t alphaCutoff) {
    std::map<std::uint32_t,std::uint32_t> histogram;
    for(const auto& image:images){if(image.width==0||image.height==0||image.pixels.size()!=static_cast<std::uint64_t>(image.width)*image.height*4)throw std::invalid_argument("RGBA image has invalid pixels");
        for(std::size_t i=0;i<image.pixels.size();i+=4){if(image.pixels[i+3]<alphaCutoff)continue;
            const std::uint32_t key=((image.pixels[i]>>3)<<10)|((image.pixels[i+1]>>3)<<5)|(image.pixels[i+2]>>3);++histogram[key];}}
    ColorBox initial;initial.colors.reserve(histogram.size());for(const auto& [key,count]:histogram)initial.colors.push_back({
        static_cast<std::uint8_t>(((key>>10)&31)*255/31),static_cast<std::uint8_t>(((key>>5)&31)*255/31),static_cast<std::uint8_t>((key&31)*255/31),count});measure(initial);
    std::vector<ColorBox> boxes;if(!initial.colors.empty())boxes.push_back(std::move(initial));
    const std::size_t target=255;
    while(boxes.size()<target){auto it=std::max_element(boxes.begin(),boxes.end(),[](const auto& a,const auto& b){
            const auto ar=(a.maxR-a.minR)+(a.maxG-a.minG)+(a.maxB-a.minB);const auto br=(b.maxR-b.minR)+(b.maxG-b.minG)+(b.maxB-b.minB);
            return ar*a.population<br*b.population;});
        if(it==boxes.end()||it->colors.size()<2)break;auto pair=split(std::move(*it));*it=std::move(pair.first);boxes.push_back(std::move(pair.second));}
    Palette palette=makeRgb332Palette();std::size_t box=0;for(std::size_t i=0;i<palette.size()&&box<boxes.size();++i){if(i==transparentIndex)continue;palette[i]=average(boxes[box++]);}
    palette[transparentIndex]={255,0,255,0};return palette;
}

IndexedImage quantizeImage(const RgbaImage& image,const Palette& palette,std::uint8_t transparentIndex,std::uint8_t alphaCutoff) {
    if(image.width==0||image.height==0||image.pixels.size()!=static_cast<std::uint64_t>(image.width)*image.height*4)throw std::invalid_argument("RGBA image has invalid pixels");
    IndexedImage out;out.width=image.width;out.height=image.height;out.transparentIndex=transparentIndex;out.sourceRgba=image.pixels;out.alphaCutoff=alphaCutoff;out.pixels.resize(static_cast<std::size_t>(image.width)*image.height);
    for(std::size_t p=0,i=0;p<out.pixels.size();++p,i+=4){if(image.pixels[i+3]<alphaCutoff){out.pixels[p]=transparentIndex;continue;}
        std::uint32_t best=std::numeric_limits<std::uint32_t>::max();std::uint8_t index=0;
        for(std::size_t c=0;c<palette.size();++c){if(c==transparentIndex)continue;const int dr=static_cast<int>(image.pixels[i])-palette[c].r;
            const int dg=static_cast<int>(image.pixels[i+1])-palette[c].g;const int db=static_cast<int>(image.pixels[i+2])-palette[c].b;
            const auto distance=static_cast<std::uint32_t>(dr*dr+dg*dg+db*db);if(distance<best){best=distance;index=static_cast<std::uint8_t>(c);}}
        out.pixels[p]=index;}
    return out;
}

AssetConversionResult convertAsset(const RgbaImage& image,const Palette& palette,std::uint8_t transparentIndex,std::uint8_t alphaCutoff) {
    AssetConversionResult result;result.indexed=quantizeImage(image,palette,transparentIndex,alphaCutoff);double squaredError=0;
    for(std::size_t i=0,p=0;i<result.indexed.pixels.size();++i,p+=4){if(image.pixels[p+3]<alphaCutoff){++result.diagnostic.transparentPixels;continue;}++result.diagnostic.opaquePixels;const auto& mapped=palette[result.indexed.pixels[i]];
        const double dr=static_cast<double>(image.pixels[p])-mapped.r,dg=static_cast<double>(image.pixels[p+1])-mapped.g,db=static_cast<double>(image.pixels[p+2])-mapped.b;squaredError+=dr*dr+dg*dg+db*db;}
    if(result.diagnostic.opaquePixels)result.diagnostic.meanSquaredRgbError=squaredError/(result.diagnostic.opaquePixels*3.0);
    const auto stride=(static_cast<std::size_t>(image.width)+3)&~std::size_t{3};result.bottomUpStride4Pixels.assign(stride*image.height,transparentIndex);
    for(std::uint32_t y=0;y<image.height;++y){const auto source=static_cast<std::size_t>(image.height-1-y)*image.width;std::copy_n(result.indexed.pixels.begin()+static_cast<std::ptrdiff_t>(source),image.width,result.bottomUpStride4Pixels.begin()+static_cast<std::ptrdiff_t>(y*stride));}
    return result;
}

} // namespace graphboard::authoring
