/*
 * Copyright (C) 2016 Stuart Howarth <showarth@marxoft.co.uk>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 1.1
import Qt.labs.folderlistmodel 1.0
import com.nokia.symbian 1.1

MySheet {
    id: root

    property bool showFiles: false
    property string startFolder: Settings.downloadPath

    signal fileChosen(string filePath)

    acceptButtonText: folderText.text == "" ? "" : qsTr("Done")
    rejectButtonText: qsTr("Cancel")
    content: Item {
        anchors.fill: parent

        Label {
            id: folderText

            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: platformStyle.paddingLarge
            }

            font.bold: true
            elide: Text.ElideRight
            color: "#0881cb"
            text: showFiles ? fileList.chosenFile.slice(fileList.chosenFile.lastIndexOf("/") + 1)
                            : folderListModel.folder.toString().slice(8)
        }

        SeparatorLabel {
            id: separator

            anchors {
                left: parent.left
                right: parent.right
                top: folderText.bottom
                topMargin: platformStyle.paddingLarge
            }

            text: showFiles ? qsTr("Files") : qsTr("Folders")
        }

        MyListView {
            id: fileList

            property string chosenFile: ""

            anchors {
                top: separator.bottom
                topMargin: platformStyle.paddingLarge
                left: parent.left
                right: parent.right
                bottom: backButton.top
                bottomMargin: platformStyle.paddingLarge
            }
            clip: true
            model: FolderListModel {
                id: folderListModel

                nameFilters: root.showFiles ? [] : ["*.foo_bar"]
                folder: "file://" + root.startFolder
                showDotAndDotDot: false
                showDirs: true
            }
            delegate: FileBrowserDelegate {
                id: delegate

                onClicked: folderListModel.isFolder(index) ? folderListModel.folder = filePath
                                                           : fileList.chosenFile = filePath
            }
        }

        MyScrollBar {
            flickableItem: fileList
        }

        MyToolButton {
            id: backButton

            z: 1000
            anchors {
                left: parent.left
                bottom: parent.bottom
            }
            iconSource: "toolbar-back"
            toolTipText: qsTr("Back")
            onClicked: folderListModel.folder = folderListModel.parentFolder
        }
    }

    Label {
        id: noResultsText

        anchors {
            fill: parent
            margins: platformStyle.paddingLarge
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        font.pixelSize: 40
        color: platformStyle.colorNormalMid
        text: qsTr("Folder empty")
        visible: (fileList.count == 0) && (root.status == DialogStatus.Open)
    }

    onAccepted: root.fileChosen(root.showFiles ? fileList.chosenFile.slice(8)
                                               : folderListModel.folder.toString().slice(8))
}
