#ifndef CLAMDB_H
#define CLAMDB_H

#include <QWidget>

namespace Ui {
class ClamDB;
}
class WalletModel;
class ClamourPage;
class NotaryPage;

class ClamDB : public QWidget
{
    Q_OBJECT

public:
    explicit ClamDB(QWidget *parent = 0);
    ~ClamDB();

    void setModel(WalletModel *model);

private:
    Ui::ClamDB *ui;
    WalletModel *model;
    ClamourPage *clamourPage;
    NotaryPage *notaryPage;
};

#endif // CLAMDB_H
