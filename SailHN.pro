TARGET = harbour-sailhn

CONFIG += sailfishapp

SOURCES += \
    src/main.cpp \
    src/cookiejar.cpp \
    src/hackernewsapi.cpp \
    src/hnmanager.cpp \
    src/item.cpp \
    src/newsmodel.cpp \
    src/user.cpp

HEADERS += \
    src/cookiejar.h \
    src/hackernewsapi.h \
    src/hnmanager.h \
    src/item.h \
    src/item_p.h \
    src/newsmodel.h \
    src/user.h \
    src/user_p.h

OTHER_FILES += \
    qml/cover/CoverPage.qml \
    qml/SailHN.qml \
    qml/pages/CommentsPage.qml \
    qml/pages/ItemDelegate.qml \
    qml/pages/ItemWebView.qml \
    qml/pages/AskStoriesPage.qml \
    qml/pages/BestStoriesPage.qml \
    qml/pages/JobStoriesPage.qml \
    qml/pages/NewStoriesPage.qml \
    qml/pages/RepliesPage.qml \
    qml/pages/Reply.qml \
    qml/pages/ReplyDelegate.qml \
    qml/pages/Settings.qml \
    qml/pages/Submit.qml \
    qml/pages/ShowStoriesPage.qml \
    qml/pages/StoriesListView.qml \
    qml/pages/TopStoriesPage.qml \
    harbour-sailhn.desktop \
    rpm/harbour-sailhn.changes \
    rpm/harbour-sailhn.spec \
    rpm/harbour-sailhn.yaml \
    translations/*.ts

CONFIG += sailfishapp_i18n

TRANSLATIONS += translations/harbour-sailhn-de.ts \
    translations/harbour-sailhn-it.ts \
    translations/harbour-sailhn-nl.ts \
    translations/harbour-sailhn-sr.ts \
    translations/harbour-sailhn-tr.ts
