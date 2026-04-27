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
    contentHeight: content.height
    menu: replyMenu
    onClicked: {
        if (kids.length !== 0)
            pageStack.push(Qt.resolvedUrl("RepliesPage.qml"), {
                "parentId": id,
                "dead": dead,
                "kids": kids
            });
    }

    Column {
        id: content

        width: parent.width

        Text {
            width: parent.width
            textFormat: Text.RichText
            text: "<style>a:link{color: " + Theme.highlightColor + ";}</style>" + itemText
            color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
            font.pixelSize: Theme.fontSizeMedium
            wrapMode: Text.Wrap
            onLinkActivated: {
                Qt.openUrlExternally(link);
            }
        }

        Row {
            spacing: Theme.paddingSmall

            anchors.right: parent.right
            anchors.rightMargin: Theme.horizontalPageMargin

            Row {
                visible: kids.length !== 0
                spacing: Theme.paddingSmall

                Label {
                    color: listItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeSmall
                    text: kids.length
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
                anchors.verticalCenter: parent.verticalCenter
                text: by + " · " + Format.formatDate(time, Formatter.DurationElapsed)
            }
        }
    }

    ContextMenu {
        id: replyMenu

        enabled: manager.isAuthenticated() && !dead

        MenuItem {
            text: qsTr("Reply")
            onClicked: pageStack.push(Qt.resolvedUrl("Reply.qml"), {
                "parentId": id
            })
        }
    }
}
