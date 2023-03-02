#include <algorithm>
#include <flow/color_represent.hpp>

namespace flow::qwidgets {

    detail::color_represent::color_represent(QWidget* parent) : QWidget(parent) {
        QVBoxLayout* main_layout = new QVBoxLayout(this);
        std::array<QHBoxLayout*, m_COMPONENTS_COUNT> component_layouts;

        std::ranges::generate(component_layouts, [](){return new QHBoxLayout();});
        std::ranges::generate(labels, [](){return new QLabel();});
        std::ranges::generate(spin_boxes, [](){return new QSpinBox();});

        for (std::size_t i = 0; i < component_layouts.size(); ++i) {
            component_layouts[i]->addWidget(labels[i]);
            component_layouts[i]->addWidget(spin_boxes[i]);
        }

        std::ranges::for_each(labels, [](auto&& label){label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);});
        std::ranges::for_each(component_layouts, [&](auto&& layout){main_layout->addLayout(layout);});
    }

} // namespace flow::qwidgets
