#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatarea.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->ChatTab->addTab(new ChatArea(), "t1");

    model = new QStringListModel();
    ui->list->setModel(model);
}

MainWindow::~MainWindow()
{
    recvThread->stop();
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    ui->statusBar->setStatusTip("Connecting ...");
    QString userId = ui->userId->text();
    QString password = ui->password->text();
    jid = JID(userId.toStdString());
    client = new Client(jid, password.toStdString());
    //client->setServer("talk.google.com");
    client->registerConnectionListener(this);
    client->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);
    client->rosterManager()->registerRosterListener(this);
    client->setSasl(true);

    recvThread = new RecvThread(client);
    recvThread->start();
    client->connect(false);

    vcardManager = new VCardManager(client);
}

void MainWindow::onConnect()
{
    ui->statusBar->showMessage("Logged in");
    qDebug() << "ConnListener::onConnect()" << endl;
    vcardManager->fetchVCard(jid.bare(), this);
}

void MainWindow::onDisconnect(ConnectionError e)
{
    ui->statusBar->showMessage("Disconnected "+QString::number(e));
    qDebug() << "ConnListener::onDisconnect() " << e <<" "<<ConnNoSupportedAuth << endl;
}

void MainWindow::handleLog(LogLevel level, LogArea area, const string &message)
{
    qDebug()<< "log: level: "<<level<<", area: "<<area<<", "<<message.c_str();
}

void MainWindow::handleVCard(const JID &jid, const VCard *v)
{

    qDebug()<<"Received a vcard"<<endl;
    if( !v )
    {
        qDebug()<<"empty vcard!"<<endl;
        return;
    }

    VCard* vcard = new VCard( *v );
    string fn = vcard->formattedname();
    qDebug()<<fn.c_str()<<endl;
    VCard::Photo photo = vcard->photo();
    ui->nameLabel->setText(QString(fn.c_str()));
    string p = photo.binval;
    for (int i=0; i<10; i++) cout<<(int) p[i]<<" "; cout<<endl;
    QByteArray imageData(p.c_str(), p.size());
    QImage image;
    qDebug()<<image.loadFromData(imageData, "PNG");
    qDebug()<<image.save("/home/dipal/a.png", "PNG");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    ui->imageLabel->show();
}

void MainWindow::handleVCardResult(VCardHandler::VCardContext context, const JID &jid, StanzaError se)
{

}

void MainWindow::insertFriend(QString friendJid)
{
    model->insertRow(model->rowCount());
    QModelIndex index = model->index(model->rowCount()-1);
    model->setData(index, friendJid);
}


void MainWindow::handleItemAdded(const JID &jid)
{
}

void MainWindow::handleItemSubscribed(const JID &jid)
{
}

void MainWindow::handleItemRemoved(const JID &jid)
{
}

void MainWindow::handleItemUpdated(const JID &jid)
{
}

void MainWindow::handleItemUnsubscribed(const JID &jid)
{
}

void MainWindow::handleRoster(const Roster &roster)
{
    Roster::const_iterator it = roster.begin();
    for( ; it != roster.end(); ++it )
    {
        insertFriend((*it).second->jidJID().full().c_str());
    }
}

void MainWindow::handleRosterPresence(const RosterItem &item, const string &resource, Presence::PresenceType presence, const string &msg)
{
}

void MainWindow::handleSelfPresence(const RosterItem &item, const string &resource, Presence::PresenceType presence, const string &msg)
{
}

bool MainWindow::handleSubscriptionRequest(const JID &jid, const string &msg)
{
}

bool MainWindow::handleUnsubscriptionRequest(const JID &jid, const string &msg)
{
}

void MainWindow::handleNonrosterPresence(const Presence &presence)
{
}

void MainWindow::handleRosterError(const IQ &iq)
{
}
