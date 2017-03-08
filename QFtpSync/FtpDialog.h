#pragma once

#include <QDialog>

#include "ui_CFtpDialog.h"
#include "SFtpData.h"

class CFtpDialog : public QDialog
{
  Q_OBJECT

public:
  CFtpDialog(QWidget *parent = nullptr);
  CFtpDialog(const SFtpData, QWidget *parent = nullptr);

  ~CFtpDialog();

  SFtpData getFtpData();

protected slots:
  void changeLocalDir();
  void textChanged(const QString&);

private:
  void initUi();
  void okEnabled();

  Ui::CFtpDialog ui;
};
