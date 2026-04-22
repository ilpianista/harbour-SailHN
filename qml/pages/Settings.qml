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
import harbour.sailhn 1.0

Page {
    function authenticate() {
        manager.authenticate(username.text.trim(), password.text.trim());
        login.enabled = false;
        busy.visible = busy.running = true;
        msg.visible = false;
    }

    function isAuthenticated(isAuth) {
        username.enabled = password.enabled = !isAuth;
        login.visible = !isAuth;
        logout.enabled = isAuth;
        appWindow.authenticated = isAuth;
        if (!isAuth) {
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

    allowedOrientations: Orientation.All
    onStatusChanged: {
        if (status === PageStatus.Active) {
            username.text = manager.getUsername();
            if (username.text.length > 0)
                password.forceActiveFocus();

            var isAuth = manager.isAuthenticated();
            isAuthenticated(isAuth);
            if (isAuth)
                updateDetails();
        }
    }

    Connections {
        target: manager
        onAuthenticated: {
            console.log("Authenticated: " + result);
            busy.visible = busy.running = false;
            login.visible = false;
            isAuthenticated(result);
            if (!result)
                msg.visible = true;
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

                text: qsTr("Log out")
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
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("Account")
            }

            TextField {
                id: username

                width: parent.width
                label: qsTr("Username")
                placeholderText: qsTr("Username")

                EnterKey.enabled: text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: password.focus = true
            }

            TextField {
                id: password

                width: parent.width
                label: qsTr("Password")
                placeholderText: qsTr("Password")
                echoMode: TextInput.Password

                EnterKey.enabled: text.length > 0 && username.text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-accept"
                EnterKey.onClicked: authenticate()
            }

            Button {
                id: login

                text: qsTr("Login")
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: username.text.length > 0 && password.text.length > 0
                onClicked: authenticate()
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
                        Qt.openUrlExternally(link);
                    }
                }
            }
        }

        VerticalScrollDecorator {}
    }
}
