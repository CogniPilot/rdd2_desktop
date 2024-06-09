#include "rviz_common/display_context.hpp"
#include <rclcpp/qos.hpp>
#include <rdd2_rviz_plugins/rdd2_panel.hpp>
#include <rviz_common/panel.hpp>

#include <QVBoxLayout>

// Tell pluginlib about these classes.  It is important to do this in
// global scope, outside our package's namespace.
#include <pluginlib/class_list_macros.hpp>
#include <synapse_msgs/msg/detail/status__struct.hpp>
PLUGINLIB_EXPORT_CLASS(
    rdd2_rviz_plugins::RDD2Panel,
    rviz_common::Panel)

namespace rdd2_rviz_plugins {
RDD2Panel::RDD2Panel(QWidget* parent)
    : rviz_common::Panel(parent)
{
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* top = new QVBoxLayout();
    QHBoxLayout* bottom = new QHBoxLayout();
    QVBoxLayout* bottom_left = new QVBoxLayout();
    QVBoxLayout* bottom_right = new QVBoxLayout();
    layout->addLayout(top);
    layout->addLayout(bottom);
    bottom->addLayout(bottom_left);
    bottom->addLayout(bottom_right);

    // label_status_message_.setVisible(true);

    top->addWidget(&label_status_message_);

    bottom_left->addWidget(&label_safety_);
    bottom_left->addWidget(&label_arming_);
    bottom_left->addWidget(&label_mode_);
    bottom_left->addWidget(&label_fuel_);

    bottom_right->addWidget(&label_fuel_percentage_);
    bottom_right->addWidget(&label_power_);
    bottom_right->addWidget(&label_joy_status_);
    bottom_right->addWidget(&label_joy_source_);

    setStyleSheet(
        "font: bold;"
        "font-size: 20px;"
        "color: black;"
        "border-style: outset;"
        "border-width: 2px;"
        "border-color: grey;"
        "background-color: lightgrey;"
        "selection-color: lightgrey;"
        "selection-background-color: grey;");
    setLayout(layout);
}

void RDD2Panel::onInitialize()
{
    node_ = getDisplayContext()->getRosNodeAbstraction().lock()->get_raw_node();
    sub_status_ = node_->create_subscription<synapse_msgs::msg::Status>(
        "/cerebri/out/status", rclcpp::SystemDefaultsQoS(),
        [this](const synapse_msgs::msg::Status::SharedPtr msg) {
            // std::cout << "recevied status: "  << msg->status_message.data() << std::endl;
            label_status_message_.setText("status: " + QString(msg->status_message.data()));
            QString arming_str = "arming: ";
            if (msg->arming == msg->ARMING_ARMED) {
                arming_str += " armed";
            } else if (msg->arming == msg->ARMING_DISARMED) {
                arming_str += " disarmed";
            } else if (msg->arming == msg->ARMING_UNKNOWN) {
                arming_str += " unknown";
            }
            label_arming_.setText(arming_str);

            QString fuel_str = "fuel status: ";
            if (msg->fuel == msg->FUEL_UNKNOWN) {
                fuel_str += " unknown";
            } else if (msg->fuel == msg->FUEL_LOW) {
                fuel_str += " low";
            } else if (msg->fuel == msg->FUEL_NOMINAL) {
                fuel_str += " nominal";
            } else if (msg->fuel == msg->FUEL_CRITICAL) {
                fuel_str += " critical";
            } else if (msg->fuel == msg->FUEL_OVER_CAPACITY) {
                fuel_str += " over capacity";
            }
            label_fuel_.setText(fuel_str);

            label_fuel_percentage_.setText(QString("fuel: ") + QString::number(msg->fuel_percentage) + "%");

            QString joy_str = "joy: ";
            if (msg->joy == msg->JOY_UNKNOWN) {
                joy_str += " unknown";
            } else if (msg->joy == msg->JOY_LOSS) {
                joy_str += " loss";
            } else if (msg->joy == msg->JOY_DISABLED) {
                joy_str += " disabled";
            } else if (msg->joy == msg->JOY_NOMINAL) {
                joy_str += " nominal";
            } else {
                joy_str += " unhandled";
            }
            label_joy_status_.setText(joy_str);

            QString joy_source_str = "joy source: ";
            if (msg->joy_source == msg->JOY_SOURCE_UNKNOWN) {
                joy_source_str += " unknown";
            } else if (msg->joy_source == msg->JOY_SOURCE_ETHERNET) {
                joy_source_str += " ethernet";
            } else if (msg->joy_source == msg->JOY_SOURCE_CAN) {
                joy_source_str += " can";
            } else if (msg->joy_source == msg->JOY_SOURCE_RADIO_CONTROL) {
                joy_source_str += " radio control";
            } else {
                joy_source_str += " unhandled";
            }
            label_joy_source_.setText(joy_source_str);

            QString safety_str = "safety: ";
            if (msg->safety == msg->SAFETY_UNKNOWN) {
                safety_str += " unknown";
            } else if (msg->safety == msg->SAFETY_UNSAFE) {
                safety_str += " unsafe";
            } else if (msg->safety == msg->SAFETY_SAFE) {
                safety_str += " safe";
            }
            label_safety_.setText(safety_str);

            QString mode_str = "mode: ";
            if (msg->mode == msg->MODE_UNKNOWN) {
                mode_str += " unknown";
            } else if (msg->mode == msg->MODE_ACTUATORS) {
                mode_str += " actuators";
            } else if (msg->mode == msg->MODE_ATTITUDE_RATE) {
                mode_str += " attitude rate";
            } else if (msg->mode == msg->MODE_ATTITUDE) {
                mode_str += " attitude";
            } else if (msg->mode == msg->MODE_ALTITUDE) {
                mode_str += " altitude";
            } else if (msg->mode == msg->MODE_VELOCITY) {
                mode_str += " velocity";
            } else if (msg->mode == msg->MODE_POSITION) {
                mode_str += " position";
            } else if (msg->mode == msg->MODE_ACCELERATION) {
                mode_str += " acceleration";
            } else if (msg->mode == msg->MODE_BEZIER) {
                mode_str += " bezier";
            } else {
                mode_str += " unhandled";
            }
            label_mode_.setText(mode_str);

            label_power_.setText(QString("power: ") + QString::number(msg->power, 'f', 2) + "A");
        });
}

void update()
{
}
} // namespace rdd2_rviz_plugins

// vi: set ts=4 sw=4 et
