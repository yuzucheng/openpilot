#include "selfdrive/ui/qt/offroad/sunnypilot/userfunc_settings.h"

UserFuncPanel::UserFuncPanel(QWidget *parent) : QFrame(parent) {
  main_layout = new QStackedLayout(this);

  ListWidget *list = new ListWidget(this, false);

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

  auto toggle_sc = new ParamControl(
    "SteerCruiseTune",
    tr("Enable Steer-based Cruise Speed Control (S-CSC)"),
    tr("Use steer tune cruise speed"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_sc);
  toggles["SteerCruiseTune"] = toggle_sc;

  list->addItem(horizontal_line()); // 添加分割线

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

  list->addItem(new LabelControl(tr("Stop Distance")));

  stop_distance = new StopDistance();
  connect(stop_distance, &SPOptionControl::updateLabels, stop_distance, &StopDistance::refresh);
  list->addItem(stop_distance);

  list->addItem(new LabelControl(tr("Comfort Brake")));
  comfort_break = new ComfortBrake();
  connect(comfort_break, &SPOptionControl::updateLabels, comfort_break, &ComfortBrake::refresh);
  list->addItem(comfort_break);

  list->addItem(new LabelControl(tr("Start Accel")));
  start_accel = new StartAccel();
  connect(start_accel, &SPOptionControl::updateLabels, start_accel, &StartAccel::refresh);
  list->addItem(start_accel);

  list->addItem(new LabelControl(tr("vEgoStopping")));
  vego_stopping = new vEgoStopping();
  connect(vego_stopping, &SPOptionControl::updateLabels, vego_stopping, &vEgoStopping::refresh);
  list->addItem(vego_stopping);

  list->addItem(horizontal_line());

  auto toggle_traffic_light = new ParamControl(
    "EnhanceTrafficLight",
    tr("Enhance trafficLight"),
    tr("Enable the Enhance trafficLight function"),
    "../assets/offroad/icon_blank.png",
    this
  );
  list->addItem(toggle_traffic_light);
  toggles["EnhanceTrafficLight"] = toggle_traffic_light;

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

  list->addItem(horizontal_line());

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
}

void UserFuncPanel::showEvent(QShowEvent *event) {
  updateToggles();
}

void UserFuncPanel::hideEvent(QHideEvent *event) {
  main_layout->setCurrentWidget(userScreen);
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

