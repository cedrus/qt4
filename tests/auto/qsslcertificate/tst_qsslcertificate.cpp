/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest/QtTest>
#include <qsslcertificate.h>
#include <qsslkey.h>
#include <qsslsocket.h>

#ifdef Q_OS_SYMBIAN
// In Symbian OS test data is located in applications private dir
// Current path (C:\private\<UID>) contains only ascii chars
#define SRCDIR "./"
#endif

class tst_QSslCertificate : public QObject
{
    Q_OBJECT

    struct CertInfo {
        QFileInfo fileInfo;
        QFileInfo fileInfo_digest_md5;
        QFileInfo fileInfo_digest_sha1;
        QSsl::EncodingFormat format;
        CertInfo(const QFileInfo &fileInfo, QSsl::EncodingFormat format)
            : fileInfo(fileInfo), format(format) {}
    };

    QList<CertInfo> certInfoList;
    QMap<QString, QString> subjAltNameMap;
    QMap<QString, QString> pubkeyMap;
    QMap<QString, QString> md5Map;
    QMap<QString, QString> sha1Map;

    void createTestRows();
#ifndef QT_NO_OPENSSL
    void compareCertificates(const QSslCertificate & cert1, const QSslCertificate & cert2);
#endif

    QString oldCurrentDir;
public:
    tst_QSslCertificate();
    virtual ~tst_QSslCertificate();

public slots:
    void initTestCase_data();
    void init();
    void cleanup();

#ifndef QT_NO_OPENSSL
private slots:
    void emptyConstructor();
    void constructor_data();
    void constructor();
    void constructingGarbage();
    void copyAndAssign_data();
    void copyAndAssign();
    void digest_data();
    void digest();
    void alternateSubjectNames_data();
    void alternateSubjectNames();
    void publicKey_data();
    void publicKey();
    void toPemOrDer_data();
    void toPemOrDer();
    void fromDevice();
    void fromPath_data();
    void fromPath();
    void certInfo();
    void certInfoQByteArray();
    void task256066toPem();
    void nulInCN();
    void nulInSan();
    void largeSerialNumber();
// ### add tests for certificate bundles (multiple certificates concatenated into a single
//     structure); both PEM and DER formatted
#endif
};

tst_QSslCertificate::tst_QSslCertificate()
{
    QDir dir(SRCDIR + QLatin1String("/certificates"));
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Readable);
    QRegExp rxCert(QLatin1String("^.+\\.(pem|der)$"));
    QRegExp rxSan(QLatin1String("^(.+\\.(?:pem|der))\\.san$"));
    QRegExp rxPubKey(QLatin1String("^(.+\\.(?:pem|der))\\.pubkey$"));
    QRegExp rxDigest(QLatin1String("^(.+\\.(?:pem|der))\\.digest-(md5|sha1)$"));
    foreach (QFileInfo fileInfo, fileInfoList) {
        if (rxCert.indexIn(fileInfo.fileName()) >= 0)
            certInfoList <<
                CertInfo(fileInfo,
                         rxCert.cap(1) == QLatin1String("pem") ? QSsl::Pem : QSsl::Der);
        if (rxSan.indexIn(fileInfo.fileName()) >= 0)
            subjAltNameMap.insert(rxSan.cap(1), fileInfo.absoluteFilePath());
        if (rxPubKey.indexIn(fileInfo.fileName()) >= 0)
            pubkeyMap.insert(rxPubKey.cap(1), fileInfo.absoluteFilePath());
        if (rxDigest.indexIn(fileInfo.fileName()) >= 0) {
            if (rxDigest.cap(2) == QLatin1String("md5"))
                md5Map.insert(rxDigest.cap(1), fileInfo.absoluteFilePath());
            else
                sha1Map.insert(rxDigest.cap(1), fileInfo.absoluteFilePath());
        }
    }
}

tst_QSslCertificate::~tst_QSslCertificate()
{
}

void tst_QSslCertificate::initTestCase_data()
{
}

void tst_QSslCertificate::init()
{
    QString srcdir(QLatin1String(SRCDIR));
    if (!srcdir.isEmpty()) {
        oldCurrentDir = QDir::current().absolutePath();
        QDir::setCurrent(srcdir);
    }
}

void tst_QSslCertificate::cleanup()
{
    if (!oldCurrentDir.isEmpty()) {
        QDir::setCurrent(oldCurrentDir);
    }

}

static QByteArray readFile(const QString &absFilePath)
{
    QFile file(absFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QWARN("failed to open file");
        return QByteArray();
    }
    return file.readAll();
}

#ifndef QT_NO_OPENSSL

void tst_QSslCertificate::emptyConstructor()
{
    if (!QSslSocket::supportsSsl())
        return;

    QSslCertificate certificate;
    QVERIFY(certificate.isNull());
    //make sure none of the functions crash  (task 203035)
    QVERIFY(!certificate.isValid());
    QCOMPARE(certificate.version() , QByteArray());
    QCOMPARE(certificate.serialNumber(), QByteArray());
    QCOMPARE(certificate.digest(), QCryptographicHash::hash(QByteArray(), QCryptographicHash::Md5));
    QCOMPARE(certificate.issuerInfo(QSslCertificate::Organization), QString());
    QCOMPARE(certificate.subjectInfo(QSslCertificate::Organization), QString());
    QCOMPARE(certificate.alternateSubjectNames(),(QMultiMap<QSsl::AlternateNameEntryType, QString>()));
#ifndef QT_NO_TEXTSTREAM
    QCOMPARE(certificate.effectiveDate(), QDateTime());
    QCOMPARE(certificate.expiryDate(), QDateTime());
#endif
}

