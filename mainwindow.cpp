#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QMediaPlayer>
#include <QListWidget>
#include <QDirIterator>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
//youtube-dl -x --get-url --format=bestaudio https://www.youtube.com/watch?v=pxcRWdGKSko&ab_channel=T-Series
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer::positionChanged, this , &MainWindow::on_positionChange);
    connect(player, &QMediaPlayer::durationChanged, this , &MainWindow::on_durationChange);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this,&MainWindow::onListItemDoubleClicked);
    path ="/home/yash/Music/SONGS";
    loadSongs(path);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_playbackBar_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_volumeBar_sliderMoved(int position)
{
    player->setVolume(position);
    if(ui->volumeBar->value()>2)
    {
        isMuted=false;
        ui->volumeButton->setIcon(QIcon(":/new/icons/icons/volume.png"));
    }
    else
    {
        isMuted=true;
        ui->volumeButton->setIcon(QIcon(":/new/icons/icons/mute.png"));
    }
}
void MainWindow::on_positionChange(qint64 position)
{
    ui->playbackBar->setValue(position);
    QString currDur=QDateTime::fromTime_t(position/1000).toUTC().toString("hh:mm:ss");
    QString ts=currDur+"/"+duration;
    ui->timeStamp->setText(ts);
    if(currDur==duration)
    {
        qDebug() << "done!";
        MainWindow::on_nextButton_clicked();
    }

}
void MainWindow::on_durationChange(qint64 position)
{
    ui->playbackBar->setMaximum(position);
    duration=QDateTime::fromTime_t(position/1000).toUTC().toString("hh:mm:ss");
    QString ts="00:00:00/"+duration;
    ui->timeStamp->setText(ts);
}

void MainWindow::on_pauseButton_clicked()
{
    if(!isPause)
    {
        //not paused
        ui->pauseButton->setIcon(QIcon(":/new/icons/icons/play.jpg"));
        player->pause();
        isPause=true;
    }
    else
    {
        ui->pauseButton->setIcon(QIcon(":/new/icons/icons/pause.png"));
        isPause=false;
        player->play();
    }
}
void MainWindow::loadSongs(QString path)
{
    player->stop();
    ui->listWidget->clear();
    QDirIterator it(path, {"*.mp3", "*.wav"}, QDir::Files);
    while (it.hasNext())
    {
        qDebug() << it.next();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setIcon(QIcon(":/new/icons/icons/play.jpg"));
        newItem->setText(it.fileName());
        ui->listWidget->addItem(newItem);
    }
}

void MainWindow::on_actionSet_Songs_Path_triggered()
{
    path = QFileDialog::getExistingDirectory(this,"Choose The Path");
    loadSongs(path);
}

void MainWindow::on_actionCurrent_Working_Path_triggered()
{
    QString message="The Current Path is Set to " +path;
    QMessageBox::information(this,"Current Set Path is",message);
}
void MainWindow::onListItemDoubleClicked(QListWidgetItem* i)
{
//    qDebug()<<ui->listWidget->row(i);
    curridx=i;

    QString song= path+"/"+i->text();
    player->setMedia(QUrl::fromLocalFile(song));
    player->play();
    isPause=false;
    ui->pauseButton->setIcon(QIcon(":/new/icons/icons/pause.png"));

}

void MainWindow::on_nextButton_clicked()
{
    QListWidgetItem *newItem;
    qDebug()<<ui->listWidget->count()<< " "<<ui->listWidget->row(curridx);
    if(ui->listWidget->row(curridx)+1==ui->listWidget->count())
    {
        newItem = ui->listWidget->item(0);
        curridx=newItem;
    }
    else
    {
        newItem = ui->listWidget->item(ui->listWidget->row(curridx)+1);
        curridx=newItem;
    }
    ui->listWidget->setItemSelected(curridx,true);
    if(newItem==NULL)
    {
        qDebug()<<"end of queue";
    }
    else
    {
    QString song= path+"/"+newItem->text();
    player->setMedia(QUrl::fromLocalFile(song));
    player->play();
    }
}

void MainWindow::on_previousButton_clicked()

{
    QListWidgetItem *newItem;
    qDebug()<<ui->listWidget->count()<< " "<<ui->listWidget->row(curridx);
    if(ui->listWidget->row(curridx)==0)
    {
        newItem = ui->listWidget->item(ui->listWidget->count() -1);
        curridx=newItem;
    }
    else
    {
        newItem = ui->listWidget->item(ui->listWidget->row(curridx)-1);
        curridx=newItem;
    }
    ui->listWidget->setItemSelected(curridx,true);
    if(newItem==NULL)
    {
        qDebug()<<"end of queue";
    }
    else
    {
    QString song= path+"/"+newItem->text();
    player->setMedia(QUrl::fromLocalFile(song));
    player->play();
    }
}

void MainWindow::on_volumeButton_clicked()
{
    if(isMuted)
    {
        isMuted=false;
        player->setVolume(100);
        ui->volumeBar->setValue(100);
        ui->volumeButton->setIcon(QIcon(":/new/icons/icons/volume.png"));
    }
    else
    {
        isMuted=true;
        player->setVolume(0);
        ui->volumeBar->setValue(0);
        ui->volumeButton->setIcon(QIcon(":/new/icons/icons/mute.png"));
    }
}
