#ifndef CCONFIGDATAMODEL_H
#define CCONFIGDATAMODEL_H

#include <QAbstractTableModel>
#include <QVector>

class CConfigDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CConfigDataModel(QObject *parent = nullptr);

    QVector<QStringList> configData;

signals:


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDropActions() const;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
};

#endif // CCONFIGDATAMODEL_H
