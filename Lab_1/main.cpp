#include <QApplication>             //интерфейс
#include <QQmlApplicationEngine>    // для движка QML
#include <QCryptographicHash>       //для вычисления криптографических хеш-сумм.
#include "cryptocontroller.h"
#include <QObject>                  //для объектов Qt
#include <QQmlContext>              //движок QML
#include <Windows.h>                //включает функции WinAPI
#include <QMessageBox>              //для работы с диалоговыми окнами
#include <QProcess>                 //для запуска внешних программ и связи с ними ("DebugProtector.exe")
#include <iostream> //#include <atlstr.h>               //удалить

typedef unsigned long long QWORD;



int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;                                               // создание объекта движка QML

    QByteArray data = "ff*rt8ferg__=-*&YT";
    data = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    qDebug() << data;

    //encrypt_login_or_password();
    //encrypt_db_file();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    cryptoController crp;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("cryptoController", &crp);


    //защита от отладки методом самоотладки
        QProcess *satelliteProcess = new QProcess();
        int pid = QApplication::applicationPid();                                         //идентификатор процесса приложения
        qDebug() << "pid = " << pid;
        QStringList arguments = {QString::number(pid)};
        qDebug() << "arguments = " << arguments;
        satelliteProcess->start("C:\\lab_1\\DebugProtector.exe", arguments);     //запускает процесс "DebugProtector.exe"
        bool ProtectorStarted = satelliteProcess->waitForStarted(1000);
        qDebug() << "ProtectorStarted = " << ProtectorStarted;

    // защита от отладки на основе использования функции API Windows IsDebuggerPresent()
    //    bool debuggerFound = IsDebuggerPresent();                    // IsDebuggerPresent() WinAPI
    //    if (debuggerFound) {
    //        qDebug() << "Debugger is found !!! Teminating the App !!!";
    //        std::cout << "Debugger is found !!! Teminating the App !!!" << std::endl;
    //        return -1;
    //    }

    //защита от модификации (патча)
    //     чтобы определить начальный адрес, с которого приложение размещается в виртуальной памяти,
    //     вызывается WinAPI-функция GetModuleHandle
        QWORD moduleBase = (QWORD)GetModuleHandle(NULL);
        QWORD text_segment_start = moduleBase + 0x1000; // сегмент .text располагается с отступом 0x1000
        qDebug() << "text_segment_start = " << Qt::hex << text_segment_start;

    //     Определить длину сегмента из заголовка    (IMAGE_DOS_HEADER и IMAGE_NT_HEADERS WinAPI)
        PIMAGE_DOS_HEADER pIHD = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
        PIMAGE_NT_HEADERS pINH = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + pIHD->e_lfanew);

        QWORD size_of_text = pINH->OptionalHeader.SizeOfCode;  // размер сегмента .text


        QByteArray text_segment_contents = QByteArray((char*)text_segment_start, size_of_text);// помещаем содержимое сегмента .text в массив char QByteArray
        QByteArray current_hash = QCryptographicHash::hash(text_segment_contents, QCryptographicHash::Sha256);//получаем хэш сегмента .text
        QByteArray current_hash_base64 = current_hash.toBase64();//переводим хэш в формат Base64, т.к. некоторые символы могут быть нечитыаемы в исходном хэше

        const QByteArray hash0_base64 = QByteArray("0R4qn9pm/A9wDo0z+8px+wJ46rTfLZI0txU7nLYKmIc=");//задаем константу, в которой будет храниться эталонный хэш

        //для проверки выводим все данные в отладку
        qDebug() << "text_segment_start = " << Qt::hex << text_segment_start;
        qDebug() << "size_of_text = " << size_of_text;
        qDebug() << "text_segment_contents = " << Qt::hex << text_segment_contents.first(100);
        qDebug() << "current_hash_base64 = " << current_hash_base64;

        bool checkresult = (current_hash_base64==hash0_base64);//проверка эталлоного хэша с фактическим
        qDebug() << "checkresult = " << checkresult;//вывод результата проверки
//это раскомментить для проверки
//        if (!checkresult){//если false, то вывод сообщения о модификации приложения
//            int result = QMessageBox::critical(nullptr, "Внимание!", "Приложение модифицировано");
//            return -1;
//        }

    //QMessageBox::critical(nullptr, "Внимание!", "Приложение модифицировано");

//    QObject *rootItem = engine.rootObjects()[0];
//    cryptoController crp;
//    QObject::connect(rootItem, SIGNAL(check_pwd_qml()), &crp, SLOT(check_password()));

        engine.load(url);


    return app.exec();
}