Q_DECLARE_METATYPE(QSsl::EncodingFormat);

void tst_QSslCertificate::createTestRows()
{
    QTest::addColumn<QString>("absFilePath");
    QTest::addColumn<QSsl::EncodingFormat>("format");
    foreach (CertInfo certInfo, certInfoList) {
        QTest::newRow(certInfo.fileInfo.fileName().toLatin1())
            << certInfo.fileInfo.absoluteFilePath() << certInfo.format;
    }
}

void tst_QSslCertificate::constructor_data()
{
    createTestRows();
}

void tst_QSslCertificate::constructor()
{
    if (!QSslSocket::supportsSsl())
        return;

    QFETCH(QString, absFilePath);
    QFETCH(QSsl::EncodingFormat, format);

    QByteArray encoded = readFile(absFilePath);
    QSslCertificate certificate(encoded, format);
    QVERIFY(!certificate.isNull());
}

void tst_QSslCertificate::constructingGarbage()
{
    if (!QSslSocket::supportsSsl())
        return;

    QByteArray garbage("garbage");
    QSslCertificate certificate(garbage);
    QVERIFY(certificate.isNull());
}

void tst_QSslCertificate::copyAndAssign_data()
{
    createTestRows();
}

void tst_QSslCertificate::compareCertificates(
    const QSslCertificate & cert1, const QSslCertificate & cert2)
{
    QCOMPARE(cert1.isNull(), cert2.isNull());
    // Note: in theory, the next line could fail even if the certificates are identical!
    QCOMPARE(cert1.isValid(), cert2.isValid());
    QCOMPARE(cert1.version(), cert2.version());
    QCOMPARE(cert1.serialNumber(), cert2.serialNumber());
    QCOMPARE(cert1.digest(), cert2.digest());
    QCOMPARE(cert1.toPem(), cert2.toPem());
    QCOMPARE(cert1.toDer(), cert2.toDer());
    for (int info = QSslCertificate::Organization;
         info <= QSslCertificate::StateOrProvinceName; info++) {
        const QSslCertificate::SubjectInfo subjectInfo = (QSslCertificate::SubjectInfo)info;
        QCOMPARE(cert1.issuerInfo(subjectInfo), cert2.issuerInfo(subjectInfo));
        QCOMPARE(cert1.subjectInfo(subjectInfo), cert2.subjectInfo(subjectInfo));
    }
    QCOMPARE(cert1.alternateSubjectNames(), cert2.alternateSubjectNames());
    QCOMPARE(cert1.effectiveDate(), cert2.effectiveDate());
    QCOMPARE(cert1.expiryDate(), cert2.expiryDate());
    QCOMPARE(cert1.version(), cert2.version());
    QCOMPARE(cert1.serialNumber(), cert2.serialNumber());
    // ### add more functions here ...
}

void tst_QSslCertificate::copyAndAssign()
{
    if (!QSslSocket::supportsSsl())
        return;

    QFETCH(QString, absFilePath);
    QFETCH(QSsl::EncodingFormat, format);

    QByteArray encoded = readFile(absFilePath);
    QSslCertificate certificate(encoded, format);

    QVERIFY(!certificate.isNull());

    QSslCertificate copied(certificate);
    compareCertificates(certificate, copied);

    QSslCertificate assigned = certificate;
    compareCertificates(certificate, assigned);
}

void tst_QSslCertificate::digest_data()
{
    QTest::addColumn<QString>("absFilePath");
    QTest::addColumn<QSsl::EncodingFormat>("format");
    QTest::addColumn<QString>("absFilePath_digest_md5");
    QTest::addColumn<QString>("absFilePath_digest_sha1");
    foreach (CertInfo certInfo, certInfoList) {
        QString certName = certInfo.fileInfo.fileName();
        QTest::newRow(certName.toLatin1())
            << certInfo.fileInfo.absoluteFilePath()
            << certInfo.format
            << md5Map.value(certName)
            << sha1Map.value(certName);
    }
}

// Converts a digest of the form '{MD5|SHA1} Fingerprint=AB:B8:32...' to binary format.
static QByteArray convertDigest(const QByteArray &input)
{
    QByteArray result;
    QRegExp rx(QLatin1String("(?:=|:)([0-9A-Fa-f]{2})"));
    int pos = 0;
    while ((pos = rx.indexIn(input, pos)) != -1) {
        result.append(rx.cap(1).toLatin1());
        pos += rx.matchedLength();
    }
    return QByteArray::fromHex(result);
}

void tst_QSslCertificate::digest()
{
    if (!QSslSocket::supportsSsl())
        return;

    QFETCH(QString, absFilePath);
    QFETCH(QSsl::EncodingFormat, format);
    QFETCH(QString, absFilePath_digest_md5);
    QFETCH(QString, absFilePath_digest_sha1);

    QByteArray encoded = readFile(absFilePath);
    QSslCertificate certificate(encoded, format);
    QVERIFY(!certificate.isNull());

    if (!absFilePath_digest_md5.isEmpty())
        QCOMPARE(convertDigest(readFile(absFilePath_digest_md5)),
                 certificate.digest(QCryptographicHash::Md5));

    if (!absFilePath_digest_sha1.isEmpty())
        QCOMPARE(convertDigest(readFile(absFilePath_digest_sha1)),
                 certificate.digest(QCryptographicHash::Sha1));
}

