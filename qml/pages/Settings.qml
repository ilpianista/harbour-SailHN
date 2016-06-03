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
import harbour.sailhn 1.0

Page {

    allowedOrientations: Orientation.All

    Connections {
        target: manager

        onAuthenticated: {
            console.log("Authenticated: " + result);

            busy.visible = busy.running = false;

            isAuthenticated(result);

            if (!result) {
                msg.visible = true;
            }
        }

        onLoggedUserFetched: {
            updateDetails();
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {

            MenuItem {
                id: logout
                text: qsTr("Logout")

                onClicked: {
                    manager.logout();
                    isAuthenticated(false);
                }
            }
        }

        Column {
            id: column
            x: Theme.horizontalPageMargin
            width: parent.width - Theme.horizontalPageMargin * 2

            PageHeader {
                title: qsTr("Settings")
            }

            TextField {
                id: username
                width: parent.width
                focus: text.length === 0
                placeholderText: qsTr("Username")
            }

            TextField {
                id: password
                width: parent.width
                focus: username.text.length > 0 && text.length === 0
                placeholderText: qsTr("Password")
                echoMode: TextInput.Password
            }

            Button {
                id: login
                text: qsTr("Login");
                anchors.horizontalCenter: parent.horizontalCenter

                onClicked: {
                    if (username.text.length > 0 && password.text.length > 0) {
                        manager.authenticate(username.text, password.text);
                        login.enabled = false;
                        busy.visible = busy.running = true;
                        msg.visible = false;
                    }
                }
            }

            Label {
                id: msg
                visible: false
                text: qsTr("Login failed")
                color: Theme.highlightColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            BusyIndicator {
                id: busy
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Column {
                id: details
                width: parent.width
                visible: false

                SectionHeader {
                    text: qsTr("User details")
                }

                DetailItem {
                    id: created
                    width: parent.width
                    label: qsTr("created")
                }

                DetailItem {
                    id: karma
                    width: parent.width
                    label: qsTr("karma")
                }

                Text {
                    id: about
                    width: parent.width
                    text: qsTr("About")
                    enabled: false
                    textFormat: Text.RichText
                    color: Theme.secondaryColor
                    font.pixelSize: Theme.fontSizeMedium
                    wrapMode: Text.Wrap

                    onLinkActivated: {
                        console.log("Opening external browser: " + link);
                        Qt.openUrlExternally(link)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        username.text = manager.getUsername();
        var isAuth = manager.isAuthenticated();
        isAuthenticated(isAuth);

        if (isAuth) {
            updateDetails();
        }
    }

    function isAuthenticated(isAuth) {
        username.enabled = password.enabled = login.enabled = !isAuth;
        logout.enabled = isAuth;

        if (isAuth) {
            login.text = qsTr("Logged");
        } else {
            login.text = qsTr("Login");
            details.visible = false;
            password.text = "";
        }
    }

    function updateDetails() {
        var user = manager.loggedUser();
        details.visible = true;
        created.value = Qt.formatDateTime(user.created);
        karma.value = user.karma;
        about.text = "<style>a:link{color: " + Theme.highlightColor + ";}</style>" + user.about;
    }
}
