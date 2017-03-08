#include "FtpModel.h"

#include <QSettings>

CFtpModel::CFtpModel(QObject *parent)
  : QAbstractListModel(parent)
{
  QSettings settings("QFtpSync.ini", QSettings::IniFormat);
  int size = settings.value("size", 0).toInt();
  for (int count = 0; count < size; ++count)
  {
    settings.beginGroup(QString::number(count));
    SFtpData data;

    data.name = settings.value("name").toString();
    data.addres = settings.value("addres").toString();
    data.port = settings.value("port").toInt();
    data.user = settings.value("user").toString();
    data.pass = settings.value("pass").toString();
    data.removeDir = settings.value("removeDir").toString();
    data.localDir = settings.value("localDir").toString();
    data.periodSec = settings.value("periodSec").toInt();
    data.lastStart = QDateTime::currentDateTime().addSecs(-data.periodSec);
    m_data.push_back(data);
    settings.endGroup();
  }

}

CFtpModel::~CFtpModel()
{
  QSettings settings("QFtpSync.ini", QSettings::IniFormat);
 
  int size = settings.value("size", 0).toInt();
  for (int count = 0; count < size; ++count)
    settings.remove(QString::number(count));

  settings.setValue("size", m_data.size());
  for (int count = 0; count < m_data.size(); ++count)
  {
    settings.beginGroup(QString::number(count));

    settings.setValue("name", m_data[count].name);
    settings.setValue("addres", m_data[count].addres);
    settings.setValue("port", m_data[count].port);
    settings.setValue("user", m_data[count].user);
    settings.setValue("pass", m_data[count].pass);
    settings.setValue("removeDir", m_data[count].removeDir);
    settings.setValue("localDir", m_data[count].localDir);
    settings.setValue("periodSec", m_data[count].periodSec);
    settings.endGroup();
  }
}

int CFtpModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
  return m_data.size();
}

QVariant CFtpModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
  if (role == Qt::DisplayRole)
    return m_data[index.row()].name;

  return QVariant();
}

bool CFtpModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/)
{
  if (role == Qt::EditRole)
  {
    m_data[index.row()].name = value.toString();
  }
  return true;
}

Qt::ItemFlags CFtpModel::flags(const QModelIndex &index) const
{
  return Qt::ItemFlag::ItemIsEditable | Qt::ItemFlag::ItemIsSelectable | Qt::ItemFlag::ItemIsEnabled;
}

void CFtpModel::addFtpData(SFtpData ftpData)
{
  beginInsertRows(index(m_data.size()), 1, 1);
  m_data.push_back(ftpData);
  endInsertRows();
}

SFtpData CFtpModel::getFtpData(const QModelIndex& index)
{
  return m_data.at(index.row());
}

void CFtpModel::updateFtpData(const QModelIndex& index, SFtpData data)
{
  m_data[index.row()] = data;
}

void CFtpModel::deleteElement(const QModelIndex& index)
{
  beginRemoveRows(index, 1, 1);
  m_data.removeAt(index.row());
  endRemoveRows();
}
