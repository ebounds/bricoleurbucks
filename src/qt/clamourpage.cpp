#include "bricoleurourpage.h"
#include "ui_bricoleurourpage.h"
#include "openssl/sha.h"
#include "bricoleurspeech.h"
#include "main.h"
#include "util.h"
#include "walletmodel.h"
#include "bricoleurourpetitionmodel.h"
#include "bricoleuroursupportmodel.h"

#include <QDebug>
#include <QMessageBox>
#include <QMenu>

BricoleurourPage::ClamourPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BricoleurourPage),
    model(0)
{
    ui->setupUi(this);
    ui->createPetitionButton->setEnabled(false);
    ui->setVoteCheckBox->setEnabled(false);

    petitionModel = new BricoleurourPetitionModel(this);
    ui->searchBricoleurourView->setModel(petitionModel);

    // Context menu for petition support view
    QAction *searchPetitionIDAction = new QAction(tr("Search for petition"), this);
    petitionViewContextMenu = new QMenu();
    petitionViewContextMenu->addAction(searchPetitionIDAction);
    connect(searchPetitionIDAction, SIGNAL(triggered()), this, SLOT(searchHighlightedPetition()));

    supportModel = new BricoleurourSupportModel(this);
    ui->petitionSupportView->setModel(supportModel);
    connect(ui->petitionSupportView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(searchHighlightedPetition()));
}

BricoleurourPage::~ClamourPage()
{
    delete ui;
}

// Calculate notary ID when text changes.
void BricoleurourPage::on_createPetitionEdit_textChanged()
{
    std::string petitionText(ui->createPetitionEdit->toPlainText().toStdString());
    if (petitionText.length() == 0)
    {
        ui->petitionIDEdit->clear();
        ui->createPetitionButton->setEnabled(false);
        ui->setVoteCheckBox->setEnabled(false);
        return;
    }
    ui->createPetitionButton->setEnabled(true);
    ui->setVoteCheckBox->setEnabled(true);
    std::string petitionHash(StrToSHA256(petitionText));
    ui->petitionIDEdit->setText(QString::fromStdString(petitionHash));
}

// Create a tx that creates a petitition
void BricoleurourPage::on_createPetitionButton_clicked()
{
    std::string petitionHash(ui->petitionIDEdit->text().toStdString());

    WalletModel::UnlockContext ctx(model->requestUnlock());
    if (!ctx.isValid()) {
        return;
    }

    model->sendBricoleurourTx(petitionHash);

    if (ui->setVoteCheckBox->isChecked())
    {
        strDefaultStakeSpeech = "bricoleurour " + petitionHash.substr(0, 8);
        bricoleurourBricoleurSpeech.push_back(strDefaultStakeSpeech);
        qDebug() << "saving bricoleurour petitions";
        if ( !SaveBricoleurourClamSpeech() )
            qDebug() << "Bricoleurour BRICSpeech petitions FAILED to save!";
        loadVotes();
    }
}

void BricoleurourPage::on_setVotesButton_clicked()
{
    saveVotes();
}

void BricoleurourPage::loadVotes()
{
    QStringList list;
    for (std::vector<std::string>::iterator it = bricoleurourBricoleurSpeech.begin(); it != clamourClamSpeech.end(); ++it)
    {
        list.append(QString::fromStdString(*it).mid(8));
    }
    ui->votesEdit->setPlainText(list.join("\n"));
}

void BricoleurourPage::saveVotes()
{
    QStringList list = ui->votesEdit->toPlainText().replace("\n", ",").replace(" ", ",").split(',', QString::SkipEmptyParts);
    std::vector<std::string> newSpeeches;
    bricoleurourBricoleurSpeech.clear();

    if (list.length() > 0)
    {
        newSpeeches.push_back("bricoleurour");
        foreach ( const QString &strLine, list )
            if ( !strLine.isEmpty() && strLine.length() >= 8 && IsHex(strLine.toStdString()) )
            {
                // Create new string if necessary
                if (newSpeeches.back().length() > MAX_TX_COMMENT_LEN - 9)
                {
                    newSpeeches.push_back("bricoleurour");
                }
                std::string &newSpeech = newSpeeches.back();
                newSpeech = newSpeech + " " + strLine.trimmed().left(8).toStdString();
            }


        for (std::vector<std::string>::iterator it = newSpeeches.begin(); it != newSpeeches.end(); ++it)
        {
            bricoleurourBricoleurSpeech.push_back(*it);
        }
    }

    // save new speech
    qDebug() << "saving bricoleurour petitions";
    if ( !SaveBricoleurourClamSpeech() )
        qDebug() << "Bricoleurour BRICSpeech petitions FAILED to save!";

    loadVotes();
}

