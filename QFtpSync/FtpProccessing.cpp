#include "FtpProccessing.h"

#include<QFile>
#include<QDebug>
#include <QFileInfo>
#include <QDir>

#include <FtpClient/FTPClient.h>

using namespace nsFTP;


CFtpProccessing::CFtpProccessing(SFtpData& ftpData, QObject *parent)
  : QThread(parent)
  , m_ftpData(ftpData)
{
}


CFtpProccessing::~CFtpProccessing()
{
}


void CFtpProccessing::run()
{
  CFTPClient ftpClient;
  CLogonInfo logonInfo(m_ftpData.addres.toStdWString(),
    m_ftpData.port,
    m_ftpData.user.toStdWString(),
    m_ftpData.pass.toStdWString());

  // connect to server
  if (!ftpClient.Login(logonInfo))
  {
    qDebug() << "Can’t login!";
    return;
  }
  std::wstring dir = m_ftpData.removeDir.toStdWString();
  list(&ftpClient, dir);
  // disconnect
  ftpClient.Logout();
//  deleteLater();
}


bool CFtpProccessing::list(CFTPClient* ftpClient, std::wstring& dir)
{
  TFTPFileStatusShPtrVec vstrFileList;
  
  if (!ftpClient->NameList(dir, vstrFileList, true))
    return false;

  if (vstrFileList.empty())
    return false;

  for (auto data : vstrFileList)
  {
    qDebug() << "file " << QString::fromStdWString(data->Name());
    std::wstring dir = data->Name() + L"/";
    if (!list(ftpClient, dir))
    {
      QString result = createDestination(QString::fromStdWString(data->Name()));
      if (!QFile::exists(result))
      {
        createFullPath(result);
        ftpClient->DownloadFile(data->Name(), result.toStdWString(),CRepresentation(CType::Image()), true);
      }
    }
  }
  return true;
}


QString CFtpProccessing::createDestination(QString sourceName)
{
  QString result = sourceName;
  result.remove(0, m_ftpData.removeDir.size());
  result = m_ftpData.localDir + result;
  result = result.replace(QString::fromStdWString(L"/"), QString::fromStdWString(L"\\"));
  return result;
}


void CFtpProccessing::createFullPath(QString destinationName)
{
  QFileInfo info(destinationName);
  QDir dir(info.dir());
  QString absolutPath = dir.absolutePath();
  dir.mkpath(absolutPath);
}

