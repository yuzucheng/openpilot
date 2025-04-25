#include "selfdrive/ui/qt/offroad/sunnypilot/userfunc_settings.h"

UserFuncPanel::UserFuncPanel(QWidget *parent) : QFrame(parent) {
  main_layout = new QStackedLayout(this);

  ListWidget *list = new ListWidget(this, false);

  list->addItem(new LabelControl(tr("Turn Speed Configuration")));

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

  // 添加分割线
  list->addItem(horizontal_line());

  //list->addItem(new LabelControl(tr("User Configuration2")));

  /*
  list->addItem(new LabelControl(tr("User Configuration2")));
  list->addItem(horizontal_line());

  list->addItem(new LabelControl(tr("User Configuration3)));
  list->addItem(horizontal_line());

  list->addItem(new LabelControl(tr("User Configuration4")));
  list->addItem(horizontal_line());

  list->addItem(new LabelControl(tr("User Configuration5")));
  list->addItem(horizontal_line());
  */

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