void tst_QSslCertificate::alternateSubjectNames_data()
{
    QTest::addColumn<QString>("certFilePath");
    QTest::addColumn<QSsl::EncodingFormat>("format");
    QTest::addColumn<QString>("subjAltNameFilePath");

    foreach (CertInfo certInfo, certInfoList) {
        QString certName = certInfo.fileInfo.fileName();
        if (subjAltNameMap.contains(certName))
            QTest::newRow(certName.toLatin1())
                << certInfo.fileInfo.absoluteFilePath()
                << certInfo.format
                << subjAltNameMap.value(certName);
    }
}

void tst_QSslCertificate::alternateSubjectNames()
{
    if (!QSslSocket::supportsSsl())
        return;

    QFETCH(QString, certFilePath);
    QFETCH(QSsl::EncodingFormat, format);
    QFETCH(QString, subjAltNameFilePath);

    QByteArray encodedCert = readFile(certFilePath);
    QSslCertificate certificate(encodedCert, format);
    QVERIFY(!certificate.isNull());

    QByteArray fileContents = readFile(subjAltNameFilePath);

    const QMultiMap<QSsl::AlternateNameEntryType, QString> altSubjectNames =
        certificate.alternateSubjectNames();

    // verify that each entry in subjAltNames is present in fileContents
    QMapIterator<QSsl::AlternateNameEntryType, QString> it(altSubjectNames);
    while (it.hasNext()) {
        it.next();
        QString type;
        if (it.key() == QSsl::EmailEntry)
            type = QLatin1String("email");
        else if (it.key() == QSsl::DnsEntry)
            type = QLatin1String("DNS");
        else
            QFAIL("unsupported alternative name type");
        QString entry = QString("%1:%2").arg(type).arg(it.value());
        QVERIFY(fileContents.contains(entry.toAscii()));
    }

    // verify that each entry in fileContents is present in subjAltNames
    QRegExp rx(QLatin1String("(email|DNS):([^,\\r\\n]+)"));
    for (int pos = 0; (pos = rx.indexIn(fileContents, pos)) != -1; pos += rx.matchedLength()) {
        QSsl::AlternateNameEntryType key;
        if (rx.cap(1) == QLatin1String("email"))
            key = QSsl::EmailEntry;
        else if (rx.cap(1) == QLatin1String("DNS"))
            key = QSsl::DnsEntry;
        else
            QFAIL("unsupported alternative name type");
        QVERIFY(altSubjectNames.contains(key, rx.cap(2)));
    }
}

void tst_QSslCertificate::publicKey_data()
{
    QTest::addColumn<QString>("certFilePath");
    QTest::addColumn<QSsl::EncodingFormat>("format");
    QTest::addColumn<QString>("pubkeyFilePath");

    foreach (CertInfo certInfo, certInfoList) {
        QString certName = certInfo.fileInfo.fileName();
        if (pubkeyMap.contains(certName))
            QTest::newRow(certName.toLatin1())
                << certInfo.fileInfo.absoluteFilePath()
                << certInfo.format
                << pubkeyMap.value(certName);
    }
}

void tst_QSslCertificate::publicKey()
{
    if (!QSslSocket::supportsSsl())
        return;

    QFETCH(QString, certFilePath);
    QFETCH(QSsl::EncodingFormat, format);
    QFETCH(QString, pubkeyFilePath);

    QByteArray encodedCert = readFile(certFilePath);
    QSslCertificate certificate(encodedCert, format);
    QVERIFY(!certificate.isNull());

    QByteArray encodedPubkey = readFile(pubkeyFilePath);
    QSslKey pubkey(encodedPubkey, QSsl::Rsa, format, QSsl::PublicKey); // ### support DSA as well!
    QVERIFY(!pubkey.isNull());

    QCOMPARE(certificate.publicKey(), pubkey);
}

void tst_QSslCertificate::toPemOrDer_data()
{
    createTestRows();
}

static const char BeginCertString[] = "-----BEGIN CERTIFICATE-----";
static const char EndCertString[] = "-----END CERTIFICATE-----";

// Returns, in Pem-format, the first certificate found in a Pem-formatted block
// (Note that such a block may contain e.g. a private key at the end).
static QByteArray firstPemCertificateFromPem(const QByteArray &pem)
{
    int startPos = pem.indexOf(BeginCertString);
    int endPos = pem.indexOf(EndCertString);
    if (startPos == -1 || endPos == -1)
        return QByteArray();
    return pem.mid(startPos, endPos + sizeof(EndCertString) - startPos);
}

void tst_QSslCertificate::toPemOrDer()
{
    if (!QSslSocket::supportsSsl())
        return;

    QFETCH(QString, absFilePath);
    QFETCH(QSsl::EncodingFormat, format);

    QByteArray encoded = readFile(absFilePath);
    QSslCertificate certificate(encoded, format);
    QVERIFY(!certificate.isNull());
    if (format == QSsl::Pem) {
        encoded.replace('\r',"");
        QByteArray firstPem = firstPemCertificateFromPem(encoded);
        QCOMPARE(certificate.toPem(), firstPem);
    } else {
        // ### for now, we assume that DER-encoded certificates don't contain bundled stuff
        QCOMPARE(certificate.toDer(), encoded);
    }
}

