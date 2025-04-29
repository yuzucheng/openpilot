#pragma once

#include "common/model.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"

class StopDistance : public SPOptionControl {
  Q_OBJECT

public:
  StopDistance();
  void refresh();
private:
  Params params;
};

class CruiseOnDist : public SPOptionControl {
  Q_OBJECT

public:
  CruiseOnDist();
  void refresh();
private:
  Params params;
};

class ComfortBrake : public SPOptionControl {
  Q_OBJECT

public:
  ComfortBrake();
  void refresh();
private:
  Params params;
};

class StartAccel : public SPOptionControl {
  Q_OBJECT

public:
  StartAccel();
  void refresh();
private:
  Params params;
};

class vEgoStopping : public SPOptionControl {
  Q_OBJECT

public:
  vEgoStopping();
  void refresh();
private:
  Params params;
};

class ExperimentalModeSpeed : public SPOptionControl {
  Q_OBJECT

public:
  ExperimentalModeSpeed();
  void refresh();
private:
  Params params;
};

class ExperimentalModeAndSpeed : public SPOptionControl {
  Q_OBJECT

public:
  ExperimentalModeAndSpeed();
  void refresh();
private:
  Params params;
};

class ExperimentalModeAngle : public SPOptionControl {
  Q_OBJECT

public:
  ExperimentalModeAngle();
  void refresh();
private:
  Params params;
};

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

  StopDistance *stop_distance;
  ComfortBrake *comfort_break;
  StartAccel *start_accel;
  vEgoStopping *vego_stopping;

  ExperimentalModeAngle *experimental_mode_angle;
  ExperimentalModeAndSpeed *experimental_mode_and_speed;
  ExperimentalModeSpeed *experimental_mode_speed;
};
