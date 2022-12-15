#ifndef UTIL_COLOR_HPP
#define UTIL_COLOR_HPP

#include <iostream>
#include <sstream>

namespace util
{
    bool is_hex_str(const char t_hex[6])
    { for (int i = 0; i < 6; i++) if (!isxdigit(t_hex[i])) return false; return true; }

    enum class color_system
    {
        rgb,
        rgba,
        hex,
        hsv,

        hsl,
        cmyk,
        pantone,
        ral,
    };
    
    template<color_system T = color_system::rgba>
    struct color
    {
        color() { throw std::invalid_argument("this color system has not been implamented yet"); }
    };

    template<>
    struct color<color_system::rgba>
    {
        color() {}
        color(const double& t_r, const double& t_g, const double& t_b, const double& t_a ) { r = t_r; g = t_g; b = t_b; a = t_a; }

        color(const color<color_system::rgba> &t_color) { r = t_color.r; g = t_color.g; b = t_color.b; a = t_color.a; }
        color(color<color_system::rgba> &&t_color) { r = std::move(t_color.r); g = std::move(t_color.g); b = std::move(t_color.b); a = std::move(t_color.a); }

        color(const color<color_system::rgb> &t_color, const double& t_a = 1);
        color(color<color_system::rgb> &&t_color, const double t_a = 1);
        color(const color<color_system::hex> &t_hex);
        color(const color<color_system::hsv> &t_hsv);

        color<color_system::rgb> rgb() const;
        color<color_system::hex> hex() const;
        // color<color_system::hsv> hsv() const;

        double operator [](const int& t_index) const { switch(t_index) { case 0: return r; case 1: return g; case 2: return b; case 3: return a; default: throw std::out_of_range("the index is out of range"); } }
        double& operator [](const int& t_index) { switch(t_index) { case 0: return r; case 1: return g; case 2: return b; case 3: return a; default: throw std::out_of_range("the index is out of range"); } }

        friend std::ostream& operator<<(std::ostream& t_out, const color<color_system::rgba>& t_color)
        { t_out << "{r: " << t_color.r << ", g: " << t_color.g << ", b: " << t_color.b << ", a: " << t_color.a << "}"; return t_out; }

        double r;
        double g;
        double b;
        double a;
    };

    template<>
    struct color<color_system::rgb>
    {
        color() {}
        color(const double& t_r, const double& t_g, const double& t_b) { r = t_r; g = t_g; b = t_b; }

        color(const color<color_system::rgb> &t_color) { r = t_color.r; g = t_color.g; b = t_color.b; }
        color(color<color_system::rgb> &&t_color) { r = std::move(t_color.r); g = std::move(t_color.g); b = std::move(t_color.b); }

        color(const color<color_system::rgba> &t_color);
        color(color<color_system::rgba> &&t_color);
        color(const color<color_system::hex> &t_hex);
        color(const color<color_system::hsv> &t_hsv);

        color<color_system::rgba> rgba() const;
        color<color_system::hex> hex() const;
        // color<color_system::hsv> hsv() const;

        double operator [](const int& t_index) const { switch(t_index) { case 0: return r; case 1: return g; case 2: return b; default: throw std::out_of_range("the index is out of range"); } }
        double& operator [](const int& t_index) { switch(t_index) { case 0: return r; case 1: return g; case 2: return b; default: throw std::out_of_range("the index is out of range"); } }

        friend std::ostream& operator<<(std::ostream& t_out, const color<color_system::rgb>& t_color)
        { t_out << "{r: " << t_color.r << ", g: " << t_color.g << ", b: " << t_color.b << "}"; return t_out; }

        double r;
        double g;
        double b;
    };


    template<>
    struct color<color_system::hex>
    {
        color() {}
        color(const char *t_hex) { if (strlen(t_hex) != 6) throw std::invalid_argument("hex string must be 6 digits"); if (!is_hex_str(t_hex)) throw std::invalid_argument("must be a hexideciamal number"); memcpy(hex, t_hex, sizeof(hex)); }
        color(const std::string& t_hex) : color(t_hex.c_str()) {}

