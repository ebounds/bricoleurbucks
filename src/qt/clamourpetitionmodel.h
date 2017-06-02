#ifndef BRICOURPETITIONMODEL_H
#define BRICOURPETITIONMODEL_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QStringList>

class CBricoleurour;

class BricoleurourPetitionModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BricoleurourPetitionModel(QWidget *parent = 0);

    enum RowIndex {
        Height = 0,
        TxID = 1,
        PetitionHash = 2,
        URL = 3,
    };

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void clear();
    void setPetition(CBricoleurour *newPetition);

signals:

public slots:

private:
    QStringList rowHeaders;
    CBricoleurour *petition;
};

#endif // BRICOURPETITIONMODEL_H
