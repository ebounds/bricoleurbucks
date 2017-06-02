#include "bricoleuroursupportmodel.h"

#include "guiutil.h"

BricoleurourSupportModel::ClamourSupportModel(QWidget *parent) :
    QAbstractTableModel(parent)
{
    columns << tr("Petition ID") << tr("Support");
}

int BricoleurourSupportModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return support.size();
}

int BricoleurourSupportModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return columns.length();
}

QVariant BricoleurourSupportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    std::pair<std::string, int> petition = support.at(index.row());
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0:
            return QString::fromStdString(petition.first);
        case 1:
            return petition.second;
        }
    }
    return QVariant();
}

QVariant BricoleurourSupportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        return columns.at(section);
    }
    return QVariant();
}

Qt::ItemFlags BricoleurourSupportModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void BricoleurourSupportModel::clear()
{
    beginResetModel();
    support.clear();
    endResetModel();
}

void BricoleurourSupportModel::setSupport(std::vector<std::pair<std::string, int> > newSupport)
{
    beginResetModel();
    support = newSupport;
    endResetModel();
}
