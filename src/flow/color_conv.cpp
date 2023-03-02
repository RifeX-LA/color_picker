#include <cmath>
#include <flow/color_conv.hpp>

namespace flow::color {

    xyz to_xyz_converter::operator()(const xyz& xyz) const {
        return xyz;
    }

    xyz to_xyz_converter::operator()(const rgb& rgb) const {
        static const auto conv_fn = [](double x) {
            return x >= 0.04045 ? std::pow((x + 0.055) / 1.055, 2.4) : x / 12.92;
        };

        const auto [r, g, b] = rgb.components();

        const double rn = conv_fn(static_cast<double>(r) / 255) * 100;
        const double gn = conv_fn(static_cast<double>(g) / 255) * 100;
        const double bn = conv_fn(static_cast<double>(b) / 255) * 100;

        const int x = 0.4124564 * rn + 0.3575761 * gn + 0.1804375 * bn;
        const int y = 0.2126729 * rn + 0.7151522 * gn + 0.072175 * bn;
        const int z = 0.0193339 * rn + 0.119192 * gn + 0.9503041 * bn;

        return {x, y, z};
    }

    xyz to_xyz_converter::operator()(const lab& lab) const {
        static const auto conv_fn = [](double x) {
            auto cbr = x * x * x;
            return cbr >= 0.008856 ? cbr : (x - 16.0 / 116.0) / 7.787;
        };

        const auto [l, a, b] = lab.components();

        const int x = conv_fn(a / 500.0 + (l + 16) / 116.0) * detail::yw;
        const int y = conv_fn((l + 16) / 116.0) * detail::xw;
        const int z = conv_fn((l + 16) / 116.0 - b / 200.0) * detail::zw;

        return {x, y, z};
    }

    lab to_lab_converter::operator()(const lab& lab) const {
        return lab;
    }

    lab to_lab_converter::operator()(const xyz& xyz) const {
        static const auto conv_fn = [](double x) {
            return x >= 0.008856 ? std::cbrt(x) : 7.787 * x + 16.0 / 116.0;
        };

        const auto [x, y, z] = xyz.components();

        const int l = 116 * conv_fn(y / detail::yw) - 16;
        const int a = 500 * (conv_fn(x / detail::xw) - conv_fn(y / detail::yw));
        const int b = 200 * (conv_fn(y / detail::yw) - conv_fn(z / detail::zw));

        return {l, a, b};
    }

    lab to_lab_converter::operator()(const rgb& rgb) const {
        return (*this)(to_xyz_converter{}(rgb));
    }


    rgb to_rgb_converter::operator()(const rgb& rgb) const {
        return rgb;
    }

    rgb to_rgb_converter::operator()(const xyz& xyz) const {
        static const auto conv_fn = [](double x) {
            return x >= 0.0031308 ? 1.055 * std::pow(x, 1.0 / 2.4) - 0.055 : 12.92 * x;
        };

        static const auto component_conv = [](double x) {
            return static_cast<int>(std::round(conv_fn(x) * 255));
        };

        const auto [x, y, z] = xyz.components();

        const double rn = (3.2404542 * x - 1.5371385 * y - 0.4985314 * z) / 100;
        const double gn = (-0.969266 * x + 1.8760108 * y + 0.0415560 * z) / 100;
        const double bn = (0.0556434 * x - 0.2040259 * y + 1.0572252 * z) / 100;

        return {component_conv(rn), component_conv(gn), component_conv(bn)};
    }

    rgb to_rgb_converter::operator()(const lab& lab) const {
        return (*this)(to_xyz_converter{}(lab));
    }

} // namespace flow::color
