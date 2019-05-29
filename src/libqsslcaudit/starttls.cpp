
#include "starttls.h"
#include "debug.h"

#ifdef UNSAFE_QSSL
#include "sslunsafesocket.h"
#else
#include <QSslSocket>
#endif


void handleStartTlsFtp(XSslSocket *const socket)
{
    int attempt = 0;
    QByteArray readData;

    WHITE("initiating FTP STARTTLS sequence");

    socket->write("220 ready.\r\n");

    while (attempt < 16) {
        socket->waitForReadyRead(5000);
        readData = socket->readAll();
        if (readData == QByteArray("FEAT\r\n")) {
            socket->write("211-Features supported:\r\n");
            socket->write("AUTH TLS\r\n");
            socket->write("211 End FEAT.\r\n");
        } else if (readData == QByteArray("AUTH TLS\r\n")) {
            socket->write("234 AUTH TLS successful.\r\n");
            break;
        } else {
            attempt++;
        }
    }

    if (attempt >= 16) {
        RED("unexpected STARTTLS sequence");
    } else {
        WHITE("FTP STARTTLS sequence completed");
    }
}

void handleStartTlsSmtp(XSslSocket *const socket)
{
    // copy-pasted from https://en.wikipedia.org/wiki/Opportunistic_TLS
    int attempt = 0;
    QByteArray readData;

    WHITE("initiating SMTP STARTTLS sequence");

    socket->write("220 mail.example.org ESMTP service ready\r\n");

    while (attempt < 16) {
        socket->waitForReadyRead(5000);
        readData = socket->readAll();
        if (readData.startsWith(QByteArray("EHLO "))) {
            socket->write("250-mail.example.org offers a warm hug of welcome\r\n");
            socket->write("250 STARTTLS\r\n");
        } else if (readData == QByteArray("STARTTLS\r\n")) {
            socket->write("220 Go ahead\r\n");
            break;
        } else {
            attempt++;
        }
    }

    if (attempt >= 16) {
        RED("unexpected STARTTLS sequence");
    } else {
        WHITE("SMTP STARTTLS sequence completed");
    }
}

void handleStartTlsXmpp(XSslSocket *const socket)
{
    QByteArray readData;

    WHITE("initiating XMPP STARTTLS sequence");

    socket->waitForReadyRead(5000);
    readData = socket->readAll();

    if (!readData.startsWith(QByteArray("<stream:stream xmlns='jabber:client'"))) {
        RED("unexpected STARTTLS sequence");
        return;
    }

    socket->write("<stream:stream xmlns:stream='http://etherx.jabber.org/streams' xmlns='jabber:client' xml:lang='en' version='1.0'><stream:features><starttls xmlns='urn:ietf:params:xml:ns:xmpp-tls'><required/></starttls></stream:features>");
    socket->flush();

    socket->waitForReadyRead(5000);
    readData = socket->readAll();

    if (!readData.startsWith(QByteArray("<starttls xmlns='urn:ietf:params:xml:ns:xmpp-tls'>"))) {
        RED("unexpected STARTTLS sequence");
        return;
    }

    socket->write("<proceed xmlns='urn:ietf:params:xml:ns:xmpp-tls'/>");
    socket->flush();

    WHITE("FTP STARTTLS sequence completed");
}
