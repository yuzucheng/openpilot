#include <cassert>
#include <cmath>
#include <string>
#include <tuple>
#include <vector>
#include <thread> //차선캘리

#include <QDebug>
#include <QProcess>

#include "common/watchdog.h"
#include "common/util.h"
#include "selfdrive/ui/qt/network/networking.h"
#include "selfdrive/ui/qt/offroad/settings.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/widgets/prime.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include "selfdrive/ui/qt/offroad/developer_panel.h"

TogglesPanel::TogglesPanel(SettingsWindow *parent) : ListWidget(parent) {
  // param, title, desc, icon
  std::vector<std::tuple<QString, QString, QString, QString>> toggle_defs{
    {
      "OpenpilotEnabledToggle",
      tr("Enable openpilot"),
      tr("Use the openpilot system for adaptive cruise control and lane keep driver assistance. Your attention is required at all times to use this feature. Changing this setting takes effect when the car is powered off."),
      "../assets/img_chffr_wheel.png",
    },
    {
      "ExperimentalMode",
      tr("Experimental Mode"),
      "",
      "../assets/img_experimental_white.svg",
    },
    {
      "DisengageOnAccelerator",
      tr("Disengage on Accelerator Pedal"),
      tr("When enabled, pressing the accelerator pedal will disengage openpilot."),
      "../assets/offroad/icon_disengage_on_accelerator.svg",
    },
    {
      "IsLdwEnabled",
      tr("Enable Lane Departure Warnings"),
      tr("Receive alerts to steer back into the lane when your vehicle drifts over a detected lane line without a turn signal activated while driving over 31 mph (50 km/h)."),
      "../assets/offroad/icon_warning.png",
    },
    {
      "AlwaysOnDM",
      tr("Always-On Driver Monitoring"),
      tr("Enable driver monitoring even when openpilot is not engaged."),
      "../assets/offroad/icon_monitoring.png",
    },
    {
      "RecordFront",
      tr("Record and Upload Driver Camera"),
      tr("Upload data from the driver facing camera and help improve the driver monitoring algorithm."),
      "../assets/offroad/icon_monitoring.png",
    },
    {
      "IsMetric",
      tr("Use Metric System"),
      tr("Display speed in km/h instead of mph."),
      "../assets/offroad/icon_metric.png",
    },
  };


  std::vector<QString> longi_button_texts{tr("Aggressive"), tr("Standard"), tr("Relaxed") , tr("MoreRelaxed") };
  long_personality_setting = new ButtonParamControl("LongitudinalPersonality", tr("Driving Personality"),
                                          tr("Standard is recommended. In aggressive mode, openpilot will follow lead cars closer and be more aggressive with the gas and brake. "
                                             "In relaxed mode openpilot will stay further away from lead cars. On supported cars, you can cycle through these personalities with "
                                             "your steering wheel distance button."),
                                          "../assets/offroad/icon_speed_limit.png",
                                          longi_button_texts);

  // set up uiState update for personality setting
  QObject::connect(uiState(), &UIState::uiUpdate, this, &TogglesPanel::updateState);

  for (auto &[param, title, desc, icon] : toggle_defs) {
    auto toggle = new ParamControl(param, title, desc, icon, this);

    bool locked = params.getBool((param + "Lock").toStdString());
    toggle->setEnabled(!locked);

    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    // insert longitudinal personality after NDOG toggle
    if (param == "DisengageOnAccelerator") {
      addItem(long_personality_setting);
    }
  }

  // Toggles with confirmation dialogs
  toggles["ExperimentalMode"]->setActiveIcon("../assets/img_experimental.svg");
  toggles["ExperimentalMode"]->setConfirmation(true, true);
}

void TogglesPanel::updateState(const UIState &s) {
  const SubMaster &sm = *(s.sm);

  if (sm.updated("selfdriveState")) {
    auto personality = sm["selfdriveState"].getSelfdriveState().getPersonality();
    if (personality != s.scene.personality && s.scene.started && isVisible()) {
      long_personality_setting->setCheckedButton(static_cast<int>(personality));
    }
    uiState()->scene.personality = personality;
  }
}

void TogglesPanel::expandToggleDescription(const QString &param) {
  toggles[param.toStdString()]->showDescription();
}

void TogglesPanel::showEvent(QShowEvent *event) {
  updateToggles();
}

void TogglesPanel::updateToggles() {
  auto experimental_mode_toggle = toggles["ExperimentalMode"];
  const QString e2e_description = QString("%1<br>"
                                          "<h4>%2</h4><br>"
                                          "%3<br>"
                                          "<h4>%4</h4><br>"
                                          "%5<br>")
                                  .arg(tr("openpilot defaults to driving in <b>chill mode</b>. Experimental mode enables <b>alpha-level features</b> that aren't ready for chill mode. Experimental features are listed below:"))
                                  .arg(tr("End-to-End Longitudinal Control"))
                                  .arg(tr("Let the driving model control the gas and brakes. openpilot will drive as it thinks a human would, including stopping for red lights and stop signs. "
                                          "Since the driving model decides the speed to drive, the set speed will only act as an upper bound. This is an alpha quality feature; "
                                          "mistakes should be expected."))
                                  .arg(tr("New Driving Visualization"))
                                  .arg(tr("The driving visualization will transition to the road-facing wide-angle camera at low speeds to better show some turns. The Experimental mode logo will also be shown in the top right corner."));

  const bool is_release = params.getBool("IsReleaseBranch");
  auto cp_bytes = params.get("CarParamsPersistent");
  if (!cp_bytes.empty()) {
    AlignedBuffer aligned_buf;
    capnp::FlatArrayMessageReader cmsg(aligned_buf.align(cp_bytes.data(), cp_bytes.size()));
    cereal::CarParams::Reader CP = cmsg.getRoot<cereal::CarParams>();

    if (hasLongitudinalControl(CP)) {
      // normal description and toggle
      experimental_mode_toggle->setEnabled(true);
      experimental_mode_toggle->setDescription(e2e_description);
      long_personality_setting->setEnabled(true);
    } else {
      // no long for now
      experimental_mode_toggle->setEnabled(false);
      long_personality_setting->setEnabled(false);
      params.remove("ExperimentalMode");

      const QString unavailable = tr("Experimental mode is currently unavailable on this car since the car's stock ACC is used for longitudinal control.");

      QString long_desc = unavailable + " " + \
                          tr("openpilot longitudinal control may come in a future update.");
      if (CP.getExperimentalLongitudinalAvailable()) {
        if (is_release) {
          long_desc = unavailable + " " + tr("An alpha version of openpilot longitudinal control can be tested, along with Experimental mode, on non-release branches.");
        } else {
          long_desc = tr("Enable the openpilot longitudinal control (alpha) toggle to allow Experimental mode.");
        }
      }
      experimental_mode_toggle->setDescription("<b>" + long_desc + "</b><br><br>" + e2e_description);
    }

    experimental_mode_toggle->refresh();
  } else {
    experimental_mode_toggle->setDescription(e2e_description);
  }
}

