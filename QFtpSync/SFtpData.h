#pragma once

#include <QDateTime>
#include <QString>

struct SFtpData
{
  QString name;
  QString addres;
  quint16 port;
  QString user;
  QString pass;
  QString removeDir;
  QString localDir;
  qint32 periodSec;

  QDateTime lastStart;
};
