TARGET = harbour-hackernews

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/newsmodel.cpp \
    src/hackernewsapi.cpp

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/HackerNews.qml \
    qml/pages/DetailsPage.qml \
    qml/pages/ItemDelegate.qml \
    qml/pages/NewStoriesPage.qml \
    qml/pages/TopStoriesPage.qml \
    harbour-hackernews.desktop \
    rpm/harbour-hackernews.changes \
    rpm/harbour-hackernews.spec \
    rpm/harbour-hackernews.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/harbour-hackernews-it.ts

HEADERS += \
    src/newsmodel.h \
    src/hackernewsapi.h