DevicePanel::DevicePanel(SettingsWindow *parent) : ListWidget(parent) {
  setSpacing(50);
  addItem(new LabelControl(tr("Dongle ID"), getDongleId().value_or(tr("N/A"))));
  addItem(new LabelControl(tr("Serial"), params.get("HardwareSerial").c_str()));

  // power buttons
  QHBoxLayout* power_layout = new QHBoxLayout();
  power_layout->setSpacing(30);

  QPushButton* reboot_btn = new QPushButton(tr("Reboot"));
  reboot_btn->setObjectName("reboot_btn");
  power_layout->addWidget(reboot_btn);
  QObject::connect(reboot_btn, &QPushButton::clicked, this, &DevicePanel::reboot);
  //차선캘리
  QPushButton *reset_CalibBtn = new QPushButton(tr("ReCalibration"));
  reset_CalibBtn->setObjectName("reset_CalibBtn");
  power_layout->addWidget(reset_CalibBtn);
  QObject::connect(reset_CalibBtn, &QPushButton::clicked, this, &DevicePanel::calibration);

  QPushButton* poweroff_btn = new QPushButton(tr("Power Off"));
  poweroff_btn->setObjectName("poweroff_btn");
  power_layout->addWidget(poweroff_btn);
  QObject::connect(poweroff_btn, &QPushButton::clicked, this, &DevicePanel::poweroff);

  if (false && !Hardware::PC()) {
      connect(uiState(), &UIState::offroadTransition, poweroff_btn, &QPushButton::setVisible);
  }

  addItem(power_layout);

  QHBoxLayout* init_layout = new QHBoxLayout();
  init_layout->setSpacing(30);

  QPushButton* init_btn = new QPushButton(tr("Git Pull & Reboot"));
  init_btn->setObjectName("init_btn");
  init_layout->addWidget(init_btn);
  //QObject::connect(init_btn, &QPushButton::clicked, this, &DevicePanel::reboot);
  QObject::connect(init_btn, &QPushButton::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("Git pull & Reboot?"), tr("Yes"), this)) {

      QProcess process;
      process.start("git", QStringList() << "fetch");
      if (!process.waitForFinished()) {
        ConfirmationDialog::alert(tr("Git fetch process timed out."), this);
        return;
      }
      if (process.exitStatus() != QProcess::NormalExit) {
        ConfirmationDialog::alert(tr("Git fetch process crashed."), this);
        return;
      }
      if (process.exitCode() != 0) {
        ConfirmationDialog::alert(tr("Failed to fetch updates."), this);
        return;
      }

      // Git status to check if there are new updates
      process.start("git", QStringList() << "status" << "-uno");
      process.waitForFinished();

      QString output = process.readAllStandardOutput();
      if (output.isEmpty()) {
        ConfirmationDialog::alert(tr("Failed to read Git status."), this);
        return;
      }
      if (!output.contains("Your branch is behind")) {
        ConfirmationDialog::alert(tr("Already up to date."), this);
        return;
      }

      // Git pull to apply updates
      process.start("git", QStringList() << "pull");
      process.waitForFinished();

      if (process.exitCode() != 0) {
        ConfirmationDialog::alert(tr("Git pull failed. Please check the logs."), this);
        return;
      }

      ConfirmationDialog::alert(tr("Git pull successful. Rebooting..."), this);

      //emit parent->closeSettings();
      //DevicePanel::reboot();
      params.putBool("DoReboot", true);

    }
    });

  QPushButton* default_btn = new QPushButton(tr("Set default"));
  default_btn->setObjectName("default_btn");
  init_layout->addWidget(default_btn);
  //QObject::connect(default_btn, &QPushButton::clicked, this, &DevicePanel::poweroff);
  QObject::connect(default_btn, &QPushButton::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("Set to default?"), tr("Yes"), this)) {
      //emit parent->closeSettings();
      QTimer::singleShot(1000, []() {
        printf("Set to default\n");
        Params().putInt("SoftRestartTriggered", 2);
        printf("Set to default2\n");
        });
    }
    });

  setStyleSheet(R"(
    #reboot_btn { height: 120px; border-radius: 15px; background-color: #2CE22C; }
    #reboot_btn:pressed { background-color: #24FF24; }
    #reset_CalibBtn { height: 120px; border-radius: 15px; background-color: #FFBB00; }
    #reset_CalibBtn:pressed { background-color: #FF2424; }
    #poweroff_btn { height: 120px; border-radius: 15px; background-color: #E22C2C; }
    #poweroff_btn:pressed { background-color: #FF2424; }
    #init_btn { height: 120px; border-radius: 15px; background-color: #2C2CE2; }
    #init_btn:pressed { background-color: #2424FF; }
    #default_btn { height: 120px; border-radius: 15px; background-color: #BDBDBD; }
    #default_btn:pressed { background-color: #A9A9A9; }
  )");
  addItem(init_layout);

  pair_device = new ButtonControl(tr("Pair Device"), tr("PAIR"),
                                  tr("Pair your device with comma connect (connect.comma.ai) and claim your comma prime offer."));
  connect(pair_device, &ButtonControl::clicked, [=]() {
    PairingPopup popup(this);
    popup.exec();
  });
  addItem(pair_device);

  // offroad-only buttons

  auto dcamBtn = new ButtonControl(tr("Driver Camera"), tr("PREVIEW"),
                                   tr("Preview the driver facing camera to ensure that driver monitoring has good visibility. (vehicle must be off)"));
  connect(dcamBtn, &ButtonControl::clicked, [=]() { emit showDriverView(); });
  addItem(dcamBtn);

  auto retrainingBtn = new ButtonControl(tr("Review Training Guide"), tr("REVIEW"), tr("Review the rules, features, and limitations of openpilot"));
  connect(retrainingBtn, &ButtonControl::clicked, [=]() {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to review the training guide?"), tr("Review"), this)) {
      emit reviewTrainingGuide();
    }
  });
  addItem(retrainingBtn);

  if (Hardware::TICI()) {
    auto regulatoryBtn = new ButtonControl(tr("Regulatory"), tr("VIEW"), "");
    connect(regulatoryBtn, &ButtonControl::clicked, [=]() {
      const std::string txt = util::read_file("../assets/offroad/fcc.html");
      ConfirmationDialog::rich(QString::fromStdString(txt), this);
    });
    addItem(regulatoryBtn);
  }

  auto translateBtn = new ButtonControl(tr("Change Language"), tr("CHANGE"), "");
  connect(translateBtn, &ButtonControl::clicked, [=]() {
    QMap<QString, QString> langs = getSupportedLanguages();
    QString selection = MultiOptionDialog::getSelection(tr("Select a language"), langs.keys(), langs.key(uiState()->language), this);
    if (!selection.isEmpty()) {
      // put language setting, exit Qt UI, and trigger fast restart
      params.put("LanguageSetting", langs[selection].toStdString());
      qApp->exit(18);
      watchdog_kick(0);
    }
  });
  addItem(translateBtn);

  QObject::connect(uiState()->prime_state, &PrimeState::changed, [this] (PrimeState::Type type) {
    pair_device->setVisible(type == PrimeState::PRIME_TYPE_UNPAIRED);
  });
  QObject::connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
    for (auto btn : findChildren<ButtonControl *>()) {
      if (btn != pair_device) {
        btn->setEnabled(offroad);
      }
    }
    translateBtn->setEnabled(true);
  });

}

