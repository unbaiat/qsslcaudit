#include "test.h"
#include "ssltests.h"
#include "ciphers.h"

#include <QCoreApplication>

#ifdef UNSAFE_QSSL
#include "sslunsafesocket.h"
#else
#include <QSslSocket>
#endif

// Target SslTest is SslTestCiphersTls10Med:
// "test for TLS 1.0 protocol and MEDIUM grade ciphers support"


// do verify peer certificate, use TLSv1.1 and stronger protocols
// check for proper test result code
class Test01 : public Test
{
    Q_OBJECT
public:
    Test01(int id, QString testBaseName, QList<SslTest *> sslTests) : Test(id, testBaseName, sslTests) {
        socket = nullptr;
    }

    ~Test01() {
        delete socket;
    }

    void setTestsSettings()
    {
        testSettings.setUserCN("www.example.com");
    }

    void executeNextSslTest()
    {
        if (!socket) {
            socket = new XSslSocket;

            socket->setPeerVerifyMode(XSslSocket::VerifyPeer);
            socket->setProtocol(XSsl::TlsV1_1OrLater);

            connect(socket, &XSslSocket::encrypted, [=]() {
                printTestFailed("encrypted session was established, but should not");
            });
        }

        socket->connectToHostEncrypted("localhost", 8443);
    }

    void verifySslTestResult()
    {
        if (currentSslTest()->result() == SslTestResult::Success) {
            setResult(0);
            printTestSucceeded();
        } else {
            setResult(-1);
            printTestFailed(QString("unexpected test result (%1)")
                            .arg(sslTestResultToString(currentSslTest()->result())));
        }
    }

private:
    XSslSocket *socket;

};

// do verify peer certificate, use TlsV1_0 protocol with medium ciphers
// check for proper test result code
class Test02 : public Test
{
    Q_OBJECT
public:
    Test02(int id, QString testBaseName, QList<SslTest *> sslTests) : Test(id, testBaseName, sslTests) {
        socket = nullptr;
    }

    ~Test02() {
        delete socket;
    }

    void setTestsSettings()
    {
        testSettings.setUserCN("www.example.com");
    }

    void executeNextSslTest()
    {
        if (!socket) {
            socket = new XSslSocket;

            socket->setPeerVerifyMode(XSslSocket::VerifyPeer);
            socket->setProtocol(XSsl::TlsV1_0);
            QList<XSslCipher> mediumCiphers;
            QStringList opensslCiphers = ciphers_medium_str.split(":");

            for (int i = 0; i < opensslCiphers.size(); i++) {
                XSslCipher cipher = XSslCipher(opensslCiphers.at(i));

                if (!cipher.isNull())
                    mediumCiphers << cipher;
            }
            if (mediumCiphers.size() == 0) {
                setResult(-1);
                printTestFailed();
                QThread::currentThread()->quit();
                return;
            }
            socket->setCiphers(mediumCiphers);

            connect(socket, &XSslSocket::encrypted, [=]() {
                printTestFailed("encrypted session was established, but should not");
            });
        }

        socket->connectToHostEncrypted("localhost", 8443);
    }

    void verifySslTestResult()
    {
        if (currentSslTest()->result() == SslTestResult::ProtoAccepted) {
            setResult(0);
            printTestSucceeded();
        } else {
            setResult(-1);
            printTestFailed(QString("unexpected test result (%1)")
                            .arg(sslTestResultToString(currentSslTest()->result())));
        }
    }

private:
    XSslSocket *socket;

};

// do verify peer certificate, use TlsV1_0 protocol with high ciphers
// check for proper test result code
class Test03 : public Test
{
    Q_OBJECT
public:
    Test03(int id, QString testBaseName, QList<SslTest *> sslTests) : Test(id, testBaseName, sslTests) {
        socket = nullptr;
    }

    ~Test03() {
        delete socket;
    }

    void setTestsSettings()
    {
        testSettings.setUserCN("www.example.com");
    }

    void executeNextSslTest()
    {
        if (!socket) {
            socket = new XSslSocket;

            socket->setPeerVerifyMode(XSslSocket::VerifyPeer);
            socket->setProtocol(XSsl::TlsV1_0);
            QList<XSslCipher> highCiphers;
            QStringList opensslCiphers = ciphers_high_str.split(":");

            for (int i = 0; i < opensslCiphers.size(); i++) {
                XSslCipher cipher = XSslCipher(opensslCiphers.at(i));

                if (!cipher.isNull())
                    highCiphers << cipher;
            }
            if (highCiphers.size() == 0) {
                setResult(-1);
                printTestFailed();
                QThread::currentThread()->quit();
                return;
            }
            socket->setCiphers(highCiphers);

            connect(socket, &XSslSocket::encrypted, [=]() {
                printTestFailed("encrypted session was established, but should not");
            });
        }

        socket->connectToHostEncrypted("localhost", 8443);
    }

    void verifySslTestResult()
    {
        if (currentSslTest()->result() == SslTestResult::Success) {
            setResult(0);
            printTestSucceeded();
        } else {
            setResult(-1);
            printTestFailed(QString("unexpected test result (%1)")
                            .arg(sslTestResultToString(currentSslTest()->result())));
        }
    }

private:
    XSslSocket *socket;

};

