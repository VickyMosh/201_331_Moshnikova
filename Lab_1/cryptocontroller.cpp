#include "cryptocontroller.h"
#include <QString>            //для работ со строками
#include <openssl/evp.h>      //для использования библиотек OpenSSL
#include <QFile>              //для работ с файлами
#include <QIODevice>          //устройство ввода/вывода
#include <QObject>            //для объектов Qt
#include <openssl/conf.h>     //OpenSSL
#include <QQmlContext>        //движок QML
#include <QDebug>             //for debugging
#include <QClipboard>         //для использования буфера обмена
#include <QGuiApplication>    //интерфейс
#include <QJsonDocument>      //для работы с Json
#include <QJsonObject>        //для создания JSON-объекта
#include <QJsonArray>         //для создания JSON-массива
#include <QByteArray>         //массив байтов
#include <QProcess>           //для запуска внешних программ и связи с ними
#include <Windows.h>          //включает функции WinAPI
#include <iostream>
#include <string.h>           //для работы со строками

using namespace std;

cryptoController::cryptoController(QObject *parent) : QObject(parent) //вызов конструктора
{

}

//функция выполняет операцию копирования расшифрованного текста в буфер обмена
//вход: текст; pin
void cryptoController::text_to_clipboard(QString text, QString pin) {
    text = decrypt_login_or_password(text, pin);//расшифровываем текст
    QClipboard *clipboard = QGuiApplication::clipboard();//инициализируем буфер
    QString originalText = clipboard->text();
    clipboard->setText(text);//передаем в буфер текст
}

// функция для шифрования паролей и логинов: на вход поступает пароль password (пин-код);
// out.txt - файл, в который записываем вручную текст до выполнения функции
// in.txt - файл, куда записываются зашифрованные данные после выполнения фукнции
void cryptoController::encrypt_login_or_password(QString password) {
    EVP_CIPHER_CTX *ctx;               //Объявляем переменную EVP_CIPHER_CTX
    if(!(ctx = EVP_CIPHER_CTX_new())){ // выделение памяти для ctx
        return;
    }

    //пароль расширяется до ключа из 32 символов                                32 байта = 256 бит
    QString key = password + ((QString)"0").repeated(32 - password.length());

    //инициализирующий вектор для шифра
    iv = (unsigned char*) key.data();

    //инициализация шифра
    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
    {
        return;
    }

    //для хранения зашифрованных и открытых данных
    unsigned char ciphertext[256] = {0};//зашифрованные данные
    unsigned char plaintext[256] = {0};//открытый текст
    int len = 0, plaintext_len = 256;//длина блока расшифрования

    //открываем файл, в который вручную записали текст
    QFile out("C:\\lab_1\\out.txt");
    bool is_opened = out.open(QIODevice::ReadOnly);
    if(!is_opened) {
        return;
    }

    //читаем файл порционно, по 256 символов, записываем текст в plaintext,
    //переменная plaintext_len хранит число полученных символов
    plaintext_len = out.read((char *)plaintext, 256);

    //Открываем файл для записи зашифрованных данных
    QFile in("C:\\lab_1\\in.txt");      //файл, куда записываются зашифрованные данные
    in.open(QIODevice::WriteOnly);

    QByteArray ciphertext_qbyte;  //промежуточное звено, которое будет хранить в себе зашифрованные данных

    //зашифрование текста
    while(plaintext_len > 0){
        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) {   //ф. EVP_EncryptUpdate для шифрования данных
            return;
        }

        ciphertext_qbyte = QByteArray(((char *) ciphertext), len);//преобразование из char в QByteArray
        in.write(ciphertext_qbyte.toBase64());//переводим в формат Base64 и записываем в файл
        plaintext_len = out.read((char *)plaintext, 256);//берем следующие 256 символов
    }

    //закрываем оба файла
    in.close();
    out.close();

    EVP_CIPHER_CTX_free(ctx);      //освобождение ресурсов, выделенных под шифрование

}

//функция, к которой мы обращаемся, когда нажимаем на логин или пароль для копирования в буфер обмена
//расшифровывает и возвращает логин или пароль
//вход: логин или пароль; ПИН
QString cryptoController::decrypt_login_or_password(QString text, QString password) {
    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){ //EVP_CIPHER_CTX_new() Allocates and returns a cipher context
        return "";
    }

    //пароль расширяется до ключа из 32 символов, заполняем нулями
    QString key = password + ((QString)"0").repeated(32 - password.length());

    //инициализирующий вектор для шифра
    iv = (unsigned char*) key.data();

    //инициализация шифра
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv)) //EVP_DecryptInit_ex для дешифровки
    {
        return "";
    }

    unsigned char deciphertext[256] = {0};//расшифрованный текст
    unsigned char ciphertext[256] = {0};//открытый текст
    int len = 0, ciphertext_len = 256;//длина блока расшифрования

    //переводим из формата Base64 полученную строку
    QByteArray line = QByteArray::fromBase64(text.toUtf8());
    int s = line.size();//получаем длину строки
    //если длина строки меньше 256 символов, то присваеваем длине блока длину строки
    if (s < 256)
        ciphertext_len = s;
    memcpy(ciphertext, line.toStdString().c_str(), ciphertext_len); //записываем в plaintext строку


    //QFile file("C:\\lab_1\\out.txt");
    //file.open(QIODevice::WriteOnly);
    QByteArray deciphertext_qbyte;//переменная, которая будет хранить в себе расшифрованный текст в формате QByteArray

    //расшифрование текста
    while(s > 0){
        if(1 != EVP_DecryptUpdate(ctx, deciphertext, &len, ciphertext, ciphertext_len))
        {
            return "";
        }

        //перевод из char в QByteArray
        deciphertext_qbyte = QByteArray(((char *) deciphertext), len);
        //file.write(ciphertext_qbyte);

        line.remove(0, 256);       //порция данных размером 256 байт. Затем удаляются первые 256 байт, чтобы считать следующие 256 байт
        s-=256;
        if (s<0)
            break;
        if (s<256)
            ciphertext_len=s;
        memcpy(ciphertext, line.toStdString().c_str(), ciphertext_len);
    }

    //file.close();



    EVP_CIPHER_CTX_free(ctx);

    return deciphertext_qbyte;   //расшифрованные данные
}

