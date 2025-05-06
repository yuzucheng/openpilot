#include "selfdrive/ui/qt/offroad/sunnypilot/userfunc_settings.h"

UserFuncPanel::UserFuncPanel(QWidget *parent) : QFrame(parent) {
  main_layout = new QStackedLayout(this);

  ListWidget *list = new ListWidget(this, false);

  //============================================================
  list->addItem(new LabelControl(tr("Turn Configuration")));

  auto toggle_vc = new ParamControl(
    "TurnVisionCruise",
    tr("Enable Vision-based Cruise Speed Control (V-CSC)"),
    tr("Use vision path tune cruise speed"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_vc);
  toggles["TurnVisionCruise"] = toggle_vc;

  //list->addItem(new LabelControl(tr("Turn Steep Ness")));
  label_turn_steep_ness = new LabelControl(tr("Turn Steep Ness"));
  list->addItem(label_turn_steep_ness);

  turn_steep_ness = new TurnSteepNess();
  connect(turn_steep_ness, &SPOptionControl::updateLabels, turn_steep_ness, &TurnSteepNess::refresh);
  list->addItem(turn_steep_ness);

  //list->addItem(new LabelControl(tr("Turn Lat Accel")));
  label_turn_lat_accel = new LabelControl(tr("Turn Lat Accel"));
  list->addItem(label_turn_lat_accel);

  turn_lat_acc = new TurnLatAccel();
  connect(turn_lat_acc, &SPOptionControl::updateLabels, turn_lat_acc, &TurnLatAccel::refresh);
  list->addItem(turn_lat_acc);

  //list->addItem(new LabelControl(tr("Turn max factor")));
  label_turn_max_factor = new LabelControl(tr("Turn max factor"));
  list->addItem(label_turn_max_factor);

  turn_max_factor = new TurnMaxFactor();
  connect(turn_max_factor, &SPOptionControl::updateLabels, turn_max_factor, &TurnMaxFactor::refresh);
  list->addItem(turn_max_factor);

  //控制控件的显示
  connect(toggles["TurnVisionCruise"], &ToggleControl::toggleFlipped, [=](bool state) {
    label_turn_steep_ness->setVisible(state);
    label_turn_lat_accel->setVisible(state);
    label_turn_max_factor->setVisible(state);

    turn_steep_ness->setEnabled(state);
    turn_steep_ness->setVisible(state);
    turn_lat_acc->setEnabled(state);
    turn_lat_acc->setVisible(state);
    turn_max_factor->setEnabled(state);
    turn_max_factor->setVisible(state);
  });
  label_turn_steep_ness->setVisible(toggles["TurnVisionCruise"]->isToggled());
  label_turn_lat_accel->setVisible(toggles["TurnVisionCruise"]->isToggled());
  label_turn_max_factor->setVisible(toggles["TurnVisionCruise"]->isToggled());

  turn_steep_ness->setEnabled(toggles["TurnVisionCruise"]->isToggled());
  turn_steep_ness->setVisible(toggles["TurnVisionCruise"]->isToggled());
  turn_lat_acc->setEnabled(toggles["TurnVisionCruise"]->isToggled());
  turn_lat_acc->setVisible(toggles["TurnVisionCruise"]->isToggled());
  turn_max_factor->setEnabled(toggles["TurnVisionCruise"]->isToggled());
  turn_max_factor->setVisible(toggles["TurnVisionCruise"]->isToggled());

  auto toggle_sc = new ParamControl(
    "SteerCruiseTune",
    tr("Enable Steer-based Cruise Speed Control (S-CSC)"),
    tr("Use steer tune cruise speed"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_sc);
  toggles["SteerCruiseTune"] = toggle_sc;

  auto toggle_dis_traffic_light = new ParamControl(
    "DisEnhTrafficLightTurn",
    tr("Disable Enhance trafficLight when turning"),
    tr("Disable the Enhance trafficLight function when turning"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_dis_traffic_light);
  toggles["DisEnhTrafficLightTurn"] = toggle_dis_traffic_light;

  list->addItem(horizontal_line()); // 添加分割线

  //============================================================
  /*
  list->addItem(new LabelControl(tr("Cruise Configuration")));

  auto toggle_auto_cruise = new ParamControl(
    "AutoCruise",
    tr("Enable Auto Cruise"),
    tr("Automatic cruise will be activated when the conditions are met"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_auto_cruise);
  toggles["AutoCruise"] = toggle_auto_cruise;

  list->addItem(new LabelControl(tr("Cruise On Distance")));

  auto cruise_on_distance = new CruiseOnDist();
  connect(cruise_on_distance, &SPOptionControl::updateLabels, cruise_on_distance, &CruiseOnDist::refresh);
  list->addItem(cruise_on_distance);

  list->addItem(horizontal_line()); // 添加分割线
  */

  //============================================================
  toggle_dm = new ParamControl(
    "DisableDM",
    tr("Disable DM"),
    tr("Disable driver monitoring"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_dm);
  toggles["DisableDM"] = toggle_dm;

  auto toggle_dp = new ParamControl(
    "DynamicPersonality",
    tr("Enable Dynamic Personality"),
    tr("Enable this to allow sunnypilot to dynamically adjust following distance and reaction based on your \"Driving Personality\" setting. "
        "Instead of predefined settings for each personality, every personality now adapts dynamically according to your speed and the distance to the lead car."),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_dp);
  toggles["DynamicPersonality"] = toggle_dp;

  list->addItem(new LabelControl(tr("Stop Distance")));

  stop_distance = new StopDistance();
  connect(stop_distance, &SPOptionControl::updateLabels, stop_distance, &StopDistance::refresh);
  list->addItem(stop_distance);

  list->addItem(new LabelControl(tr("Comfort Brake")));
  comfort_break = new ComfortBrake();
  connect(comfort_break, &SPOptionControl::updateLabels, comfort_break, &ComfortBrake::refresh);
  list->addItem(comfort_break);

  list->addItem(new LabelControl(tr("Max Stop Accel")));
  max_stop_accel = new MaxStopAccel();
  connect(max_stop_accel, &SPOptionControl::updateLabels, max_stop_accel, &MaxStopAccel::refresh);
  list->addItem(max_stop_accel);

  list->addItem(new LabelControl(tr("Start Accel")));
  start_accel = new StartAccel();
  connect(start_accel, &SPOptionControl::updateLabels, start_accel, &StartAccel::refresh);
  list->addItem(start_accel);

  list->addItem(new LabelControl(tr("Stop Accel")));
  stop_accel = new StopAccel();
  connect(stop_accel, &SPOptionControl::updateLabels, stop_accel, &StopAccel::refresh);
  list->addItem(stop_accel);

  list->addItem(new LabelControl(tr("vEgoStopping")));
  vego_stopping = new vEgoStopping();
  connect(vego_stopping, &SPOptionControl::updateLabels, vego_stopping, &vEgoStopping::refresh);
  list->addItem(vego_stopping);

  list->addItem(horizontal_line());

  //============================================================
  auto toggle_traffic_light = new ParamControl(
    "EnhanceTrafficLight",
    tr("Enhance trafficLight"),
    tr("Enable the Enhance trafficLight function"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_traffic_light);
  toggles["EnhanceTrafficLight"] = toggle_traffic_light;

  auto toggle_red_light_dis_enh_func = new ParamControl(
    "DisEnhanceTrafficRedLight",
    tr("Disable enhance after red light"),
    tr("Disable the enhance trafficLight function after red light"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_red_light_dis_enh_func);
  toggles["DisEnhanceTrafficRedLight"] = toggle_red_light_dis_enh_func;

  auto cruise_smooth = new ParamControl(
    "CruiseSmooth",
    tr("Cruise Smooth"),
    tr("Smooth the accel when starting the cruise"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(cruise_smooth);
  toggles["CruiseSmooth"] = cruise_smooth;

  auto user_accel_tb = new ParamControl(
    "UserAccelTable",
    tr("User accel limit table"),
    tr("Using the user accel limit table"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(user_accel_tb);
  toggles["UserAccelTable"] = user_accel_tb;

  auto accel_smooth = new ParamControl(
    "AccelSmooth",
    tr("Accel Smooth"),
    tr("Smooth the cruise accel"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(accel_smooth);
  toggles["AccelSmooth"] = accel_smooth;

  list->addItem(horizontal_line());

  //============================================================
  list->addItem(new LabelControl(tr("Condition Experimental Mode")));

  auto cond_experi_mode = new ParamControl(
    "ConditionExperimentalMode",
    tr("Enable Condition Experimental Mode"),
    tr("Enable this toggle to setting the experimental mode, Disable experimental mode when the speed is greater than the specified value or the Angle is greater than the specified value"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(cond_experi_mode);
  toggles["ConditionExperimentalMode"] = cond_experi_mode;

  //list->addItem(new LabelControl(tr("Experimental SteerAngle And Speed Setting")));
  label_experimental_angle_and_speed = new LabelControl(tr("Experimental SteerAngle And Speed Setting"));
  list->addItem(label_experimental_angle_and_speed);

  experimental_mode_angle = new ExperimentalModeAngle();
  connect(experimental_mode_angle, &SPOptionControl::updateLabels, experimental_mode_angle, &ExperimentalModeAngle::refresh);
  list->addItem(experimental_mode_angle);

  experimental_mode_and_speed = new ExperimentalModeAndSpeed();
  connect(experimental_mode_and_speed, &SPOptionControl::updateLabels, experimental_mode_and_speed, &ExperimentalModeAndSpeed::refresh);
  list->addItem(experimental_mode_and_speed);

  //list->addItem(new LabelControl(tr("Experimental Speed Setting")));
  label_experimental_speed = new LabelControl(tr("Experimental Speed Setting"));
  list->addItem(label_experimental_speed);

  experimental_mode_speed = new ExperimentalModeSpeed();
  connect(experimental_mode_speed, &SPOptionControl::updateLabels, experimental_mode_speed, &ExperimentalModeSpeed::refresh);
  list->addItem(experimental_mode_speed);

  //控制控件的显示
  connect(toggles["ConditionExperimentalMode"], &ToggleControl::toggleFlipped, [=](bool state) {
    label_experimental_angle_and_speed->setVisible(state);
    label_experimental_speed->setVisible(state);

    experimental_mode_angle->setEnabled(state);
    experimental_mode_angle->setVisible(state);
    experimental_mode_and_speed->setEnabled(state);
    experimental_mode_and_speed->setVisible(state);
    experimental_mode_speed->setEnabled(state);
    experimental_mode_speed->setVisible(state);
  });
  label_experimental_angle_and_speed->setVisible(toggles["ConditionExperimentalMode"]->isToggled());
  label_experimental_speed->setVisible(toggles["ConditionExperimentalMode"]->isToggled());

  experimental_mode_angle->setEnabled(toggles["ConditionExperimentalMode"]->isToggled());
  experimental_mode_angle->setVisible(toggles["ConditionExperimentalMode"]->isToggled());
  experimental_mode_and_speed->setEnabled(toggles["ConditionExperimentalMode"]->isToggled());
  experimental_mode_and_speed->setVisible(toggles["ConditionExperimentalMode"]->isToggled());
  experimental_mode_speed->setEnabled(toggles["ConditionExperimentalMode"]->isToggled());
  experimental_mode_speed->setVisible(toggles["ConditionExperimentalMode"]->isToggled());

  list->addItem(horizontal_line());
  //========================================================================

  param_watcher = new ParamWatcher(this);
  QObject::connect(param_watcher, &ParamWatcher::paramChanged, [=](const QString &param_name, const QString &param_value) {
    updateToggles();
  });

  userScreen = new QWidget(this);
  QVBoxLayout* vlayout = new QVBoxLayout(userScreen);
  vlayout->setContentsMargins(50, 20, 50, 20);

  scrollView = new ScrollView(list, this);
  vlayout->addWidget(scrollView, 1);
  main_layout->addWidget(userScreen);

  main_layout->setCurrentWidget(userScreen);

  // trigger offroadTransition when going onroad/offroad
  connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
    start_accel->setEnabled(offroad);
    stop_accel->setEnabled(offroad);
    vego_stopping->setEnabled(offroad);
    toggle_dm->setEnabled(offroad);
  });
}

void UserFuncPanel::showEvent(QShowEvent *event) {
  updateToggles();
}

void UserFuncPanel::hideEvent(QHideEvent *event) {
  main_layout->setCurrentWidget(userScreen);
}

void UserFuncPanel::updateToggles(const QString &param_name, const QString &param_value) {
  if (!isVisible()) return;


}


void UserFuncPanel::updateToggles() {

  if (!isVisible()) {
    return;
  }

  auto custom_stock_long_param = params.getBool("CustomStockLong");
  //auto v_tvs = toggles["TurnVisionCruise"];
  //auto is_offroad = params.getBool("IsOffroad");

  auto cp_bytes = params.get("CarParamsPersistent");
  if (!cp_bytes.empty()) {
    AlignedBuffer aligned_buf;
    capnp::FlatArrayMessageReader cmsg(aligned_buf.align(cp_bytes.data(), cp_bytes.size()));
    cereal::CarParams::Reader CP = cmsg.getRoot<cereal::CarParams>();

    if (hasLongitudinalControl(CP) || custom_stock_long_param) {
      //v_tvs->setEnabled(true);
    } else {
      //v_tvs->setEnabled(false);
    }
  } else {
    //v_tvs->setEnabled(false);
  }
}

CruiseOnDist::CruiseOnDist() : SPOptionControl(
  "CruiseOnDist",
  "",
  tr("Cruise On Distance value."),
  "../assets/offroad/icon_blank.png",
  {0, 100},
  1) {

  refresh();
}

void CruiseOnDist::refresh() {
  QString option = QString::fromStdString(params.get("CruiseOnDist"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    setLabel(QString::number(int_value) + " m");  // 直接整数显示
  } else {
    setLabel(option + " m");  // 如果转换失败，直接显示原值
  }
}

StopDistance::StopDistance() : SPOptionControl(
  "StopDistance",
  "",
  tr("Stop Distance value."),
  "../assets/offroad/icon_blank.png",
  {10, 200},
  1) {

  refresh();
}

void StopDistance::refresh() {
  QString option = QString:: fromStdString(params.get("StopDistance"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    double real_value = int_value / 10.0;  // 假设是除以10
    setLabel(QString::number(real_value, 'f', 1) + " m");  // 保留1位小数
  } else {
    setLabel(option + " m");  // 如果转换失败，直接显示原值
  }
}

ComfortBrake::ComfortBrake() : SPOptionControl(
  "ComfortBrake",
  "",
  tr("Comfort Break Setting."),
  "../assets/offroad/icon_blank.png",
  {5, 50},
  1) {

  refresh();
}

void ComfortBrake::refresh() {
  QString option = QString:: fromStdString(params.get("ComfortBrake"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    double real_value = int_value / 10.0;  // 假设是除以10
    setLabel(QString::number(real_value, 'f', 1)+ " m/s^2");  // 保留1位小数
  } else {
    setLabel(option+ " m/s^2");  // 如果转换失败，直接显示原值
  }
}

StartAccel::StartAccel() : SPOptionControl(
  "StartAccel",
  "",
  tr("StartAccel"),
  "../assets/offroad/icon_blank.png",
  {0, 20},
  1) {

  refresh();
}

void StartAccel::refresh() {
  QString option = QString::fromStdString(params.get("StartAccel"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    if(int_value > 0){
      double real_value = int_value / 10.0;  // 假设是除以10
      setLabel(QString::number(real_value, 'f', 1)+ " m/s^2");  // 保留1位小数
    }
    else{
      setLabel(tr("Auto"));
    }
  } else {
    setLabel(option+ " m/s^2");  // 如果转换失败，直接显示原值
  }
}

StopAccel::StopAccel() : SPOptionControl(
  "StopAccel",
  "",
  tr("StopAccel"),
  "../assets/offroad/icon_blank.png",
  {-40, 0},
  1) {

  refresh();
}

void StopAccel::refresh() {
  QString option = QString::fromStdString(params.get("StopAccel"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    if(int_value < 0){
      double real_value = int_value / 10.0;  // 假设是除以10
      setLabel(QString::number(real_value, 'f', 1)+ " m/s^2");  // 保留1位小数
    }
    else{
      setLabel(tr("Auto"));
    }
  } else {
    setLabel(option+ " m/s^2");  // 如果转换失败，直接显示原值
  }
}

MaxStopAccel::MaxStopAccel() : SPOptionControl(
  "MaxStopAccel",
  "",
  tr("MaxStopAccel"),
  "../assets/offroad/icon_blank.png",
  {-40, 0},
  1) {

  refresh();
}

void MaxStopAccel::refresh() {
  QString option = QString::fromStdString(params.get("MaxStopAccel"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    if(int_value < 0){
      double real_value = int_value / 10.0;  // 假设是除以10
      setLabel(QString::number(real_value, 'f', 1)+ " m/s^2");  // 保留1位小数
    }
    else{
      setLabel(tr("Auto"));
    }
  } else {
    setLabel(option+ " m/s^2");  // 如果转换失败，直接显示原值
  }
}

vEgoStopping::vEgoStopping() : SPOptionControl(
  "vEgoStopping",
  "",
  tr("vEgoStopping"),
  "../assets/offroad/icon_blank.png",
  {1, 10},
  1) {

  refresh();
}

void vEgoStopping::refresh() {
  QString option = QString::fromStdString(params.get("vEgoStopping"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    double real_value = int_value / 10.0;
    setLabel(QString::number(real_value, 'f', 1) + " m/s");
  } else {
    setLabel(option+ " m/s");  // 如果转换失败，直接显示原值
  }
}

ExperimentalModeAngle::ExperimentalModeAngle() : SPOptionControl(
  "ExperimentalModeAngle",
  "",
  tr("Disable Experimental Mode when the angle large than the value."),
  "../assets/offroad/icon_blank.png",
  {0, 90},
  5) {

  refresh();
}

void ExperimentalModeAngle::refresh() {
  QString option = QString:: fromStdString(params.get("ExperimentalModeAngle"));
  if (option == "0") {
    setLabel(tr("Disable"));
  } else {
    setLabel(option + " " + tr("Degree"));
  }
}

ExperimentalModeSpeed::ExperimentalModeSpeed() : SPOptionControl(
  "ExperimentalModeSpeed",
  "",
  tr("Disable Experimental Mode when the speed large than the value."),
  "../assets/offroad/icon_blank.png",
  {0, 255},
  5) {

  refresh();
}

void ExperimentalModeSpeed::refresh() {
  QString option = QString:: fromStdString(params.get("ExperimentalModeSpeed"));
  bool is_metric = params.getBool("IsMetric");

  if (option == "0") {
    setLabel(tr("Disable"));
  } else {
    setLabel(option + " " + (is_metric ? tr("km/h") : tr("mph")));
  }
}

ExperimentalModeAndSpeed::ExperimentalModeAndSpeed() : SPOptionControl(
  "ExperimentalModeAndSpeed",
  "",
  tr("Disable Experimental Mode when the speed large than the value."),
  "../assets/offroad/icon_blank.png",
  {0, 255},
  1) {

  refresh();
}

void ExperimentalModeAndSpeed::refresh() {
  QString option = QString:: fromStdString(params.get("ExperimentalModeAndSpeed"));
  bool is_metric = params.getBool("IsMetric");

  if (option == "0") {
    setLabel(tr("Disable"));
  } else {
    setLabel(option + " " + (is_metric ? tr("km/h") : tr("mph")));
  }
}

TurnSteepNess::TurnSteepNess() : SPOptionControl(
  "TurnSteepNess",
  "",
  tr("TurnSteepNess"),
  "../assets/offroad/icon_blank.png",
  {10, 200},
  1) {

  refresh();
}

void TurnSteepNess::refresh() {
  QString option = QString::fromStdString(params.get("TurnSteepNess"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    double real_value = int_value / 10.0;
    setLabel(QString::number(real_value, 'f', 1));
  } else {
    setLabel(option);  // 如果转换失败，直接显示原值
  }
}

TurnLatAccel::TurnLatAccel() : SPOptionControl(
  "TurnLatAccel",
  "",
  tr("TurnLatAccel"),
  "../assets/offroad/icon_blank.png",
  {1, 40},
  1) {

  refresh();
}

void TurnLatAccel::refresh() {
  QString option = QString::fromStdString(params.get("TurnLatAccel"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    double real_value = int_value / 10.0;
    setLabel(QString::number(real_value, 'f', 1) + " m/s^2");
  } else {
    setLabel(option+ " m/s^2");  // 如果转换失败，直接显示原值
  }
}

TurnMaxFactor::TurnMaxFactor() : SPOptionControl(
  "TurnMaxFactor",
  "",
  tr("TurnMaxFactor"),
  "../assets/offroad/icon_blank.png",
  {1, 10},
  1) {

  refresh();
}

void TurnMaxFactor::refresh() {
  QString option = QString::fromStdString(params.get("TurnMaxFactor"));
  bool ok;
  int int_value = option.toInt(&ok);
  if (ok) {
    double real_value = int_value / 10.0;
    setLabel(QString::number(real_value, 'f', 1));
  } else {
    setLabel(option);  // 如果转换失败，直接显示原值
  }
}
