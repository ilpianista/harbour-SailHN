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

Page {

    allowedOrientations: Orientation.All

    Connections {
        target: manager

        onSubmitted: {
            console.log("Submitted: " + result);

            busy.visible = busy.running = false;
            submit.enabled = true;
            msg.visible = true;

            if (!result) {
                msg.text = qsTr("Error during submission");
            } else {
                msg.text = qsTr("Submitted!");
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
                title: qsTr("Submit")
            }

            TextField {
                id: title
                width: parent.width
                focus: true
                placeholderText: qsTr("Title")

                onTextChanged: submit.enabled = (text.length > 0 && (url.text.length > 0 || itemText.text.length > 0))
            }

            TextField {
                id: url
                width: parent.width
                placeholderText: qsTr("Url")

                onTextChanged: submit.enabled = (title.text.length > 0 && (text.length > 0 || itemText.text.length > 0))
            }

            Label {
                text: qsTr("or")
                color: Theme.secondaryHighlightColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            TextArea {
                id: itemText
                width: parent.width
                placeholderText: qsTr("Text")

                onTextChanged: submit.enabled = (title.text.length > 0 && (url.text.length > 0 || text.length > 0))
            }

            Button {
                id: submit
                text: qsTr("Submit");
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: false

                onClicked: {
                    manager.submit(title.text, url.text, itemText.text);
                    submit.enabled = false;
                    busy.visible = busy.running = true;
                    msg.visible = false;
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
