//
//  Color.hpp
//  SDLSkeleton
//
//  Created by James Folk on 3/31/20.
//

#ifndef Color_hpp
#define Color_hpp

#include "glm/glm.hpp"
#include <string>

class Color {
    glm::vec4 mRGBColor;

    /*! \brief Convert RGB to HSV color space

      Converts a given set of RGB values `r', `g', `b' into HSV
      coordinates. The input RGB values are in the range [0, 1], and the
      output HSV values are in the ranges h = [0, 360], and s, v = [0,
      1], respectively.

      \param fR Red component, used as input, range: [0, 1]
      \param fG Green component, used as input, range: [0, 1]
      \param fB Blue component, used as input, range: [0, 1]
      \param fH Hue component, used as output, range: [0, 360]
      \param fS Hue component, used as output, range: [0, 1]
      \param fV Hue component, used as output, range: [0, 1]

    */
    static void RGBtoHSV(const float fR, const float fG, const float fB,
                         float &fH, float &fS, float &fV);

    /*! \brief Convert HSV to RGB color space

      Converts a given set of HSV values `h', `s', `v' into RGB
      coordinates. The output RGB values are in the range [0, 1], and
      the input HSV values are in the ranges h = [0, 360], and s, v =
      [0, 1], respectively.

     \param fH Hue component, used as input, range: [0, 360]
     \param fS Hue component, used as input, range: [0, 1]
     \param fV Hue component, used as input, range: [0, 1]
      \param fR Red component, used as output, range: [0, 1]
      \param fG Green component, used as output, range: [0, 1]
      \param fB Blue component, used as output, range: [0, 1]

    */
    static void HSVtoRGB(const float fH, const float fS, const float fV,
                         float &fR, float &fG, float &fB);

  public:
    Color();
    Color(const Color &rhs);
    const Color &operator=(const Color &rhs);

    virtual ~Color();

    static glm::vec4 rgb2hsv(const glm::vec4 &rgb);

    static glm::vec4 hsv2rgb(const glm::vec4 &hsv);

    static Color modifyHue(const Color &color, const float h);
    static Color modifySaturation(const Color &color, const float s);
    static Color modifyValue(const Color &color, const float v);

    void setHSV(const float fH, const float fS, const float fV);
    void setHSV(const glm::vec4 &rgb);
    void setRGB(const float fR, const float fG, const float fB);
    void setRGB(const std::string &hex);
    void setRGB(const int hex);
    void setRGB(const glm::vec4 &rgb);

    glm::vec4 getHSV() const;
    glm::vec4 getRGB() const;
    
    float red()const;
    float green()const;
    float blue()const;
    
    float hue()const;
    float saturation()const;
    float value()const;
    
};

#endif /* Color_hpp */