//функция проверяет введенный в первое окно пин-код, соответствует ли сохраненному зашифрованному паролю
//ПИН: 1327
//вход: ПИН
bool cryptoController::check_password(QString password) {
    //encrypt_db_file("1327");                          //закомментировать, когда наполнишь
    //encrypt_login_or_password("1327");                //закомментировать, когда наполнишь
    EVP_CIPHER_CTX *ctx;
    if(!(ctx = EVP_CIPHER_CTX_new())){
        return 0;
    }

    QString key = password + ((QString)"0").repeated(32 - password.length());

    iv = (unsigned char*) key.data();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
    {
        return false;
    }

    unsigned char ciphertext[256] = {0};
    unsigned char plaintext[256] = {0};
    int len = 0, plaintext_len = password.length();

    memcpy(plaintext, password.toStdString().c_str(), password.size());


    //введенный ПИН зашифровывается
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    {
        return false;
    }

    QString password_cipher = (char *)ciphertext;
    QByteArray password_cipher_qbyte = (char*) ciphertext;

    char buffer[256] = {0};
    QFile source_file("C:\\lab_1\\password_crypt.txt");
    bool is_opened = source_file.open(QIODevice::ReadOnly);
    //source_file.write(password_cipher_qbyte.toBase64());
    source_file.read(buffer, 256);


    source_file.close();
    EVP_CIPHER_CTX_free(ctx);

    //возвращается значение считанного и зашифрованного ПИНа с клавиатуры с уже хранящимся зашифрованным ПИНом в .txt файле
    return QString(buffer) == password_cipher_qbyte.toBase64();
}

//расшифрование db_crypt.json файла
void cryptoController::decrypt_db_file(QString password, int record_id, int is_password)
{
        EVP_CIPHER_CTX *ctx;
        if(!(ctx = EVP_CIPHER_CTX_new())){
            return;
        }

        //пароль расширяется до ключа из 32 символов
        QString key = password + ((QString)"0").repeated(32 - password.length());

        //инициализируем вектор
        iv = (unsigned char*) key.data();

        //инициализация шифра
        if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
        {
            return;
        }

        unsigned char deciphertext[256] = {0};//расшифрованный текст
        unsigned char ciphertext[256] = {0};//зашифрованный текст
        int len = 0, ciphertext_len = 0;//длина блока расшифрования
        QString db_decrypted = "";//расшифрованный текст, передающийся на форму

        QFile source_file("C:\\lab_1\\db_crypt.json");
        bool is_opened = source_file.open(QIODevice::ReadOnly);
        if(!is_opened) {
            return;
        }

        //читаем файл порционно, по 256 символов, записываем текст в plaintext,
        //переменная ciphertext_len хранит число полученных символов
        ciphertext_len = source_file.read((char *)ciphertext, 256);

        //расшифрование текста
        while(ciphertext_len > 0){
            if(1 != EVP_DecryptUpdate(ctx, deciphertext, &len, ciphertext, ciphertext_len))
            {
                return;
            }

            QByteArray deciphertext_qbyte = QByteArray(((char *) deciphertext), len);
            db_decrypted += deciphertext_qbyte;

            ciphertext_len = source_file.read((char *)ciphertext, 256);
        }

        EVP_CIPHER_CTX_free(ctx);

        if(is_password == -1)
            emit sendDbToQml(db_decrypted);
}


void cryptoController::encrypt_db_file(QString password, int record_id, int is_password)
{

    EVP_CIPHER_CTX *ctx;
        if(!(ctx = EVP_CIPHER_CTX_new())){ //EVP_CIPHER_CTX_new() creates a cipher context.
            return;
        }

        //пароль расширяется до ключа из 32 символов
        QString key = password + ((QString)"0").repeated(32 - password.length());

        //инциализируем вектор
        iv = (unsigned char*) key.data();

        //зашифрование данных
        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, (unsigned char*) key.data(), iv))
        {
            return;
        }

        unsigned char ciphertext[256] = {0};//зашифрованный текст
        unsigned char plaintext[256] = {0};//открытый текст
        int len = 0, plaintext_len = 0;//длина блока зашифрования

        QFile source_file("C:\\lab_1\\db.json");
        bool is_opened = source_file.open(QIODevice::ReadOnly);
        if(!is_opened) {
            return;
        }

        //читаем файл порционно, по 256 символов, записываем текст в plaintext,
        //переменная plaintext_len хранит число полученных символов
        plaintext_len = source_file.read((char *)plaintext, 256);

        QFile file("C:\\lab_1\\db_crypt.json");
        file.open(QIODevice::WriteOnly);

        //зашифрование данных
        while(plaintext_len > 0){
            if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            {
                return;
            }

            QByteArray ciphertext_qbyte = QByteArray(((char *) ciphertext), len);

            file.write((char *)ciphertext, len);
            plaintext_len = source_file.read((char *)plaintext, 256);
        }


        EVP_CIPHER_CTX_free(ctx);
}

