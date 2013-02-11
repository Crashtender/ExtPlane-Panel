#include "extplaneclient.h"
#include <QDebug>

ExtPlaneClient::ExtPlaneClient(QObject *parent, QString name, ClientDataRefProvicer *drp) :
    QObject(parent), _name(name), _connection(drp)
{
}

ExtPlaneClient::~ExtPlaneClient() {
    foreach(ClientDataRef *ref, _dataRefs) {
        qDebug() << Q_FUNC_INFO << "unsubbing ref " << ref->name();
        _dataRefs.removeOne(ref);
        _connection->unsubscribeDataRef(ref);
    }
}

ClientDataRef* ExtPlaneClient::subscribeDataRef(QString name, double accuracy) {
    ClientDataRef *ref = _connection->subscribeDataRef(name, accuracy);
    connect(ref, SIGNAL(changed(ClientDataRef*)), this, SLOT(cdrChanged(ClientDataRef*)));
    connect(ref, SIGNAL(destroyed(QObject*)), this, SLOT(refDestroyed(QObject*)));
    _dataRefs.append(ref);
    return ref;
}

void ExtPlaneClient::refDestroyed(QObject* refqo) {
    _dataRefs.removeOne(static_cast<ClientDataRef*>(refqo));
}

void ExtPlaneClient::cdrChanged(ClientDataRef *ref) {
    double value;
    bool ok;
    
    value = ref->valueString().toDouble(&ok);
    if (ok){
        emit refChanged(ref->name(), value);
    } else {
        qDebug() << Q_FUNC_INFO << "unable to convert to double " << ref->valueString();
        emit refChanged(ref->name(), ref->valueString());
    }
}

void ExtPlaneClient::unsubscribeDataRef(QString name) {
    qDebug() << Q_FUNC_INFO << name;
    foreach(ClientDataRef *ref, _dataRefs) {
        if(ref->name() == name) {
//            disconnect(ref, 0, this, 0);
            _dataRefs.removeOne(ref);
            _connection->unsubscribeDataRef(ref);
            return;
        }
    }
    Q_ASSERT(false);
}

void ExtPlaneClient::keyPress(int id) {
    _connection->keyPress(id);
}

void ExtPlaneClient::buttonPress(int id) {
    _heldButtons.insert(id);
    _connection->buttonPress(id);
}

void ExtPlaneClient::buttonRelease(int id) {
    if(!_heldButtons.contains(id)) return;
    _heldButtons.remove(id);
    _connection->buttonRelease(id);
}

void ExtPlaneClient::valueSet(ClientDataRef *ref) {
    _connection->setValue(ref->name(), ref->valueString());
}

void ExtPlaneClient::unsubscribed(ClientDataRef *ref) {
    unsubscribeDataRef(ref->name());
}