void tst_QSslCertificate::fromDevice()
{
    QTest::ignoreMessage(QtWarningMsg, "QSslCertificate::fromDevice: cannot read from a null device");
    QList<QSslCertificate> certs = QSslCertificate::fromDevice(0); // don't crash
    QVERIFY(certs.isEmpty());
}

void tst_QSslCertificate::fromPath_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("syntax");
    QTest::addColumn<bool>("pemencoding");
    QTest::addColumn<int>("numCerts");

    QTest::newRow("empty fixed pem") << QString() << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("empty fixed der") << QString() << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("empty regexp pem") << QString() << int(QRegExp::RegExp) << true << 0;
    QTest::newRow("empty regexp der") << QString() << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("empty wildcard pem") << QString() << int(QRegExp::Wildcard) << true << 0;
    QTest::newRow("empty wildcard der") << QString() << int(QRegExp::Wildcard) << false << 0;
    QTest::newRow("\"certificates\" fixed pem") << QString("certificates") << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("\"certificates\" fixed der") << QString("certificates") << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("\"certificates\" regexp pem") << QString("certificates") << int(QRegExp::RegExp) << true << 0;
    QTest::newRow("\"certificates\" regexp der") << QString("certificates") << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("\"certificates\" wildcard pem") << QString("certificates") << int(QRegExp::Wildcard) << true << 0;
    QTest::newRow("\"certificates\" wildcard der") << QString("certificates") << int(QRegExp::Wildcard) << false << 0;
    QTest::newRow("\"certificates/cert.pem\" fixed pem") << QString("certificates/cert.pem") << int(QRegExp::FixedString) << true << 1;
    QTest::newRow("\"certificates/cert.pem\" fixed der") << QString("certificates/cert.pem") << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("\"certificates/cert.pem\" regexp pem") << QString("certificates/cert.pem") << int(QRegExp::RegExp) << true << 1;
    QTest::newRow("\"certificates/cert.pem\" regexp der") << QString("certificates/cert.pem") << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("\"certificates/cert.pem\" wildcard pem") << QString("certificates/cert.pem") << int(QRegExp::Wildcard) << true << 1;
    QTest::newRow("\"certificates/cert.pem\" wildcard der") << QString("certificates/cert.pem") << int(QRegExp::Wildcard) << false << 0;
    QTest::newRow("\"certificates/*\" fixed pem") << QString("certificates/*") << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("\"certificates/*\" fixed der") << QString("certificates/*") << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("\"certificates/*\" regexp pem") << QString("certificates/*") << int(QRegExp::RegExp) << true << 0;
    QTest::newRow("\"certificates/*\" regexp der") << QString("certificates/*") << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("\"certificates/*\" wildcard pem") << QString("certificates/*") << int(QRegExp::Wildcard) << true << 4;
    QTest::newRow("\"certificates/*\" wildcard der") << QString("certificates/*") << int(QRegExp::Wildcard) << false << 0;
    QTest::newRow("\"c*/c*.pem\" fixed pem") << QString("c*/c*.pem") << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("\"c*/c*.pem\" fixed der") << QString("c*/c*.pem") << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("\"c*/c*.pem\" regexp pem") << QString("c*/c*.pem") << int(QRegExp::RegExp) << true << 0;
    QTest::newRow("\"c*/c*.pem\" regexp der") << QString("c*/c*.pem") << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("\"c*/c*.pem\" wildcard pem") << QString("c*/c*.pem") << int(QRegExp::Wildcard) << true << 4;
    QTest::newRow("\"c*/c*.pem\" wildcard der") << QString("c*/c*.pem") << int(QRegExp::Wildcard) << false << 0;
    QTest::newRow("\"d*/c*.pem\" fixed pem") << QString("d*/c*.pem") << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("\"d*/c*.pem\" fixed der") << QString("d*/c*.pem") << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("\"d*/c*.pem\" regexp pem") << QString("d*/c*.pem") << int(QRegExp::RegExp) << true << 0;
    QTest::newRow("\"d*/c*.pem\" regexp der") << QString("d*/c*.pem") << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("\"d*/c*.pem\" wildcard pem") << QString("d*/c*.pem") << int(QRegExp::Wildcard) << true << 0;
    QTest::newRow("\"d*/c*.pem\" wildcard der") << QString("d*/c*.pem") << int(QRegExp::Wildcard) << false << 0;
    QTest::newRow("\"c.*/c.*.pem\" fixed pem") << QString("c.*/c.*.pem") << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("\"c.*/c.*.pem\" fixed der") << QString("c.*/c.*.pem") << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("\"c.*/c.*.pem\" regexp pem") << QString("c.*/c.*.pem") << int(QRegExp::RegExp) << true << 4;
    QTest::newRow("\"c.*/c.*.pem\" regexp der") << QString("c.*/c.*.pem") << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("\"c.*/c.*.pem\" wildcard pem") << QString("c.*/c.*.pem") << int(QRegExp::Wildcard) << true << 0;
    QTest::newRow("\"c.*/c.*.pem\" wildcard der") << QString("c.*/c.*.pem") << int(QRegExp::Wildcard) << false << 0;
    QTest::newRow("\"d.*/c.*.pem\" fixed pem") << QString("d.*/c.*.pem") << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("\"d.*/c.*.pem\" fixed der") << QString("d.*/c.*.pem") << int(QRegExp::FixedString) << false << 0;
    QTest::newRow("\"d.*/c.*.pem\" regexp pem") << QString("d.*/c.*.pem") << int(QRegExp::RegExp) << true << 0;
    QTest::newRow("\"d.*/c.*.pem\" regexp der") << QString("d.*/c.*.pem") << int(QRegExp::RegExp) << false << 0;
    QTest::newRow("\"d.*/c.*.pem\" wildcard pem") << QString("d.*/c.*.pem") << int(QRegExp::Wildcard) << true << 0;
    QTest::newRow("\"d.*/c.*.pem\" wildcard der") << QString("d.*/c.*.pem") << int(QRegExp::Wildcard) << false << 0;
