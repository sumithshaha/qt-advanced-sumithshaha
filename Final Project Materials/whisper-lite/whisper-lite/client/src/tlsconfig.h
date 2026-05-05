#pragma once

#include <QSslConfiguration>
#include <QSslError>
#include <QSslCertificate>
#include <QByteArray>
#include <QList>

/**
 * TLS configuration helpers for Whisper.
 *
 * Two layers of protection:
 *   1. The pinned CA certificate is loaded from Qt resources (:/whisper_ca.pem)
 *      and set as the only acceptable trust anchor.
 *   2. The peer's Subject Public Key Info (SPKI) SHA-256 hash is compared
 *      against a hard-coded pin (:/spki_pin.txt). Even if a malicious CA
 *      somehow signed a cert for the same hostname, the pin would still
 *      catch it.
 *
 * In debug builds we additionally tolerate the HostNameMismatch error
 * (because '10.0.2.2' usually doesn't appear in the cert's CN, only as SAN).
 * Release builds reject any error that isn't covered by the pin.
 */
class TlsConfig
{
public:
    /** Returns a QSslConfiguration with TLS 1.2+ and our pinned CA. */
    static QSslConfiguration pinned();

    /**
     * Decide whether to ignore the given SSL errors based on SPKI pin match.
     * @param peerCert  the certificate the peer presented
     * @param errors    the list of SSL errors Qt raised
     * @return true if the connection should be allowed, false to abort.
     */
    static bool acceptablePinned(const QSslCertificate &peerCert,
                                 const QList<QSslError> &errors);

    /** The expected SPKI SHA-256 pin (loaded once from :/spki_pin.txt). */
    static QByteArray expectedSpkiSha256();

    /** Compute the SPKI SHA-256 of any certificate (helper for diagnostics). */
    static QByteArray computeSpkiSha256(const QSslCertificate &cert);
};
