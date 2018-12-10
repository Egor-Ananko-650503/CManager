QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

#Boost
INCLUDEPATH += "C:/Program Files/boost/boost_1_67_0/boost_mingw_53_32/include/boost-1_67"
LIBS += -"LC:/Program Files/boost/boost_1_67_0/boost_mingw_53_32/lib" \
#        -llibboost_system-mgw53-mt-x32-1_67 \
        -llibboost_system-mgw53-mt-d-x32-1_67 \
#        -llibboost_filesystem-mgw53-mt-x32-1_67 \
        -llibboost_filesystem-mgw53-mt-d-x32-1_67 \
#        -llibboost_date_time-mgw53-mt-x32-1_67 \
        -llibboost_date_time-mgw53-mt-d-x32-1_67

INCLUDEPATH += d:\Projects\Qt\3TPO\AnManager
LIBS += d:\Projects\Qt\3TPO\AnManager\pathcch.lib

TEMPLATE = app

HEADERS += ../../filemodel.h \
            ../../junctionpoint.h


SOURCES +=  tst_logic_tests.cpp \
            ../../filemodel.cpp \
            ../../junctionpoint.cpp
