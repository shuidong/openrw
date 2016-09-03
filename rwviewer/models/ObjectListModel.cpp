#include "ObjectListModel.hpp"

ObjectListModel::ObjectListModel(GameData *dat, QObject *parent)
    : QAbstractTableModel(parent), _gameData(dat)
{
}

int ObjectListModel::rowCount(const QModelIndex &parent) const
{
  return _gameData->objectTypes.size();
}

int ObjectListModel::columnCount(const QModelIndex &parent) const
{
  return 3;
}

static std::map<ModelDataType, QString> gDataType = {
    {ModelDataType::SimpleInfo,  "Object"},
    {ModelDataType::VehicleInfo, "Vehicle"},
    {ModelDataType::PedInfo,     "Pedestrian"},
    {ModelDataType::ClumpInfo,   "Cutscene"}};

QVariant ObjectListModel::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    auto id = index.internalId();
    if (id == -1) return QVariant::Invalid;
    if (index.column() == 0) {
      return id;
    } else if (index.column() == 1) {
      auto object = _gameData->objectTypes[id];
      if (gDataType[object->type].isEmpty()) {
        return QString("Unknown");
      }
      return gDataType[object->type];
    } else if (index.column() == 2) {
      auto object = _gameData->objectTypes[id];
      return QString::fromStdString(object->modelName);
    }
  }
  return QVariant::Invalid;
}

QVariant ObjectListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return "ID";
      case 1:
        return "Type";
      case 2:
        return "Model";
    }
  }
  return QVariant::Invalid;
}

QModelIndex ObjectListModel::index(int row, int column, const QModelIndex &parent) const
{
  auto it = _gameData->objectTypes.begin();
  for (int i = 0; i < row; i++) it++;
  auto id = it->second->id;

  return hasIndex(row, column, parent) ? createIndex(row, column, id) : QModelIndex();
}
