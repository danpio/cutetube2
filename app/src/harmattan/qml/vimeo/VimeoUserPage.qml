/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 3, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 1.1
import com.nokia.meego 1.0
import cuteTube 2.0
import QVimeo 1.0 as QVimeo
import ".."
import "file:///usr/lib/qt4/imports/com/nokia/meego/UIConstants.js" as UI

MyPage {
    id: root

    function load(userOrId) {
        user.loadUser(userOrId);

        if ((user.id) && (!user.subscribed) && (Vimeo.userId)) {
            user.checkIfSubscribed();
        }
    }

    title: view.currentTab.title
    showProgressIndicator: (user.status == QVimeo.ResourcesRequest.Loading)
                           || (view.currentTab.showProgressIndicator)
    tools: view.currentTab.tools ? view.currentTab.tools : ownTools
    onToolsChanged: if (status == PageStatus.Active) appWindow.pageStack.toolBar.setTools(tools, "set");

    ToolBarLayout {
        id: ownTools

        visible: false

        BackToolIcon {}
    }

    VimeoUser {
        id: user

        onStatusChanged: {
            switch (status) {
            case QVimeo.ResourcesRequest.Ready:
                if ((!subscribed) && (Vimeo.userId)) {
                    checkIfSubscribed();
                }

                break;
            case QVimeo.ResourcesRequest.Failed:
                infoBanner.showMessage(errorString);
                break;
            default:
                break;
            }
        }
    }

    TabView {
        id: view

        anchors.fill: parent
        visible: user.id != ""

        Tab {
            id: infoTab

            width: view.width
            height: view.height
            title: user.username ? user.username + "'s " + qsTr("channel") : qsTr("Channel")
            tools: ToolBarLayout {

                BackToolIcon {}

                MyToolIcon {
                    platformIconId: user.subscribed ? "toolbar-cancle" : "toolbar-add"
                    enabled: (Vimeo.userId) && (Vimeo.hasScope(Vimeo.INTERACT_SCOPE)) && (user.id)
                             && (user.id != Vimeo.userId)
                    onClicked: user.subscribed ? user.unsubscribe() : user.subscribe()
                }
            }

            Flow {
                id: flow

                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    margins: UI.PADDING_DOUBLE
                }

                spacing: UI.PADDING_DOUBLE

                Avatar {
                    id: avatar

                    width: height
                    height: Math.floor(parent.width / 4)
                    source: user.thumbnailUrl
                    enabled: false
                }

                Item {
                    width: parent.width - avatar.width - UI.PADDING_DOUBLE
                    height: avatar.height
                }

                Label {
                    id: nameLabel

                    font.bold: true
                    text: user.username
                }

                Label {
                    id: statsLabel

                    width: parent.width - nameLabel.width - UI.PADDING_DOUBLE
                    horizontalAlignment: Text.AlignRight
                    font.pixelSize: UI.FONT_SMALL
                    font.family: UI.FONT_FAMILY_LIGHT
                    text: Utils.formatLargeNumber(user.subscriberCount) + " " + qsTr("subscribers")
                }
            }

            Flickable {
                id: flicker

                anchors {
                    left: parent.left
                    right: parent.right
                    top: flow.bottom
                    topMargin: UI.PADDING_DOUBLE
                    bottom: parent.bottom
                }
                clip: true
                contentHeight: descriptionLabel.height + UI.PADDING_DOUBLE * 2

                Label {
                    id: descriptionLabel

                    anchors {
                        left: parent.left
                        leftMargin: UI.PADDING_DOUBLE
                        right: parent.right
                        rightMargin: UI.PADDING_DOUBLE
                        top: parent.top
                    }
                    text: user.description ? Utils.toRichText(user.description) : qsTr("No description")
                    onLinkActivated: {
                        var resource = Resources.getResourceFromUrl(link);

                        if (resource.service != Resources.VIMEO) {
                            Qt.openUrlExternally(link);
                            return;
                        }

                        if (resource.type == Resources.USER) {
                            appWindow.pageStack.push(Qt.resolvedUrl("VimeoUserPage.qml")).load(resource.id);
                        }
                        else if (resource.type == Resources.PLAYLIST) {
                            appWindow.pageStack.push(Qt.resolvedUrl("VimeoPlaylistPage.qml")).load(resource.id);
                        }
                        else {
                            appWindow.pageStack.push(Qt.resolvedUrl("VimeoVideoPage.qml")).load(resource.id);
                        }
                    }
                }
            }

            ScrollDecorator {
                flickableItem: flicker
            }

            states: State {
                name: "landscape"
                when: !appWindow.inPortrait

                AnchorChanges {
                    target: flow
                    anchors.right: parent.horizontalCenter
                }

                AnchorChanges {
                    target: flicker
                    anchors {
                        left: flow.right
                        top: parent.top
                    }
                }
            }
        }

        TabLoader {
            id: uploadsTab

            width: view.width
            height: view.height
            title: user.username + "'s " + qsTr("videos")
            tab: Component {
                VimeoVideosTab {
                    Component.onCompleted: model.list("/users/" + user.id + "/videos", {per_page: MAX_RESULTS})
                }
            }
        }

        TabLoader {
            id: favouritesTab

            width: view.width
            height: view.height
            title: user.username + "'s " + qsTr("likes")
            tab: Component {
                VimeoVideosTab {
                    Component.onCompleted: model.list("/users/" + user.id + "/likes", {per_page: MAX_RESULTS})
                }
            }
        }

        TabLoader {
            id: playlistsTab

            width: view.width
            height: view.height
            title: user.username + "'s " + qsTr("albums")
            tab: Component {
                VimeoPlaylistsTab {
                    Component.onCompleted: model.list("/users/" + user.id + "/albums", {per_page: MAX_RESULTS})
                }
            }
        }

        TabLoader {
            id: subscriptionsTab

            width: view.width
            height: view.height
            title: user.username + "'s " + qsTr("subscriptions")
            tab: Component {
                VimeoUsersTab {
                    Component.onCompleted: model.list("/users/" + user.id + "/following", {per_page: MAX_RESULTS})
                }
            }
        }
    }
}
