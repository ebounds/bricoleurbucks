#ifndef BRICOURSUPPORTMODEL_H
#define BRICOURSUPPORTMODEL_H

#include <QWidget>
#include <QAbstractTableModel>
#include <QStringList>

class BricoleurourSupportModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BricoleurourSupportModel(QWidget *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void clear();
    void setSupport(std::vector<std::pair<std::string, int> > newSupport);

signals:

public slots:

private:
    QStringList columns;
    std::vector<std::pair<std::string, int> > support;
};

#endif // BRICOURSUPPORTMODEL_H
