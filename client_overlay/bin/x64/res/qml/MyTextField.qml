import QtQuick 2.7
import QtQuick.Controls 2.0
import pottedmeat7.keyboardinput 1.0

TextField {
	property int keyBoardUID: 0
    property string savedText: ""
    id: myTextField
    color: myPalette.windowText //"#000000"
    text: ""
    font.pointSize: 19
    background: Button {
        hoverEnabled: true
        background: Rectangle {
            color: myPalette.base //"#c0c0c0" // parent.hovered ? "#0D5834" : "#0D5834"
            border.color: myPalette.mid //"#000000"
            border.width: 2
        }
        onClicked: {
            myTextField.forceActiveFocus()
        }
    }
    onActiveFocusChanged: {
        if (activeFocus) {
            if (!OverlayController.desktopMode) {
                OverlayController.showKeyboard(text, keyBoardUID)
            } else {
                savedText = text
            }
        }
    }
    onEditingFinished: {
        if (OverlayController.desktopMode && savedText !== text) {
            myTextField.onInputEvent(text)
        }
    }
    function onInputEvent(input) {
        text = input
	}
    Connections {
        target: OverlayController
        onKeyBoardInputSignal: {
            if (userValue == keyBoardUID) {
                if (myTextField.text !== input) {
                    myTextField.onInputEvent(input)
                }
            }
        }
    }
}
