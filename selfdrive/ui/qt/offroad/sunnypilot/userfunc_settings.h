#pragma once

#include "common/model.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"

class TurnSteepNess : public SPOptionControl {
  Q_OBJECT

public:
  TurnSteepNess();
  void refresh();
private:
  Params params;
};

class TurnLatAccel : public SPOptionControl {
  Q_OBJECT

public:
  TurnLatAccel();
  void refresh();
private:
  Params params;
};

class TurnMaxFactor : public SPOptionControl {
  Q_OBJECT

public:
  TurnMaxFactor();
  void refresh();
private:
  Params params;
};

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

class StopAccel : public SPOptionControl {
  Q_OBJECT

public:
  StopAccel();
  void refresh();
private:
  Params params;
};

class MaxStopAccel : public SPOptionControl {
  Q_OBJECT

public:
  MaxStopAccel();
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

class JEgoCost : public SPOptionControl {
  Q_OBJECT

public:
  JEgoCost();
  void refresh();
private:
  Params params;
};

class AChangeCostStart : public SPOptionControl {
  Q_OBJECT

public:
  AChangeCostStart();
  void refresh();
private:
  Params params;
};

class AChangeCost : public SPOptionControl {
  Q_OBJECT

public:
  AChangeCost();
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
  void updateToggles(const QString &param_name, const QString &param_value);

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
  StopAccel *stop_accel;
  MaxStopAccel *max_stop_accel;
  vEgoStopping *vego_stopping;

  ExperimentalModeAngle *experimental_mode_angle;
  ExperimentalModeAndSpeed *experimental_mode_and_speed;
  ExperimentalModeSpeed *experimental_mode_speed;

  TurnSteepNess *turn_steep_ness;
  TurnLatAccel *turn_lat_acc;
  TurnMaxFactor *turn_max_factor;

  JEgoCost *j_ego_cost;
  AChangeCostStart *a_change_cost_start;
  AChangeCost *a_change_cost;

  LabelControl *label_turn_steep_ness;
  LabelControl *label_turn_lat_accel;
  LabelControl *label_turn_max_factor;

  LabelControl *label_experimental_angle_and_speed;
  LabelControl *label_experimental_speed;
  ParamControl *toggle_dm;
};
