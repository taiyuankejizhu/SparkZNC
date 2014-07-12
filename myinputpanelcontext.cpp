#include "myinputpanelcontext.h"
#include "qdebug.h"

MyInputPanelContext::MyInputPanelContext()
{
}

MyInputPanelContext::MyInputPanelContext(QWidget *parent)
{
    connect(parent, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));
    connect(this, SIGNAL(setVisible(bool)),parent, SLOT(setVisible(bool)));
}

MyInputPanelContext::~MyInputPanelContext()
{
}

bool MyInputPanelContext::filterEvent(const QEvent* event)
{
    if (event->type() == QEvent::RequestSoftwareInputPanel) {
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

void MyInputPanelContext::sendCharacter(QChar character)
{
    QPointer<QWidget> w = focusWidget();

    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
    QApplication::sendEvent(w, &keyRelease);
}
