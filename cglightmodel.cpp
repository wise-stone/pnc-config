#include "cglightmodel.h"
#include <QMimeData>
#include <QDataStream>
#include <QIODevice>
#include <QDebug>


CGLightModel::CGLightModel(QObject *parent) : QAbstractTableModel(parent)
{
    // get data from guide light dat file
    guideLightPorts << "port01"
         << "port02"
         << "port03"
         << "port04"
         << "port05"
         << "port06"
         << "port07"
         << "port08"
         << "port09"
         << "port10";

}

int CGLightModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return guideLightPorts.length();
}

int CGLightModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CGLightModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= rowCount(QModelIndex())
            || index.column() < 0 || index.column() >= columnCount(QModelIndex()))
        return QVariant();

    if (role == Qt::DisplayRole)
        return guideLightPorts.at(index.row());

    return QVariant();
}

QStringList CGLightModel::mimeTypes() const
{
    QStringList mimeType;
    mimeType << "plain/text";
    return mimeType;
}

QMimeData *CGLightModel::mimeData(const QModelIndexList &indexes) const
{
    qDebug() << "[CGLightModel::mimeData]" << Qt::endl;

    QMimeData* mimeData = new QMimeData;
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    for (const QModelIndex& index : indexes)
    {
        if (index.isValid())
        {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }

    mimeData->setData("plain/text", encodedData);
    return mimeData;
}

bool CGLightModel::canDropMimeData(const QMimeData *data,
                                   Qt::DropAction action,
                                   int row, int column,
                                   const QModelIndex &parent) const
{
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(column);
    Q_UNUSED(parent);

    if (!data->hasFormat("plain/text"))
        return false;
    return true;
}

bool CGLightModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "[CGLightModel::setData]" << Qt::endl;
    if (role == Qt::DisplayRole)
    {
        if (index.isValid())
        {
            QString port = guideLightPorts[index.row()];
            // will not update ports data.
    //        guideLightPorts[index.row()] = value.toString();

            qDebug() << QString("port:[%1], guideLightPorts.at[%2][%3]=[%4]").arg(port).arg(index.row()).arg(index.column())
                        .arg(guideLightPorts[index.row()]) << Qt::endl;
        }
        else
        {
            qDebug() << QString("out of data range!! index.row[%1].col[%2]")
                        .arg(index.row()).arg(index.column())
                        << Qt::endl;
        }
        return true;
    }
    return false;
}

bool CGLightModel::dropMimeData(const QMimeData *data,
                                Qt::DropAction action,
                                int row, int column,
                                const QModelIndex &parent)
{

    if (!canDropMimeData(data, action, row, column, parent))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    // don't overwrite the existing item or don't append new item on Guide light ports
    // just return base class implementation

    int startRow = row;
    if (parent.isValid())
        startRow = parent.row();

    QByteArray encoded = data->data("plain/text");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    qDebug() << QString("CGLightModel::dropMimeData - row[%1], col[%2].. startRow[%3].")
             .arg(row).arg(column).arg(startRow)
             << Qt::endl;

    while (!stream.atEnd())
    {
        QString text;
        stream >> text;
        newItems << text;
        rows++;

        qDebug() << QString("CGLightModel::dropMimeData - SETTING rows[%1].data[%2]")
                    .arg(rows).arg(text) << Qt::endl;
    }

    insertRows(startRow, rows, QModelIndex());
    for (const QString& text : qAsConst(newItems))
    {
        QModelIndex idx = index(startRow, 0, QModelIndex());
        setData(idx, text, Qt::DisplayRole);

        qDebug() << QString("CGLightModel::dropMimeData - setData row[%1], col[1] - data[%2]")
                    .arg(startRow).arg(text) << Qt::endl;

        startRow++;
    }

    return true;
}

Qt::ItemFlags CGLightModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;

    qDebug() << QString("[CGLightModel::flags] idex : row[%1], col[%2], valid[%3]")
             .arg(index.row()).arg(index.column()).arg(index.isValid())
             << Qt::endl;


    if (index.isValid() && index.column() == 0)
    {
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
        return Qt::ItemIsDragEnabled | defaultFlags;
    }
    else
    {
        return defaultFlags;
    }
}

Qt::DropActions CGLightModel::supportedDropActions() const
{
    return Qt::MoveAction /*| Qt::CopyAction*/;
}

