# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

TARGET = Babel

QT += core gui network

QT += widgets

QMAKE_CXXFLAGS = -std=c++11

win32:LIBS += -lws2_32 -lIphlpapi
win32:LIBS += -L$$PWD/client_babel/lib/WINDOWS -lportaudio_x86 -lopus -lsilk_common -lsilk_fixed -lsilk_float

unix:LIBS += -L$$PWD/client_babel/lib/LINUX -lopus -lportaudio

LIBS += -L$$PWD/client_babel/lib/OSX -lopus -lportaudio

HEADERS = \
   client_babel/incs/Audio/AudioEncapsuler.hh \
   client_babel/incs/Audio/audiosettings.h \
   client_babel/incs/Audio/EncodeEncapsuler.hh \
   client_babel/incs/Audio/IAudioObserver.hh \
   client_babel/incs/Audio/opus.h \
   client_babel/incs/Audio/opus_custom.h \
   client_babel/incs/Audio/opus_defines.h \
   client_babel/incs/Audio/opus_multistream.h \
   client_babel/incs/Audio/opus_types.h \
   client_babel/incs/Audio/portaudio.h \
   client_babel/incs/Errors/NetworkError.hh \
   client_babel/incs/GUI/QTGUI.hh \
   client_babel/incs/GUI/AGUI.hh \
   client_babel/incs/GUI/Pages/form.h \
   client_babel/incs/GUI/Pages/login.h \
   client_babel/incs/GUI/TermGUI.hh \
   client_babel/incs/GUI/Elements/customitem.h \
   client_babel/incs/GUI/Elements/callitem.h \
   client_babel/incs/GUI/Elements/dialogbasic.h \
   client_babel/incs/GUI/Elements/dialog.h \
   client_babel/incs/GUI/Elements/dialogrequest.h \
   client_babel/incs/Networking/ClientSocket.hh \
   client_babel/incs/Networking/ClientUDP.hh \
   client_babel/incs/Networking/RequestManager.hh \
   client_babel/incs/Networking/Select.hh \
   client_babel/incs/Networking/SocketList.hh \
   client_babel/incs/Threading/AThreadController.hpp \
   client_babel/incs/Threading/TaskThread.hh \
   client_babel/incs/Threading/ThreadPool.hh \
   client_babel/incs/Client.hh \
   client_babel/incs/IGUI.hh \
   client_babel/incs/stdafx.h \
   client_babel/incs/targetver.h \
   protocol/protocol.hh \
   server/Networking/incs/ISocket.hh \
   server/Networking/incs/SocketManager.hh \
   server/Networking/incs/UnixSocket.hh \
   server/Networking/incs/WinSocket.hh \
    client_babel/incs/GUI/AElement.h \
    client_babel/incs/GUI/Pages/init.h \
    client_babel/incs/GUI/Elements/messages.h \
    client_babel/incs/GUI/Elements/singlemessage.h \
    client_babel/incs/GUI/Elements/dialogcall.h \
    client_babel/incs/GUI/Elements/dialogerror.h

SOURCES = \
    client_babel/srcs/Audio/AudioEncapsuler.cpp \
    client_babel/srcs/Audio/EncodeEncapsuler.cpp \
    client_babel/srcs/Errors/NetworkError.cpp \
    client_babel/srcs/GUI/AGUI.cpp \
    client_babel/srcs/GUI/QTGUI.cpp \
    client_babel/srcs/GUI/Pages/form.cpp \
    client_babel/srcs/GUI/Pages/login.cpp \
    client_babel/srcs/GUI/main.cpp \
    client_babel/srcs/GUI/Elements/customitem.cpp \
    client_babel/srcs/GUI/Elements/callitem.cpp \
    client_babel/srcs/GUI/Elements/dialogbasic.cpp \
    client_babel/srcs/GUI/Elements/dialog.cpp \
    client_babel/srcs/GUI/Elements/dialogrequest.cpp \
    client_babel/srcs/Networking/ClientSocket.cpp \
    client_babel/srcs/Networking/ClientUDP.cpp \
    client_babel/srcs/Networking/Select.cpp \
    client_babel/srcs/Threading/TaskThread.cpp \
    client_babel/srcs/Threading/ThreadPool.cpp \
    client_babel/srcs/Client.cpp \
    client_babel/srcs/stdafx.cpp \
    server/Networking/srcs/SocketManager.cpp \
    server/Networking/srcs/UnixSocket.cpp \
    server/Networking/srcs/WinSocket.cpp \
    server/Networking/srcs/ISocket.cpp \
    client_babel/srcs/GUI/AElement.cpp \
    client_babel/srcs/GUI/Pages/init.cpp \
    client_babel/srcs/GUI/Elements/messages.cpp \
    client_babel/srcs/GUI/Elements/singlemessage.cpp \
    client_babel/srcs/GUI/Elements/dialogcall.cpp \
    client_babel/srcs/GUI/Elements/dialogerror.cpp

INCLUDEPATH = \
    client_babel/incs \
    client_babel/incs/Audio \
    client_babel/incs/GUI \
    client_babel/incs/Errors \
    client_babel/incs/Networking \
    client_babel/incs/Threading \
    client_babel/incs/GUI/Pages \
    client_babel/incs/GUI/Elements \
    server/Networking \
    server/Networking/incs \
    ./protocol/

RESOURCES += \
    client_babel/resources/resources.qrc

FORMS += \
    client_babel/views/login.ui \
    client_babel/views/form.ui \
    client_babel/views/customitem.ui \
    client_babel/views/callitem.ui \
    client_babel/views/dialogbasic.ui \
    client_babel/views/dialog.ui \
    client_babel/views/dialogrequest.ui \
    client_babel/views/init.ui \
    client_babel/views/messages.ui \
    client_babel/views/singlemessage.ui \
    client_babel/views/dialogcall.ui \
    client_babel/views/dialogerror.ui

DISTFILES +=
