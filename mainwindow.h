#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMediaPlayer>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void on_durationChange(qint64);
    void on_positionChange(qint64);
    void loadSongs(QString);
    void onListItemDoubleClicked(QListWidgetItem*);
    void doDownload(QString);
    QNetworkReply *reply;
    bool isPause;
    bool isMuted;
    QString path;
    QString duration;
    QString QfileName;
    QMediaPlayer *player;
    QListWidgetItem* curridx;
    QNetworkAccessManager *manager;
    ~MainWindow();

private slots:
    void on_playbackBar_sliderMoved(int position);

    void on_volumeBar_sliderMoved(int position);

    void on_pauseButton_clicked();

    void on_actionSet_Songs_Path_triggered();

    void on_actionCurrent_Working_Path_triggered();

    void on_nextButton_clicked();

    void on_previousButton_clicked();

    void on_volumeButton_clicked();

    void replyFinished (QNetworkReply *reply);
    void on_downloadButton_clicked();

    void on_SetPath_clicked();

    void on_Refresh_clicked();

    void on_actionRefresh_List_triggered();

    void on_actionAbout_triggered();

    void downloadProgress(qint64, qint64);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
