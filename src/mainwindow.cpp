#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    ui->menuGame->menuAction()->setVisible(false); //hide gameview topbar menu option

    stackedWidget = ui->stackedWidget;
    stackedWidget->setCurrentIndex(0);
    //topbar connections
    connect(ui->actionFullscreen, &QAction::triggered, this, &MainWindow::fullscreen);
    connect (ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);

    //Main menu button connections
    connect(ui->startGameButton, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
    connect(ui->quitButton, &QPushButton::clicked, this, &MainWindow::quit);

    //settings menu button connections
    connect(ui->exitSettingsButton, &QPushButton::clicked, this, &MainWindow::backToMenu);

    //gameview button connections
    connect(ui->gameToMenuButton, &QPushButton::clicked, this, &MainWindow::backToMenu);
}

void MainWindow::fullscreen() //Fullscreen button functionality
{
        if (windowState() & Qt::WindowFullScreen) {
            // If the window is already fullscreen, exit fullscreen
            setWindowState(Qt::WindowMaximized);
        } else {
            // Otherwise, enter fullscreen
            setWindowState(Qt::WindowFullScreen);
        }
}

void MainWindow::switchToPage(int pageIndex)
{
    stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::backToMenu()
{
    ui->menuGame->menuAction()->setVisible(false);
    switchToPage(0);
}

void MainWindow::startGame()
{
    ui->menuGame->menuAction()->setVisible(true);
    switchToPage(2);
}

void MainWindow::openSettings()
{
    switchToPage(1);
}

void MainWindow::quit()
{
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
