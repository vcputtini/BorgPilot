#ifndef LISTWIDGET_DELETE_P_H
#define LISTWIDGET_DELETE_P_H

#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include <QPair>
#include <QSet>
#include <unordered_map>

#include "types_global.h"

/*!
 * \brief The ListWidget_Delete_P class
 */
class ListWidget_Delete_P : public QListWidget
{
  Q_OBJECT

public:
  explicit ListWidget_Delete_P(QWidget* parent = nullptr);
  void populate();
  void unCheckAllOptions();

  enum class DeleteOptions
  {
    DryRun = 0,
    List,
    Stats,
    Force,
    SaveSpace,
    KeepSecurityInfo
  };

private:
  AppTypes::SelectedPairs getSelectedPairs() const;
  static const std::unordered_map<DeleteOptions,
                                  std::pair<std::string, std::string>>
    Delete_m_;

private slots:
  void onCheckboxStateChanged();

signals:
  void itemsSelectedChanged(QSet<QPair<QString, QString>> selected);
};

#endif // LISTWIDGET_DELETE_P_H
