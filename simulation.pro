#-------------------------------------------------
#
# Project created by QtCreator 2016-06-03T09:18:36
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG += c++14

TARGET = simulation
TEMPLATE = app

msvc:QMAKE_CXXFLAGS_RELEASE += /openmp
gcc:QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -fopenmp -D_GLIBCXX_PARALLEL
gcc:QMAKE_LFLAGS_RELEASE += -fopenmp

SOURCES += main.cpp\
    simulation.cpp \
    rps_ca/sim_rps_ca.cpp \
    mainwindow.cpp \
    imagewidget.cpp \
    NM/sim_nm.cpp \
    pd3s/sim_pd3s.cpp \
    LV6s2a/sim_lv6s2a.cpp \
    tox2x/sim_tox2x.cpp \
    simulationmanager.cpp \
    simulationsession.cpp \
    randgen.cpp \
    simparameterwidget.cpp \
    socdil_mix_imit/sim_socdil_mix_imit.cpp \
    socdil_mix_logit/sim_socdil_mix_logit.cpp \
    socdil_lev_imit/sim_socdil_lev_imit.cpp \
    socdil_lev_logit/sim_socdil_lev_logit.cpp \
    LV4s1a/sim_lv4s1a.cpp \
    coordtest/sim_coordtest.cpp

HEADERS  += \
    randgen.h \
    simulation.h \
    rps_ca/sim_rps_ca.h \
    mainwindow.h \
    imagewidget.h \
    NM/sim_nm.h \
    pd3s/sim_pd3s.h \
    LV6s2a/sim_lv6s2a.h \
    tox2x/sim_tox2x.h \
    simulationmanager.h \
    simulationsession.h \
    simparameterwidget.h \
    overloadselector.h \
    socdil_mix_imit/sim_socdil_mix_imit.h \
    socdil_mix_logit/sim_socdil_mix_logit.h \
    socdil_lev_imit/sim_socdil_lev_imit.h \
    socdil_lev_logit/sim_socdil_lev_logit.h \
    LV4s1a/sim_lv4s1a.h \
    coordtest/sim_coordtest.h

DISTFILES += \
    ../The Promise.docx
