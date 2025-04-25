#pragma once

#include "common/model.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"

class UserFuncPanel : public QFrame {
  Q_OBJECT

public:
  explicit UserFuncPanel(QWidget *parent = nullptr);
  void showEvent(QShowEvent *event) override;
  void hideEvent(QHideEvent* event) override;

public slots:
  void updateToggles();

private:
  QStackedLayout* main_layout = nullptr;
  QWidget* userScreen = nullptr;
  Params params;
  std::map<std::string, ParamControl*> toggles;
  ParamWatcher *param_watcher;
  ScrollView *scrollView = nullptr;
};
