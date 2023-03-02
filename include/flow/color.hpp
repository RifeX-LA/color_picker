#pragma once

#include <QColor>

namespace flow::color {

    template <typename... Types>
    class color_base {
    public:
        using components_t = std::tuple<Types...>;

        explicit constexpr color_base(const Types&... t)
        noexcept((... && std::is_nothrow_copy_constructible_v<Types>)): m_components(t...) {}

        explicit constexpr color_base(const components_t& components)
        noexcept(std::is_nothrow_copy_constructible_v<components_t>) : m_components(components) {}

        [[nodiscard]] constexpr components_t components() const {
            return m_components;
        }

    private:
        components_t m_components;
    };

    struct three_component_color : color_base<int, int, int> {
        constexpr three_component_color(int first, int second, int third)
        noexcept : color_base<int, int, int>(first, second, third) {}
    };

    struct rgb final : three_component_color {
        constexpr rgb(int r, int g, int b) noexcept : three_component_color(r, g, b) {}

        rgb(const QColor& color) noexcept: three_component_color(color.red(), color.green(), color.blue()) {}

        [[nodiscard]] constexpr QColor to_qcolor() const noexcept {
            auto [r, g, b] = components();
            return QColor(r, g, b);
        }
    };

    struct xyz final : three_component_color {
        constexpr xyz(int x, int y, int z) noexcept: three_component_color(x, y, z) {}
    };

    struct lab final : three_component_color {
        constexpr lab(int l, int a, int b) noexcept : three_component_color(l, a, b) {}
    };

} // namespace flow::color
