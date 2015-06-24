/*
  The MIT License (MIT)

  Copyright (c) 2015 Andrea Scarpino <me@andreascarpino.it>

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

ListItem {
    width: ListView.view.width
    menu: menu

    Column {
        x: Theme.horizontalPageMargin
        width: parent.width - Theme.horizontalPageMargin * 2

        Label {
            color: Theme.primaryColor
            width: parent.width
            text: title
            font.pixelSize: Theme.fontSizeSmall
            truncationMode: TruncationMode.Fade
        }

        Label {
            width: parent.width
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeExtraSmall
            horizontalAlignment: Text.AlignRight

            text: {
                var txt = score + ' ';
                if (score === 1) {
                    txt += qsTr("point");
                } else {
                    txt += qsTr("points");
                }

                if (kids.length !== 0) {
                    txt += " - " + kids.length + ' ';
                    if (kids.length === 1) {
                        txt += qsTr("comment");
                    } else {
                        txt += qsTr("comments");
                    }
                }

                return txt + " - " + Qt.formatDateTime(time, "ddd, hh:mm");
            }
        }
    }

    onClicked: {
        if (kids.length !== 0) {
            pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), {kids: kids});
        }
    }

    ContextMenu {
        id: menu

        MenuItem {
            text: qsTr("Open external url")

            onClicked: {
                console.log("Opening external browser: " + url);
                Qt.openUrlExternally(url);
            }
        }

        MenuItem {
            text: qsTr("Show details")

            onClicked: {
                pageStack.push(Qt.resolvedUrl("DetailsPage.qml"), {
                    by: by,
                    kids: kids,
                    score: score,
                    time: time,
                    title: title,
                    url: url
                });
            }
        }
    }
}
