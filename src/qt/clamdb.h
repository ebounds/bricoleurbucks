#ifndef BRICDB_H
#define BRICDB_H

#include <QWidget>

namespace Ui {
class BricoleurDB;
}
class WalletModel;
class BricoleurourPage;
class NotaryPage;

class BricoleurDB : public QWidget
{
    Q_OBJECT

public:
    explicit BricoleurDB(QWidget *parent = 0);
    ~BricoleurDB();

    void setModel(WalletModel *model);

private:
    Ui::BricoleurDB *ui;
    WalletModel *model;
    BricoleurourPage *bricoleurourPage;
    NotaryPage *notaryPage;
};

#endif // BRICDB_H
