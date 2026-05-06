import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform // Para FolderDialog nativo

Rectangle {
    id: root
    width: 750; height: 450
    color: "#f5f5f5"

    FolderDialog {
        id: folderDlg
        property string targetField: ""
        onAccepted: {
            if (targetField === "script") backend.localScriptPath = folderPath
            else backend.localLogPath = folderPath
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Toolbar Customizada
        ToolBar {
            Layout.fillWidth: true
            RowLayout {
                ToolButton {
                    text: qsTr("Save")
                    icon.name: "document-save"
                    onClicked: backend.save()
                }
                ToolButton {
                    text: qsTr("Reload")
                    icon.name: "edit-undo"
                    onClicked: backend.reload()
                }
            }
        }

        // Formulário
        Pane {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            ColumnLayout {
                width: parent.width * 0.8
                anchors.centerIn: parent
                spacing: 20

                // Campo Script Path
                RowLayout {
                    Label { text: qsTr("Local script path:"); Layout.preferredWidth: 120 }
                    TextField {
                        text: backend.localScriptPath
                        Layout.fillWidth: true
                        onTextEdited: backend.localScriptPath = text
                    }
                    Button { 
                        text: "..." 
                        onClicked: { folderDlg.targetField = "script"; folderDlg.open() }
                    }
                }

                // Campo Log Path
                RowLayout {
                    Label { text: qsTr("Local log path:"); Layout.preferredWidth: 120 }
                    TextField {
                        text: backend.localLogPath
                        Layout.fillWidth: true
                        onTextEdited: backend.localLogPath = text
                    }
                    Button { 
                        text: "..." 
                        onClicked: { folderDlg.targetField = "log"; folderDlg.open() }
                    }
                }
            }
        }
    }
}