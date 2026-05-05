#!/usr/bin/env bash
# Generates a self-signed certificate suitable for development with the
# Whisper Android client. SANs include:
#   - localhost / 127.0.0.1   (server-side dev)
#   - 10.0.2.2                (Android emulator's view of host loopback)
#   - whisper.local           (mDNS-style alias if you set one up)
#
# In production replace this with a Let's Encrypt cert on a real domain.

set -euo pipefail

cat > san.cnf <<'EOF'
[req]
distinguished_name = dn
x509_extensions = v3
prompt = no

[dn]
C  = FI
ST = Pirkanmaa
L  = Tampere
O  = Whisper Dev
CN = whisper.local

[v3]
basicConstraints       = critical,CA:TRUE
keyUsage               = critical,digitalSignature,keyCertSign,keyEncipherment
extendedKeyUsage       = serverAuth
subjectAltName         = @alt
subjectKeyIdentifier   = hash

[alt]
DNS.1 = localhost
DNS.2 = whisper.local
IP.1  = 127.0.0.1
IP.2  = 10.0.2.2
EOF

openssl req -x509 -nodes -newkey rsa:2048 \
    -keyout key.pem -out cert.pem \
    -days 825 -config san.cnf -extensions v3

# Compute the SPKI SHA-256 pin that the client will hard-code
echo
echo "=== TLS pin (paste this into client/android/res/raw/spki_pin.txt) ==="
openssl x509 -in cert.pem -pubkey -noout \
    | openssl pkey -pubin -outform DER \
    | openssl dgst -sha256 -binary \
    | base64

echo
echo "Generated cert.pem and key.pem with SANs for localhost, 10.0.2.2,"
echo "whisper.local. Validity: 825 days."
