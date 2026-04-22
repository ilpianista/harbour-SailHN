/*
  The MIT License (MIT)

  Copyright (c) 2015-2023 Andrea Scarpino <andrea@scarpino.dev>

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

CoverBackground {
    CoverPlaceholder {
        text: "SailHN"
        icon.source: "/usr/share/icons/hicolor/86x86/apps/harbour-sailhn.png"
        visible: appWindow.itemTitle.length === 0
    }

    Column {
        id: column
        anchors.centerIn: parent
        x: Theme.paddingSmall
        width: parent.width - Theme.paddingSmall * 2
        visible: appWindow.itemTitle.length > 0

        Label {
            width: parent.width
            wrapMode: Text.Wrap
            maximumLineCount: 4
            elide: Text.ElideRight
            font.pixelSize: Theme.fontSizeMedium
            text: appWindow.itemTitle
        }

        Label {
            width: parent.width
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignRight
            text: {
                var txt = appWindow.itemScore + ' ';
                if (appWindow.itemScore === 1)
                    txt += qsTr("point");
                else
                    txt += qsTr("points");
            }
        }

        Label {
            width: parent.width
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignRight
            visible: appWindow.itemDescendants > 0
            text: {
                var txt = appWindow.itemDescendants + ' ';
                if (appWindow.itemDescendants === 1)
                    txt += qsTr("comment");
                else
                    txt += qsTr("comments");
            }
        }
    }

    CoverActionList {
        enabled: appWindow.authenticated && appWindow.itemTitle.length === 0

        CoverAction {
            id: cover
            iconSource: "image://theme/icon-cover-new"

            onTriggered: {
                appWindow.activate();
                pageStack.push(Qt.resolvedUrl("../pages/Submit.qml"));
            }
        }
    }
}
