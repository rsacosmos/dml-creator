/*
    This file is part of DML Creator SDK.

    DML Creator SDK is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DML Creator SDK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with DML Creator SDK.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QDebug>
#include "MainWindow.h"

void formatMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "--:%s:%u: %s\n", context.file, context.line, localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "WW:%s:%u: %s\n", context.file, context.line, localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "EE:%s:%u: %s\n", context.file, context.line, localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "XX:%s:%u: %s\n", context.file, context.line, localMsg.constData());
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(formatMessageOutput);

    QApplication app(argc, argv);

    app.setOrganizationName("AFDI");
    app.setOrganizationDomain("domestandards.org");
    app.setApplicationName("DML Creator");
    app.setApplicationVersion("0.1");

    MainWindow window;
    window.show();

    return app.exec();
}
