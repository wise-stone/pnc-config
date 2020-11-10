#include "cconfigdatamodel.h"
#include <QMimeData>
#include <QIODevice>
#include <QDataStream>
#include <QDebug>

CConfigDataModel::CConfigDataModel(QObject *parent) : QAbstractTableModel(parent)
{

    for (int i = 0; i < 15; i++)
    {
        QStringList config;
        config << QString("UNIT #%1").arg(i)
               << QString("port #%1").arg(i)
               << QString("Description ___ #%1").arg(i);

        configData.append(config);
    }
}

int CConfigDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
//    qDebug() << QString("CConfigDataModel::rowCount [%1]")
//             .arg(configData.size())
//             << Qt::endl;

    return configData.size();
}

int CConfigDataModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
//    qDebug() << QString("CConfigDataModel::columnCount [%1]")
//             .arg(configData[0].size())
//             << Qt::endl;
    return configData[0].size();
}

QVariant CConfigDataModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << QString("CConfigDataModel::data starts") << Qt::endl;

    if (!index.isValid())
        return QVariant();
    if (index.row() < 0 || index.row() >= rowCount(QModelIndex())
            || index.column() < 0 || index.column() >= columnCount(QModelIndex()))
        return QVariant();
    if (role == Qt::DisplayRole)
    {
//        qDebug() << QString("CConfigDataModel::data configData[%1][%2]-[%3]")
//                    .arg(index.row())
//                    .arg(index.column())
//                    .arg(configData.at(index.row()).at(index.column()))
//                 << Qt::endl;
        return configData.at(index.row()).at(index.column());
    }
    return QVariant();
}

QStringList CConfigDataModel::mimeTypes() const
{
    QStringList mimeType;
    mimeType << "plain/text";
    return mimeType;
}

QMimeData *CConfigDataModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData* mimeData = new QMimeData;
    QByteArray encoded;
    QDataStream stream(&encoded, QIODevice::WriteOnly);

    for (const QModelIndex& index : indexes)
    {
        if (index.isValid())
        {
            QString text = data(index, Qt::DisplayRole).toString();
            stream << text;
        }
    }

    mimeData->setData("plain/text", encoded);
    return mimeData;
}

bool CConfigDataModel::canDropMimeData(const QMimeData *data,
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

bool CConfigDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::DisplayRole)
    {
        QStringList list = configData[index.row()];
        list[index.column()] = value.toString();
        configData[index.row()] = list;

        qDebug() << QString("text:[%1], configData.at[%2][%3]=[%4]").arg(list[index.column()]).arg(index.row()).arg(index.column())
                    .arg(configData.at(index.row()).at(index.column())) << Qt::endl;
        return true;
    }
    return false;
}

bool CConfigDataModel::dropMimeData(const QMimeData *data,
                                    Qt::DropAction action,
                                    int row, int column,
                                    const QModelIndex &parent)
{
    qDebug() << QString("CConfigDataModel::dropMimeData row[%1], col[%2], action[%3]")
             .arg(row).arg(column).arg(action)
             << Qt::endl;
    if (parent.isValid())
    {
        qDebug() << QString("CConfigDataModel::dropMimeData [Parent] row[%1], col[%2], data[%3]")
                 .arg(parent.row()).arg(parent.column()).arg(configData.at(parent.row()).at(parent.column()))
                 << Qt::endl;

    }

    if (!canDropMimeData(data, action, row, column, parent))
    {
        qDebug() << QString("CConfigDataModel::dropMimeData - false return at canDropMimeData( ))") << Qt::endl;
        return false;
    }

    if (action == Qt::IgnoreAction)
    {
        qDebug() << QString("CConfigDataModel::dropMimeData - true return with IgnoreAction") << Qt::endl;
        return true;
    }
/*
    if (row < 0 || row >= configData.size())
    {
        qDebug() << QString("CConfigDataModel::dropMimeData - false return with row[%1]").arg(row) << Qt::endl;
        return false;
    }
    // if the drop didnt' occurr inside model.
    if (!parent.isValid())
    {
        qDebug() << QString("CConfigDataModel::dropMimeData - false return as parent is NOT valid.") << Qt::endl;
        return false;
    }
*/

/*
    // this is enough for single selection, but...
    if (data->hasText())
    {
        bool bRtn = setData(parent, data->text(), Qt::DisplayRole);
        qDebug() << QString("CConfigDataModel::dropMimeData - return of setData[%1].").arg(bRtn) << Qt::endl;
    }
*/

    int startRow = row;
    if (parent.isValid())
        startRow = parent.row();

    QByteArray encoded = data->data("plain/text");
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    QStringList newItems;
    int rows = 0;

    qDebug() << QString("CConfigDataModel::dropMimeData - row[%1], col[%2].. startRow[%3].")
             .arg(row).arg(column).arg(startRow)
             << Qt::endl;

    while (!stream.atEnd())
    {
        QString text;
        stream >> text;
        newItems << text;
        rows++;

        qDebug() << QString("CConfigDataModel::dropMimeData - SETTING rows[%1].data[%2]")
                    .arg(rows).arg(text) << Qt::endl;
    }

    insertRows(startRow, rows, QModelIndex());
    for (const QString& text : qAsConst(newItems))
    {
        QModelIndex idx = index(startRow, 1, QModelIndex());
        setData(idx, text, Qt::DisplayRole);

        qDebug() << QString("CConfigDataModel::dropMimeData - setData row[%1], col[1] - data[%2]")
                    .arg(startRow).arg(text) << Qt::endl;

        startRow++;

    }

    return true;
}

Qt::ItemFlags CConfigDataModel::flags(const QModelIndex &index) const
{
    // only drag and drop are enabled for valid items.
    if(index.isValid() && index.column() == 1)
    {
//        qDebug() << QString("CConfigDataModel::flags - row[%1], col[%2] Drag/Drop Enabled")
//                            .arg(index.row()).arg(index.column()) << Qt::endl;

        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractTableModel::flags(index);
    }

    return QAbstractTableModel::flags(index);
}

Qt::DropActions CConfigDataModel::supportedDropActions() const
{
    // item needs to be cleared when drag/drop
    return Qt::MoveAction /*| Qt::CopyAction*/;
}


