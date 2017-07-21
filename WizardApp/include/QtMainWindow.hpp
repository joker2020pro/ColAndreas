#pragma once

#include <thread>
#include <mutex>
#include <queue>

#include <QMainWindow>
#include <QString>
#include <QTimer>

namespace Ui
{
	class MainWindow;
	class licenseWidget;
	class settingsWidget;
	class workWidget;
}

class MainWindow : public QMainWindow
{
public:
	MainWindow();
	~MainWindow();
	
	void beginWork();
	void addMessage(const char *msg);
	void addFormattedMessage(const char *format, ...);
	void setProgress(int progress);
	void reportProgress(int progress, const char *msg);
	void updateThread();
	
	void hideAll();
	void showCurrent();
	
	void next();
	void back();
	void cancel();
	
	void acceptsLicense(bool accepts);
	void browseGame();
	void browseOutput();

    void modpackCheckBox(bool enable);
    void browseModPackIde();
    void browseModPackIpl();
    void browseModPackImg();

    void linkDownloadMultiplayer();
    void linkDownloadMultiplayerIde();
	
private:
	Ui::MainWindow *ui = NULL;
	Ui::licenseWidget *ui_license = NULL;
	Ui::settingsWidget *ui_settings = NULL;
	Ui::workWidget *ui_work = NULL;
	
	QWidget *licenseWidget = NULL;
	QWidget *settingsWidget = NULL;
	QWidget *workWidget = NULL;
	QTimer *threadTimer = NULL;
	
	QString gamePath;
	QString outputPath;

    QString mpIdePath;
    QString mpIplPath;
    QString mpImgPath;
	
	std::thread *workerThread = NULL;
	std::mutex workerMutex;
	
	unsigned int workProgress = 0;
	std::queue<std::string*> workMessages;
	
	unsigned int currentPage = 0;
};

extern MainWindow *mainWindow;
