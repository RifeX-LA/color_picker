#include <flow/color_picker.hpp>

namespace flow::qwidgets {

    color_picker::color_picker(QDialog* parent) : QDialog(parent) {
        QHBoxLayout* main_layout = new QHBoxLayout(this);
        QVBoxLayout* color_wheel_layout = new QVBoxLayout();
        QVBoxLayout* color_represents_layout = new QVBoxLayout();

        m_color_wheel->setWheelWidth(25);
        m_color_preview->setFixedHeight(25);

        color_wheel_layout->addWidget(m_color_wheel);
        color_wheel_layout->addWidget(m_color_preview);

        m_set_common_range(m_rgb_represent.get()->spin_boxes, 0, 255);
        m_set_common_range(m_xyz_represent.get()->spin_boxes, 0, 100);
        m_set_common_range(m_lab_represent.get()->spin_boxes, -128, 128);
        m_lab_represent.get()->spin_boxes[0]->setRange(0, 100);

        color_represents_layout->addWidget(m_rgb_represent.get());
        color_represents_layout->addWidget(m_xyz_represent.get());
        color_represents_layout->addWidget(m_lab_represent.get());

        main_layout->addLayout(color_wheel_layout);
        main_layout->addLayout(color_represents_layout);

        m_set_common_color_with_color_wheel(m_DEFAULT_COLOR);
        m_connect_color_handlers();

        setMinimumWidth(500);
    }

    void color_picker::color_wheel_changed(const QColor& qcolor) {
        auto set_color = [this](auto&& color){m_set_common_color(color);};
        m_invoke_without_color_handlers(set_color, color::rgb(qcolor));
    }

    void color_picker::rgb_spin_box_changed(int) {
        auto value_changed = [this](auto&& color_represent){m_spin_box_changed(color_represent);};
        m_invoke_without_color_handlers(value_changed, m_rgb_represent);
    }

    void color_picker::xyz_spin_box_changed(int) {
        auto value_changed = [this](auto&& color_represent){m_spin_box_changed(color_represent);};
        m_invoke_without_color_handlers(value_changed, m_xyz_represent);
    }

    void color_picker::lab_spin_box_changed(int) {
        auto value_changed = [this](auto&& color_represent){m_spin_box_changed(color_represent);};
        m_invoke_without_color_handlers(value_changed, m_lab_represent);
    }

} // namespace flow::qwidgets
