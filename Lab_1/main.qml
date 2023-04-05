import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.0
import "MaterialDesign.js" as MD


ApplicationWindow {
    id: window
    width: 500
    height: 480
    visible: true
    title: qsTr("LB1")

    property int recordId: -1
    property int isPassword: -1
    property string pin_code: ""


    Connections {
            target: cryptoController
            onSendMessageToQml: {                // открывает диалоговое окно и выводит текст сообщения в текстовое поле
                dialog.open()
                dialogText.text = message
            }
            onSendDbToQml: {
                listmodel1.clear()
                let json_db = JSON.parse(db_decrypted)               // парсит расшифрованную базу данных
                for(let i = 0; i < json_db["sites"].length; i++) {
                    let json_db_model = {}
                    json_db_model["site"] = json_db["sites"][i]["site"]
                    json_db_model["login"] = json_db["sites"][i]["login"]
                    json_db_model["password"] = json_db["sites"][i]["password"]
                    listmodel1.append(json_db_model)
                    listmodel2.append(json_db_model)
                }
            }
    }


    ListModel {
        id: listmodel2
        // Items таблицы бд
    }

    //функция поиска
    function finder() {                                        //очищаем содержимое
        listmodel1.clear()
            for(var i = 0; i < listmodel2.count; ++i) {
                if (listmodel2.get(i).site.includes(edtSearch.text)) {  //если содержит свойство site в listmodel2 текст, введенный в поле edtSearch. Если да, переходим к следующей строке, иначе проходим дальше.
                    listmodel1.append(listmodel2.get(i))
                }
            }
        }
    //копирования в буфер, предварительно расшифровав
    function get_password(crypt_password, pin) {
            cryptoController.text_to_clipboard(crypt_password, pin)
        }
    //копирования в буфер, предварительно расшифровав
    function getLogin(crypt_login, pin){
        cryptoController.text_to_clipboard(crypt_login, pin)
    }

    //Проверка пароля и расшифровка второго слоя БД
    function crypt_controller(password) {
            let is_correct_passwd = cryptoController.check_password(password)  //Проверка пароля
            if(is_correct_passwd) {
                pin_code=password                                    //сохраняем введенный пароль в переменную для передачи
                if(recordId != -1 && isPassword != -1)
                {
                    cryptoController.encrypt_db_file(password, recordId, isPassword)     //для шифровки файла
                    cryptoController.decrypt_db_file(password, recordId, isPassword)    //для расшифровки файла
                    recordId = -1
                    isPassword = -1
                }
                else
//                    cryptoController.encrypt_db_file(password)
                    cryptoController.decrypt_db_file(password)               //для расшифровки файла
                stackView.push(pageMain)
            } else {
                stackView.push(pageError)
            }
    }


    FontLoader {
        id: iconFont
        source: "../fonts/MaterialIcons-Regular.ttf"
    }


    StackView {
        anchors.fill: parent
        id: stackView

        initialItem: Page {                //страница входа
            id: pageLogin

            GridLayout {
                anchors.fill: parent
                rowSpacing: 10
                rows: 4
                flow: GridLayout.TopToBottom

                Item { // заполнение пространства сверху
                    Layout.row: 0
                    Layout.fillHeight: true
                }

                TextField {               //поле ввода
                    id: password_code
                    echoMode: TextField.Password
                    passwordCharacter: "✱"
                    Layout.row: 1
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                }

                Button {
                    id: login_button
                        text: qsTr("Войти")
                        Layout.row: 2
                        Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                        onClicked:{
                            crypt_controller(password_code.text)
                            password_code.text = ""
                        }
                }

                Item { // заполнение пространства снизу
                    Layout.row: 4
                    Layout.fillHeight: true
                }
            }
        }

        Page {                      //страница при неверном пин
            id: pageError
            visible: false

            GridLayout {
                anchors.fill: parent
                rowSpacing: 10
                rows: 3
                flow: GridLayout.TopToBottom

                Item { // заполнение пространства сверху
                    Layout.row: 0
                    Layout.fillHeight: true
                }
                Label {
                    id: error_code
                    text: "Код неверный!"
                    Layout.row: 1
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                }

                Button {
                    id: back_to_login_page
                    text: qsTr("Назад")
                    Layout.row: 2
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                    onClicked:{
                        if( stackView.depth > 1 ) {
                            stackView.pop()
                        }
                    }
                }
                Item { // заполнение пространства
                    Layout.row: 3
                    Layout.fillHeight: true
                }
            }
        }

        Page {                     //страница с данными
            id: pageMain
            visible: false

            GridLayout {
                anchors.fill: parent

                RowLayout {
                    TextField {
                        id: edtChange
                        Layout.column: 0
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        rightPadding: height
                        Layout.preferredHeight: 20
                        Layout.fillWidth: true
                        background: Item {

                        }
                    }

                    TextField {                //поле поиска
                        id: edtSearch
                        Layout.column: 1
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter    //выравниваем TextField по верт в центре и горизонт влево
                        rightPadding: height                                //по ширине
                        Layout.fillWidth: true
                        placeholderText: "Поиск"

                        }
                    Button {
                        Layout.column: 2
                        anchors.right: parent.right
                        text: qsTr("Найти")
                        onClicked: {
                            finder()
                        }

                    }
                }


                ListView {
                    id: listView
                    Layout.preferredWidth: parent.width
                    Layout.fillHeight: true
                    Layout.row: 2
                    height: 200
                    spacing: 2
                    clip: true
                    delegate: Rectangle {
                        width: listView.width
                        height: 50
                        radius: 2                      //закругление
                        border.color: "light grey"
                        border.width: 0.5
                        RowLayout {
                            anchors.fill: parent
                            Label {
                                text: site
                                Layout.minimumWidth: 150
                                Layout.leftMargin: 40
                            }
                            TextField {
                                id: edtLogin
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 59
                                Layout.leftMargin: 80
                                Layout.column: 0
                                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                echoMode: TextField.Password
                                passwordCharacter: "✱"
                                text: login
                                readOnly: true
                                background: Item {

                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        getLogin(edtLogin.text, pin_code)
                                    }
                                }
                            }
                            TextField {
                                id: edtPassword
                                Layout.minimumWidth: 0
                                Layout.maximumWidth: 59
                                Layout.leftMargin: 80
                                Layout.column: 0
                                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                echoMode: TextField.Password
                                passwordCharacter: "✱"
                                text: password
                                readOnly: true
                                background: Item {

                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        // Копирует в буфер обмена расшифрованное значение пароля
                                        get_password(edtPassword.text, pin_code)
                                    }
                                }
                            }
                            Item {
                                Layout.fillWidth: true
                            }
                        }
                    }
                    model:ListModel {
                        id: listmodel1
                        // Items таблицы бд
                    }
                }
            }

        }

    }
}