#ifdef Q_OS_LINUX
    QTest::newRow("absolute path wildcard pem") << QString(QDir::currentPath() + "/certificates/*.pem") << int(QRegExp::Wildcard) << true << 4;
#endif

    QTest::newRow("trailing-whitespace") << QString("more-certificates/trailing-whitespace.pem") << int(QRegExp::FixedString) << true << 1;
    QTest::newRow("no-ending-newline") << QString("more-certificates/no-ending-newline.pem") << int(QRegExp::FixedString) << true << 1;
    QTest::newRow("malformed-just-begin") << QString("more-certificates/malformed-just-begin.pem") << int(QRegExp::FixedString) << true << 0;
    QTest::newRow("malformed-just-begin-no-newline") << QString("more-certificates/malformed-just-begin-no-newline.pem") << int(QRegExp::FixedString) << true << 0;
}

void tst_QSslCertificate::fromPath()
{
    QFETCH(QString, path);
    QFETCH(int, syntax);
    QFETCH(bool, pemencoding);
    QFETCH(int, numCerts);

    QCOMPARE(QSslCertificate::fromPath(path,
                                       pemencoding ? QSsl::Pem : QSsl::Der,
                                       QRegExp::PatternSyntax(syntax)).size(),
             numCerts);
}

void tst_QSslCertificate::certInfo()
{
// MD5 Fingerprint=B6:CF:57:34:DA:A9:73:21:82:F7:CF:4D:3D:85:31:88
// SHA1 Fingerprint=B6:D1:51:82:E0:29:CA:59:96:38:BD:B6:F9:40:05:91:6D:49:09:60
// Certificate:
//     Data:
//         Version: 1 (0x0)
//         Serial Number: 17 (0x11)
//         Signature Algorithm: sha1WithRSAEncryption
//         Issuer: C=AU, ST=Queensland, O=CryptSoft Pty Ltd, CN=Test CA (1024 bit)
//         Validity
//             Not Before: Apr 17 07:40:26 2007 GMT
//             Not After : May 17 07:40:26 2007 GMT
//         Subject: CN=name/with/slashes, C=NO
//         Subject Public Key Info:
//             Public Key Algorithm: rsaEncryption
//             RSA Public Key: (1024 bit)
//                 Modulus (1024 bit):
//                     00:eb:9d:e9:03:ac:30:4f:a9:58:03:44:c7:18:26:
//                     2f:48:93:d5:ac:a0:fb:e8:53:c4:7b:2a:01:89:e6:
//                     fc:5a:0c:c5:f5:21:f8:d7:4a:92:02:67:db:f1:9f:
//                     36:9a:62:9d:f3:ce:48:8e:ba:ed:5a:a8:9d:4f:bb:
//                     24:16:43:4c:b5:79:08:f6:d9:22:8f:5f:15:0a:43:
//                     25:03:7a:9d:a7:af:e3:26:b1:53:55:5e:60:57:c8:
//                     ed:2f:1c:f3:36:0a:78:64:91:f9:17:a7:34:d7:8b:
//                     bd:f1:fc:d1:8c:4f:a5:96:75:b2:7b:fc:21:f0:c7:
//                     d9:5f:0c:57:18:b2:af:b9:4b
//                 Exponent: 65537 (0x10001)
//     Signature Algorithm: sha1WithRSAEncryption
//         95:e6:94:e2:98:33:57:a2:98:fa:af:50:b9:76:a9:51:83:2c:
//         0b:61:a2:36:d0:e6:90:6d:e4:f8:c4:c7:50:ef:17:94:4e:21:
//         a8:fa:c8:33:aa:d1:7f:bc:ca:41:d6:7d:e7:44:76:c0:bf:45:
//         4a:76:25:42:6d:53:76:fd:fc:74:29:1a:ea:2b:cc:06:ab:d1:
//         b8:eb:7d:6b:11:f7:9b:41:bb:9f:31:cb:ed:4d:f3:68:26:ed:
//         13:1d:f2:56:59:fe:6f:7c:98:b6:25:69:4e:ea:b4:dc:c2:eb:
//         b7:bb:50:18:05:ba:ad:af:08:49:fe:98:63:55:ba:e7:fb:95:
//         5d:91
    static const char pem[] =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIB8zCCAVwCAREwDQYJKoZIhvcNAQEFBQAwWzELMAkGA1UEBhMCQVUxEzARBgNV\n"
        "BAgTClF1ZWVuc2xhbmQxGjAYBgNVBAoTEUNyeXB0U29mdCBQdHkgTHRkMRswGQYD\n"
        "VQQDExJUZXN0IENBICgxMDI0IGJpdCkwHhcNMDcwNDE3MDc0MDI2WhcNMDcwNTE3\n"
        "MDc0MDI2WjApMRowGAYDVQQDExFuYW1lL3dpdGgvc2xhc2hlczELMAkGA1UEBhMC\n"
        "Tk8wgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAOud6QOsME+pWANExxgmL0iT\n"
        "1ayg++hTxHsqAYnm/FoMxfUh+NdKkgJn2/GfNppinfPOSI667VqonU+7JBZDTLV5\n"
        "CPbZIo9fFQpDJQN6naev4yaxU1VeYFfI7S8c8zYKeGSR+RenNNeLvfH80YxPpZZ1\n"
        "snv8IfDH2V8MVxiyr7lLAgMBAAEwDQYJKoZIhvcNAQEFBQADgYEAleaU4pgzV6KY\n"
        "+q9QuXapUYMsC2GiNtDmkG3k+MTHUO8XlE4hqPrIM6rRf7zKQdZ950R2wL9FSnYl\n"
        "Qm1Tdv38dCka6ivMBqvRuOt9axH3m0G7nzHL7U3zaCbtEx3yVln+b3yYtiVpTuq0\n"
        "3MLrt7tQGAW6ra8ISf6YY1W65/uVXZE=\n"
        "-----END CERTIFICATE-----\n";
    static const char der[] =   // hex encoded
        "30:82:01:f3:30:82:01:5c:02:01:11:30:0d:06:09:2a"
        "86:48:86:f7:0d:01:01:05:05:00:30:5b:31:0b:30:09"
        "06:03:55:04:06:13:02:41:55:31:13:30:11:06:03:55"
        "04:08:13:0a:51:75:65:65:6e:73:6c:61:6e:64:31:1a"
        "30:18:06:03:55:04:0a:13:11:43:72:79:70:74:53:6f"
        "66:74:20:50:74:79:20:4c:74:64:31:1b:30:19:06:03"
        "55:04:03:13:12:54:65:73:74:20:43:41:20:28:31:30"
        "32:34:20:62:69:74:29:30:1e:17:0d:30:37:30:34:31"
        "37:30:37:34:30:32:36:5a:17:0d:30:37:30:35:31:37"
        "30:37:34:30:32:36:5a:30:29:31:1a:30:18:06:03:55"
        "04:03:13:11:6e:61:6d:65:2f:77:69:74:68:2f:73:6c"
        "61:73:68:65:73:31:0b:30:09:06:03:55:04:06:13:02"
        "4e:4f:30:81:9f:30:0d:06:09:2a:86:48:86:f7:0d:01"
        "01:01:05:00:03:81:8d:00:30:81:89:02:81:81:00:eb"
        "9d:e9:03:ac:30:4f:a9:58:03:44:c7:18:26:2f:48:93"
        "d5:ac:a0:fb:e8:53:c4:7b:2a:01:89:e6:fc:5a:0c:c5"
        "f5:21:f8:d7:4a:92:02:67:db:f1:9f:36:9a:62:9d:f3"
        "ce:48:8e:ba:ed:5a:a8:9d:4f:bb:24:16:43:4c:b5:79"
        "08:f6:d9:22:8f:5f:15:0a:43:25:03:7a:9d:a7:af:e3"
        "26:b1:53:55:5e:60:57:c8:ed:2f:1c:f3:36:0a:78:64"
        "91:f9:17:a7:34:d7:8b:bd:f1:fc:d1:8c:4f:a5:96:75"
        "b2:7b:fc:21:f0:c7:d9:5f:0c:57:18:b2:af:b9:4b:02"
        "03:01:00:01:30:0d:06:09:2a:86:48:86:f7:0d:01:01"
        "05:05:00:03:81:81:00:95:e6:94:e2:98:33:57:a2:98"
        "fa:af:50:b9:76:a9:51:83:2c:0b:61:a2:36:d0:e6:90"
        "6d:e4:f8:c4:c7:50:ef:17:94:4e:21:a8:fa:c8:33:aa"
        "d1:7f:bc:ca:41:d6:7d:e7:44:76:c0:bf:45:4a:76:25"
        "42:6d:53:76:fd:fc:74:29:1a:ea:2b:cc:06:ab:d1:b8"
        "eb:7d:6b:11:f7:9b:41:bb:9f:31:cb:ed:4d:f3:68:26"
        "ed:13:1d:f2:56:59:fe:6f:7c:98:b6:25:69:4e:ea:b4"
        "dc:c2:eb:b7:bb:50:18:05:ba:ad:af:08:49:fe:98:63"
        "55:ba:e7:fb:95:5d:91";

    QSslCertificate cert =  QSslCertificate::fromPath("certificates/cert.pem", QSsl::Pem,
                                                      QRegExp::FixedString).first();
    QVERIFY(!cert.isNull());

    QCOMPARE(cert.issuerInfo(QSslCertificate::Organization), QString("CryptSoft Pty Ltd"));
    QCOMPARE(cert.issuerInfo(QSslCertificate::CommonName), QString("Test CA (1024 bit)"));
    QCOMPARE(cert.issuerInfo(QSslCertificate::LocalityName), QString());
    QCOMPARE(cert.issuerInfo(QSslCertificate::OrganizationalUnitName), QString());
    QCOMPARE(cert.issuerInfo(QSslCertificate::CountryName), QString("AU"));
    QCOMPARE(cert.issuerInfo(QSslCertificate::StateOrProvinceName), QString("Queensland"));

    QCOMPARE(cert.issuerInfo("O"), QString("CryptSoft Pty Ltd"));
    QCOMPARE(cert.issuerInfo("CN"), QString("Test CA (1024 bit)"));
    QCOMPARE(cert.issuerInfo("L"), QString());
    QCOMPARE(cert.issuerInfo("OU"), QString());
    QCOMPARE(cert.issuerInfo("C"), QString("AU"));
    QCOMPARE(cert.issuerInfo("ST"), QString("Queensland"));

    QCOMPARE(cert.subjectInfo(QSslCertificate::Organization), QString());
    QCOMPARE(cert.subjectInfo(QSslCertificate::CommonName), QString("name/with/slashes"));
    QCOMPARE(cert.subjectInfo(QSslCertificate::LocalityName), QString());
    QCOMPARE(cert.subjectInfo(QSslCertificate::OrganizationalUnitName), QString());
    QCOMPARE(cert.subjectInfo(QSslCertificate::CountryName), QString("NO"));
    QCOMPARE(cert.subjectInfo(QSslCertificate::StateOrProvinceName), QString());

    QCOMPARE(cert.subjectInfo("O"), QString());
    QCOMPARE(cert.subjectInfo("CN"), QString("name/with/slashes"));
    QCOMPARE(cert.subjectInfo("L"), QString());
    QCOMPARE(cert.subjectInfo("OU"), QString());
    QCOMPARE(cert.subjectInfo("C"), QString("NO"));
    QCOMPARE(cert.subjectInfo("ST"), QString());

    QCOMPARE(cert.version(), QByteArray::number(1));
    QCOMPARE(cert.serialNumber(), QByteArray::number(17));

    QCOMPARE(cert.toPem().constData(), (const char*)pem);
    QCOMPARE(cert.toDer(), QByteArray::fromHex(der));

    QCOMPARE(cert.digest(QCryptographicHash::Md5),
             QByteArray::fromHex("B6:CF:57:34:DA:A9:73:21:82:F7:CF:4D:3D:85:31:88"));
    QCOMPARE(cert.digest(QCryptographicHash::Sha1),
             QByteArray::fromHex("B6:D1:51:82:E0:29:CA:59:96:38:BD:B6:F9:40:05:91:6D:49:09:60"));

    QCOMPARE(cert.effectiveDate().toUTC(), QDateTime(QDate(2007, 4, 17), QTime(7,40,26), Qt::UTC));
    QCOMPARE(cert.expiryDate().toUTC(), QDateTime(QDate(2007, 5, 17), QTime(7,40,26), Qt::UTC));
    QVERIFY(!cert.isValid());   // cert has expired

    QSslCertificate copy = cert;
    QVERIFY(cert == copy);
    QVERIFY(!(cert != copy));

    QCOMPARE(cert, QSslCertificate(pem, QSsl::Pem));
    QCOMPARE(cert, QSslCertificate(QByteArray::fromHex(der), QSsl::Der));
}

