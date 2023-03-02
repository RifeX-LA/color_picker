#pragma once

#include <flow/color.hpp>

namespace flow::color {

    namespace detail {

        inline constexpr double xw = 95.047;
        inline constexpr double yw = 100;
        inline constexpr double zw = 108.883;

        template <typename Color>
        inline constexpr bool is_color_v = std::is_base_of_v<three_component_color, Color> || std::is_same_v<QColor, Color>;

    } // namespace detail

    struct to_xyz_converter {
        xyz operator()(const xyz& xyz) const;
        xyz operator()(const rgb& rgb) const;
        xyz operator()(const lab& lab) const;
    };

    struct to_lab_converter {
        lab operator()(const lab& lab) const;
        lab operator()(const xyz& xyz) const;
        lab operator()(const rgb& rgb) const;
    };

    struct to_rgb_converter {
        rgb operator()(const rgb& rgb) const;
        rgb operator()(const xyz& xyz) const;
        rgb operator()(const lab& lab) const;
    };

    template <typename ToColor>
    class converter {
        static_assert(detail::is_color_v<ToColor>, "Type must be a color");

        constexpr auto m_get_converter() const noexcept {
            if constexpr (std::is_same_v<ToColor, rgb>) {
                return to_rgb_converter{};
            }
            else if constexpr (std::is_same_v<ToColor, xyz>) {
                return to_xyz_converter{};
            }
            else {
                return to_lab_converter{};
            }
        }

    public:
        template <typename FromColor>
        ToColor operator()(const FromColor& color) const {
            static_assert(detail::is_color_v<FromColor>, "Type must be a color");

            return m_get_converter()(color);
        }
    };

} // namespace flow::color
