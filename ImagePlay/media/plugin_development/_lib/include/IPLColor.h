//#############################################################################
//  File:      IPLColor.h
//  Author:    Kaspar Schmid
//  Date:      November 2014
//#############################################################################

#ifndef IPLCOLOR_H
#define IPLCOLOR_H

#include "IPL_global.h"

//-----------------------------------------------------------------------------
//! IPLColor container and helper class
/*!
 * Saves a RGB color and converts to and from HSL, HSV
 */
class IPLColor {
private:
    ipl_basetype r;
    ipl_basetype g;
    ipl_basetype b;

public:
    IPLColor() {
        this->r = 0.0f;
        this->g = 0.0f;
        this->b = 0.0f;
    }

    IPLColor(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    IPLColor(const IPLColor* other) {
        this->r = other->r;
        this->g = other->g;
        this->b = other->b;
    }

    IPLColor& operator=(const IPLColor& other) {
         r = other.r;
         g = other.g;
         b = other.b;
         return *this;
     }

    static IPLColor fromRGB(float r, float g, float b)
    {
        return IPLColor(r, g, b);
    }

    static IPLColor fromHSL(float h, float s, float l)
    {
        float rgb[3];
        hslToRgb(h, s, l, rgb);

        IPLColor color(rgb[0], rgb[1], rgb[2]);
        return color;
    }


    static IPLColor fromHSV(float h, float s, float v)
    {
        float rgb[3];
        hsvToRgb(h, s, v, rgb);

        IPLColor color(rgb[0], rgb[1], rgb[2]);
        return color;
    }

    float red() const
    {
        return r;
    }

    float green() const
    {
        return g;
    }

    float blue() const
    {
        return b;
    }

    float hue() const
    {
        float hsv[3];
        rgbToHsv(r, g, b, hsv);
        return hsv[0];
    }

    float saturation() const
    {
        float hsv[3];
        rgbToHsv(r, g, b, hsv);
        return hsv[1];
    }

    float value() const
    {

        float hsv[3];
        rgbToHsv(r, g, b, hsv);
        return hsv[2];
    }

    float lightness() const
    {
        float hsl[3];
        rgbToHsl(r, g, b, hsl);
        return hsl[2];
    }


    // https://github.com/ratkins/RGBConverter/blob/master/RGBConverter.cpp
    static void rgbToHsl(float r, float g, float b, float hsl[])
    {
        float max = threeway_max(r, g, b);
        float min = threeway_min(r, g, b);
        float h = (max + min) / 2;
        float s = h;
        float l = h;

        if (max == min) {
            h = s = 0; // achromatic
        } else {
            float d = max - min;
            s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
            if (max == r) {
                h = (g - b) / d + (g < b ? 6 : 0);
            } else if (max == g) {
                h = (b - r) / d + 2;
            } else if (max == b) {
                h = (r - g) / d + 4;
            }
            h /= 6;
        }
        hsl[0] = h;
        hsl[1] = s;
        hsl[2] = l;
    }

    static void hslToRgb(float h, float s, float l, float rgb[])
    {
        float r, g, b;

        if (s == 0) {
            r = g = b = l; // achromatic
        } else {
            float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            float p = 2 * l - q;
            r = hue2rgb(p, q, h + 1.0f/3.0f);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1.0f/3.0f);
        }

        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
    }

    static void rgbToHsv(float r, float g, float b, float hsv[])
    {
        float max = threeway_max(r, g, b), min = threeway_min(r, g, b);
        float h = max;
        float s = max;
        float v = max;

        float d = max - min;
        s = max == 0 ? 0 : d / max;

        if (max == min) {
            h = 0; // achromatic
        } else {
            if (max == r) {
                h = (g - b) / d + (g < b ? 6 : 0);
            } else if (max == g) {
                h = (b - r) / d + 2;
            } else if (max == b) {
                h = (r - g) / d + 4;
            }
            h /= 6;
        }

        hsv[0] = h;
        hsv[1] = s;
        hsv[2] = v;
    }

    static void hsvToRgb(float h, float s, float v, float rgb[]) {
        float r, g, b;

        int i = int(h * 6);
        float f = h * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        switch(i % 6){
            case 0: r = v, g = t, b = p; break;
            case 1: r = q, g = v, b = p; break;
            case 2: r = p, g = v, b = t; break;
            case 3: r = p, g = q, b = v; break;
            case 4: r = t, g = p, b = v; break;
            case 5: r = v, g = p, b = q; break;
        }

        rgb[0] = r;
        rgb[1] = g;
        rgb[2] = b;
    }

    static float threeway_max(float a, float b, float c)
    {
        return std::max(a, std::max(b, c));
    }

    static float threeway_min(float a, float b, float c)
    {
        return std::min(a, std::min(b, c));
    }

    static float hue2rgb(float p, float q, float t)
    {
        if(t < 0) t += 1;
        if(t > 1) t -= 1;
        if(t < 1.0f/6.0f) return p + (q - p) * 6.0f * t;
        if(t < 1.0f/2.0f) return q;
        if(t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6.0f;
        return p;
    }
};

#endif // IPLCOLOR_H
