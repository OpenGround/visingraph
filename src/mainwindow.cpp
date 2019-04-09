#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWidget = new MainWidget;
    connect(ui->continueCalcAction, SIGNAL(triggered()), mainWidget, SLOT(loadCalculationState()));
    setCentralWidget(mainWidget);

}

MainWindow::~MainWindow()
{
    delete mainWidget;
    delete ui;
}