void DevicePanel::updateCalibDescription() {
  QString desc =
      tr("openpilot requires the device to be mounted within 4° left or right and "
         "within 5° up or 9° down. openpilot is continuously calibrating, resetting is rarely required.");
  std::string calib_bytes = params.get("CalibrationParams");
  if (!calib_bytes.empty()) {
    try {
      AlignedBuffer aligned_buf;
      capnp::FlatArrayMessageReader cmsg(aligned_buf.align(calib_bytes.data(), calib_bytes.size()));
      auto calib = cmsg.getRoot<cereal::Event>().getLiveCalibration();
      if (calib.getCalStatus() != cereal::LiveCalibrationData::Status::UNCALIBRATED) {
        double pitch = calib.getRpyCalib()[1] * (180 / M_PI);
        double yaw = calib.getRpyCalib()[2] * (180 / M_PI);
        desc += tr(" Your device is pointed %1° %2 and %3° %4.")
                    .arg(QString::number(std::abs(pitch), 'g', 1), pitch > 0 ? tr("down") : tr("up"),
                         QString::number(std::abs(yaw), 'g', 1), yaw > 0 ? tr("left") : tr("right"));
      }
    } catch (kj::Exception) {
      qInfo() << "invalid CalibrationParams";
    }
  }
  qobject_cast<ButtonControl *>(sender())->setDescription(desc);
}

void DevicePanel::reboot() {
  if (!uiState()->engaged()) {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to reboot?"), tr("Reboot"), this)) {
      // Check engaged again in case it changed while the dialog was open
      if (!uiState()->engaged()) {
        params.putBool("DoReboot", true);
      }
    }
  } else {
    ConfirmationDialog::alert(tr("Disengage to Reboot"), this);
  }
}

//차선캘리
void execAndReboot(const std::string& cmd) {
    system(cmd.c_str());
    Params().putBool("DoReboot", true);
}

void DevicePanel::calibration() {
  if (!uiState()->engaged()) {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to reset calibration?"), tr("ReCalibration"), this)) {
      if (!uiState()->engaged()) {
        std::thread worker(execAndReboot, "cd /data/params/d_tmp;  rm -f CalibrationParams");
        worker.detach();
      }
    }
  } else {
    ConfirmationDialog::alert(tr("Reboot & Disengage to Calibration"), this);
  }
}

void DevicePanel::poweroff() {
  if (!uiState()->engaged()) {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to power off?"), tr("Power Off"), this)) {
      // Check engaged again in case it changed while the dialog was open
      if (!uiState()->engaged()) {
        params.putBool("DoShutdown", true);
      }
    }
  } else {
    ConfirmationDialog::alert(tr("Disengage to Power Off"), this);
  }
}

void SettingsWindow::showEvent(QShowEvent *event) {
  setCurrentPanel(0);
}

void SettingsWindow::setCurrentPanel(int index, const QString &param) {
  panel_widget->setCurrentIndex(index);
  nav_btns->buttons()[index]->setChecked(true);
  if (!param.isEmpty()) {
    emit expandToggleDescription(param);
  }
}

SettingsWindow::SettingsWindow(QWidget *parent) : QFrame(parent) {

  // setup two main layouts
  sidebar_widget = new QWidget;
  QVBoxLayout *sidebar_layout = new QVBoxLayout(sidebar_widget);
  panel_widget = new QStackedWidget();

  // close button
  QPushButton *close_btn = new QPushButton(tr("×"));
  close_btn->setStyleSheet(R"(
    QPushButton {
      font-size: 140px;
      padding-bottom: 20px;
      border-radius: 100px;
      background-color: #292929;
      font-weight: 400;
    }
    QPushButton:pressed {
      background-color: #3B3B3B;
    }
  )");
  close_btn->setFixedSize(200, 200);
  sidebar_layout->addSpacing(45);
  sidebar_layout->addWidget(close_btn, 0, Qt::AlignCenter);
  QObject::connect(close_btn, &QPushButton::clicked, this, &SettingsWindow::closeSettings);

  // setup panels
  DevicePanel *device = new DevicePanel(this);
  QObject::connect(device, &DevicePanel::reviewTrainingGuide, this, &SettingsWindow::reviewTrainingGuide);
  QObject::connect(device, &DevicePanel::showDriverView, this, &SettingsWindow::showDriverView);

  TogglesPanel *toggles = new TogglesPanel(this);
  QObject::connect(this, &SettingsWindow::expandToggleDescription, toggles, &TogglesPanel::expandToggleDescription);

  auto networking = new Networking(this);
  QObject::connect(uiState()->prime_state, &PrimeState::changed, networking, &Networking::setPrimeType);

  QList<QPair<QString, QWidget *>> panels = {
    {tr("Device"), device},
    {tr("Network"), networking},
    {tr("Toggles"), toggles},
    {tr("Software"), new SoftwarePanel(this)},
    {tr("Carrot"), new CarrotPanel(this)},
    {tr("Developer"), new DeveloperPanel(this)},
  };

  nav_btns = new QButtonGroup(this);
  for (auto &[name, panel] : panels) {
    QPushButton *btn = new QPushButton(name);
    btn->setCheckable(true);
    btn->setChecked(nav_btns->buttons().size() == 0);
    btn->setStyleSheet(R"(
      QPushButton {
        color: grey;
        border: none;
        background: none;
        font-size: 65px;
        font-weight: 500;
      }
      QPushButton:checked {
        color: white;
      }
      QPushButton:pressed {
        color: #ADADAD;
      }
    )");
    btn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    nav_btns->addButton(btn);
    sidebar_layout->addWidget(btn, 0, Qt::AlignRight);

    const int lr_margin = name != tr("Network") ? 50 : 0;  // Network panel handles its own margins
    panel->setContentsMargins(lr_margin, 25, lr_margin, 25);

    ScrollView *panel_frame = new ScrollView(panel, this);
    panel_widget->addWidget(panel_frame);

    QObject::connect(btn, &QPushButton::clicked, [=, w = panel_frame]() {
      btn->setChecked(true);
      panel_widget->setCurrentWidget(w);
    });
  }
  sidebar_layout->setContentsMargins(50, 50, 100, 50);

  // main settings layout, sidebar + main panel
  QHBoxLayout *main_layout = new QHBoxLayout(this);

  sidebar_widget->setFixedWidth(500);
  main_layout->addWidget(sidebar_widget);
  main_layout->addWidget(panel_widget);

  setStyleSheet(R"(
    * {
      color: white;
      font-size: 50px;
    }
    SettingsWindow {
      background-color: black;
    }
    QStackedWidget, ScrollView {
      background-color: #292929;
      border-radius: 30px;
    }
  )");
}


#include <QScroller>
#include <QListWidget>

