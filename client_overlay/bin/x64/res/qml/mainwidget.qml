import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0


Rectangle {
    SystemPalette { id: myPalette; colorGroup: SystemPalette.Active }

    id: root
    color: "transparent"

    width: 1500
    height: 800

    property MainConfPage mainConfPage:  MainConfPage {
        stackView: mainView
    }

    StackView {
        id: mainView
        anchors.fill: parent

        function startTimer() {
        	currentItem.startTimer()
        }
        function stopTimer() {
        	currentItem.stopTimer()
        }

		pushEnter: Transition {
			PropertyAnimation {
				property: "x"
				from: mainView.width
				to: 0
				duration: 200
			}
		}
		pushExit: Transition {
			PropertyAnimation {
				property: "x"
				from: 0
				to: -mainView.width
				duration: 200
			}
		}
		popEnter: Transition {
			PropertyAnimation {
				property: "x"
				from: -mainView.width
				to: 0
				duration: 200
			}
		}
		popExit: Transition {
			PropertyAnimation {
				property: "x"
				from: 0
				to: mainView.width
				duration: 200
			}
		}

        initialItem: mainConfPage
    }
}
