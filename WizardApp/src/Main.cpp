#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

/*#ifdef WIN32
    #include <QtPlugin>
    Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#endif*/

#include <include/QtMainWindow.hpp>
#include <include/BuildManager.h>

char str[512];

int main(int argc, char **argv) 
{
	QApplication app(argc, argv);
	
	QSplashScreen splash(QPixmap(":/splash.gif"));
	splash.show();
	
	mainWindow = new MainWindow();
	
	QTimer::singleShot(3000, mainWindow, 
		[&]()
		{
			splash.finish(mainWindow);
			mainWindow->show();
		}
	);
	
	return app.exec();
}

void doWork(std::string gamePath, std::string outputPath, bool SAMPObjs, bool CustomObjs, std::string ide, std::string ipl, std::string img)
{
    BuildManager *thisBuild = new BuildManager(gamePath, outputPath, SAMPObjs, CustomObjs, ide, ipl, img);
	
    mainWindow->reportProgress(1, "Обработка: img-архивы (.img)");
	thisBuild->ExtractImageFiles();
    mainWindow->reportProgress(15, "Готово.\n");

    mainWindow->reportProgress(1, "Обработка: файлы коллизий (.col)");
	thisBuild->ExtractCollisionFiles();
    mainWindow->reportProgress(15, "Готово.\n");

    mainWindow->reportProgress(1, "Обработка: файлы размещения объектов (.ipl)");
	thisBuild->ExtractItemPlacementFiles();
    mainWindow->reportProgress(15, "Готово.\n");

    mainWindow->reportProgress(1, "Обработка: файлы объявления объектов (.ide)");
	thisBuild->ExtractItemDefinitionFiles();
    mainWindow->reportProgress(15, "Готово.\n");

    mainWindow->reportProgress(1, "Сборка структуры базы данных, это может занять время...");
    thisBuild->PrepareDatabaseStructures();
    mainWindow->reportProgress(15, "Готово.\n");

    mainWindow->reportProgress(1, "Запись базы данных...");
	thisBuild->WriteBinaryFile(outputPath.c_str());
    mainWindow->reportProgress(15, "Готово.\n");
	
    mainWindow->addMessage("Всё готово!");
	mainWindow->setProgress(100);
}
