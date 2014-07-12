#ifndef MYINPUTPANELCONTEXT_H
#define MYINPUTPANELCONTEXT_H

#include <QtGui/QInputContext>
#include <QtCore>
#include "keyboard.h"

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

signals:
    void setVisible(bool);
private slots:
    void sendCharacter(QChar character);

};

#endif // MYINPUTPANELCONTEXT_H