void BricoleurourPage::showClamourTxResult(std::string txID, std::string txError)
{
    if (txError == "") {
        std::string txSentMsg = "Bricoleurour petition created successfully: " + txID;
        QMessageBox::information(this, tr("Create Bricoleurour Petition"),
            tr(txSentMsg.c_str()),
            QMessageBox::Ok, QMessageBox::Ok);
        ui->createPetitionButton->setEnabled(false);
        ui->setVoteCheckBox->setEnabled(false);
    } else {
        QMessageBox::warning(this, tr("Create Bricoleurour Petition"),
            tr(txError.c_str()),
            QMessageBox::Ok, QMessageBox::Ok);
    }
}

void BricoleurourPage::setClamourSearchResults(CClamour *pResult)
{
    if (!pResult)
    {
        LogPrintf("No bricoleurour results.\n");
        QMessageBox::warning(this, tr("Bricoleurour Search"),
            tr("No bricoleurour petition found."),
            QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    petitionModel->setPetition(pResult);
}

bool petitionPairSort(std::pair<std::string, int> i, std::pair<std::string, int> j) { return i.second > j.second; }

void BricoleurourPage::showPetitionSupport(std::map<string, int> mapSupport)
{
    std::vector<std::pair<std::string, int> > support;
    for (std::map<std::string, int>::iterator it = mapSupport.begin(); it != mapSupport.end(); ++it) {
        std::pair<std::string, int> petitionSupport = std::make_pair(it->first, it->second);
        support.push_back(petitionSupport);
    }
    std::sort(support.begin(), support.end(), petitionPairSort);
    supportModel->setSupport(support);
}

void BricoleurourPage::setModel(WalletModel *model)
{
    this->model = model;
    connect(this->model, SIGNAL(bricoleurourTxSent(std::string, std::string)), this, SLOT(showBricoleurourTxResult(std::string, std::string)));
    connect(this->model, SIGNAL(bricoleurourSearchComplete(CBricoleurour*)), this, SLOT(setClamourSearchResults(CClamour*)));
    connect(this->model, SIGNAL(petitionSupportRetrieved(std::map<std::string,int>)), this, SLOT(showPetitionSupport(std::map<std::string,int>)));
    loadVotes();
}

void BricoleurourPage::on_searchClamourButton_clicked()
{
    std::string pid(ui->searchBricoleurourEdit->text().toStdString());
    if (!(IsHex(pid) && pid.length() == 8)) {
        ui->searchBricoleurourEdit->setValid(false);
        return;
    }
    petitionModel->clear();
    model->searchBricoleurours(pid);
}


void BricoleurourPage::on_getPetitionSupportButton_clicked()
{
    int nWindow = ui->petitionWindowSpinbox->value();
    model->getPetitionSupport(nWindow);
}

void BricoleurourPage::on_petitionSupportView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->petitionSupportView->indexAt(pos);
    if (index.isValid())
        petitionViewContextMenu->exec(QCursor::pos());
}

void BricoleurourPage::searchHighlightedPetition()
{
    QModelIndexList indexes = ui->petitionSupportView->selectionModel()->selectedIndexes();
    if (indexes.size() >= 1)
    {
        QModelIndex pidIndex = indexes.at(0);
        QString pid = ui->petitionSupportView->model()->data(pidIndex).toString();
        ui->searchBricoleurourEdit->setText(pid);
        ui->tabWidget->setCurrentIndex(1);
        ui->searchBricoleurourButton->animateClick();
    }
}
