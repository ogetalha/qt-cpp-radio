import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Radio Player"

    ListModel {
        id: stationModel
        ListElement {
            name: "Radio Paradise"
            url: "http://stream.radioparadise.com/mp3-192"
        }
        ListElement {
            name: "Swiss Jazz"
            url: "http://stream.srg-ssr.ch/m/rsj/mp3_128"
        }
        ListElement {
            name: "BBC Radio 1"
            url: "http://bbcmedia.ic.llnwd.net/stream/bbcmedia_radio1_mf_p"
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 20
        anchors.margins: 20

        ComboBox {
            id: stationBox
            Layout.fillWidth: true
            model: stationModel
            textRole: "name"
            currentIndex: 0
        }

        RowLayout {
            spacing: 10
            Layout.alignment: Qt.AlignHCenter

            Button {
                text: "Play"
                enabled: !radioStatus.playing
                onClicked: {
                    let station = stationModel.get(stationBox.currentIndex);
                    console.log("Playing:", station.name, station.url);
                    radioStatus.play(station.url);
                }
            }

            Button {
                text: "Pause"
                enabled: radioStatus.playing && !radioStatus.paused
                onClicked: radioStatus.pause()
            }

            Button {
                text: "Resume"
                enabled: radioStatus.playing && radioStatus.paused
                onClicked: radioStatus.resume()
            }

            Button {
                text: "Stop"
                enabled: radioStatus.playing
                onClicked: radioStatus.stop()
            }
        }

        Text {
            text: "Now Playing: " + stationModel.get(stationBox.currentIndex).name
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            font.pixelSize: 14
        }

        Text {
            text: "Status: " + (radioStatus.playing ? (radioStatus.paused ? "Paused" : "Playing") : "Stopped")
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 12
            color: radioStatus.playing ? (radioStatus.paused ? "orange" : "green") : "gray"
        }

        Text {
            text: "URL: " + radioStatus.currentUrl
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.Wrap
            font.pixelSize: 10
            color: "gray"
        }
    }
}
