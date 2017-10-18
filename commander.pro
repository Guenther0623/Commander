QT 			+= core gui
LIBS			+= -L/usr/local/lib/ -lserial

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = commander
TEMPLATE = app

SOURCES		+= main.cpp \ 
			window.cpp \
			expThread.cpp \
			arduino.cpp \
			logger.cpp

HEADERS 	+= window.h \ 
			defines.h \ 
			expThread.h \
			arduino.h \
			logger.h
