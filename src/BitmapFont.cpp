//
//  BitmapFont.cpp
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include "BitmapFont.h"
#include "Util.h"

//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Dss data = nlohmann::json::parse(jsonString);

#include "nlohmann/json.hpp"

#include <optional>
#include <regex>
#include <stdexcept>
#include <stdio.h>

#include "SDL.h"

#include "stb_image.h"
#include "stb_image_write.h"

namespace BMF {
    using nlohmann::json;

    inline json get_untyped(const json &j, const char *property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json &j, std::string property) {
        return get_untyped(j, property.data());
    }

    class Info {
      public:
        Info() = default;
        virtual ~Info() = default;

      private:
        std::string fontface;
        int64_t lineHeight;
        int64_t base;

      public:
        const std::string &getFontface() const { return fontface; }
        std::string &getMutableFontface() { return fontface; }
        void setFontface(const std::string &value) { this->fontface = value; }

        const int64_t &getLineHeight() const { return lineHeight; }
        int64_t &getMutableLineHeight() { return lineHeight; }
        void setLineHeight(const int64_t &value) { this->lineHeight = value; }

        const int64_t &getBase() const { return base; }
        int64_t &getMutableBase() { return base; }
        void setBase(const int64_t &value) { this->base = value; }
    };

    class Dss {
      public:
        Dss() = default;
        virtual ~Dss() = default;

      private:
        Info info;
        std::map<std::string, int64_t> kernings;
        std::map<std::string, int64_t> mapping;
        std::vector<std::map<std::string, int64_t>> frames;

      public:
        const Info &getInfo() const { return info; }
        Info &getMutableInfo() { return info; }
        void setInfo(const Info &value) { this->info = value; }

        const std::map<std::string, int64_t> &getKernings() const {
            return kernings;
        }
        std::map<std::string, int64_t> &getMutableKernings() {
            return kernings;
        }
        void setKernings(const std::map<std::string, int64_t> &value) {
            this->kernings = value;
        }

        const std::map<std::string, int64_t> &getMapping() const {
            return mapping;
        }
        std::map<std::string, int64_t> &getMutableMapping() { return mapping; }
        void setMapping(const std::map<std::string, int64_t> &value) {
            this->mapping = value;
        }

        const std::vector<std::map<std::string, int64_t>> &getFrames() const {
            return frames;
        }
        std::vector<std::map<std::string, int64_t>> &getMutableFrames() {
            return frames;
        }
        void
        setFrames(const std::vector<std::map<std::string, int64_t>> &value) {
            this->frames = value;
        }
    };
} // namespace BMF

namespace nlohmann {
    void from_json(const json &j, BMF::Info &x);
    void to_json(json &j, const BMF::Info &x);

    void from_json(const json &j, BMF::Dss &x);
    void to_json(json &j, const BMF::Dss &x);

    inline void from_json(const json &j, BMF::Info &x) {
        x.setFontface(j.at("fontface").get<std::string>());
        x.setLineHeight(j.at("lineHeight").get<int64_t>());
        x.setBase(j.at("base").get<int64_t>());
    }

    inline void to_json(json &j, const BMF::Info &x) {
        j = json::object();
        j["fontface"] = x.getFontface();
        j["lineHeight"] = x.getLineHeight();
        j["base"] = x.getBase();
    }

    inline void from_json(const json &j, BMF::Dss &x) {
        x.setInfo(j.at("info").get<BMF::Info>());
        x.setKernings(j.at("kernings").get<std::map<std::string, int64_t>>());
        x.setMapping(j.at("mapping").get<std::map<std::string, int64_t>>());
        x.setFrames(
            j.at("frames").get<std::vector<std::map<std::string, int64_t>>>());
    }

    inline void to_json(json &j, const BMF::Dss &x) {
        j = json::object();
        j["info"] = x.getInfo();
        j["kernings"] = x.getKernings();
        j["mapping"] = x.getMapping();
        j["frames"] = x.getFrames();
    }
} // namespace nlohmann

