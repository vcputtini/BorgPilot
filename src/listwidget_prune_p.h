#ifndef LISTWIDGET_PRUNE_P_H
#define LISTWIDGET_PRUNE_P_H

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QObject>
#include <QPair>
#include <QWidget>

#include "types_global.h"

class ListWidget_Prune_P : public QListWidget
{
  Q_OBJECT

public:
  explicit ListWidget_Prune_P(QWidget* parent = nullptr);
  void populate();
  void unCheckAllOptions();

  enum class PruneOptions
  {
    DryRun = 0x00,
    List,
    Stats,
    SaveSpace,
    KeepWithin,
    KeepLast,
    KeepSecondly,
    KeepMinutely,
    KeepHourly,
    KeepDaily,
    KeepWeekly,
    KeepMonthly,
    Keep13Weekly,
    Keep3Monthly,
    KeepYearly,
    None
  };

private:
  AppTypes::SelectedPairs getSelectedPairs() const;
  static const std::unordered_map<PruneOptions,
                                  std::pair<std::string, std::string>>
    Prune_m_;

private slots:
  void onCheckboxStateChanged();

signals:
  void itemsSelectedChanged(QSet<QPair<QString, QString>> selected);
};

#endif // ListWidget_Prune_P_H
