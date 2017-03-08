#pragma once

#include <QThread>

#include "SFtpData.h"

namespace nsFTP
{
  class CFTPClient;
}

class CFtpProccessing: public QThread
{
  Q_OBJECT

public:
  CFtpProccessing(SFtpData&, QObject *parent);
  ~CFtpProccessing();

protected:
  void run() override;

private:
  bool list(nsFTP::CFTPClient* ftpClient, std::wstring& dir);
  QString createDestination(QString sourceName);
  void createFullPath(QString destinationName);
private:
  SFtpData m_ftpData;
};