        color(const color<color_system::hex>& t_color) { memcpy(hex, t_color.hex, sizeof(hex)); }
        color(color<color_system::hex>&& t_color) { std::swap(hex, t_color.hex); }

        color(const color<color_system::rgb>& t_color);
        color(const color<color_system::rgba>& t_color);
        color(const color<color_system::hsv>& t_color);

        color<color_system::rgb> rgb() const;
        color<color_system::rgba> rgba() const;
        // color<color_system::hsv> hsv() const;

        friend std::ostream& operator<<(std::ostream& t_out, const color<color_system::hex>& t_color)
        { t_out << "#" << std::string(t_color.hex); return t_out; }

        char hex[6];
    };

    template<>
    struct color<color_system::hsv>
    {
        color() {}
        color(const double& t_h, const double& t_s, const double& t_v) { h = t_h; s = t_s; v = t_v; }

        color(const color<color_system::hsv>& t_color) { h = t_color.h; s = t_color.s; v = t_color.v; }
        color(color<color_system::hsv>&& t_color) { h = std::move(t_color.h); s = std::move(t_color.s); v = std::move(t_color.v); }

        // color(const color<color_system::rgb>& t_color);
        // color(const color<color_system::rgba>& t_color);
        // color(const color<color_system::hex>& t_color);

        friend std::ostream& operator<<(std::ostream& t_out, const color<color_system::hsv>& t_color)
        { t_out << "{h: " << t_color.h << ", s: " << t_color.s << ", v: " << t_color.v << "}"; return t_out; }

        // color<color_system::rgb> rgb() const;
        // color<color_system::hex> hex() const;
        // color<color_system::rgba> rgba() const;

        double h;
        double s;
        double v;
    };

    #pragma region color<rgba>
    color<color_system::rgba>::color(const color<color_system::rgb> &t_color, const double& t_a) { r = t_color.r; g = t_color.g; b = t_color.b; a = t_a; }
    color<color_system::rgba>::color(color<color_system::rgb> &&t_color, const double t_a) { r = std::move(t_color.r); g = std::move(t_color.g); b = std::move(t_color.b); a = t_a; }
    color<color_system::rgba>::color(const color<color_system::hex> &t_hex) : color(color<color_system::rgb>(t_hex)) {}
    color<color_system::rgba>::color(const color<color_system::hsv> &t_hsv) : color(color<color_system::rgb>(t_hsv)) {}
    
    color<color_system::rgb> color<color_system::rgba>::rgb() const { return color<color_system::rgb>((*this)); }
    color<color_system::hex> color<color_system::rgba>::hex() const { return color<color_system::hex>((*this)); }
    // color<color_system::hsv> color<color_system::rgba>::hsv() const { return color<color_system::hsv>((*this)); }
    #pragma endregion

    #pragma region color<rgb>
    color<color_system::rgb>::color(const color<color_system::rgba> &t_color) { r = t_color.r; g = t_color.g; b = t_color.b; }
    color<color_system::rgb>::color(color<color_system::rgba> &&t_color) { r = std::move(t_color.r); g = std::move(t_color.g); b = std::move(t_color.b); }
    color<color_system::rgb>::color(const color<color_system::hex> &t_hex)
    {
        for (int i = 0; i < 3; i++)
        {
            std::string pair;
            pair.push_back(t_hex.hex[i*2 + 0]);
            pair.push_back(t_hex.hex[i*2 + 1]);

            (*this)[i] = ((double)std::stoi(pair, nullptr, 16)) / 255.;
        }
    }
    color<color_system::rgb>::color(const color<color_system::hsv> &t_hsv)
    {
        double c = t_hsv.s*t_hsv.v;
        double hprime = fmod(t_hsv.h / 60.0, 6);
        double x = c * (1 - std::fabs(fmod(hprime, 2) - 1));
        double m = t_hsv.v - c;
        
        if(0 <= hprime && hprime < 1) {
            r = c; g = x; b = 0;
        } else if(1 <= hprime && hprime < 2) {
            r = x; g = c; b = 0;
        } else if(2 <= hprime && hprime < 3) {
            r = 0; g = c; b = x;
        } else if(3 <= hprime && hprime < 4) {
            r = 0; g = x; b = c;
        } else if(4 <= hprime && hprime < 5) {
            r = x; g = 0; b = c;
        } else if(5 <= hprime && hprime < 6) {
            r = c; g = 0; b = x;
        } else {
            r = 0; g = 0; b = 0;
        }

        r += m;
        g += m;
        b += m;
    }
    
