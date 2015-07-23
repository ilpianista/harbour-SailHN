TARGET = harbour-sailhn

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/newsmodel.cpp \
    src/hackernewsapi.cpp \
    src/item.cpp

HEADERS += \
    src/newsmodel.h \
    src/hackernewsapi.h \
    src/item.h \
    src/item_p.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/SailHN.qml \
    qml/pages/CommentsPage.qml \
    qml/pages/ItemDelegate.qml \
    qml/pages/AskStoriesPage.qml \
    qml/pages/JobStoriesPage.qml \
    qml/pages/NewStoriesPage.qml \
    qml/pages/RepliesPage.qml \
    qml/pages/ReplyDelegate.qml \
    qml/pages/ShowStoriesPage.qml \
    qml/pages/TopStoriesPage.qml \
    harbour-sailhn.desktop \
    rpm/harbour-sailhn.changes \
    rpm/harbour-sailhn.spec \
    rpm/harbour-sailhn.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-sailhn-it.ts \
    translations/harbour-sailhn-tr.ts
