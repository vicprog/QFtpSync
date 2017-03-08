#pragma once

#include <QtWidgets/QWidget>

#include "ui_QFtpSync.h"

#include "FtpModel.h"
#include <QMutex>

class QFtpSync: public QWidget
{
  Q_OBJECT

public:
  QFtpSync(QWidget *parent = Q_NULLPTR);
  ~QFtpSync();

private slots:
  void addFtp();
  void editFtp();
  void delFtp();
  void currentChanged(QModelIndex, QModelIndex);

  void enableButton(bool enabled);

protected:
  void timerEvent(QTimerEvent *event);

private:
  QMutex m_mutex;
  Ui::QFtpSyncClass ui;
  CFtpModel m_model;
  int m_timerId;
  bool m_proccessing;
  int m_cur = 0;
};