struct BitmapFontData {
    BMF::Dss _jsonData;
    int _width, _height, _channels_in_file;
    void *_imageFileData;

    ~BitmapFontData() { free(_imageFileData); }
};

BitmapFont *BitmapFont::sBitmapFont = nullptr;

BitmapFont::BitmapFont()
    : mCurrentBounds(glm::vec2(std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max())),
      mCurrentFontName(""), mCurrentPrintf(""),
      mGeometry(new NJLIC::SpriteGeometry()), mShader(new NJLIC::Shader()) {}

BitmapFont::~BitmapFont() {
    //    delete mMainNode;
    delete mShader;
    delete mGeometry;
    for (Map::iterator iter = mFontMap.begin(); iter != mFontMap.end();
         ++iter) {
        delete iter->second;
    }
}

bool BitmapFont::load(const std::string &fontName) {
    std::string fontNameLower(Util::tolower(fontName));

    if (mFontMap.find(fontNameLower) == mFontMap.end()) {
        const std::string base_path(SDL_GetBasePath());
        static char buff[4096];

        BitmapFontData *bfd = new BitmapFontData();

        size_t jsonFileSize;
        std::string defaultJson("assets/fonts/%s.json");
        snprintf(buff, sizeof(buff), defaultJson.c_str(), fontName.c_str());
        char *jsonFileBuffer = Util::loadFile(std::string(buff), jsonFileSize);
        if (nullptr != jsonFileBuffer) {

#if !(defined(NDEBUG))
            std::string n(std::to_string(Util::timeSinceEpochMillisec()));
            n += "_bitmapfont_";
            n += fontName;
            n += ".json";
            FILE *fp = fopen(n.c_str(), "wb");
            if (fp) {
                fwrite(jsonFileBuffer, sizeof(char), jsonFileSize, fp);

                fclose(fp);
            } else {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                             "!!! Failed to create file on the disk\n");
            }
#endif

            bfd->_jsonData = nlohmann::json::parse(jsonFileBuffer);
            free(jsonFileBuffer);
        }

        const char *vertShader =
            Util::loadFile("assets/shaders/StandardShader2.vert");
        const char *fragShader =
            Util::loadFile("assets/shaders/StandardShader2.frag");

        if (nullptr != vertShader && nullptr != fragShader) {
            const std::string &vertexSource(vertShader);
            const std::string &fragmentSource(fragShader);

            if (mShader->load(vertexSource, fragmentSource)) {

                std::string defaultImage("assets/fonts/%s.png");
                snprintf(buff, sizeof(buff), defaultImage.c_str(),
                         fontName.c_str());

                mGeometry->load(mShader, 4096);

                if (!mGeometry->loadDiffuseMatrial(mShader,
                                                   std::string(buff))) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                                 "Unable to use %s\n",
                                 std::string(buff).c_str());
                }
            }
        }

        mFontMap.insert(Pair(fontNameLower, bfd));

        return true;
    }
    return false;
}

bool BitmapFont::unLoad(const std::string &fontName) {
    std::string fontNameLower(Util::tolower(fontName));

    Map::iterator iter = mFontMap.find(fontNameLower);
    if (iter != mFontMap.end()) {
        BitmapFontData *bfd = iter->second;
        delete bfd;

        mFontMap.erase(iter);

        return true;
    }
    return false;
}

const glm::vec2 &BitmapFont::getCurrentBounds() const { return mCurrentBounds; }

const std::string &BitmapFont::getCurrentFontName() const {
    return mCurrentFontName;
}

void BitmapFont::setCurrentBounds(const glm::vec2 &bounds) {
    mCurrentBounds = bounds;
}

void BitmapFont::setCurrentFontName(const std::string &fontName) {
    mCurrentFontName = Util::tolower(fontName);
    load(mCurrentFontName);
}

