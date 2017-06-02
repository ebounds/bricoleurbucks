#include "bricoleurdb.h"
#include "ui_bricoleurdb.h"
#include "walletmodel.h"
#include "bricoleurourpage.h"
#include "notarypage.h"

BricoleurDB::ClamDB(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BricoleurDB),
    model(0),
    bricoleurourPage(0),
    notaryPage(0)
{
    ui->setupUi(this);

    notaryPage = new NotaryPage();
    ui->stackedWidget->addWidget(this->notaryPage);
    ui->pageList->addItem("Notary");

    bricoleurourPage = new BricoleurourPage();
    ui->stackedWidget->addWidget(this->bricoleurourPage);
    ui->pageList->addItem("BRICour");

    connect(ui->pageList, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

BricoleurDB::~ClamDB()
{
    delete ui;
}

void BricoleurDB::setModel(WalletModel *model)
{
    this->model = model;
    this->bricoleurourPage->setModel(model);
    this->notaryPage->setModel(model);
    ui->pageList->setCurrentRow(0);
}

