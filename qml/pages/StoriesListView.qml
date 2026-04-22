/*
  The MIT License (MIT)

  Copyright (c) 2016-2021 Andrea Scarpino <andrea@scarpino.dev>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailhn 1.0

SilicaListView {
    id: listView

    property string pageTitle
    property alias submitEnabled: submit.enabled
    property alias stories: model
    property bool loadingMore: false

    signal refreshClicked

    Component.onCompleted: {
        appWindow.itemTitle = "";
        appWindow.itemScore = 0;
        appWindow.itemDescendants = 0;
    }

    Connections {
        target: manager
        onLoggedUserFetched: {
            submit.enabled = true;
            appWindow.authenticated = true;
        }
    }

    Connections {
        target: model
        onRowsInserted: {
            loadingMore = false;
        }
    }

    onAtYEndChanged: {
        if (atYEnd && !loadingMore && listView.count > 0) {
            loadingMore = true;
            model.nextItems();
        }
    }

    PullDownMenu {
        MenuItem {
            text: qsTr("Account")
            onClicked: pageStack.push(Qt.resolvedUrl("Settings.qml"))
        }

        MenuItem {
            id: submit

            text: qsTr("Submit")
            enabled: false
            onClicked: pageStack.push(Qt.resolvedUrl("Submit.qml"))
        }

        MenuItem {
            text: qsTr("Refresh")
            onClicked: listView.refreshClicked()
        }
    }

    footer: Column {
        width: parent.width
        height: visible ? implicitHeight + Theme.paddingLarge * 2 : 0
        visible: loadingMore && listView.count > 0

        BusyIndicator {
            anchors.horizontalCenter: parent.horizontalCenter
            size: BusyIndicatorSize.Small
            running: loadingMore
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("Loading more…")
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
        }
    }

    VerticalScrollDecorator {}

    model: NewsModel {
        id: model
    }

    header: PageHeader {
        title: pageTitle
    }

    delegate: ItemDelegate {}
}
