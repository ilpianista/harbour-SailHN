TARGET = harbour-sailhn

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/newsmodel.cpp \
    src/hackernewsapi.cpp \
    src/item.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/SailHN.qml \
    qml/pages/DetailsPage.qml \
    qml/pages/ItemDelegate.qml \
    qml/pages/NewStoriesPage.qml \
    qml/pages/TopStoriesPage.qml \
    harbour-sailhn.desktop \
    rpm/harbour-sailhn.changes \
    rpm/harbour-sailhn.spec \
    rpm/harbour-sailhn.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-sailhn-it.ts

HEADERS += \
    src/newsmodel.h \
    src/hackernewsapi.h \
    src/item.h \
    src/item_p.h