void tst_QSslCertificate::certInfoQByteArray()
{
    QSslCertificate cert =  QSslCertificate::fromPath("certificates/cert.pem", QSsl::Pem,
                                                      QRegExp::FixedString).first();
    QVERIFY(!cert.isNull());

    // in this test, check the bytearray variants before the enum variants to see if
    // we fixed a bug we had with lazy initialization of the values.
    QCOMPARE(cert.issuerInfo("CN"), QString("Test CA (1024 bit)"));
    QCOMPARE(cert.subjectInfo("CN"), QString("name/with/slashes"));
}

void tst_QSslCertificate::task256066toPem()
{
    // a certificate whose PEM encoding's length is a multiple of 64
    const char *mycert = "-----BEGIN CERTIFICATE-----\n" \
                         "MIIEGjCCAwKgAwIBAgIESikYSjANBgkqhkiG9w0BAQUFADBbMQswCQYDVQQGEwJF\n" \
                         "RTEiMCAGA1UEChMZQVMgU2VydGlmaXRzZWVyaW1pc2tlc2t1czEPMA0GA1UECxMG\n" \
                         "RVNURUlEMRcwFQYDVQQDEw5FU1RFSUQtU0sgMjAwNzAeFw0wOTA2MDUxMzA2MTha\n" \
                         "Fw0xNDA2MDkyMTAwMDBaMIGRMQswCQYDVQQGEwJFRTEPMA0GA1UEChMGRVNURUlE\n" \
                         "MRcwFQYDVQQLEw5hdXRoZW50aWNhdGlvbjEhMB8GA1UEAxMYSEVJQkVSRyxTVkVO\n" \
                         "LDM3NzA5MjcwMjg1MRAwDgYDVQQEEwdIRUlCRVJHMQ0wCwYDVQQqEwRTVkVOMRQw\n" \
                         "EgYDVQQFEwszNzcwOTI3MDI4NTCBnzANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEA\n" \
                         "k2Euwhm34vu1jOFp02J5fQRx9LW2C7x78CbJ7yInoAKn7QR8UdxTU7mJk90Opejo\n" \
                         "71RUi2/aYl4jCr9gr99v2YoLufMRwAuqdmwmwqH1WAHRUtIcD0oPdKyelmmn9ig0\n" \
                         "RV+yJLNT3dnyrwPw+uuzDe3DeKepGKE4lxexliCaAx0CAyCMW6OCATEwggEtMA4G\n" \
                         "A1UdDwEB/wQEAwIEsDAdBgNVHSUEFjAUBggrBgEFBQcDAgYIKwYBBQUHAwQwPAYD\n" \
                         "VR0fBDUwMzAxoC+gLYYraHR0cDovL3d3dy5zay5lZS9jcmxzL2VzdGVpZC9lc3Rl\n" \
                         "aWQyMDA3LmNybDAgBgNVHREEGTAXgRVzdmVuLmhlaWJlcmdAZWVzdGkuZWUwUQYD\n" \
                         "VR0gBEowSDBGBgsrBgEEAc4fAQEBATA3MBIGCCsGAQUFBwICMAYaBG5vbmUwIQYI\n" \
                         "KwYBBQUHAgEWFWh0dHA6Ly93d3cuc2suZWUvY3BzLzAfBgNVHSMEGDAWgBRIBt6+\n" \
                         "jIdXlYB4Y/qcIysroDoYdTAdBgNVHQ4EFgQUKCjpDf+LcvL6AH0QOiW6rMTtB/0w\n" \
                         "CQYDVR0TBAIwADANBgkqhkiG9w0BAQUFAAOCAQEABRyRuUm9zt8V27WuNeXtCDmU\n" \
                         "MGzA6g4QXNAd2nxFzT3k+kNzzQTOcgRdmjiEPuK49On+GWnBr/5MSBNhbCJVPWr/\n" \
                         "yym1UYTBisaqhRt/N/kwZqd0bHeLJk+ZxSePXRyqkp9H8KPWqz7H+O/FxRS4ffxo\n" \
                         "Q9Clem+e0bcjNlL5xXiRGycBeZq8cKj+0+A/UuattznQlvHdlCEsSeu1fPOORqFV\n" \
                         "fZur4HC31lQD7xVvETLiL83CtOQC78+29XPD6Zlrrc5OF2yibSVParY19b8Zh6yu\n" \
                         "p1dNvN8pBgXGrsyxRonwHooV2ghGNmGILkpdvlQfnxeCUg4erfHjDdSY9vmT7w==\n" \
                         "-----END CERTIFICATE-----\n";

    QByteArray pem1(mycert);
    QSslCertificate cert1(pem1);
    QVERIFY(!cert1.isNull());
    QByteArray pem2(cert1.toPem());
    QSslCertificate cert2(pem2);
    QVERIFY(!cert2.isNull());
    QCOMPARE(pem1, pem2);
}

