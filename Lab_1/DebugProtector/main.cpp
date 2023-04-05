#include <Windows.h>
#include <QDebug>
#include <QCoreApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    //запуск основного приложения
    QCoreApplication app(argc, argv);

    QProcess *process = new QProcess();
    QString program = "C:\\build-lab_1-Desktop_Qt_6_4_2_MSVC2019_64bit-Debug\\debug\\lab_1.exe";
    process->start(program);

    // Получить из аргументов PID процесса, который надо защищать
    DWORD pid = (DWORD) atoi(argv[1]);

    bool isAttached = DebugActiveProcess(pid); // Подключаемся к нему как отладчик

    if(!isAttached) { // Проверяем, удалось ли подключиться, обработка возможных ошибок
        DWORD lastError = GetLastError();
        qDebug() << "GetLastError()" << lastError;
        return lastError;
    }
       //пропускаем поступающие сигналы
    DEBUG_EVENT DbgEvent;

    while(1) { //
        WaitForDebugEvent(&DbgEvent, INFINITE);                                       //выгрузка очередного сообщения о сессии отладки
        ContinueDebugEvent(DbgEvent.dwProcessId, DbgEvent.dwThreadId, DBG_CONTINUE); //продолжение работы отлаж приложения
    }

    return app.exec();
}
