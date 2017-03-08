#pragma once

#include <QAbstractListModel>

#include "SFtpData.h"

class CFtpModel: public QAbstractListModel
{
  Q_OBJECT

public:
  CFtpModel(QObject *parent = nullptr);
  ~CFtpModel();

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;


  void addFtpData(SFtpData);
  SFtpData getFtpData(const QModelIndex &index);
  void updateFtpData(const QModelIndex &index, SFtpData);
  void deleteElement(const QModelIndex &index);
public:
  QList<SFtpData> m_data;
};