    color<color_system::rgba> color<color_system::rgb>::rgba() const { return color<color_system::rgba>((*this)); }
    color<color_system::hex> color<color_system::rgb>::hex() const { return color<color_system::hex>((*this)); }
    // color<color_system::hsv> color<color_system::rgb>::hsv() const { return color<color_system::hsv>((*this)); }
    #pragma endregion

    #pragma region color<hex>
    color<color_system::hex>::color(const color<color_system::rgb>& t_color)
    {
        for (int i = 0; i < 3; i++)
        {
            std::ostringstream ss;
            ss << std::hex << int(t_color[i] * 255);

            hex[i*2+0] = ss.str()[0];
            hex[i*2+1] = ss.str()[1];
        }
    }
    color<color_system::hex>::color(const color<color_system::rgba>& t_color) : color(t_color.rgb()) {}
    color<color_system::hex>::color(const color<color_system::hsv>& t_color) : color(color<color_system::rgb>(t_color).hex()) {}
    
    color<color_system::rgb> color<color_system::hex>::rgb() const { return color<color_system::rgb>((*this)); }
    // color<color_system::hsv> color<color_system::hex>::hsv() const { return color<color_system::hsv>((*this)); }
    color<color_system::rgba> color<color_system::hex>::rgba() const { return color<color_system::rgba>((*this)); }
    #pragma endregion

    #pragma region color<hsv>
    // color<color_system::hsv>::color(const color<color_system::rgb>& t_color)
    // {
    //     double cmax = std::max(std::max(t_color.r, t_color.g), t_color.b);
    //     double cmin = std::min(std::min(t_color.r, t_color.g), t_color.b);
    //     double delta = cmax - cmin;
        
    //     if(delta > 0) {
    //         if(cmax == t_color.r) {
    //             h = 60 * (fmod(((t_color.g - t_color.b) / delta), 6));
    //         } else if(cmax == t_color.g) {
    //             h = 60 * (((t_color.b - t_color.r) / delta) + 2);
    //         } else if(cmax == t_color.b) {
    //             h = 60 * (((t_color.r - t_color.g) / delta) + 4);
    //         }
            
    //         if(cmax > 0) {
    //             s = delta / cmax;
    //         } else {
    //             s = 0;
    //         }
            
    //         v = cmax;
    //     } else {
    //         h = 0;
    //         s = 0;
    //         v = cmax;
    //     }
        
    //     if(h < 0) {
    //         h = 360 + h;
    //     }
    // }
    // color<color_system::hsv>::color(const color<color_system::rgba>& t_color) : color(t_color.rgb()) {}
    // color<color_system::hsv>::color(const color<color_system::hex>& t_color) : color(t_color.rgb()) {}

    // color<color_system::rgb> color<color_system::hsv>::rgb() const { return color<color_system::rgb>((*this)); }
    // color<color_system::hex> color<color_system::hsv>::hex() const { return color<color_system::hex>((*this)); }
    // color<color_system::rgba> color<color_system::hsv>::rgba() const { return color<color_system::rgba>((*this)); }

    #pragma endregion
}

#endif