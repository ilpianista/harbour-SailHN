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

Page {
    property var by
    property var kids
    property var score
    property var time
    property var title
    property var url

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: childrenRect.height

        Column {
            x: Theme.horizontalPageMargin
            width: parent.width - Theme.horizontalPageMargin * 2
            anchors.fill: parent
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Details")
            }

            DetailItem {
                label: qsTr("Title")
                value: title
            }

            DetailItem {
                label: qsTr("Author")
                value: by
            }

            DetailItem {
                label: qsTr("When")
                value: Qt.formatDateTime(time);
            }

            DetailItem {
                label: qsTr("Score")
                value: score
            }

            DetailItem {
                label: qsTr("Comments")
                value: kids.length
            }

            // DetailItem does not expose onLinkActivated signal,
            // this is a workaround to get it
            Row {
                x: Theme.paddingMedium
                spacing: Theme.paddingMedium
                width: parent.width

                Label {
                    id: urlLabel
                    text: qsTr("Url")
                    color: Theme.secondaryHighlightColor
                    font.pixelSize: Theme.fontSizeSmall
                }

                Text {
                    width: parent.width - urlLabel.width - Theme.paddingMedium * 3
                    textFormat: Text.RichText
                    text: {
                        var txt = "<style>a:link{color: " + Theme.highlightColor + ";}</style>";
                        txt += "<a href=\"" + url + "\" rel=\"nofollow\">" + url + "</a>";
                        return txt;
                    }
                    wrapMode: Text.WrapAnywhere
                    color: Theme.highlightColor
                    font.pixelSize: Theme.fontSizeSmall

                    onLinkActivated: Qt.openUrlExternally(url)
                }
            }
        }
    }
}
