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

CoverBackground {

    Connections {
        target: manager

        onSearchResult: {
            placeholder.visible = !found;

            if (found) {
                title.text = lyric.artist + " - " + lyric.song;
                itemText.text = lyric.text;
            }
            itemInfo.visible = found;
        }
    }

    CoverPlaceholder {
        id: placeholder
        text: "SailHN"
        icon.source: "/usr/share/icons/hicolor/86x86/apps/harbour-sailhn.png"
    }

    Column {
        id: itemInfo
        anchors.centerIn: parent
        x: Theme.paddingSmall
        width: parent.width - Theme.paddingSmall * 2

        Label {
            id: title
            width: parent.width
            wrapMode: Text.Wrap
            font.pixelSize: Theme.fontSizeMedium
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: itemText
            width: parent.width
            wrapMode: Text.Wrap
            maximumLineCount: 10
            elide: Text.ElideRight
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }
}
