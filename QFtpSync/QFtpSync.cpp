#include "FtpDialog.h"

#include "QFtpSync.h"
#include <QSettings>
#include "FtpProccessing.h"


QFtpSync::QFtpSync(QWidget *parent)
  : QWidget(parent)
  , m_proccessing(false)
{
  ui.setupUi(this);
  ui.listView->setModel(&m_model);
  ui.listView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
  ui.listView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
  ui.listView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked | QAbstractItemView::EditTrigger::SelectedClicked);
  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(currentChanged(QModelIndex, QModelIndex)));
  connect(ui.pbAdd, SIGNAL(pressed()), SLOT(addFtp()));
  connect(ui.pbEdit, SIGNAL(pressed()), SLOT(editFtp()));
  connect(ui.pbDelete, SIGNAL(pressed()), SLOT(delFtp()));
  m_timerId = startTimer(1000);
}

QFtpSync::~QFtpSync()
{
  killTimer(m_timerId);
}

void QFtpSync::addFtp()
{
  CFtpDialog dialog;
  if (dialog.exec())
    m_model.addFtpData(dialog.getFtpData());
}

void QFtpSync::editFtp()
{
  QModelIndex index = *ui.listView->selectionModel()->selectedIndexes().begin();
  auto ftpData = m_model.getFtpData(index);
  CFtpDialog dialog(ftpData);
  if (dialog.exec())
    m_model.updateFtpData(index, dialog.getFtpData());
}

void QFtpSync::delFtp()
{
  m_model.deleteElement(*ui.listView->selectionModel()->selectedIndexes().begin());
  enableButton(false);
}

void QFtpSync::currentChanged(QModelIndex cur, QModelIndex)
{
  bool enabled = cur.isValid();
  enableButton(enabled);
}

void QFtpSync::enableButton(bool enabled)
{
  ui.pbDelete->setEnabled(enabled);
  ui.pbEdit->setEnabled(enabled);
}

void QFtpSync::timerEvent(QTimerEvent* event)
{
  QMutexLocker locker(&m_mutex);
  CFtpProccessing* ref = findChild<CFtpProccessing *>();
  if (!ref)
  {
    QDateTime curDateTime = QDateTime::currentDateTime();
    if (m_cur >= m_model.rowCount())
      m_cur = 0;
    
    for (; m_cur < m_model.rowCount(); ++m_cur)
    {
      auto& ftpData = m_model.m_data[m_cur];
      auto secsFromLastStart = ftpData.lastStart.secsTo(curDateTime);
      if (secsFromLastStart > ftpData.periodSec)
      {
        ftpData.lastStart = curDateTime;
        CFtpProccessing* proccessing = new CFtpProccessing(ftpData, this);
        proccessing->start();
        ++m_cur;
        return;
      }
    }
  }
  else
  {
    if (ref->isFinished())
      delete ref;
  }
}
