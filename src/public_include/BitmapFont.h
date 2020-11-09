//
//  BitmapFont.h
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#ifndef BitmapFont_h
#define BitmapFont_h

#include <map>
#include <string>

#include "Node.h"
#include "glm/glm.hpp"

#include "Shader.h"
#include "SpriteGeometry.h"

#include "Scene.h"

struct BitmapFontData;

struct LetterFrameInfo {
    float height;
    float id;
    float sourceHeight;
    float sourceWidth;
    float sourceX;
    float sourceY;
    float width;
    float x;
    float xadvance;
    float xoffset;
    float y;
    float yoffset;
    float scale;

    LetterFrameInfo(std::map<std::string, int64_t> map)
        : height(map["height"]), id(map["id"]),
          sourceHeight(map["sourceHeight"]), sourceWidth(map["sourceWidth"]),
          sourceX(map["sourceX"]), sourceY(map["sourceY"]), width(map["width"]),
          x(map["x"]), xadvance(map["xadvance"]), xoffset(map["xoffset"]),
          y(map["y"]), yoffset(map["yoffset"]), scale(.000237) {}
};

class BitmapFont {

    using Map = std::map<std::string, BitmapFontData *>;
    using Pair = std::pair<std::string, BitmapFontData *>;

    Map mFontMap;

    static BitmapFont *sBitmapFont;
    BitmapFont();
    ~BitmapFont();

    glm::vec2 mCurrentBounds;
    std::string mCurrentFontName;
    std::string mCurrentPrintf;

    NJLIC::SpriteGeometry *mGeometry;
    NJLIC::Shader *mShader;

    bool load(const std::string &fontName);
    bool unLoad(const std::string &fontName);

    //    NJLIC::Node *mMainNode;
    int mSpacesInTab = 2;
    int mMaxWidth = 1024;
    enum { ALIGN_LEFT, ALIGN_MIDDLE, ALIGN_RIGHT, NUM_ALIGNS };

  public:
    static BitmapFont *getInstance() {
        if (nullptr == sBitmapFont)
            sBitmapFont = new BitmapFont();
        return sBitmapFont;
    }

    static void destroyInstance() {
        if (nullptr != sBitmapFont)
            delete sBitmapFont;
        sBitmapFont = nullptr;
    }

    const glm::vec2 &getCurrentBounds() const;
    const std::string &getCurrentFontName() const;

    void setCurrentBounds(const glm::vec2 &bounds);
    void setCurrentFontName(const std::string &fontName);

    NJLIC::Node *printf(NJLIC::Scene *scene, const char *fmt, ...);

  protected:
    using Frame = std::map<std::string, int64_t>;
    using FrameVector = std::vector<Frame>;

  public:
    NJLIC::Node *renderLetter(int ascii, const LetterFrameInfo &charData);
};

#endif /* BitmapFont_hpp */
