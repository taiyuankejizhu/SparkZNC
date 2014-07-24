#include "startmenu.h"
#include "ui_startmenu.h"
#include "sparkinfo.h"
#include "qdebug.h"

StartMenu::StartMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartMenu)
{
    ui->setupUi(this);

    selected = 0;

    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer ,SIGNAL(timeout()) ,this ,SLOT(selectItem()));

    QPixmap shut_pix = QPixmap(":/shut.png");
    QPixmap restart_pix = QPixmap(":/restart.png");
    QPixmap sound_pix = QPixmap(":/sound.png");
    QPixmap setting_pix = QPixmap(":/setting.png");
    QPixmap sleep_pix = QPixmap(":/sleep.png");
    QPixmap password_pix = QPixmap(":/password.png");
    QPixmap clear_pix = QPixmap(":/clear.png");

    ui->label_shut->setPixmap(shut_pix);
    ui->label_restart->setPixmap(restart_pix);
    ui->label_sound->setPixmap(sound_pix);
    ui->label_setting->setPixmap(setting_pix);
    ui->label_sleep->setPixmap(sleep_pix);
    ui->label_password->setPixmap(password_pix);
    ui->label_clear->setPixmap(clear_pix);

}

void StartMenu::keyPressEvent(QKeyEvent *k)
{
    QPixmap pix;

    if(!k->isAutoRepeat()){
        k->accept();
        switch(k->key())
        {
        case Qt::Key_Down:
            if(selected == 0)
                selected = 9;
            else
                selected--;
            updateSelect();
            break;
        case Qt::Key_Up:
            if(selected > 8)
                selected = 0;
            else
                selected++;
            updateSelect();
            break;
        case Qt::Key_F1:
            selected = 1;
            if(spark_info->b_array[B_SHUTDOWN]){
                pix = QPixmap(":/shut.png");
                ui->label_shut->setPixmap(pix);
            }
            else{
                pix = QPixmap(":/shut_b.png");
                ui->label_shut->setPixmap(pix);
            }
            updateSelect();
            break;
        case Qt::Key_F2:
            selected = 2;
            if(spark_info->b_array[B_REBOOT]){
                pix = QPixmap(":/restart.png");
                ui->label_restart->setPixmap(pix);
            }
            else{
                pix = QPixmap(":/restart_b.png");
                ui->label_restart->setPixmap(pix);
            }
            updateSelect();
            break;
        case Qt::Key_F3:
            selected = 3;
            if(spark_info->b_array[B_SLEEP]){
                pix = QPixmap(":/sleep.png");
                ui->label_sleep->setPixmap(pix);
            }
            else{
                pix = QPixmap(":/sleep_b.png");
                ui->label_sleep->setPixmap(pix);
            }
            updateSelect();
            break;
        case Qt::Key_F4:
            selected = 4;
            updateSelect();
            break;
        case Qt::Key_F5:
            selected = 5;
            updateSelect();
            break;
        case Qt::Key_F6:
            selected = 6;
            updateSelect();
            break;
        case Qt::Key_F7:
            selected = 7;
            updateSelect();
            break;
        case Qt::Key_F8:
            selected = 8;
            updateSelect();
            break;
        case Qt::Key_Enter:
            selected = 0;
            updateSelect();
            emit finish();
            break;
        default:
            break;
        }
        if(!timer->isActive()){
            timer->start();
            qDebug()<<"timer start!";
        }
    }
    else
        k->ignore();
}

void StartMenu::keyReleaseEvent(QKeyEvent *k)
{
    QPixmap pix;

    if(!k->isAutoRepeat()){
        k->accept();

        if(timer->isActive()){
            timer->stop();
            qDebug()<<"timer stop!";
        }
        updateIcon();

        switch(k->key())
        {
        case Qt::Key_F1:
            break;
        case Qt::Key_F2:
            break;
        case Qt::Key_F3:
            break;
        case Qt::Key_F4:
            break;
        case Qt::Key_F5:
            break;
        case Qt::Key_F6:
            break;
        case Qt::Key_F7:
            break;
        case Qt::Key_F8:
            break;
        default :
            break;
        }

    }
    else
        k->ignore();
}

void StartMenu::updateSelect()
{
    ui->frame_1->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_2->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_3->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_4->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_5->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_6->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_7->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_8->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");
    ui->frame_9->setStyleSheet("QFrame{background-color:rgb(255,255,255);}");

    switch(selected)
    {
    case 0:
        break;
    case 1:
        ui->frame_1->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 2:
        ui->frame_2->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 3:
        ui->frame_3->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 4:
        ui->frame_4->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 5:
        ui->frame_5->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 6:
        ui->frame_6->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 7:
        ui->frame_7->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 8:
        ui->frame_8->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    case 9:
        ui->frame_9->setStyleSheet("QFrame{background-color:rgb(255,204,0);}");
        break;
    default:
        break;
    }
    this->setFocus();
}

void StartMenu::selectItem()
{
    qDebug()<<"timer out!";
    switch(selected)
    {
    case 0:
        break;
    case 1:
        if(spark_info->b_array[B_SHUTDOWN]){
            spark_info->setBool(B_SHUTDOWN ,false);
        }
        else{
            spark_info->setBool(B_SHUTDOWN ,true);
        }
        emit finish();
        break;
    case 2:
        if(spark_info->b_array[B_REBOOT]){
            spark_info->setBool(B_REBOOT ,false);
        }
        else{
            spark_info->setBool(B_REBOOT ,true);
        }
        emit finish();
        break;
    case 3:
        if(spark_info->b_array[B_SLEEP]){
            spark_info->setBool(B_SLEEP ,false);
        }
        else{
            spark_info->setBool(B_SLEEP ,true);
        }
        emit finish();
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    case 8:
        break;
    case 9:
        break;
    default:
        break;
    }
    if(timer->isActive())
        timer->stop();
}

void StartMenu::updateIcon()
{
    QPixmap pix;

    if(spark_info->b_array[B_SHUTDOWN]){
        pix = QPixmap(":/shut_b.png");
        ui->label_shut->setPixmap(pix);
    }
    else{
        pix = QPixmap(":/shut.png");
        ui->label_shut->setPixmap(pix);
    }

    if(spark_info->b_array[B_SLEEP]){
        pix = QPixmap(":/sleep_b.png");
        ui->label_sleep->setPixmap(pix);
    }
    else{
        pix = QPixmap(":/sleep.png");
        ui->label_sleep->setPixmap(pix);
    }

    if(spark_info->b_array[B_REBOOT]){
        pix = QPixmap(":/restart_b.png");
        ui->label_restart->setPixmap(pix);
    }
    else{
        pix = QPixmap(":/restart.png");
        ui->label_restart->setPixmap(pix);
    }
}

StartMenu::~StartMenu()
{
    delete ui;
}
