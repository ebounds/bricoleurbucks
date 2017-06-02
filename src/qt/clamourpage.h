#ifndef BRICOURPAGE_H
#define BRICOURPAGE_H

#include <map>

#include <QWidget>

namespace Ui {
class BricoleurourPage;
}
class WalletModel;
class CBricoleurour;
class BricoleurourPetitionModel;
class BricoleurourSupportModel;

QT_BEGIN_NAMESPACE
class QMenu;
QT_END_NAMESPACE

class BricoleurourPage : public QWidget
{
    Q_OBJECT

public:
    explicit BricoleurourPage(QWidget *parent = 0);
    ~BricoleurourPage();

    void setModel(WalletModel *model);

public slots:
    void showBricoleurourTxResult(std::string txID, std::string txError);
    void setBricoleurourSearchResults(CClamour *pResult);
    void showPetitionSupport(std::map<std::string, int> mapSupport);

private slots:
    void on_createPetitionEdit_textChanged();

    void on_createPetitionButton_clicked();

    void on_setVotesButton_clicked();

    void on_searchBricoleurourButton_clicked();

    void on_getPetitionSupportButton_clicked();

    void on_petitionSupportView_customContextMenuRequested(const QPoint &pos);

    void searchHighlightedPetition();

private:
    Ui::BricoleurourPage *ui;
    WalletModel *model;
    BricoleurourPetitionModel *petitionModel;
    BricoleurourSupportModel *supportModel;
    QMenu *petitionViewContextMenu;

    void loadVotes();
    void saveVotes();
};

#endif // BRICOURPAGE_H
