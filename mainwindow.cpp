#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cglightmodel.h"
#include "cconfigdatamodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // port table setup
    ui->portTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->portTable->setDragEnabled(true);
    ui->portTable->setAcceptDrops(true);
    ui->portTable->setDropIndicatorShown(true);

    CGLightModel* model = new CGLightModel(this);
    ui->portTable->setModel(model);

    // table setup
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setDragEnabled(true);
    ui->tableView->setAcceptDrops(true);
    ui->tableView->setDropIndicatorShown(true);

    CConfigDataModel* configModel = new CConfigDataModel(this);
    ui->tableView->setModel(configModel);


}

MainWindow::~MainWindow()
{
    delete ui;
}