void tst_QSslCertificate::nulInCN()
{
    QList<QSslCertificate> certList =
        QSslCertificate::fromPath(SRCDIR "more-certificates/badguy-nul-cn.crt");
    QCOMPARE(certList.size(), 1);

    const QSslCertificate &cert = certList.at(0);
    QVERIFY(!cert.isNull());

    QString cn = cert.subjectInfo(QSslCertificate::CommonName);
    QVERIFY(cn != "www.bank.com");

    static const char realCN[] = "www.bank.com\\x00.badguy.com";
    QCOMPARE(cn, QString::fromLatin1(realCN, sizeof realCN - 1));
}

void tst_QSslCertificate::nulInSan()
{
    QList<QSslCertificate> certList =
        QSslCertificate::fromPath(SRCDIR "more-certificates/badguy-nul-san.crt");
    QCOMPARE(certList.size(), 1);

    const QSslCertificate &cert = certList.at(0);
    QVERIFY(!cert.isNull());

    QMultiMap<QSsl::AlternateNameEntryType, QString> san = cert.alternateSubjectNames();
    QVERIFY(!san.isEmpty());

    QString dnssan = san.value(QSsl::DnsEntry);
    QVERIFY(!dnssan.isEmpty());
    QVERIFY(dnssan != "www.bank.com");

    static const char realSAN[] = "www.bank.com\0www.badguy.com";
    QCOMPARE(dnssan, QString::fromLatin1(realSAN, sizeof realSAN - 1));
}

void tst_QSslCertificate::largeSerialNumber()
{
    QList<QSslCertificate> certList =
        QSslCertificate::fromPath(SRCDIR "more-certificates/cert-large-serial-number.pem");

    QCOMPARE(certList.size(), 1);

    const QSslCertificate &cert = certList.at(0);
    QVERIFY(!cert.isNull());
    QCOMPARE(cert.serialNumber(), QByteArray("01:02:03:04:05:06:07:08:09:10:aa:bb:cc:dd:ee:ff:17:18:19:20"));
}

#endif // QT_NO_OPENSSL

QTEST_MAIN(tst_QSslCertificate)
#include "tst_qsslcertificate.moc"
