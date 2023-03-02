#pragma once

#include <algorithm>

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <flow/color_conv.hpp>

namespace flow::qwidgets {

    namespace detail {

        struct color_represent : QWidget {
            Q_OBJECT

            static constexpr std::size_t m_COMPONENTS_COUNT = 3;

        public:
            std::array<QLabel*, m_COMPONENTS_COUNT> labels;
            std::array<QSpinBox*, m_COMPONENTS_COUNT> spin_boxes;

            explicit color_represent(QWidget* parent = nullptr);
        };

    } // namespace detail

    template <typename ColorT>
    class color_represent {
        detail::color_represent* m_color_represent_impl;
        color::converter<ColorT> m_converter;

    public:
        explicit color_represent(QWidget* parent = nullptr)
            : m_color_represent_impl(new detail::color_represent(parent)) {}

        explicit color_represent(const QString& l1, const QString& l2, const QString& l3, QWidget* parent = nullptr)
            : m_color_represent_impl(new detail::color_represent(parent)) {

            get()->labels[0]->setText(l1);
            get()->labels[1]->setText(l2);
            get()->labels[2]->setText(l3);
        }

        detail::color_represent* get() const {
            return m_color_represent_impl;
        }

        ColorT getColor() const {
            auto [sb1, sb2, sb3] = get()->spin_boxes;
            return {sb1->value(), sb2->value(), sb3->value()};
        }

        template <typename FromColor>
        void setColor(const FromColor& color_from) const {
            static_assert(color::detail::is_color_v<FromColor>);

            ColorT color = m_converter(color_from);
            auto [sb1, sb2, sb3] = get()->spin_boxes;
            auto [first, second, third] = color.components();

            sb1->setValue(first);
            sb2->setValue(second);
            sb3->setValue(third);
        }
    };

} // namespace flow::qwidgets
