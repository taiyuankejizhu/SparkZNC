#include "startmenu.h"
#include "ui_startmenu.h"
#include "sparkinfo.h"
#include "qdebug.h"

#define UNSELECTED_STYLE "QFrame{background-color:rgb(255,255,255);}"
#define SELECTED_STYLE "QFrame{background-color:rgb(255,204,0);}"

StartMenu::StartMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartMenu)
{
    ui->setupUi(this);

    selected = 0;

    clear = new ClearDialog(parent);
    clear->hide();

    key = new KeyDialog(parent);
    key->setModal(true);
    key->hide();

    setting = new SettingDialog(parent);
    setting->setModal(true);
    setting->hide();

    timer = new QTimer(this);
    timer->setInterval(500);
    connect(timer ,SIGNAL(timeout()) ,this ,SLOT(selectItem()));

    connect(clear ,SIGNAL(finished(int)) ,this ,SLOT(dialogFinish(int)));
    connect(key ,SIGNAL(finished(int)) ,this ,SLOT(dialogFinish(int)));
    connect(setting ,SIGNAL(finished(int)) ,this ,SLOT(dialogFinish(int)));

    updateIcon();
}

void StartMenu::keyPressEvent(QKeyEvent *k)
{
    if(!k->isAutoRepeat()){
        k->accept();
        doFnPress(k->key());
    }
    else
        k->ignore();
}

void StartMenu::keyReleaseEvent(QKeyEvent *k)
{
    if(!k->isAutoRepeat()){
        k->accept();
        doFnRelease(k->key());
    }
    else
        k->ignore();
}

void StartMenu::mousePressEvent(QMouseEvent *m)
{
    int index = 0;
    int fram = ui->frame_1->height();
    index = m->y() / fram;

    switch(index){
    case 0:
        doFnPress(Qt::Key_F9);
        break;
    case 1:
        doFnPress(Qt::Key_F8);
        break;
    case 2:
        doFnPress(Qt::Key_F7);
        break;
    case 3:
        doFnPress(Qt::Key_F6);
        break;
    case 4:
        doFnPress(Qt::Key_F5);
        break;
    case 5:
        doFnPress(Qt::Key_F4);
        break;
    case 6:
        doFnPress(Qt::Key_F3);
        break;
    case 7:
        doFnPress(Qt::Key_F2);
        break;
    case 8:
        doFnPress(Qt::Key_F1);
        break;
    default:
        break;
    }
}

void StartMenu::mouseReleaseEvent(QMouseEvent *m)
{
    int index = 0;
    int fram = ui->frame_1->height();
    index = m->y() / fram;

    switch(index){
    case 0:
        doFnRelease(Qt::Key_F9);
        break;
    case 1:
        doFnRelease(Qt::Key_F8);
        break;
    case 2:
        doFnRelease(Qt::Key_F7);
        break;
    case 3:
        doFnRelease(Qt::Key_F6);
        break;
    case 4:
        doFnRelease(Qt::Key_F5);
        break;
    case 5:
        doFnRelease(Qt::Key_F4);
        break;
    case 6:
        doFnRelease(Qt::Key_F3);
        break;
    case 7:
        doFnRelease(Qt::Key_F2);
        break;
    case 8:
        doFnRelease(Qt::Key_F1);
        break;
    default:
        break;
    }
}

