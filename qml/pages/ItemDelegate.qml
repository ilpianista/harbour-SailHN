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
    id: listItem
    width: ListView.view.width
    contentHeight: col.height + Theme.paddingSmall * 2

    onClicked: {
        pageStack.push(Qt.resolvedUrl("CommentsPage.qml"), {
            "id": id,
            "by": by,
            "dead": dead,
            "descendants": descendants,
            "itemText": itemText,
            "kids": kids,
            "score": score,
            "time": time,
            "title": title,
            "url": url
        });
    }

    Column {
        id: col

        x: Theme.horizontalPageMargin
        width: parent.width - Theme.horizontalPageMargin * 2

        Label {
            color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
            width: parent.width
            text: title
            font.pixelSize: Theme.fontSizeMedium
            truncationMode: TruncationMode.Fade
            maximumLineCount: 2
            wrapMode: Text.WordWrap
        }

        Row {
            spacing: Theme.paddingMedium
            width: childrenRect.width
            anchors.right: parent.right
            anchors.rightMargin: Theme.horizontalPageMargin

            Row {
                spacing: Theme.paddingSmall

                Label {
                    color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                    text: score
                    anchors.verticalCenter: parent.verticalCenter
                }

                Image {
                    source: "image://theme/icon-s-like"
                    width: Theme.iconSizeExtraSmall
                    height: Theme.iconSizeExtraSmall
                    anchors.verticalCenter: parent.verticalCenter
                    opacity: 0.7
                }
            }

            Row {
                visible: kids.length !== 0
                spacing: Theme.paddingSmall

                Label {
                    color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                    text: descendants
                    anchors.verticalCenter: parent.verticalCenter
                }

                Image {
                    source: "image://theme/icon-s-chat"
                    width: Theme.iconSizeExtraSmall
                    height: Theme.iconSizeExtraSmall
                    anchors.verticalCenter: parent.verticalCenter
                    opacity: 0.7
                }
            }

            Label {
                color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: Format.formatDate(time, Formatter.DurationElapsed)
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }
}
