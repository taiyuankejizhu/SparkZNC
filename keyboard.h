#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <QWidget>
#include <QList>
#include "ui_keyboard.h"

class KeyBoard: public QWidget
{
	Q_OBJECT
public:
	KeyBoard(QWidget *parent = 0);
	~KeyBoard();

signals:
    void controlGenerated(int control);
    void characterGenerated(QChar character);
public slots:
    void doButtonClicked(int idx);

private:
	void setKeyMap(const char **keymap);
	Ui::KeyBoard *m_ui;
	QList<QPushButton *> m_allKeys;

	bool m_caps;
};

#endif

