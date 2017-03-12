#include "FtpDialog.h"

#include <QFileDialog>

CFtpDialog::CFtpDialog(QWidget *parent)
  : QDialog(parent)
{
  initUi();

  okEnabled();
}

CFtpDialog::CFtpDialog(const SFtpData data, QWidget *parent /*= nullptr*/)
  : QDialog(parent)
{
  initUi();

  ui.leAddress->setText(data.addres);
  ui.lePort->setText(QString::number(data.port));
  ui.leUser->setText(data.user);
  ui.lePass->setText(data.pass);
  ui.leLocalDir->setText(data.localDir);
  ui.leRemoveDir->setText(data.removeDir);
  ui.lePeriodicity->setText(QString::number(data.periodSec));

  okEnabled();
}

void CFtpDialog::initUi()
{
  ui.setupUi(this);
  connect(ui.pbOk, SIGNAL(pressed()), SLOT(accept()));
  connect(ui.pbCancel, SIGNAL(pressed()), SLOT(reject()));
  connect(ui.pbLocalDir, SIGNAL(pressed()), SLOT(changeLocalDir()));

  connect(ui.leAddress, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(ui.lePort, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(ui.leUser, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(ui.leRemoveDir, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(ui.leLocalDir, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
  connect(ui.lePeriodicity, SIGNAL(textChanged(const QString&)), SLOT(textChanged(const QString&)));
}

CFtpDialog::~CFtpDialog()
{
}

SFtpData CFtpDialog::getFtpData()
{
  SFtpData data;
  data.addres = ui.leAddress->text();
  data.port = ui.lePort->text().toUInt();
  data.user = ui.leUser->text();
  data.pass = ui.lePass->text();
  data.localDir = ui.leLocalDir->text();
  data.removeDir = ui.leRemoveDir->text();
  data.periodSec = ui.lePeriodicity->text().toUInt();
  data.name = data.user + "@" + data.addres + ":" + QString::number(data.port) + data.removeDir;
  data.lastStart = QDateTime::currentDateTime().addSecs(-data.periodSec);
  return data;
}

void CFtpDialog::changeLocalDir()
{
  QFileDialog dialog(this, tr("Change Local Directory"), ui.leLocalDir->text());
  dialog.setFileMode(QFileDialog::DirectoryOnly);
  if (dialog.exec())
    ui.leLocalDir->setText(*dialog.selectedFiles().begin());
}

void CFtpDialog::textChanged(const QString&)
{
  okEnabled();
}

void CFtpDialog::okEnabled()
{
  if (ui.leAddress->text().isEmpty()
    || ui.lePort->text().isEmpty()
    || ui.leUser->text().isEmpty()
    || ui.leLocalDir->text().isEmpty()
    || ui.leRemoveDir->text().isEmpty()
    || ui.lePeriodicity->text().isEmpty())
    ui.pbOk->setEnabled(false);
  else
    ui.pbOk->setEnabled(true);
}
