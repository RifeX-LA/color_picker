#include <QApplication>
#include <flow/color_picker.hpp>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    flow::qwidgets::color_picker color_picker;
    color_picker.show();

    return QApplication::exec();
}
