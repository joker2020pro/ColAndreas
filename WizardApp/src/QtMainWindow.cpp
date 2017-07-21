
#ifdef WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <pwd.h>
#endif

#include <cstdarg>

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDesktopServices>

#include <include/Main.hpp>
#include <include/QtMainWindow.hpp>

#include <ui_MainWindow.h>
#include <ui_License.h>
#include <ui_Settings.h>
#include <ui_Working.h>

MainWindow *mainWindow = NULL;

MainWindow::MainWindow() : QMainWindow()
{
	// Initialize MainWindow
	
	ui = new Ui::MainWindow();
	ui->setupUi(this);
	
	this->setWindowIcon(QIcon(":/icon.ico"));
    this->setWindowTitle("ColAndreas Wizard (CR-MP 0.3e)");
	
	ui->headerImage->setPixmap(QPixmap(":/header.gif"));
	ui->statusBar->hide();
	ui->buttonBack->hide();
	ui->buttonNext->setEnabled(false);
	
	QPalette headerPalette(palette());
	headerPalette.setColor(QPalette::Background, Qt::black);
	ui->headerWidget->setAutoFillBackground(true);
	ui->headerWidget->setPalette(headerPalette);
	
	this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter,	this->size(), qApp->desktop()->availableGeometry()));
	
	// Initialize licenseWidget
	
	licenseWidget = new QWidget(this);
	ui_license = new Ui::licenseWidget();
	ui_license->setupUi(licenseWidget);
	
	QFile licenseFile(":/license.html");
	licenseFile.open(QIODevice::ReadOnly);
	ui_license->licenseContainer->setHtml(licenseFile.readAll());
	
	ui->contentLayout->addWidget(licenseWidget);
	
	// Initialize settingsWidget
	
	settingsWidget = new QWidget(this);
	ui_settings = new Ui::settingsWidget();
	ui_settings->setupUi(settingsWidget);
	settingsWidget->hide();
	
	#ifdef WIN32
    /*HKEY key;
    if(RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Rockstar Games\\GTA San Andreas\\Installation", REG_OPTION_OPEN_LINK, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS)
	{
		DWORD size = MAX_PATH;
		char path[MAX_PATH];

        if(RegQueryValueExA(key, "ExePath", NULL, NULL, (BYTE*) path, &size) == ERROR_SUCCESS)
		{
			gamePath.append(path);
			gamePath.remove('"');
			
			if(gamePath.endsWith('/') || gamePath.endsWith('\\'))
			{
				gamePath.chop(1);
			}
			
			if(!QFile::exists(gamePath + "/gta_sa.exe"))
			{
				gamePath = "";
			}
		}

		RegCloseKey(key);
	}
    ui_settings->gamePathDisplay->setText(gamePath);*/
	#endif
	
	ui->contentLayout->addWidget(settingsWidget);
	
	// Initialize workWidget
	
	workWidget = new QWidget(this);
	ui_work = new Ui::workWidget();
	ui_work->setupUi(workWidget);
	workWidget->hide();
	ui->contentLayout->addWidget(workWidget);
	
	// Initialize timer
	
	threadTimer = new QTimer(this);
	threadTimer->setInterval(250);
	threadTimer->setSingleShot(false);
	
	// Set-up signals

    modpackCheckBox(false);
	
	connect(ui->buttonNext, &QPushButton::released, this, &MainWindow::next);
	connect(ui->buttonBack, &QPushButton::released, this, &MainWindow::back);
	connect(ui->buttonCancel, &QPushButton::released, this, &MainWindow::cancel);
	
	connect(threadTimer, &QTimer::timeout, this, &MainWindow::updateThread);
	connect(ui_license->licenseAcceptRadio, &QRadioButton::toggled, this, &MainWindow::acceptsLicense);
	connect(ui_settings->gameBrowseButton, &QPushButton::released, this, &MainWindow::browseGame);
	connect(ui_settings->outputBrowseButton, &QPushButton::released, this, &MainWindow::browseOutput);

    connect(ui_settings->customObjectsBox, &QCheckBox::toggled, this, &MainWindow::modpackCheckBox);
    connect(ui_settings->mp_ide_browse_btn, &QPushButton::released, this, &MainWindow::browseModPackIde);
    connect(ui_settings->mp_ipl_browse_btn, &QPushButton::released, this, &MainWindow::browseModPackIpl);
    connect(ui_settings->mp_img_browse_btn, &QPushButton::released, this, &MainWindow::browseModPackImg);

    connect(ui_settings->link_dl_mp, &QPushButton::released, this, &MainWindow::linkDownloadMultiplayer);
    connect(ui_settings->link_dl_mpide, &QPushButton::released, this, &MainWindow::linkDownloadMultiplayerIde);
}

