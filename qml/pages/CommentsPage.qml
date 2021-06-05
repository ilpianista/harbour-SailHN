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
import harbour.sailhn 1.0

Page {
    property var id
    property var by
    property var dead
    property var itemText
    property var kids
    property var time
    property var title
    property var url

    readonly property int maxCommentsForPage: 30
    property int showingCommentsCount: maxCommentsForPage

    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: comments
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("Copy external url")

                onClicked: {
                    Clipboard.text = url;
                }
            }

            MenuItem {
                id: reply
                text: qsTr("Reply")
                enabled: !dead

                onClicked: pageStack.push(Qt.resolvedUrl("Reply.qml"), {parentId: id})
            }

            MenuItem {
                text: qsTr("Refresh")

                onClicked: {
                    model.refresh(id);
                    showingCommentsCount = maxCommentsForPage;
                }
            }
        }

        PushUpMenu {
            visible: false

            MenuItem {
                text: qsTr("Load more")

                onClicked: {
                    model.nextItems();
                    showingCommentsCount += maxCommentsForPage;
                }
            }
        }

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - Theme.horizontalPageMargin * 2
            spacing: Theme.paddingMedium

            SectionHeader {
                text: qsTr("Comments")
            }

            Label {
                width: parent.width
                text: title
                color:Theme.primaryColor
                font.pixelSize: Theme.fontSizeMedium
                wrapMode: Text.Wrap
            }

            Text {
                width: parent.implicitWidth
                visible: (itemText && !(/^\s*$/.test(itemText)))
                textFormat: Text.RichText
                text: "<style>a:link{color: " + Theme.highlightColor + ";}</style>" + itemText
                color: Theme.secondaryColor
                font.pixelSize: Theme.fontSizeMedium
                wrapMode: Text.Wrap

                onLinkActivated: {
                    console.log("Opening external browser: " + link);
                    Qt.openUrlExternally(link)
                }
            }

            // Workaround for Label that does not provide onClick?
            Text {
                width: parent.implicitWidth
                visible: (url && !(/^\s*$/.test(url)))
                textFormat: Text.RichText
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.WrapAnywhere
                text: {
                    var txt = "<style>a:link{color: " + Theme.secondaryHighlightColor + ";}</style>";
                    txt += "<a href=\"" + url + "\" rel=\"nofollow\">" + url + "</a>";
                    return txt;
                }

                onLinkActivated: {
                    console.log("Opening external browser: " + link);
                    Qt.openUrlExternally(link);
                }
            }

            Label {
                width: parent.width
                color: Theme.secondaryHighlightColor
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignRight
                text: by + " - " + Qt.formatDateTime(time, "ddd, hh:mm");
            }

            Repeater {

                model: NewsModel {
                    id: model

                    onRowsInserted: {
                        if (kids.length > showingCommentsCount) {
                            comments.pushUpMenu.visible = true;
                        }
                    }
                }

                delegate: ReplyDelegate {}
            }
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: {
        loadComments();
        reply.enabled = manager.isAuthenticated();
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            if (url && !(/^\s*$/.test(url))) {
                pageStack.pushAttached(Qt.resolvedUrl("ItemWebView.qml"), { itemUrl: url });
            }
        }
    }

    function loadComments() {
        model.loadComments(kids);
    }
}