void StartMenu::doFnPress(int i)
{
    QPixmap pix;
    switch(i)
    {
    case Qt::Key_Down:
    case Qt::Key_Left:
        parentWidget()->setFocus();
        emit finish();
        break;
    case Qt::Key_Up:
    case Qt::Key_Right:
        parentWidget()->setFocus();
        emit finish();
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
        if(!timer->isActive()){
            timer->start();
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
        if(!timer->isActive()){
            timer->start();
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
        if(!timer->isActive()){
            timer->start();
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
        selectItem();
        updateSelect();
        break;
    case Qt::Key_F9:
        selected = 9;
        selectItem();
        updateSelect();
        break;
    case Qt::Key_Enter:
        selected = 0;
        updateSelect();
        parentWidget()->setFocus();
        emit finish();
        break;
    default:
        break;
    }
}

void StartMenu::doFnRelease(int i)
{
    if(timer->isActive()){
        timer->stop();
    }
    updateIcon();

    selected = 0;
    updateSelect();

    switch(i)
    {
    case Qt::Key_F1:
        break;
    case Qt::Key_F2:
        break;
    case Qt::Key_F3:
        break;
    case Qt::Key_F4:
        if(setting->isHidden()){
            setting->show();
            setting->setFocus();
        }
        else{
            setting->hide();
        }
        emit finish();
        break;
    case Qt::Key_F5:
        if(clear->isHidden()){
            clear->show();
            clear->setFocus();
        }
        else{
            clear->hide();
        }
        emit finish();
        break;
    case Qt::Key_F6:
        if(key->isHidden()){
            key->show();
            key->setFocus();
        }
        else{
            key->hide();
        }
        emit finish();
        break;
    case Qt::Key_F7:{
        parentWidget()->setFocus();
        emit finish();
#ifndef Q_WS_X11
        Calibration cal;
        cal.exec();
#endif
        }
        break;
    case Qt::Key_F8:
        break;
    case Qt::Key_F9:
        break;
    default :
        break;
    }
}

void StartMenu::updateSelect()
{
    ui->frame_1->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_2->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_3->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_4->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_5->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_6->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_7->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_8->setStyleSheet(UNSELECTED_STYLE);
    ui->frame_9->setStyleSheet(UNSELECTED_STYLE);

    switch(selected)
    {
    case 0:
        break;
    case 1:
        ui->frame_1->setStyleSheet(SELECTED_STYLE);
        break;
    case 2:
        ui->frame_2->setStyleSheet(SELECTED_STYLE);
        break;
    case 3:
        ui->frame_3->setStyleSheet(SELECTED_STYLE);
        break;
    case 4:
        ui->frame_4->setStyleSheet(SELECTED_STYLE);
        break;
    case 5:
        ui->frame_5->setStyleSheet(SELECTED_STYLE);
        break;
    case 6:
        ui->frame_6->setStyleSheet(SELECTED_STYLE);
        break;
    case 7:
        ui->frame_7->setStyleSheet(SELECTED_STYLE);
        break;
    case 8:
        ui->frame_8->setStyleSheet(SELECTED_STYLE);
        break;
    case 9:
        ui->frame_9->setStyleSheet(SELECTED_STYLE);
        break;
    default:
        break;
    }
    this->setFocus();
}

void StartMenu::selectItem()
{
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
        parentWidget()->setFocus();
        emit finish();
        break;
    case 2:
        if(spark_info->b_array[B_REBOOT]){
            spark_info->setBool(B_REBOOT ,false);
        }
        else{
            spark_info->setBool(B_REBOOT ,true);
        }
        parentWidget()->setFocus();
        emit finish();
        break;
    case 3:
        if(spark_info->b_array[B_SLEEP]){
            spark_info->setBool(B_SLEEP ,false);
        }
        else{
            spark_info->setBool(B_SLEEP ,true);
        }
        parentWidget()->setFocus();
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
        if(spark_info->uint_array[UINT_SOUND] > 2)
            spark_info->uint_array[UINT_SOUND] = 0;
        else
            spark_info->uint_array[UINT_SOUND]++;
        break;
    case 9:
        if(spark_info->uint_array[UINT_BRIGHTNESS] > 3)
            spark_info->uint_array[UINT_BRIGHTNESS] = 0;
        else
            spark_info->uint_array[UINT_BRIGHTNESS]++;
        break;
    default:
        break;
    }
    if(timer->isActive()){
        timer->stop();
        selected = 0;
        updateSelect();
    }
}

void StartMenu::updateIcon()
{
    QPixmap pix;

    pix = QPixmap(":/shut.png");
    ui->label_shut->setPixmap(pix);

    pix = QPixmap(":/restart.png");
    ui->label_restart->setPixmap(pix);

    pix = QPixmap(":/setting.png");
    ui->label_setting->setPixmap(pix);

    pix = QPixmap(":/sleep.png");
    ui->label_sleep->setPixmap(pix);

    if(spark_info->checkTime())
        pix = QPixmap(":/password.png");
    else
        pix = QPixmap(":/unlock.png");
    ui->label_password->setPixmap(pix);

    pix = QPixmap(":/clear.png");
    ui->label_clear->setPixmap(pix);

    pix = QPixmap(":/calibration.png");
    ui->label_calibration->setPixmap(pix);

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

    if(spark_info->uint_array[UINT_SOUND] == 0){
        pix = QPixmap(":/sound_0.png");
        ui->label_sound->setPixmap(pix);
    }
    else if(spark_info->uint_array[UINT_SOUND] == 1){
        pix = QPixmap(":/sound_1.png");
        ui->label_sound->setPixmap(pix);
    }
    else if(spark_info->uint_array[UINT_SOUND] == 2){
        pix = QPixmap(":/sound_2.png");
        ui->label_sound->setPixmap(pix);
    }
    else if(spark_info->uint_array[UINT_SOUND] ==3){
        pix = QPixmap(":/sound_3.png");
        ui->label_sound->setPixmap(pix);
    }

    if(spark_info->uint_array[UINT_BRIGHTNESS] == 0){
        pix = QPixmap(":/brightness_0.png");
        ui->label_brightness->setPixmap(pix);
    }
    else if(spark_info->uint_array[UINT_BRIGHTNESS] == 1){
        pix = QPixmap(":/brightness_1.png");
        ui->label_brightness->setPixmap(pix);
    }
    else if(spark_info->uint_array[UINT_BRIGHTNESS] == 2){
        pix = QPixmap(":/brightness_2.png");
        ui->label_brightness->setPixmap(pix);
    }
    else if(spark_info->uint_array[UINT_BRIGHTNESS] == 3){
        pix = QPixmap(":/brightness_3.png");
        ui->label_brightness->setPixmap(pix);
    }
    else if(spark_info->uint_array[UINT_BRIGHTNESS] == 4){
        pix = QPixmap(":/brightness_4.png");
        ui->label_brightness->setPixmap(pix);
    }
}

void StartMenu::dialogFinish(int i)
{
    if(!clear->isHidden())
        clear->hide();
    if(!setting->isHidden())
        setting->hide();
    if(!key->isHidden())
        key->hide();
    if(i == 0)
        parentWidget()->setFocus();
    else
        parentWidget()->setFocus();
}

StartMenu::~StartMenu()
{
    delete ui;
}
