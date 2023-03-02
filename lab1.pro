QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/flow/color_conv.cpp \
    src/flow/color_picker.cpp \
    src/flow/color_represent.cpp

HEADERS += \
    include/flow/color.hpp \
    include/flow/color_conv.hpp \
    include/flow/color_picker.hpp \
    include/flow/color_represent.hpp

include(color_widgets.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
