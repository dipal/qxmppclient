#ifndef CHATAREA_H
#define CHATAREA_H

#include <QWidget>

namespace Ui {
class ChatArea;
}

class ChatArea : public QWidget
{
    Q_OBJECT

public:
    explicit ChatArea(QWidget *parent = 0);
    ~ChatArea();

private:
    Ui::ChatArea *ui;
};

#endif // CHATAREA_H
