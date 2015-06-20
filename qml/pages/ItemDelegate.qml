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

Item {
    id: item

    property bool menuOpen: items.contextMenu != null && items.contextMenu.parent === item

    width: ListView.view.width
    height: menuOpen ? items.contextMenu.height + innerItem.height : innerItem.height

    BackgroundItem {
        id: innerItem
        width: parent.width
        height: contentItem.childrenRect.height

        Column {
            x: Theme.horizontalPageMargin
            width: parent.width - Theme.horizontalPageMargin

            Label {
                width: parent.width
                text: title
                font.pixelSize: Theme.fontSizeSmall
                wrapMode: Text.WordWrap
            }

            Label {
                width: parent.width
                font.pixelSize: Theme.fontSizeTiny
                horizontalAlignment: Text.AlignRight

                text: {
                    var pts = score + ' ';
                    if (score === 1) {
                        pts += qsTr("point");
                    } else {
                        pts += qsTr("points");
                    }

                    return pts + " - " + Qt.formatDateTime(time, "ddd, hh:mm");
                }
            }
        }

        onPressAndHold: {
            if (!items.contextMenu) {
                items.contextMenu = menu.createObject(items)
            }

            items.contextMenu.show(item)
        }
    }

}
