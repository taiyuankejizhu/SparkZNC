#include "myinputpanelcontext.h"
#include "qdebug.h"

MyInputPanelContext::MyInputPanelContext()
{
}

MyInputPanelContext::MyInputPanelContext(QWidget *parent)
{
    keyboard = new KeyBoard(parent);
    keyboard->setHidden(true);

    connect(keyboard, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));
    connect(keyboard, SIGNAL(controlGenerated(int)), SLOT(sendControl(int)));
    connect(this, SIGNAL(setVisible(bool)),keyboard, SLOT(setVisible(bool)));
}

MyInputPanelContext::~MyInputPanelContext()
{
}

bool MyInputPanelContext::filterEvent(const QEvent* event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel) {
        updatePosition();
        emit setVisible(true);
        return true;
    } else if (event->type() == QEvent::CloseSoftwareInputPanel) {
        emit setVisible(false);
        return true;
    }
    return false;
}

QString MyInputPanelContext::identifierName()
{
    return "MyInputPanelContext";
}

void MyInputPanelContext::reset()
{
}

bool MyInputPanelContext::isComposing() const
{
    return false;
}

QString MyInputPanelContext::language()
{
    return "en_US";
}

void MyInputPanelContext::updatePosition()
{
    QWidget *widget = focusWidget();
    QString type;
    QPoint point;

    if (!widget){
        return;
    }else{
        type = widget->metaObject()->className();
        qDebug()<<type;
        if(type.contains("LineEdit")){
            point = QPoint(X_POS0, Y_POS0);
            keyboard->setFixedHeight(MAX_H_POS0);
            //keyboard->setSizePolicy(QSizePolicy::Maximum ,QSizePolicy::Maximum);
        }else{
            point = QPoint(X_POS0, Y_POS0);
            keyboard->setFixedHeight(MAX_H_POS0);
            //keyboard->setSizePolicy(QSizePolicy::Minimum ,QSizePolicy::Minimum);
        }
    }
    keyboard->move(point);
}

void MyInputPanelContext::sendCharacter(QChar character)
{
    QPointer<QWidget> w = focusWidget();

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    QKeyEvent keyRelease(QEvent::KeyRelease, character.unicode(), Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyRelease);
}

void MyInputPanelContext::sendControl(int control)
{
    QPointer<QWidget> w = focusWidget();

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, control, Qt::NoModifier, QString::number(control));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    QKeyEvent keyRelease(QEvent::KeyRelease, control, Qt::NoModifier, QString::number(control));
    QApplication::sendEvent(w, &keyRelease);
}
