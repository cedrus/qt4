# The tests in this .pro file _MUST_ use QtCore and QtNetwork only
# (i.e. QT=core network).
# The test system is allowed to run these tests before the rest of Qt has
# been compiled.
TEMPLATE=subdirs
SUBDIRS=\
    networkselftest \
    qabstractnetworkcache \
    qabstractsocket \
    qauthenticator \
    qeventloop \
    qftp \
    qhostaddress \
    qhostinfo \
    qhttp \
    qhttpnetworkconnection \
    qhttpnetworkreply \
    qhttpsocketengine \
    qnativesocketengine \
    qnetworkaccessmanager \
    qnetworkaddressentry \
    qnetworkconfiguration \
    qnetworkconfigurationmanager \
    qnetworkcookie \
    qnetworkcookiejar \
    qnetworkinterface \
    qnetworkproxy \
    qnetworkrequest \
    qnetworksession \
    qobjectperformance \
    qsocketnotifier \
    qsocks5socketengine \
    qsslcertificate \
    qsslcipher \
    qsslerror \
    qsslkey \
    qsslsocket \

!contains(QT_CONFIG, private_tests): SUBDIRS -= \
    qauthenticator \
    qhttpnetworkconnection \
    qhttpnetworkreply \
    qnativesocketengine \
    qsocketnotifier \
    qsocks5socketengine \

