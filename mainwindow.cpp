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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer::positionChanged, this , &MainWindow::on_positionChange);
    connect(player, &QMediaPlayer::durationChanged, this , &MainWindow::on_durationChange);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this,&MainWindow::onListItemDoubleClicked);
//    path ="/home/yash/Music/SONGS";
//    loadSongs(path);

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
}
void MainWindow::on_positionChange(qint64 position)
{
    ui->playbackBar->setValue(position);
    QString currDur=QDateTime::fromTime_t(position/1000).toUTC().toString("hh:mm:ss");
    QString ts=currDur+"/"+duration;
    ui->timeStamp->setText(ts);
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
    player->pause();
}
void MainWindow::loadSongs(QString path)
{
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

    QString song= path+"/"+i->text();
    player->setMedia(QUrl::fromLocalFile(song));
    player->play();
}
