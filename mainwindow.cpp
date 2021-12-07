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
#include<string>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>
//youtube-dl -x --get-url --format=bestaudio https://www.youtube.com/watch?v=pxcRWdGKSko&ab_channel=T-Series
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    manager = new QNetworkAccessManager(this);

    connect(player, &QMediaPlayer::positionChanged, this , &MainWindow::on_positionChange);
    connect(player, &QMediaPlayer::durationChanged, this , &MainWindow::on_durationChange);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this,&MainWindow::onListItemDoubleClicked);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));
//    connect(manager, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(getDownloadData(qint64, qint64)) );
//    path ="/home/yash/Music/SONGS";
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
    QDirIterator it(path, {"*.mp3", "*.wav","*.webm"}, QDir::Files);
    while (it.hasNext())
    {
//        qDebug() << it.next();
        it.next();
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setIcon(QIcon(":/new/icons/icons/play.jpg"));
        newItem->setText(it.fileName());
        ui->listWidget->addItem(newItem);
    }
    ui->listWidget->sortItems(Qt::AscendingOrder);
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
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
void MainWindow::doDownload(QString link)
{
    qDebug()<<"doDownload";
    reply=manager->get(QNetworkRequest(QUrl(link)));
    connect(reply , SIGNAL(downloadProgress(qint64,qint64)) , this , SLOT(downloadProgress(qint64,qint64)));

}
void MainWindow::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
        QMessageBox::critical(this,"ERROR!","The link you gave is either incorrect or The given song can not be downloaded");
        ui->linkOrName->setText("");
    }

    else
    {
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        QString Path=path+"/"+QfileName;
        QFile *file = new QFile(Path);
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
            QMessageBox::information(this,"SUCCESS!","The song "+QfileName+" is Downloaded and can be seen in the Set Path and below List");
            ui->linkOrName->setText("");
            loadSongs(path);
            ui->downProgressBar->setValue(0);
        }
        delete file;
    }

    reply->deleteLater();
}
std::string process(std::string const& s)
{
    std::string::size_type pos = s.find('\n');
    if (pos != std::string::npos)
    {
        return s.substr(0, pos);
    }
    else
    {
        return s;
    }
}
void MainWindow::on_downloadButton_clicked()
{
    std::string l="";
    std::string n="";
    if(ui->byLink->isChecked())
    {
        l=l+ "youtube-dl -x --get-url --format=bestaudio ";
        n= n+"youtube-dl -x --get-filename --format=bestaudio ";
    }
    else
    {
        l=l+ "youtube-dl --get-url --format=bestaudio --default-search ytsearch1:";
         n= n+"youtube-dl -x --get-filename --format=bestaudio --default-search ytsearch1:";
    }
    l=l+ (ui->linkOrName->text()).toStdString();
    n=n+ (ui->linkOrName->text()).toStdString();
    const char *commandForLink= l.c_str();
    const char *commandForName= n.c_str();

    qDebug()<<QString::fromStdString(commandForLink);

    std::string link= exec(commandForLink);
    link=process(link);
    std::string fileName=exec(commandForName);
    fileName=process(fileName);

    QString Qlink=QString::fromStdString(link);
    QfileName=QString::fromStdString(fileName);

    qDebug() <<Qlink;
    qDebug() <<QfileName;
    QMessageBox::information(this,"Downloading","Your file with file name "+QfileName+" is being downloaded you will be notified when it is done!");
    doDownload(Qlink);
}

void MainWindow::on_SetPath_clicked()
{
    path = QFileDialog::getExistingDirectory(this,"Choose The Path");
    loadSongs(path);
}

void MainWindow::on_Refresh_clicked()
{
    loadSongs(path);
}

void MainWindow::on_actionRefresh_List_triggered()
{
    loadSongs(path);
}

void MainWindow::on_actionAbout_triggered()
{
     QMessageBox::information(this,"About","Made by Yash Gupta\n\nLinkedin: https://www.linkedin.com/in/yash-gupta-56432a20a/ \n\nGithub: https://github.com/YASH-GU24 \n\nSource Code & Other info: https://github.com/YASH-GU24/Audio_Player \n\nLatest Release Date:07-12-2021");
}
void MainWindow::downloadProgress(qint64 ist, qint64 max)
{
    if(max>0)
    {
        ui->downProgressBar->setRange(0,max);
        ui->downProgressBar->setValue(ist);
    }
}