static QStringList get_list(const char* path) {
  QStringList stringList;
  QFile textFile(path);
  if (textFile.open(QIODevice::ReadOnly)) {
    QTextStream textStream(&textFile);
    while (true) {
      QString line = textStream.readLine();
      if (line.isNull()) {
        break;
      } else {
        stringList.append(line);
      }
    }
  }
  return stringList;
}

CarrotPanel::CarrotPanel(QWidget* parent) : QWidget(parent) {
  main_layout = new QStackedLayout(this);
  homeScreen = new QWidget(this);
  carrotLayout = new QVBoxLayout(homeScreen);
  carrotLayout->setMargin(40);

  QHBoxLayout* select_layout = new QHBoxLayout();
  select_layout->setSpacing(30);


  QPushButton* start_btn = new QPushButton(tr("Start"));
  start_btn->setObjectName("start_btn");
  QObject::connect(start_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 0;
    this->togglesCarrot(0);
    updateButtonStyles();
  });

  QPushButton* cruise_btn = new QPushButton(tr("Cruise"));
  cruise_btn->setObjectName("cruise_btn");
  QObject::connect(cruise_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 1;
    this->togglesCarrot(1);
    updateButtonStyles();
  });

  QPushButton* speed_btn = new QPushButton(tr("Speed"));
  speed_btn->setObjectName("speed_btn");
  QObject::connect(speed_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 2;
    this->togglesCarrot(2);
    updateButtonStyles();
  });

  QPushButton* latLong_btn = new QPushButton(tr("Tuning"));
  latLong_btn->setObjectName("latLong_btn");
  QObject::connect(latLong_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 3;
    this->togglesCarrot(3);
    updateButtonStyles();
  });

  QPushButton* disp_btn = new QPushButton(tr("Disp"));
  disp_btn->setObjectName("disp_btn");
  QObject::connect(disp_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 4;
    this->togglesCarrot(4);
    updateButtonStyles();
  });

  QPushButton* path_btn = new QPushButton(tr("Path"));
  path_btn->setObjectName("path_btn");
  QObject::connect(path_btn, &QPushButton::clicked, this, [this]() {
    this->currentCarrotIndex = 5;
    this->togglesCarrot(5);
    updateButtonStyles();
  });


  updateButtonStyles();

  select_layout->addWidget(start_btn);
  select_layout->addWidget(cruise_btn);
  select_layout->addWidget(speed_btn);
  select_layout->addWidget(latLong_btn);
  select_layout->addWidget(disp_btn);
  select_layout->addWidget(path_btn);
  carrotLayout->addLayout(select_layout, 0);

  QWidget* toggles = new QWidget();
  QVBoxLayout* toggles_layout = new QVBoxLayout(toggles);

  cruiseToggles = new ListWidget(this);
  cruiseToggles->addItem(new CValueControl("CruiseButtonMode", "按键: 巡航按键模式", "0:正常,1:用户模式1,2:用户模式2", "../assets/offroad/icon_road.png", 0, 2, 1));
  cruiseToggles->addItem(new CValueControl("CruiseSpeedUnit", "按键: 巡航速度单位", "加减巡航速度时的步进值", "../assets/offroad/icon_road.png", 1, 20, 1));
  cruiseToggles->addItem(new CValueControl("CruiseEcoControl", "巡航: 节能控制(4km/h)", "暂时提高设定速度以提高燃油效率。", "../assets/offroad/icon_road.png", 0, 10, 1));
  //cruiseToggles->addItem(new CValueControl("CruiseSpeedMin", "CRUISE: Speed Lower limit(10)", "Cruise control MIN speed", "../assets/offroad/icon_road.png", 5, 50, 1));
  cruiseToggles->addItem(new CValueControl("AutoSpeedUptoRoadSpeedLimit", "巡航: 自动提速百分比 (0%)", "根据前车速度可超过导航限速的百分比.", "../assets/offroad/icon_road.png", 0, 200, 10));
  //cruiseToggles->addItem(new CValueControl("AutoResumeFromGas", "GAS CRUISE ON: Use", "Auto Cruise on when GAS pedal released, 60% Gas Cruise On automatically", "../assets/offroad/icon_road.png", 0, 3, 1));
  //cruiseToggles->addItem(new CValueControl("AutoResumeFromGasSpeed", "GAS CRUISE ON: Speed(30)", "Driving speed exceeds the set value, Cruise ON", "../assets/offroad/icon_road.png", 20, 140, 5));
  //cruiseToggles->addItem(new CValueControl("TFollowSpeedAddM", "GAP: Additional TFs 40km/h(0)x0.01s", "Speed-dependent additional max(100km/h) TFs", "../assets/offroad/icon_road.png", -100, 200, 5));
  //cruiseToggles->addItem(new CValueControl("TFollowSpeedAdd", "GAP: Additional TFs 100Km/h(0)x0.01s", "Speed-dependent additional max(100km/h) TFs", "../assets/offroad/icon_road.png", -100, 200, 5));
  cruiseToggles->addItem(new CValueControl("TFollowGap1", "第1档: 跟车时间 (110)x0.01秒", "巡航跟车距离设置按键第1档对应的跟车时间", "../assets/offroad/icon_road.png", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("TFollowGap2", "第2档: 跟车时间 (120)x0.01秒", "巡航跟车距离设置按键第2档对应的跟车时间", "../assets/offroad/icon_road.png", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("TFollowGap3", "第3档: 跟车时间 (160)x0.01秒", "巡航跟车距离设置按键第3档对应的跟车时间", "../assets/offroad/icon_road.png", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("TFollowGap4", "第4档: 跟车时间 (180)x0.01秒", "巡航跟车距离设置按键第4档对应的跟车时间", "../assets/offroad/icon_road.png", 70, 300, 5));
  cruiseToggles->addItem(new CValueControl("DynamicTFollow", "动态跟车时间控制(x0.01秒)", "原跟车时间+(期望跟车距离-实际距离)x此值，范围(-0.1~100)", "../assets/offroad/icon_road.png", 0, 100, 5));
  cruiseToggles->addItem(new CValueControl("DynamicTFollowLC", "动态跟车时间控制 (变道时)x%", "变道时跟车时间x(此值/100)", "../assets/offroad/icon_road.png", 0, 100, 5));
  cruiseToggles->addItem(new CValueControl("MyDrivingMode", "驾驶模式: 选择", "1:节能,2:安全,3:常规,4:激进", "../assets/offroad/icon_road.png", 1, 4, 1));
  cruiseToggles->addItem(new CValueControl("MyDrivingModeAuto", "驾驶模式: 自动", "仅常规模式", "../assets/offroad/icon_road.png", 0, 1, 1));
  cruiseToggles->addItem(new CValueControl("TrafficLightDetectMode", "红绿灯 检测模式", "0:无动作, 1:只停车, 2: 停车 和 启动", "../assets/offroad/icon_road.png", 0, 2, 1));
  //cruiseToggles->addItem(new CValueControl("MyEcoModeFactor", "DRIVEMODE: ECO Accel ratio(80%)", "Acceleration ratio in ECO mode", "../assets/offroad/icon_road.png", 10, 95, 5));
  //cruiseToggles->addItem(new CValueControl("MySafeModeFactor", "DRIVEMODE: SAFE ratio(60%)", "Accel/StopDistance/DecelRatio/Gap control ratio", "../assets/offroad/icon_road.png", 10, 90, 10));
  //cruiseToggles->addItem(new CValueControl("MyHighModeFactor", "DRIVEMODE: HIGH ratio(100%)", "AccelRatio control ratio", "../assets/offroad/icon_road.png", 100, 300, 10));

  latLongToggles = new ListWidget(this);
  //latLongToggles->addItem(new CValueControl("AutoLaneChangeSpeed", "LaneChangeSpeed(20)", "", "../assets/offroad/icon_road.png", 1, 100, 5));
  latLongToggles->addItem(new CValueControl("UseLaneLineSpeed", "车道线模式速度(0)", "车道线模式，横向模型预测控制", "../assets/offroad/icon_logic.png", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("UseLaneLineCurveSpeed", "弯道车道线模式速度(0)", "车道线模式，高速", "../assets/offroad/icon_logic.png", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("AdjustLaneOffset", "调整车道偏移(0)cm", "", "../assets/offroad/icon_logic.png", 0, 500, 5));
  latLongToggles->addItem(new CValueControl("AdjustCurveOffset", "调整转弯车道偏移(0)cm", "", "../assets/offroad/icon_logic.png", 0, 500, 5));
  latLongToggles->addItem(new CValueControl("AdjustLaneTime", "调整车道时间偏移(5)x0.01s", "", "../assets/offroad/icon_logic.png", 0, 20, 1));
  latLongToggles->addItem(new CValueControl("CustomSR", "横向: 转向角度比率.1(0)", "Custom SteerRatio", "../assets/offroad/icon_logic.png", 0, 300, 1));
  latLongToggles->addItem(new CValueControl("SteerRatioRate", "横向: 转向比变化率x0.01(100)", "SteerRatio apply rate", "../assets/offroad/icon_logic.png", 30, 170, 1));
  latLongToggles->addItem(new CValueControl("PathOffset", "横向: 路径偏移", "(-)left, (+)right", "../assets/offroad/icon_logic.png", -150, 150, 1));
  //latLongToggles->addItem(horizontal_line());
  //latLongToggles->addItem(new CValueControl("JerkStartLimit", "LONG: JERK START(10)x0.1", "Starting Jerk.", "../assets/offroad/icon_road.png", 1, 50, 1));
  //latLongToggles->addItem(new CValueControl("LongitudinalTuningApi", "LONG: ControlType", "0:velocity pid, 1:accel pid, 2:accel pid(comma)", "../assets/offroad/icon_road.png", 0, 2, 1));
  latLongToggles->addItem(new CValueControl("LongTuningKpV", "纵向: P Gain(100)", "", "../assets/offroad/icon_logic.png", 0, 150, 5));
  latLongToggles->addItem(new CValueControl("LongTuningKiV", "纵向: I Gain(200)", "", "../assets/offroad/icon_logic.png", 0, 2000, 5));
  latLongToggles->addItem(new CValueControl("LongTuningKf", "纵向: FF Gain(100)", "", "../assets/offroad/icon_logic.png", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("LongActuatorDelay", "纵向: 执行器延迟(20)ms", "", "../assets/offroad/icon_logic.png", 0, 200, 5));
  latLongToggles->addItem(new CValueControl("VEgoStopping", "纵向: VEgoStopping(50)", "Stopping factor", "../assets/offroad/icon_logic.png", 1, 100, 5));
  latLongToggles->addItem(new CValueControl("RadarReactionFactor", "纵向: 雷达反应因子(10)", "", "../assets/offroad/icon_logic.png", 0, 200, 10));
  //latLongToggles->addItem(new CValueControl("StartAccelApply", "LONG: StartingAccel 2.0x(0)%", "정지->출발시 가속도의 가속율을 지정합니다 0: 사용안함.", "../assets/offroad/icon_road.png", 0, 100, 10));
  //latLongToggles->addItem(new CValueControl("StopAccelApply", "LONG: StoppingAccel -2.0x(0)%", "정지유지시 브레이크압을 조정합니다. 0: 사용안함. ", "../assets/offroad/icon_road.png", 0, 100, 10));
  latLongToggles->addItem(new CValueControl("LaneChangeNeedTorque", "使用扭矩变变道", "", "../assets/offroad/icon_logic.png", 0, 1, 1));
  latLongToggles->addItem(new CValueControl("StoppingAccel", "纵向: 制动时的初始减速度x0.01(-40)", "", "../assets/offroad/icon_logic.png", -100, 0, 5));
  latLongToggles->addItem(new CValueControl("StopDistanceCarrot", "纵向: 停车距离 (600)cm", "", "../assets/offroad/icon_logic.png", 300, 1000, 10));
  //latLongToggles->addItem(new CValueControl("TraffStopDistanceAdjust", "LONG: TrafficStopDistance adjust(150)cm", "", "../assets/offroad/icon_road.png", -1000, 1000, 10));
  latLongToggles->addItem(new CValueControl("ComfortBrake", "纵向: 舒适制动加速度 (240)x0.01m/s^2", "x0.01 m/s^2", "../assets/offroad/icon_logic.png", 200, 300, 1));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals1", "加速度:0km/h时(160)x0.01m/s^2", "在0km/h速度时的最大加速度值(x0.01m/s^2)", "../assets/offroad/icon_logic.png", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals2", "加速度:40km/h时(120)x0.01m/s^2", "在0km/h速度时的最大加速度值(x0.01m/s^2)", "../assets/offroad/icon_logic.png", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals3", "加速度:60km/h时(100)x0.01m/s^2", "在0km/h速度时的最大加速度值(x0.01m/s^2)", "../assets/offroad/icon_logic.png", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals4", "加速度:80km/h时(80)x0.01m/s^2", "在0km/h速度时的最大加速度值(x0.01m/s^2)", "../assets/offroad/icon_logic.png", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals5", "加速度:110km/h时(70)x0.01m/s^2", "在0km/h速度时的最大加速度值(x0.01m/s^2)", "../assets/offroad/icon_logic.png", 1, 250, 5));
  latLongToggles->addItem(new CValueControl("CruiseMaxVals6", "加速度:140km/h时(60)x0.01m/s^2", "在0km/h速度时的最大加速度值(x0.01m/s^2)", "../assets/offroad/icon_logic.png", 1, 250, 5));
  //latLongToggles->addItem(new CValueControl("CruiseMinVals", "DECEL:(120)", "감속도를 설정합니다.(x0.01m/s^2)", "../assets/offroad/icon_road.png", 50, 250, 5));
  latLongToggles->addItem(new CValueControl("MaxAngleFrames", "最大角度帧(89)", "89:基本, 自动仪表盘出现故障 85~87", "../assets/offroad/icon_logic.png", 80, 100, 1));
  latLongToggles->addItem(new CValueControl("SteerActuatorDelay", "横向:转身执行器延迟(40)ms", "标准", "../assets/offroad/icon_logic.png", 1, 100, 1));
  latLongToggles->addItem(new CValueControl("LateralTorqueCustom", "横向: 自定义扭矩(0)", "", "../assets/offroad/icon_logic.png", 0, 2, 1));
  latLongToggles->addItem(new CValueControl("LateralTorqueAccelFactor", "横向: 扭矩加速因子(2500)", "转矩（扭矩）与加速度之间的关系，参数越大可以扭矩越大", "../assets/offroad/icon_logic.png", 1000, 6000, 10));
  latLongToggles->addItem(new CValueControl("LateralTorqueFriction", "横向: 扭矩摩擦(100)", "", "../assets/offroad/icon_logic.png", 0, 1000, 10));
  latLongToggles->addItem(new CValueControl("CustomSteerMax", "横向: 自定义最大转向(0)", "", "../assets/offroad/icon_logic.png", 0, 30000, 5));
  latLongToggles->addItem(new CValueControl("CustomSteerDeltaUp", "LAT: 自定义转向增加变化量(0)", "", "../assets/offroad/icon_logic.png", 0, 50, 1));
  latLongToggles->addItem(new CValueControl("CustomSteerDeltaDown", "LAT: 自定义转向减小变化量(0)", "", "../assets/offroad/icon_logic.png", 0, 50, 1));

  dispToggles = new ListWidget(this);
  //dispToggles->addItem(new CValueControl("ShowHudMode", "DISP:Display Mode", "0:Frog,1:APilot,2:Bottom,3:Top,4:Left,5:Left-Bottom", "../assets/offroad/icon_shell.png", 0, 5, 1));
  dispToggles->addItem(new CValueControl("ShowDebugUI", "显示:调试信息", "", "../assets/offroad/icon_shell.png", 0, 2, 1));
  dispToggles->addItem(new CValueControl("ShowDateTime", "显示:时间信息", "0:无,1:时间/日期,2:时间,3:日期", "../assets/offroad/icon_calendar.png", 0, 3, 1));
  //dispToggles->addItem(new CValueControl("ShowSteerRotate", "DISP:Handle rotate", "0:None,1:Rotate", "../assets/offroad/icon_shell.png", 0, 1, 1));
  dispToggles->addItem(new CValueControl("ShowPathEnd", "显示:路径终点", "0:无,1:显示", "../assets/offroad/icon_shell.png", 0, 1, 1));
  dispToggles->addItem(new CValueControl("ShowAccelRpm", "显示:加速仪表", "0:无,1:显示,2:加速度+转速", "../assets/offroad/icon_shell.png", 0, 2, 1));
  //dispToggles->addItem(new CValueControl("ShowTpms", "DISP:TPMS", "0:None,1:Display", "../assets/offroad/icon_shell.png", 0, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowSteerMode", "DISP:Handle Display Mode", "0:Black,1:Color,2:None", "../assets/offroad/icon_shell.png", 0, 2, 1));
  dispToggles->addItem(new CValueControl("ShowDeviceState", "显示:设备状态", "0:无,1:显示", "../assets/offroad/icon_shell.png", 0, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowConnInfo", "DISP:APM connection", "0:NOne,1:Display", "../assets/offroad/icon_shell.png", 0, 1, 1));
  dispToggles->addItem(new CValueControl("ShowLaneInfo", "显示:道路显示", "-1:无, 0:路径, 1:路径+车道, 2: 路径+车道+路沿", "../assets/offroad/icon_shell.png", -1, 2, 1));
  //dispToggles->addItem(new CValueControl("ShowBlindSpot", "DISP:BSD Info", "0:None,1:Display", "../assets/offroad/icon_shell.png", 0, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowGapInfo", "DISP:GAP Info", "0:None,1:Display", "../assets/offroad/icon_shell.png", -1, 1, 1));
  //dispToggles->addItem(new CValueControl("ShowDmInfo", "DISP:DM Info", "0:None,1:Display,-1:Disable(Reboot)", "../assets/offroad/icon_shell.png", -1, 1, 1));
  dispToggles->addItem(new CValueControl("ShowRadarInfo", "显示:雷达信息", "0:无,1:显示,2:相对位置,3:停止的车辆", "../assets/offroad/icon_shell.png", 0, 3, 1));
  dispToggles->addItem(new CValueControl("ShowRouteInfo", "显示:导航路线信息", "0:无,1:显示", "../assets/offroad/icon_shell.png", 0, 1, 1));
  dispToggles->addItem(new CValueControl("ShowPlotMode", "显示:调试信息数量", "", "../assets/offroad/icon_shell.png", 0, 10, 1));
  dispToggles->addItem(new CValueControl("ShowCustomBrightness", "亮度对比度", "", "../assets/offroad/icon_brightness.png", 0, 100, 10));

  pathToggles = new ListWidget(this);
  pathToggles->addItem(new CValueControl("ShowPathModeCruiseOff", "显示: 道路形态: 巡航关闭时", "0:Normal,1,2:Rec,3,4:^^,5,6:Rec,7,8:^^,9,10,11,12:Smooth^^", "../assets/offroad/icon_shell.png", 0, 15, 1));
  pathToggles->addItem(new CValueControl("ShowPathColorCruiseOff", "显示: 道路颜色: 巡航关闭时", "(+10:Stroke)0:Red,1:Orange,2:Yellow,3:Green,4:Blue,5:Indigo,6:Violet,7:Brown,8:White,9:Black", "../assets/offroad/icon_shell.png", 0, 19, 1));
  pathToggles->addItem(new CValueControl("ShowPathMode", "显示:道路形态: 无车道线时", "0:Normal,1,2:Rec,3,4:^^,5,6:Rec,7,8:^^,9,10,11,12:Smooth^^", "../assets/offroad/icon_shell.png", 0, 15, 1));
  pathToggles->addItem(new CValueControl("ShowPathColor", "显示:道路颜色: 无车道线时", "(+10:Stroke)0:Red,1:Orange,2:Yellow,3:Green,4:Blue,5:Indigo,6:Violet,7:Brown,8:White,9:Black", "../assets/offroad/icon_shell.png", 0, 19, 1));
  pathToggles->addItem(new CValueControl("ShowPathModeLane", "显示:道路形态: 车道线模式时", "0:Normal,1,2:Rec,3,4:^^,5,6:Rec,7,8:^^,9,10,11,12:Smooth^^", "../assets/offroad/icon_shell.png", 0, 15, 1));
  pathToggles->addItem(new CValueControl("ShowPathColorLane", "显示:道路颜色: 车道线模式时", "(+10:Stroke)0:Red,1:Orange,2:Yellow,3:Green,4:Blue,5:Indigo,6:Violet,7:Brown,8:White,9:Black", "../assets/offroad/icon_shell.png", 0, 19, 1));
  pathToggles->addItem(new CValueControl("ShowPathWidth", "显示:道路宽度比率(100%)", "", "../assets/offroad/icon_shell.png", 10, 200, 10));

  startToggles = new ListWidget(this);
  QString selected = QString::fromStdString(Params().get("CarSelected3"));
  QPushButton* selectCarBtn = new QPushButton(selected.length() > 1 ? selected : tr("SELECT YOUR CAR"));
  selectCarBtn->setObjectName("selectCarBtn");
  selectCarBtn->setStyleSheet(R"(
    QPushButton {
      margin-top: 20px; margin-bottom: 20px; padding: 10px; height: 120px; border-radius: 15px;
      color: #FFFFFF; background-color: #2C2CE2;
    }
    QPushButton:pressed {
      background-color: #2424FF;
    }
  )");
  //selectCarBtn->setFixedSize(350, 100);
  connect(selectCarBtn, &QPushButton::clicked, [=]() {
    QString selected = QString::fromStdString(Params().get("CarSelected3"));

    QStringList all_items = get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars").toStdString().c_str());
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_gm").toStdString().c_str()));
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_toyota").toStdString().c_str()));
    all_items.append(get_list((QString::fromStdString(Params().getParamPath()) + "/SupportedCars_mazda").toStdString().c_str()));

    QMap<QString, QStringList> car_groups;
    for (const QString& car : all_items) {
      QStringList parts = car.split(" ", QString::SkipEmptyParts);
      if (!parts.isEmpty()) {
        QString manufacturer = parts.first();
        car_groups[manufacturer].append(car);
      }
    }

    QStringList manufacturers = car_groups.keys();
    QString selectedManufacturer = MultiOptionDialog::getSelection("Select Manufacturer", manufacturers, manufacturers.isEmpty() ? "" : manufacturers.first(), this);

    if (!selectedManufacturer.isEmpty()) {
      QStringList cars = car_groups[selectedManufacturer];
      QString selectedCar = MultiOptionDialog::getSelection("Select your car", cars, selected, this);

      if (!selectedCar.isEmpty()) {
        if (selectedCar == "[ Not Selected ]") {
          Params().remove("CarSelected3");
        } else {
          printf("Selected Car: %s\n", selectedCar.toStdString().c_str());
          Params().put("CarSelected3", selectedCar.toStdString());
          QTimer::singleShot(1000, []() {
            Params().putInt("SoftRestartTriggered", 1);
          });
          ConfirmationDialog::alert(selectedCar, this);
        }
        selected = QString::fromStdString(Params().get("CarSelected3"));
        selectCarBtn->setText((selected.isEmpty() || selected == "[ Not Selected ]") ? tr("SELECT YOUR CAR") : selected);
      }
    }
  });

  startToggles->addItem(selectCarBtn);
  startToggles->addItem(new ParamControl("HyundaiCameraSCC", "现代: 使用CAMERA SCC", "连接SCC的CAN线路到相机，一般情况不打开", "../assets/offroad/icon_shell.png", this));
  startToggles->addItem(new ParamControl("EnableRadarTracks", "启用雷达追踪", "", "../assets/offroad/icon_shell.png", this));
  startToggles->addItem(new CValueControl("CanfdHDA2", "CANFD: HDA2模式", "1:HDA2,2:HDA2+BSM", "../assets/offroad/icon_shell.png", 0, 2, 1));
  startToggles->addItem(new CValueControl("AutoCruiseControl", "自动巡航控制", "Softhold, Auto Cruise ON/OFF control", "../assets/offroad/icon_road.png", 0, 3, 1));
  startToggles->addItem(new CValueControl("CruiseOnDist", "巡航: 自动开启的距离(0cm)", "当没有踩油门和刹车时, 离前车小于此距离时开启巡航以保持安全距离.", "../assets/offroad/icon_road.png", 0, 2500, 50));
  startToggles->addItem(new CValueControl("AutoEngage", "车辆启动时自动开启的功能", "1:转向控制, 2:转向控制 和 巡航控制", "../assets/offroad/icon_road.png", 0, 2, 1));
  startToggles->addItem(new ParamControl("DisableMinSteerSpeed", "禁用自动转向的最小速度", "是否允许速度低于特定值的时候禁止转向控制", "../assets/offroad/icon_road.png", this));
  startToggles->addItem(new CValueControl("AutoGasTokSpeed", "Auto AccelTok speed", "Gas(Accel)Tok enable speed", "../assets/offroad/icon_road.png", 0, 200, 5));
  startToggles->addItem(new ParamControl("AutoGasSyncSpeed", "自动更新巡航速度", "打开此功能后，将会看到仪表盘的巡航速度会变化", "../assets/offroad/icon_road.png", this));
  startToggles->addItem(new CValueControl("SpeedFromPCM", "从车辆PCM中读取巡航速度", "Toyota设置为1, Honda设置为3", "../assets/offroad/icon_road.png", 0, 3, 1));
  startToggles->addItem(new CValueControl("SoundVolumeAdjust", "音量(100%)", "", "../assets/offroad/icon_sound.png", 5, 200, 5));
  startToggles->addItem(new CValueControl("SoundVolumeAdjustEngage", "音量, Engage(10%)", "", "../assets/offroad/icon_sound.png", 5, 200, 5));
  startToggles->addItem(new CValueControl("MaxTimeOffroadMin", "关机时间 (分钟)", "", "../assets/offroad/icon_sandtimer.png", 1, 600, 10));
  startToggles->addItem(new ParamControl("DisableDM", "关闭驾驶员监控", "", "../assets/img_driver_face_static_x.png", this));
  //startToggles->addItem(new CValueControl("CarrotCountDownSpeed", "NaviCountDown Speed(10)", "", "../assets/offroad/icon_shell.png", 0, 200, 5));
  startToggles->addItem(new CValueControl("MapboxStyle", "地图样式(0)", "", "../assets/offroad/icon_shell.png", 0, 2, 1));
  startToggles->addItem(new CValueControl("RecordRoadCam", "摄像头视频记录(0)", "1:道路摄像头, 2:道路摄像头+广角摄像头", "../assets/offroad/icon_shell.png", 0, 2, 1));
  startToggles->addItem(new ParamControl("HotspotOnBoot", "车辆启动时自动启用热点功能", "", "../assets/offroad/icon_shell.png", this));
  //startToggles->addItem(new ParamControl("NoLogging", "Disable Logger", "", "../assets/offroad/icon_shell.png", this));
  //startToggles->addItem(new ParamControl("LaneChangeNeedTorque", "LaneChange: Need Torque", "", "../assets/offroad/icon_shell.png", this));
  //startToggles->addItem(new CValueControl("LaneChangeLaneCheck", "LaneChange: Check lane exist", "(0:No,1:Lane,2:+Edge)", "../assets/offroad/icon_shell.png", 0, 2, 1));
  startToggles->addItem(new CValueControl("NNFF", "NNFF", "Twilsonco's NNFF(Reboot required)", "../assets/offroad/icon_road.png", 0, 1, 1));
  startToggles->addItem(new CValueControl("NNFFLite", "NNFFLite", "Twilsonco's NNFF-Lite(Reboot required)", "../assets/offroad/icon_road.png", 0, 1, 1));

  speedToggles = new ListWidget(this);
  speedToggles->addItem(new CValueControl("AutoCurveSpeedLowerLimit", "弯道: 最低限制速度(30)", "转弯降速的最低速度", "../assets/offroad/icon_road.png", 30, 200, 5));
  speedToggles->addItem(new CValueControl("AutoCurveSpeedFactor", "弯道: 自动控制比例(100%)", "", "../assets/offroad/icon_road.png", 50, 300, 1));
  speedToggles->addItem(new CValueControl("AutoCurveSpeedAggressiveness", "弯道: 激进比例 (100%)", "", "../assets/offroad/icon_road.png", 50, 300, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedCtrlEnd", "导航时遇测速设备提前减速时间(6s)", "当检测到有测速设备时，进行提前减速的时间", "../assets/offroad/icon_road.png", 3, 20, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedCtrlMode", "遇测速设备减速模式", "0:不减速;1:测速摄像头;2:违章摄像头;3:+移动测速", "../assets/offroad/icon_road.png", 0, 3, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedDecelRate", "遇测速设备减速度x0.01m/s^2(80)", "减速度越低，则会从越远处开始减速", "../assets/offroad/icon_road.png", 10, 200, 10));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedSafetyFactor", "测速摄像头安全因子(105%)", "", "../assets/offroad/icon_road.png", 80, 120, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedBumpTime", "减速带减速距离(1s)", "", "../assets/offroad/icon_road.png", 1, 50, 1));
  speedToggles->addItem(new CValueControl("AutoNaviSpeedBumpSpeed", "减速带速度(35Km/h)", "", "../assets/offroad/icon_road.png", 10, 100, 5));
  speedToggles->addItem(new CValueControl("AutoNaviCountDownMode", "导航倒计时模式(2)", "0: 关, 1:转弯+摄像头, 2:转弯+摄像头+减速带", "../assets/offroad/icon_road.png", 0, 2, 1));
  speedToggles->addItem(new CValueControl("TurnSpeedControlMode", "转弯速度控制模式(1)", "0: 关, 1:视觉, 2:视觉+导航路线, 3: 导航路线", "../assets/offroad/icon_road.png", 0, 3, 1));
  speedToggles->addItem(new CValueControl("MapTurnSpeedFactor", "地图转弯速度权重(100)", "", "../assets/offroad/icon_map.png", 50, 300, 5));
  speedToggles->addItem(new CValueControl("AutoTurnControl", "ATC: 自动转弯控制(0)", "0:无，1:车道变更，2:车道变更+速度，3:速度", "../assets/offroad/icon_road.png", 0, 3, 1));
  speedToggles->addItem(new CValueControl("AutoTurnControlSpeedTurn", "ATC: 转弯速度 (20)", "0:无转弯速度", "../assets/offroad/icon_road.png", 0, 100, 5));
  speedToggles->addItem(new CValueControl("AutoTurnControlTurnEnd", "ATC: 转弯控制距离时间 (6)s", "距离=速度*时间", "../assets/offroad/icon_road.png", 0, 30, 1));
  speedToggles->addItem(new CValueControl("AutoRoadSpeedAdjust", "自动调整道路限速 (50%)", "", "../assets/offroad/icon_road.png", 0, 100, 10));
  speedToggles->addItem(new CValueControl("AutoTurnMapChange", "ATC: 自动切换地图(0)", "", "../assets/offroad/icon_road.png", 0, 1, 1));

  toggles_layout->addWidget(cruiseToggles);
  toggles_layout->addWidget(latLongToggles);
  toggles_layout->addWidget(dispToggles);
  toggles_layout->addWidget(pathToggles);
  toggles_layout->addWidget(startToggles);
  toggles_layout->addWidget(speedToggles);
  ScrollView* toggles_view = new ScrollView(toggles, this);
  carrotLayout->addWidget(toggles_view, 1);

  homeScreen->setLayout(carrotLayout);
  main_layout->addWidget(homeScreen);
  main_layout->setCurrentWidget(homeScreen);

  togglesCarrot(0);
}

void CarrotPanel::togglesCarrot(int widgetIndex) {
  startToggles->setVisible(widgetIndex == 0);
  cruiseToggles->setVisible(widgetIndex == 1);
  speedToggles->setVisible(widgetIndex == 2);
  latLongToggles->setVisible(widgetIndex == 3);
  dispToggles->setVisible(widgetIndex == 4);
  pathToggles->setVisible(widgetIndex == 5);
}

void CarrotPanel::updateButtonStyles() {
  QString styleSheet = R"(
      #start_btn, #cruise_btn, #speed_btn, #latLong_btn ,#disp_btn, #path_btn {
          height: 120px; border-radius: 15px; background-color: #393939;
      }
      #start_btn:pressed, #cruise_btn:pressed, #speed_btn:pressed, #latLong_btn:pressed, #disp_btn:pressed, #path_btn:pressed {
          background-color: #4a4a4a;
      }
  )";

  switch (currentCarrotIndex) {
  case 0:
      styleSheet += "#start_btn { background-color: #33ab4c; }";
      break;
  case 1:
      styleSheet += "#cruise_btn { background-color: #33ab4c; }";
      break;
  case 2:
      styleSheet += "#speed_btn { background-color: #33ab4c; }";
      break;
  case 3:
      styleSheet += "#latLong_btn { background-color: #33ab4c; }";
      break;
  case 4:
      styleSheet += "#disp_btn { background-color: #33ab4c; }";
      break;
  case 5:
      styleSheet += "#path_btn { background-color: #33ab4c; }";
      break;
  }

  setStyleSheet(styleSheet);
}


CValueControl::CValueControl(const QString& params, const QString& title, const QString& desc, const QString& icon, int min, int max, int unit)
    : AbstractControl(title, desc, icon), m_params(params), m_min(min), m_max(max), m_unit(unit) {

    label.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    label.setStyleSheet("color: #e0e879");
    hlayout->addWidget(&label);

    QString btnStyle = R"(
      QPushButton {
        padding: 0;
        border-radius: 50px;
        font-size: 35px;
        font-weight: 500;
        color: #E4E4E4;
        background-color: #393939;
      }
      QPushButton:pressed {
        background-color: #4a4a4a;
      }
    )";

    btnminus.setStyleSheet(btnStyle);
    btnplus.setStyleSheet(btnStyle);
    btnminus.setFixedSize(150, 100);
    btnplus.setFixedSize(150, 100);
    btnminus.setText("－");
    btnplus.setText("＋");
    hlayout->addWidget(&btnminus);
    hlayout->addWidget(&btnplus);

    connect(&btnminus, &QPushButton::released, this, &CValueControl::decreaseValue);
    connect(&btnplus, &QPushButton::released, this, &CValueControl::increaseValue);

    refresh();
}

void CValueControl::showEvent(QShowEvent* event) {
    AbstractControl::showEvent(event);
    refresh();
}

void CValueControl::refresh() {
    label.setText(QString::fromStdString(Params().get(m_params.toStdString())));
}

void CValueControl::adjustValue(int delta) {
    int value = QString::fromStdString(Params().get(m_params.toStdString())).toInt();
    value = qBound(m_min, value + delta, m_max);
    Params().putInt(m_params.toStdString(), value);
    refresh();
}

void CValueControl::increaseValue() {
    adjustValue(m_unit);
}

void CValueControl::decreaseValue() {
    adjustValue(-m_unit);
}