MainWindow::~MainWindow()
{ }

void MainWindow::beginWork()
{
    if(ui_settings->customObjectsBox->isChecked())
    {
        addFormattedMessage("Начало обработки:\n\tдиректория игры = \"%s\"\n\tвыходной файл = \"%s\"\n\tSA-MP объекты = %s\n\tмод-пак = %s\n\tIDE-файл мод-пака = %s\n\tIPL-файл мод-пака = %s\n\tIMG-файл мод-пака = %s\n\n", gamePath.toLocal8Bit().data(), outputPath.toLocal8Bit().data(), (ui_settings->sampObjectsBox->isChecked() ? "да" : "нет"), (ui_settings->customObjectsBox->isChecked() ? "да" : "нет"), mpIdePath.toLocal8Bit().data(), mpIplPath.toLocal8Bit().data(), mpImgPath.toLocal8Bit().data());
    }
    else
    {
        addFormattedMessage("Начало обработки:\n\tдиректория игры = \"%s\"\n\tвыходной файл = \"%s\"\n\tSA-MP объекты = %s\n\tмод-пак = %s\n\n", gamePath.toLocal8Bit().data(), outputPath.toLocal8Bit().data(), (ui_settings->sampObjectsBox->isChecked() ? "да" : "нет"), (ui_settings->customObjectsBox->isChecked() ? "да" : "нет"));
    }

    updateThread();
	
	threadTimer->start();
    workerThread = new std::thread(doWork, gamePath.toStdString(), outputPath.toStdString(), ui_settings->sampObjectsBox->isChecked(), ui_settings->customObjectsBox->isChecked(), mpIdePath.toStdString(), mpIplPath.toStdString(), mpImgPath.toStdString());
}

void MainWindow::addMessage(const char *msg)
{
	std::lock_guard<std::mutex> lock(workerMutex);
	
	workMessages.push(new std::string(msg));
}

void MainWindow::addFormattedMessage(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	
    vsnprintf(str, sizeof(str), format, args);
	workMessages.push(new std::string(str));
	
	va_end(args);
}

void MainWindow::setProgress(int progress)
{
	std::lock_guard<std::mutex> lock(workerMutex);
	
	workProgress = progress;
}

void MainWindow::reportProgress(int progress, const char* msg)
{
	std::lock_guard<std::mutex> lock(workerMutex);
	
	workProgress += progress;
	workMessages.push(new std::string(msg));
}

void MainWindow::updateThread()
{
	if(workerMutex.try_lock())
	{
		if(workProgress)
		{
			ui_work->workProgress->setMaximum(100);
			ui_work->workProgress->setValue(workProgress);
		}
			
		while(!workMessages.empty())
		{
			std::string *s = workMessages.front();
		
			ui_work->workMessages->appendPlainText(s->c_str());
			delete s;

			workMessages.pop();
		}
		
		if(workProgress == 100)
		{
			ui->buttonNext->hide();
            ui->buttonCancel->setText(tr("Выйти"));
			
			threadTimer->stop();
			
			workerThread->join();
			delete workerThread;
			workerThread = NULL;
		}
		
		workerMutex.unlock();
	}
}

void MainWindow::hideAll()
{
	licenseWidget->hide();
	settingsWidget->hide();
}

void MainWindow::showCurrent()
{
	ui->buttonBack->show();
    ui->buttonNext->setText(tr("Далее"));
	
	switch(currentPage)
	{
		case 0:
		{
			licenseWidget->show();
			ui->buttonBack->hide();
			break;
		}
		
		case 1:
		{
			settingsWidget->show();
            ui->buttonNext->setText(tr("Начать"));
			break;
		}
		
		case 2:
		{
			workWidget->show();
			ui->buttonBack->hide();
			ui->buttonNext->setEnabled(false);
			beginWork();
			break;
		}
	}
}

