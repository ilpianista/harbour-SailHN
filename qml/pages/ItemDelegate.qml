/*
  The MIT License (MIT)

  Copyright (c) 2015-2021 Andrea Scarpino <andrea@scarpino.dev>

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
    height: col.height

    Column {
        id: col
        x: Theme.horizontalPageMargin
        width: parent.width - Theme.horizontalPageMargin * 2

        Label {
            color: Theme.primaryColor
            width: parent.width
            text: title
            font.pixelSize: Theme.fontSizeMedium
            truncationMode: TruncationMode.Fade
            maximumLineCount: 2
            wrapMode: Text.WordWrap
        }

        Label {
            width: parent.width
            color: Theme.secondaryHighlightColor
            font.pixelSize: Theme.fontSizeSmall
            horizontalAlignment: Text.AlignRight

            text: {
                var txt = score + ' ';
                if (score === 1) {
                    txt += qsTr("point");
                } else {
                    txt += qsTr("points");
                }

                if (kids.length !== 0) {
                    txt += " - " + descendants + ' ';
                    if (descendants === 1) {
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
        pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), {
                           id: id,
                           by: by,
                           dead: dead,
                           itemText: itemText,
                           kids: kids,
                           time: time,
                           title: title,
                           url: url
                       });
    }
}
