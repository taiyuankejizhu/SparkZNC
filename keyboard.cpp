#include "keyboard.h"
#include "ui_keyboard.h"
#include <QDebug>
#include <QSignalMapper>

#include "en_keymap.h"

KeyBoard::KeyBoard(QWidget *parent)
	: QWidget(parent)
{
	m_caps = false;

	m_ui = new Ui::KeyBoard;
	m_ui->setupUi(this);
    this->setGeometry(0 ,0 ,width() ,height());
    setWindowFlags(Qt::FramelessWindowHint);

	QSignalMapper* mapper = new QSignalMapper(this);
	connect(mapper, SIGNAL(mapped(int)), this, SLOT(doButtonClicked(int)));

	m_allKeys = findChildren<QPushButton *>();
	int i = 0;
	for (QList<QPushButton *>::const_iterator iter = m_allKeys.begin();
			iter != m_allKeys.end(); iter++, i++) {
		mapper->setMapping((*iter), i);
		connect(*iter, SIGNAL(clicked()), mapper, SLOT(map()));
	}

    setKeyMap(en_upper_keymap);
}

KeyBoard::~KeyBoard()
{
	delete m_ui;
}

void KeyBoard::setKeyMap(const char **keymap)
{
	int i = 0;
	for (QList<QPushButton *>::const_iterator iter = m_allKeys.begin();
			iter != m_allKeys.end(); iter++, i++) {
		(*iter)->setText(keymap[i]);
	}
}

void KeyBoard::doButtonClicked(int idx)
{
    int control;
    QChar key;

	if (m_allKeys.at(idx)->text() == "Caps") {
		m_caps = !m_caps;
		if (m_caps) {
            setKeyMap(en_upper_keymap);
        }else{
            setKeyMap(en_lower_keymap);
		}
    }else if(m_allKeys.at(idx)->text() == "<--"){
        control = Qt::Key_Backspace;
        emit controlGenerated(control);
    }else if(m_allKeys.at(idx)->text() == "Enter"){
        control = Qt::Key_Return;
        emit controlGenerated(control);
    }else{
        if(m_allKeys.at(idx)->text().length() == 1){
            key = m_allKeys.at(idx)->text().at(0);
            emit characterGenerated(key);
        }
    }
}