void MainWindow::next()
{
	if(currentPage == 1)
	{
		if(!gamePath.length())
		{
            QMessageBox::critical(this, tr("ОШИБКА"), tr("Пожалуйста, укажите директорию с установленной GTA Criminal Russia"));
			return;
		}
		
		if(!outputPath.length())
		{
            QMessageBox::critical(this, tr("ОШИБКА"), tr("Пожалуйста, укажите путь, куда сохранить файл базы ColAndreas"));
			return;
		}

        if(ui_settings->sampObjectsBox->isChecked())
        {
            if(!QFile::exists(gamePath + "/multiplayer_c/models/mp.ide"))
            {
                QMessageBox::critical(this, tr("ОШИБКА"), tr("Пожалуйста, либо перенесите файл mp.ide в папку multiplayer_c/models/, либо снимите галочку \"Подключить SA-MP объекты\""));
                return;
            }
        }

        if(ui_settings->customObjectsBox->isChecked())
        {
            if(!mpIdePath.length() || !mpIplPath.length() || !mpImgPath.length())
            {
                QMessageBox::critical(this, tr("ОШИБКА"), tr("Пожалуйста, либо укажите пути для всех файлов мод-пака, либо снимите галочку \"Подключить объекты из мод-пака\""));
                return;
            }
        }
	}
	
	hideAll();
	currentPage++;
	showCurrent();
}

void MainWindow::back()
{
	hideAll();
	currentPage--;
	showCurrent();
}

void MainWindow::cancel()
{
	QApplication::exit();
}

void MainWindow::acceptsLicense(bool accepts)
{
	if(accepts)
	{
		ui->buttonNext->setEnabled(true);
	}
	else
	{
		ui->buttonNext->setEnabled(false);
	}
}

void MainWindow::browseGame()
{
    QString selectedDir = QFileDialog::getExistingDirectory(this, tr("Укажите директорию с GTA Criminal Russia"), QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	if(selectedDir.length())
	{
		if(selectedDir.endsWith('/') || selectedDir.endsWith('\\'))
		{
			selectedDir.chop(1);
		}
		
        if(!QFile::exists(selectedDir + "/models/gamemod.img"))
		{
            QMessageBox::critical(this, tr("ОШИБКА"), tr("В указанной папке не найдена установленная GTA Criminal Russia, либо её файлы повреждены"));
		}
		else
		{
			gamePath = selectedDir;
			ui_settings->gamePathDisplay->setText(gamePath);
		}
	}
}

void MainWindow::browseOutput()
{
    QString selectedFile = QFileDialog::getSaveFileName(this, tr("Укажите, куда сохранить файл базы ColAndreas"), QDir::homePath(), tr("ColAndreas database file (*.cadb)"));
	
	if(selectedFile.length())
	{
		outputPath = selectedFile;
		ui_settings->outputPathDisplay->setText(outputPath);
	}
}

void MainWindow::modpackCheckBox(bool enable)
{
    ui_settings->mp_ide_browse_btn->setVisible(enable);
    ui_settings->mp_ide_label->setVisible(enable);
    ui_settings->mp_ide_path->setVisible(enable);

    ui_settings->mp_ipl_browse_btn->setVisible(enable);
    ui_settings->mp_ipl_label->setVisible(enable);
    ui_settings->mp_ipl_path->setVisible(enable);

    ui_settings->mp_img_browse_btn->setVisible(enable);
    ui_settings->mp_img_label->setVisible(enable);
    ui_settings->mp_img_path->setVisible(enable);
}

void MainWindow::browseModPackIde()
{
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("Выберите IDE-файл мод-пака:"), QDir::homePath(), tr("IDE-file (*.ide)"));

    if(selectedFile.length())
    {
        mpIdePath = selectedFile;

        ui_settings->mp_ide_path->setText(mpIdePath);
    }
}

void MainWindow::browseModPackIpl()
{
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("Выберите IPL-файл мод-пака:"), QDir::homePath(), tr("IPL-file (*.ipl)"));

    if(selectedFile.length())
    {
        mpIplPath = selectedFile;

        ui_settings->mp_ipl_path->setText(mpIplPath);
    }
}

void MainWindow::browseModPackImg()
{
    QString selectedFile = QFileDialog::getOpenFileName(this, tr("Выберите IMG-файл мод-пака:"), QDir::homePath(), tr("IMG-file (*.img)"));

    if(selectedFile.length())
    {
        mpImgPath = selectedFile;

        ui_settings->mp_img_path->setText(mpImgPath);
    }
}

void MainWindow::linkDownloadMultiplayer()
{
    QDesktopServices::openUrl(QUrl("http://www.gtasrv.ru/download/multiplayer-cr-g-install.exe"));
}

void MainWindow::linkDownloadMultiplayerIde()
{
    QDesktopServices::openUrl(QUrl("http://www.gtasrv.ru/readme/resources/multiplayer.ide"));
}