NJLIC::Node *BitmapFont::printf(NJLIC::Scene *scene, const char *fmt, ...) {

    char buffer[2048];
    va_list args;

    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    mCurrentPrintf = std::string(buffer);

    NJLIC::Node *mainNode = new NJLIC::Node();
    mainNode->setName(mCurrentPrintf);

    float xStart, yStart;
    xStart = yStart = 0.0;
    size_t letterIndex(0);

    NJLIC::Node *node;
    bool recycleNode(false);
    float xMax, yMax;
    float lastXAdvance(0);

    SDL_LogVerbose(SDL_LOG_CATEGORY_TEST, "BitmapFont::printf: %s\n",
                   mCurrentPrintf.c_str());

    float currentX(0.0);
    float currentY(0.0);

    BitmapFontData *bmfd = mFontMap.find(mCurrentFontName)->second;
    FrameVector frames = bmfd->_jsonData.getFrames();
    float lineHeight(bmfd->_jsonData.getInfo().getLineHeight());
    float base(bmfd->_jsonData.getInfo().getBase());
    float scale(1.0);

    bmfd->_jsonData.getKernings();

    using NodeVector = std::vector<NJLIC::Node *>;
    NodeVector nodeVector;

    for (int i = 0; i < mCurrentPrintf.size(); i++) {
        int ascii = (int)mCurrentPrintf.at(i);
        int previousAscii = 0;
        if (i - 1 >= 0) {
            previousAscii = (int)mCurrentPrintf.at(i - 1);
        }

        float kerning = 0.0;
        using KerningMap = std::map<std::string, int64_t>;

        char buff[1024];
        sprintf(buff, "%d,%d", previousAscii, ascii);
        const std::string &key(buff);
        auto _kerning = bmfd->_jsonData.getKernings().find(key);

        if (_kerning != bmfd->_jsonData.getKernings().end()) {
            kerning = float(_kerning->second);
        } else {
            kerning = 0.0;
        }

        if (ascii >= 32 and ascii <= 126) {
            // renderable letter.

            size_t idx(ascii - 32);

            if (idx < frames.size()) {
                LetterFrameInfo charData =
                    LetterFrameInfo(frames.at(ascii - 32));

                scale = charData.scale;

                NJLIC::Node *node = renderLetter(ascii, charData);
                //                float _y(((lineHeight * scale) -
                //                (charData.yoffset * scale)) -
                //                         ((lineHeight * scale) - (base *
                //                         scale)) - currentY);

                node->setOrigin(
                    glm::vec2(currentX + ((charData.xoffset + kerning) * scale),
                              currentY));

                mainNode->addChildNode(node);
                scene->addActiveNode(node);

                nodeVector.push_back(node);

                currentX += (charData.xadvance * scale);
            }

        } else {
            if (ascii == 10) {
                // line feed

                currentX = 0;
                currentY -= (base * scale);

            } else if (ascii == 9) {
                // tab
            }
        }
    }

    float xoffset(currentX / 2.0);

    for (NodeVector::iterator i = nodeVector.begin(); i != nodeVector.end();
         i++) {
        NJLIC::Node *node = *i;
        glm::vec3 o(node->getOrigin());
        o.x -= xoffset;
        node->setOrigin(o);
    }

    return mainNode;
}

NJLIC::Node *BitmapFont::renderLetter(int ascii,
                                      const LetterFrameInfo &charData) {
    NJLIC::Node *node = nullptr;

    if (ascii >= 32 and ascii <= 126) {

        node = new NJLIC::Node();

        char buff[1024];
        sprintf(buff, "%c", (char)ascii);

        node->setName(buff);
        node->addGeometry(mGeometry);
        mGeometry->setSpriteAtlasFrame(node, charData.x, charData.y,
                                       charData.width, charData.height);
        mGeometry->setDimensions(node,
                                 glm::vec2(charData.width, charData.height),
                                 glm::vec2(0.0, 1.0));
    }
    return node;
}
