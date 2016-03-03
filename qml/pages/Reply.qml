/*
  The MIT License (MIT)

  Copyright (c) 2016 Andrea Scarpino <me@andreascarpino.it>

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

Page {
    property var parentId

    allowedOrientations: Orientation.All

    Connections {
        target: manager

        onCommented: {
            console.log("Commented: " + result);

            busy.visible = busy.running = false;
            send.enabled = true;
            msg.visible = true;

            if (!result) {
                msg.text = qsTr("Error when sending");
            } else {
                msg.text = qsTr("Commented!");
            }
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - Theme.horizontalPageMargin * 2

            PageHeader {
                title: qsTr("Reply")
            }

            TextArea {
                id: comment
                width: parent.width
                focus: true
                placeholderText: qsTr("Text")
            }

            Button {
                id: send
                text: qsTr("Add Comment");
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    if (comment.text.length > 0) {
                        manager.comment(parentId, comment.text);
                        send.enabled = false;
                        busy.visible = busy.running = true;
                        msg.visible = false;
                    }
                }
            }

            Label {
                id: msg
                visible: false
                color: Theme.highlightColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            BusyIndicator {
                id: busy
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
