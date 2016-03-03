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
import harbour.sailhn 1.0

Page {
    property var parentId
    property var kids

    readonly property int maxCommentsForPage: 30
    property int showingCommentsCount: maxCommentsForPage

    allowedOrientations: Orientation.All

    SilicaFlickable {
        id: replies
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {

            MenuItem {
                id: reply
                text: qsTr("Reply")

                onClicked: pageStack.push(Qt.resolvedUrl("Reply.qml"), {parentId: parentId})
            }

            MenuItem {
                text: qsTr("Refresh")

                onClicked: {
                    loadComments();
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
                text: qsTr("Replies")
            }

            Repeater {

                model: NewsModel {
                    id: model

                    onRowsInserted: {
                        if (kids.length > showingCommentsCount) {
                            replies.pushUpMenu.visible = true;
                        }
                    }
                }

                delegate: ReplyDelegate {}
            }
        }

        VerticalScrollDecorator {}
    }

    Component.onCompleted: {
        reply.enabled = manager.isAuthenticated();
        loadComments();
    }

    function loadComments() {
        model.loadComments(kids);
    }
}
