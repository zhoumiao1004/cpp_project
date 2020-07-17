TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    wall.cpp \
    snake.cpp \
    food.cpp

HEADERS += \
    wall.h \
    snake.h \
    food.h
