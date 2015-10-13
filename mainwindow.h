#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QDebug>
#include <QThread>
#include <QStringListModel>
#include <chatarea.h>
#include <gloox/client.h>
#include <gloox/connectionlistener.h>
#include <gloox/connectiontls.h>
#include <gloox/connectiontcpclient.h>
#include <gloox/vcardhandler.h>
#include <gloox/vcardmanager.h>
#include <gloox/rosterlistener.h>
#include <gloox/rostermanager.h>
#include <gloox/vcard.h>

namespace Ui {
class MainWindow;
}

using namespace gloox;
using namespace std;

class RecvThread : public QThread
{
    Q_OBJECT

public:
    RecvThread(Client *client)
    {
        this->client = client;
    }

    void stop()
    {
        running = false;
    }

    void run()
    {
        running = true;
        while (running)
        {
            //qDebug()<<"receiving ";
            client->recv(100);
            //qDebug()<<"recv done";
        }
    }

private:
    Client *client;
    bool running{false};
};


class MainWindow : public QMainWindow, public ConnectionListener, public VCardHandler, public LogHandler, public RosterListener
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_loginButton_clicked();

    void on_list_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    virtual void onConnect();
    virtual void onDisconnect(ConnectionError e);
    virtual bool onTLSConnect(const CertInfo& info) {
        cout << "ConnListener::onTLSConnect()" << endl;
        return true;
    }

    virtual void handleLog( LogLevel level, LogArea area, const std::string& message );
    virtual void handleVCard( const JID& jid, const VCard *v );

    virtual void handleVCardResult( VCardContext context, const JID& jid,
                                    StanzaError se = StanzaErrorUndefined  );

    void insertFriend(QString friendJid);

    JID jid;
    Client* client;
    RecvThread *recvThread;
    VCardManager *vcardManager;
    QStringListModel *model;
    QMap<QString, ChatArea*> chatMap;
    QMap<QString, int> tabIndex;

    // ConnectionListener interface

    // RosterListener interface
public:
    void handleItemAdded(const JID &jid);
    void handleItemSubscribed(const JID &jid);
    void handleItemRemoved(const JID &jid);
    void handleItemUpdated(const JID &jid);
    void handleItemUnsubscribed(const JID &jid);
    void handleRoster(const Roster &roster);
    void handleRosterPresence(const RosterItem &item, const string &resource, Presence::PresenceType presence, const string &msg);
    void handleSelfPresence(const RosterItem &item, const string &resource, Presence::PresenceType presence, const string &msg);
    bool handleSubscriptionRequest(const JID &jid, const string &msg);
    bool handleUnsubscriptionRequest(const JID &jid, const string &msg);
    void handleNonrosterPresence(const Presence &presence);
    void handleRosterError(const IQ &iq);
};

#endif // MAINWINDOW_H
