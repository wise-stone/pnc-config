#ifndef CGLIGHTMODEL_H
#define CGLIGHTMODEL_H

#include <QAbstractTableModel>

class CGLightModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CGLightModel(QObject *parent = nullptr);

    QStringList guideLightPorts;
signals:


    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

    // QAbstractItemModel interface
public:
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    // QAbstractItemModel interface
public:
    int columnCount(const QModelIndex &parent) const;
};

#endif // CGLIGHTMODEL_H
