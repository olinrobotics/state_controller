/*
 * @file Behavior.cpp
 * @brief C++ library containing Behavior class and functions for interfacing
 * with behaviors.
 *
 * @author Connor Novak
 * @email olingravl@gmail.com
 * @date 2019-04-19
 * @version 2.0.0
 */

#include <vector>
#include <string>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <state_controller/BehaviorLib.h>
#include <state_controller/TwistLabeled.h>
#include <state_controller/ArrayLabeled.h>

// Constructor given ROS std_msgs/String as label
Behavior::Behavior(std_msgs::String label, const int priority=2)
 : label(label), priority(priority) { }

// Constructor given c++ std_string as label
Behavior::Behavior(std::string label_raw, const int priority=2)
 : label(), priority(priority) {
   auto label = std_msgs::String();
   label.data = label.data;
 }

int Behavior::getPriority() {
  return priority;
}

std_msgs::String Behavior::getLabel() {
  return label;
}

state_controller::TwistLabeled Behavior::getTwistMessage() {
  return twist_message;
}

state_controller::ArrayLabeled Behavior::getArrayMessage() {
  return array_message;
}

void Behavior::setMessage(state_controller::TwistLabeled msg) {
  //TODO(connor@students): Check that label is equal to id
  twist_message = msg;
}

void Behavior::setMessage(state_controller::ArrayLabeled msg) {
  //TODO(connor@students): Check that label is equal to id
  array_message = msg;
}

// Overload comparison operators to compare priorities
bool Behavior::operator==(Behavior b) {
   if (label.data == b.getLabel().data && priority == b.getPriority()) return true;
   else return false;
}
/*
bool operator<(Behavior b) {
  if (priority < b.getPriority()) return true;
  else return false;
}
bool operator>(Behavior b) {
  if (priority > b.getPriority()) return true;
  else return false;
}
bool operator>=(Behavior b) {
  if (*this>b || *this == b) return true;
  else return false;
}
bool operator<=(Behavior b) {
  if (*this<b || *this == b) return true;
  else return false;
}
*/

/* @brief function to obtain vector of current behaviors from ROS param server
 * @param[in] n: node for use in accessing param server
 * return alphabetized vector of Behavior objects as read from param server
 */
std::vector<Behavior> getBehaviors(ros::NodeHandle n) {

  // Obtain vector of all parameters
  std::vector<std::string> params;
  n.getParamNames(params);

  // Filter for parameters in /behaviors namespace
  std::vector<std::string> filteredParams (params.size());
  auto it = std::copy_if(params.begin(), params.end(), filteredParams.begin(),
               [](std::string param) {
                 if (param.length() < 11) return false;
                 return param.substr(0, 11) == "/behaviors/";
               });

  filteredParams.resize(std::distance(filteredParams.begin(),it));  // shrink container to new size
  std::vector<Behavior> behaviors;
  int counter = 0;
  std::vector<int> tmp;

  for(std::string param : filteredParams)
    // This takes advantage of the assumption that parameters come in alphabetical order.
    // First is up is the id, next is the priority after which the
    // behavior is complete and added to the vector.
    {
      n.getParam(param, tmp);
      Behavior b = Behavior(param.substr(11,param.length()), tmp[0]);
      behaviors.push_back(b);
    }
  return behaviors;
}

bool compareBehaviorPriority(Behavior b1, Behavior b2) {
  /*
   * @brief returns if b1.priority < b2.piority
   *
   * @param[in] 1st behavior to compare
   * @param[in] 2nd behavior to compare
   * @return bool if b1 < b2
   */

   return (b1.getPriority() < b2.getPriority());
}