// do not verify peer certificate, use TlsV1_0 protocol with medium ciphers
// check for proper test result code
class Test04 : public Test
{
    Q_OBJECT
public:
    Test04(int id, QString testBaseName, QList<SslTest *> sslTests) : Test(id, testBaseName, sslTests) {
        socket = nullptr;
    }

    ~Test04() {
        delete socket;
    }

    void setTestsSettings()
    {
        testSettings.setUserCN("www.example.com");
    }

    void executeNextSslTest()
    {
        if (!socket) {
            socket = new XSslSocket;

            socket->setPeerVerifyMode(XSslSocket::VerifyNone);
            socket->setProtocol(XSsl::TlsV1_0);
            QList<XSslCipher> mediumCiphers;
            QStringList opensslCiphers = ciphers_medium_str.split(":");

            for (int i = 0; i < opensslCiphers.size(); i++) {
                XSslCipher cipher = XSslCipher(opensslCiphers.at(i));

                if (!cipher.isNull())
                    mediumCiphers << cipher;
            }
            if (mediumCiphers.size() == 0) {
                setResult(-1);
                printTestFailed();
                QThread::currentThread()->quit();
                return;
            }
            socket->setCiphers(mediumCiphers);

            connect(socket, &XSslSocket::encrypted, [=]() {
            });
        }

        socket->connectToHostEncrypted("localhost", 8443);
    }

    void verifySslTestResult()
    {
        if ((currentSslTest()->result() == SslTestResult::ProtoAccepted)
                || (currentSslTest()->result() == SslTestResult::CertAccepted)) {
            setResult(0);
            printTestSucceeded();
        } else {
            setResult(-1);
            printTestFailed(QString("unexpected test result (%1)")
                            .arg(sslTestResultToString(currentSslTest()->result())));
        }
    }

private:
    XSslSocket *socket;

};

// do verify peer certificate, use TlsV1_2 protocol with medium ciphers
// check for proper test result code
class Test05 : public Test
{
    Q_OBJECT
public:
    Test05(int id, QString testBaseName, QList<SslTest *> sslTests) : Test(id, testBaseName, sslTests) {
        socket = nullptr;
    }

    ~Test05() {
        delete socket;
    }

    void setTestsSettings()
    {
        testSettings.setUserCN("www.example.com");
    }

    void executeNextSslTest()
    {
        if (!socket) {
            socket = new XSslSocket;

            socket->setPeerVerifyMode(XSslSocket::VerifyNone);
            socket->setProtocol(XSsl::TlsV1_2);
            QList<XSslCipher> mediumCiphers;
            QStringList opensslCiphers = ciphers_medium_str.split(":");
            opensslCiphers.append(ciphers_low_str.split(":"));
            opensslCiphers.append(ciphers_export_str.split(":"));
            opensslCiphers.append(ciphers_high_str.split(":"));

            for (int i = 0; i < opensslCiphers.size(); i++) {
                XSslCipher cipher = XSslCipher(opensslCiphers.at(i));

                if (!cipher.isNull())
                    mediumCiphers << cipher;
            }
            if (mediumCiphers.size() == 0) {
                setResult(-1);
                printTestFailed();
                QThread::currentThread()->quit();
                return;
            }
            socket->setCiphers(mediumCiphers);

            connect(socket, &XSslSocket::encrypted, [=]() {
                printTestFailed("encrypted session was established, but should not");
            });
        }

        socket->connectToHostEncrypted("localhost", 8443);
    }

    void verifySslTestResult()
    {
        if (currentSslTest()->result() == SslTestResult::Success) {
            setResult(0);
            printTestSucceeded();
        } else {
            setResult(-1);
            printTestFailed(QString("unexpected test result (%1)")
                            .arg(sslTestResultToString(currentSslTest()->result())));
        }
    }

private:
    XSslSocket *socket;

};


QList<Test *> createAutotests()
{
    return QList<Test *>()
            << new Test01(1, "SslTestCiphersTls10Med", QList<SslTest *>() << new SslTestCiphersTls10Med)
            << new Test02(2, "SslTestCiphersTls10Med", QList<SslTest *>() << new SslTestCiphersTls10Med)
            << new Test03(3, "SslTestCiphersTls10Med", QList<SslTest *>() << new SslTestCiphersTls10Med)
            << new Test04(4, "SslTestCiphersTls10Med", QList<SslTest *>() << new SslTestCiphersTls10Med)
            << new Test05(5, "SslTestCiphersTls10Med", QList<SslTest *>() << new SslTestCiphersTls10Med)
               ;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    TestsLauncher *testsLauncher;

    testsLauncher = new TestsLauncher(createAutotests());

    QObject::connect(testsLauncher, &TestsLauncher::autotestsFinished, [=](){
        qApp->exit(testsLauncher->testsResult());
    });

    testsLauncher->launchNextTest();

    return a.exec();
}

#include "tests_SslTestCiphersTls10Med.moc"
