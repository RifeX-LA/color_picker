#pragma once

#include <QDialog>
#include <QLabel>

#include <QtColorWidgets/color_wheel.hpp>
#include <QtColorWidgets/color_preview.hpp>

#include <flow/color_conv.hpp>
#include <flow/color_represent.hpp>

namespace flow::qwidgets {

    class color_picker : public QDialog {
        Q_OBJECT

        static constexpr auto m_DEFAULT_COLOR = color::rgb(255, 0, 0);

        color_widgets::ColorWheel* m_color_wheel = new color_widgets::ColorWheel();
        color_widgets::ColorPreview* m_color_preview = new color_widgets::ColorPreview();

        color_represent<color::rgb> m_rgb_represent{"R", "G", "B"};
        color_represent<color::xyz> m_xyz_represent{"X", "Y", "Z"};
        color_represent<color::lab> m_lab_represent{"L", "a", "b"};

        std::vector<QMetaObject::Connection> m_connections;

        template <std::size_t N>
        static void m_set_common_range(const std::array<QSpinBox*, N>& spin_boxes, int min, int max) {
            std::ranges::for_each(spin_boxes, [&](auto&& spin_box){spin_box->setRange(min, max);});
        }

        template <typename ColorT>
        void m_set_common_color(const ColorT& color) const {
            m_rgb_represent.setColor(color);
            m_xyz_represent.setColor(color);
            m_lab_represent.setColor(color);

            auto rgb = color::converter<color::rgb>{}(color);
            m_color_preview->setColor(rgb.to_qcolor());
        }

        template <typename ColorT>
        void m_set_common_color_with_color_wheel(const ColorT& color) const {
            m_set_common_color(color);
            auto rgb = color::converter<color::rgb>{}(color);
            m_color_wheel->setColor(rgb.to_qcolor());
        }

        template <typename ColorT>
        void m_spin_box_changed(const color_represent<ColorT>& color_represent) const {
            m_set_common_color_with_color_wheel(color_represent.getColor());
        }

        void m_connect_spin_box_handlers() {
            auto connect_rgb = [&, this](QSpinBox* spin_box){m_connections.push_back(QObject::connect(spin_box, SIGNAL(valueChanged(int)), this, SLOT(rgb_spin_box_changed(int))));};
            auto connect_xyz = [&, this](QSpinBox* spin_box){m_connections.push_back(QObject::connect(spin_box, SIGNAL(valueChanged(int)), this, SLOT(xyz_spin_box_changed(int))));};
            auto connect_lab = [&, this](QSpinBox* spin_box){m_connections.push_back(QObject::connect(spin_box, SIGNAL(valueChanged(int)), this, SLOT(lab_spin_box_changed(int))));};

            std::ranges::for_each(m_rgb_represent.get()->spin_boxes, connect_rgb);
            std::ranges::for_each(m_xyz_represent.get()->spin_boxes, connect_xyz);
            std::ranges::for_each(m_lab_represent.get()->spin_boxes, connect_lab);
        }

        void m_connect_color_handlers() {
            m_connections.push_back(QObject::connect(m_color_wheel, &color_widgets::ColorWheel::colorSelected, this, &color_picker::color_wheel_changed));
            m_connect_spin_box_handlers();
        }

        void m_disconnect_color_handlers()  {
            std::ranges::for_each(m_connections, [](auto&& connection){QObject::disconnect(connection);});
            m_connections.clear();
        }

        template <typename Fn, typename... Args>
        void m_invoke_without_color_handlers(Fn&& fn, Args&&... args) {
            m_disconnect_color_handlers();
            std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...);
            m_connect_color_handlers();
        }

    public:
        explicit color_picker(QDialog* parent = nullptr);

    public slots:
        void color_wheel_changed(const QColor& color);
        void rgb_spin_box_changed(int);
        void xyz_spin_box_changed(int);
        void lab_spin_box_changed(int);
    };

} // namespace flow::qwidgets
