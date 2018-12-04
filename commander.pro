
QT 		+= widgets core gui
LIBS 		+= -lserial
QMAKE_CXXFLAGS 	+= -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

TARGET = Commander
TEMPLATE += app

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	arduino.cpp \
	FishingHole/fishinghole.cpp \ 
	FishingHole/expFishThread.cpp \ 
	FishingHole/fishLogger.cpp \
	CircleMaze/circleMaze.cpp \
	CircleMaze/expCircleThread.cpp \
	CircleMaze/circleLogger.cpp

HEADERS += \
	mainwindow.h \ 
	arduino.h \
	FishingHole/fishinghole.h \ 
	FishingHole/expFishThread.h \
	FishingHole/fishLogger.h \
	CircleMaze/circleMaze.h \
	CircleMaze/expCircleThread.h \
	CircleMaze/circleLogger.h
