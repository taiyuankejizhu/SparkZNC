#ifndef MYINPUTPANELCONTEXT_H
#define MYINPUTPANELCONTEXT_H

#include <QtGui/QInputContext>
#include <QtCore>
#include "keyboard.h"

#define X_POS0 0
#define Y_POS0 0
#define MAX_H_POS0 350
#define X_POS1 0
#define Y_POS1 390
#define MAX_H_POS1 300

class MyInputPanelContext : public QInputContext
{
    Q_OBJECT

public:
    explicit MyInputPanelContext();
    explicit MyInputPanelContext(QWidget *parent = 0);
    ~MyInputPanelContext();

    bool filterEvent(const QEvent* event);

    QString identifierName();
    QString language();

    bool isComposing() const;
    void reset();

private:
    /*软件盘*/
    KeyBoard *keyboard;
    void updatePosition();

signals:
    void setVisible(bool);
private slots:
    void sendCharacter(QChar character);
    void sendControl(int control);

};

#endif // MYINPUTPANELCONTEXT_H
