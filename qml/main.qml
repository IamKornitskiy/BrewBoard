import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: "BrewBoard"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        TextField {
            id: searchField
            Layout.fillWidth: true
            placeholderText: "Enter beer name..."
            onAccepted: searchButton.clicked()
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Button {
                id: searchButton
                text: "Search"
                Layout.fillWidth: true
                onClicked: {
                    statusText.text = "Searching..."
                    searchController.search(searchField.text)
                }
            }

            Button {
                text: "Clear"
                Layout.fillWidth: true
                onClicked: {
                    searchField.clear()
                    statusText.text = "Enter a query to search"
                }
            }
        }

        Text {
            id: statusText
            text: "Enter a query to search"
            color: "#666"
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }

        ListView {
            id: resultsView
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 10
            clip: true

            model: searchController.beers  // модель BeerListModel

            delegate: Rectangle {
                width: parent.width
                height: 100
                color: "#f0f0f0"
                radius: 5

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 15

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 5

                        Text {
                            text: model.name   // доступ через роль NameRole
                            font.bold: true
                            font.pixelSize: 16
                        }

                        Text {
                            text: model.brewery
                            color: "#555"
                            font.pixelSize: 12
                        }

                        RowLayout {
                            spacing: 20
                            visible: model.abv > 0 || model.ibu > 0

                            Text {
                                visible: model.abv > 0
                                text: "ABV: " + model.abv + "%"
                                font.pixelSize: 11
                                color: "#777"
                            }

                            Text {
                                visible: model.ibu > 0
                                text: "IBU: " + model.ibu
                                font.pixelSize: 11
                                color: "#777"
                            }
                        }

                        Text {
                            text: model.description
                            color: "#888"
                            font.pixelSize: 10
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }
                    }

                    Image {
                        source: model.labelUrl
                        width: 60
                        height: 60
                        fillMode: Image.PreserveAspectCrop
                        asynchronous: true
                        visible: source.toString() !== ""
                        Rectangle {
                            anchors.fill: parent
                            color: "#ddd"
                            radius: 5
                            visible: parent.source.toString() === ""
                            Text {
                                anchors.centerIn: parent
                                text: "no\nimage"
                                font.pixelSize: 10
                                color: "#999"
                                horizontalAlignment: Text.AlignHCenter
                            }
                        }
                    }
                }
            }
        }

        Connections {
            target: searchController
            function onSearchError(error) {
                statusText.text = "Error: " + error
            }
            function onBeersChanged() {
                var count = searchController.beers ? searchController.beers.count : 0
                if (count === 0)
                    statusText.text = "No beers found"
                else
                    statusText.text = count + " beers found"
            }
            function onLoadingChanged() {
                if (searchController.loading)
                    statusText.text = "Searching..."
            }
        }
    }
}