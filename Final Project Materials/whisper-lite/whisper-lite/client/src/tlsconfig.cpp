#include "tlsconfig.h"

#include <QFile>
#include <QCryptographicHash>
#include <QSslKey>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(lcTls, "whisper.tls")

QSslConfiguration TlsConfig::pinned()
{
    QSslConfiguration cfg = QSslConfiguration::defaultConfiguration();
    cfg.setProtocol(QSsl::TlsV1_2OrLater);
    cfg.setPeerVerifyMode(QSslSocket::VerifyPeer);

    QFile caFile(QStringLiteral(":/whisper_ca.pem"));
    if (caFile.open(QIODevice::ReadOnly)) {
        const QList<QSslCertificate> ca =
            QSslCertificate::fromData(caFile.readAll(), QSsl::Pem);
        if (!ca.isEmpty()) {
            cfg.setCaCertificates(ca);
            qCInfo(lcTls) << "Loaded" << ca.size() << "pinned CA cert(s)";
        } else {
            qCWarning(lcTls) << "whisper_ca.pem present but no certs parsed";
        }
    } else {
        qCWarning(lcTls) << "No pinned CA found in resources";
    }

    return cfg;
}

QByteArray TlsConfig::expectedSpkiSha256()
{
    static QByteArray cached;
    if (!cached.isEmpty())
        return cached;

    QFile f(QStringLiteral(":/spki_pin.txt"));
    if (!f.open(QIODevice::ReadOnly)) {
        qCWarning(lcTls) << "spki_pin.txt not found in resources";
        return {};
    }
    cached = f.readAll().trimmed();
    return cached;
}

QByteArray TlsConfig::computeSpkiSha256(const QSslCertificate &cert)
{
    // The Subject Public Key Info DER blob is what we want to hash.
    // Qt exposes it via cert.publicKey().toDer(). Note that toDer() of the
    // public key alone is the SPKI block (not a wrapped envelope), which is
    // what RFC 7469 ("HTTP Public Key Pinning") and Android's Network Security
    // Config both pin against.
    const QByteArray der = cert.publicKey().toDer();
    return QCryptographicHash::hash(der, QCryptographicHash::Sha256).toBase64();
}

bool TlsConfig::acceptablePinned(const QSslCertificate &peerCert,
                                 const QList<QSslError> &errors)
{
    if (peerCert.isNull()) {
        qCWarning(lcTls) << "No peer certificate presented";
        return false;
    }

    const QByteArray expected = expectedSpkiSha256();
    const QByteArray actual   = computeSpkiSha256(peerCert);

    if (expected.isEmpty()) {
        qCWarning(lcTls) << "No SPKI pin baked in - refusing";
        return false;
    }
    if (expected != actual) {
        qCWarning(lcTls).noquote()
            << "SPKI pin MISMATCH. expected=" << expected
            << "actual=" << actual;
        return false;
    }

    qCInfo(lcTls) << "SPKI pin matched, evaluating individual SSL errors:";
    for (const QSslError &e : errors) {
        switch (e.error()) {
        case QSslError::HostNameMismatch:
#ifdef QT_DEBUG
            qCInfo(lcTls) << "  ignoring HostNameMismatch (debug build)";
            continue;
#else
            qCWarning(lcTls) << "  HostNameMismatch in release build - reject";
            return false;
#endif
        case QSslError::SelfSignedCertificate:
        case QSslError::SelfSignedCertificateInChain:
            // Pin already matched, so self-signed is fine.
            qCInfo(lcTls) << "  ignoring SelfSignedCertificate (pin matched)";
            continue;
        case QSslError::CertificateUntrusted:
            qCInfo(lcTls) << "  ignoring CertificateUntrusted (pin matched)";
            continue;
        default:
            qCWarning(lcTls) << "  unexpected SSL error:" << e.errorString();
            return false;
        }
    }
    return true;
}
