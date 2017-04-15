//#include <QLibrary>
#include <QApplication>

#include "fontconvert.h"
#include <QTextCodec>

#include <QtDebug>
#include <QFile>
#include <QTextStream>

void customMessageHandler(QtMsgType type, const char *msg)
{
        QString txt;
        switch (type) {
        //调试信息提示
        case QtDebugMsg:
                txt = QString("Debug: %1").arg(msg);
                break;

        //一般的warning提示
        case QtWarningMsg:
                txt = QString("Warning: %1").arg(msg);
        break;
        //严重错误提示
        case QtCriticalMsg:
                txt = QString("Critical: %1").arg(msg);
        break;
        //致命错误提示
        case QtFatalMsg:
                txt = QString("Fatal: %1").arg(msg);
                abort();
        }

        QFile outFile("debuglog.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << txt << endl;
}

int main(int argc, char *argv[])
{
    //QLibrary *mylib = NULL;
    //mylib = new QLibrary("./libfreetype.so");
    //mylib->load();
    //if(!mylib->isLoaded()){
    //    qDebug()<<QString::fromUtf8("failed!\n");
    //    return 0;
    //}

    QApplication app(argc,argv);

    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);

    //先注册自己的MsgHandler
    //qInstallMsgHandler(customMessageHandler);

    FontConvert *myform = new FontConvert;
    myform->show();
	return app.exec();	
}
