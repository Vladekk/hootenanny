
MAKEFILE = Makefile.qmake

TEMPLATE = subdirs

CONFIG += ordered

include(Configure.pri)

SUBDIRS += \
    tbs \
    tgs \
    hoot-core \
    hoot-core-test

nodejs {
SUBDIRS += \
    hoot-js
}

# hoot-swig is not currently in active use
#swig {
#SUBDIRS += \
#    hoot-swig
#}

rnd {
SUBDIRS += hoot-rnd
}

josm {
SUBDIRS += hoot-josm
}

SUBDIRS += \
    hoot-cmd \

cppunit {
SUBDIRS += \
    hoot-test \
}
