#include <iostream>
#include <unistd.h>

#include <ros/ros.h>
#include <seabot_fusion/DepthPose.h>
#include <seabot_piston_driver/PistonState.h>
#include <seabot_piston_driver/PistonPosition.h>
#include <seabot_depth_regulation/RegulationDebug2.h>
#include <seabot_mission/Waypoint.h>

#include <std_srvs/SetBool.h>
#include <seabot_fusion/Kalman.h>
#include <eigen3/Eigen/Dense>

#include <cmath>

using namespace std;
using namespace Eigen;

double piston_position = 0;
bool piston_switch_in = false;
bool piston_switch_out = false;

bool is_surface = true;

double depth_set_point = 0.0;
ros::WallTime t_old;

bool emergency = true; // Wait safety clearance on startup

double beta = 0.;
double l = 0.;
double coeff_A = 0.;
double coeff_B = 0.;
double tick_to_volume = 0.;
double coeff_compressibility = 0.;

enum STATE_MACHINE {STATE_SURFACE, STATE_REGULATION};
STATE_MACHINE regulation_state = STATE_SURFACE;

#define NB_STATES 4
// [Velocity; Depth; Volume; Offset]
Matrix<double, NB_STATES, 1> x = Matrix<double, NB_STATES, 1>::Zero();

void piston_callback(const seabot_piston_driver::PistonState::ConstPtr& msg){
  piston_position = msg->position;
  piston_switch_in = msg->switch_in;
  piston_switch_out = msg->switch_out;
}

void kalman_callback(const seabot_fusion::Kalman::ConstPtr& msg){
  x(0) = msg->velocity;
  x(1) = msg->depth;
  x(2) = msg->volume;
  x(3) = msg->offset;
}

void depth_set_point_callback(const seabot_mission::Waypoint::ConstPtr& msg){
  if(msg->mission_enable)
    depth_set_point = msg->depth;
  else
    depth_set_point = 0.0;
}

bool emergency_service(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &res){
  emergency = req.data;
  res.success = true;
  return true;
}

double compute_u(const Matrix<double, NB_STATES, 1> &x, double set_point){
  double e=set_point-x(1);
  double v_eq = x(2)+x(3);
  double dx1 = -coeff_A*(v_eq-coeff_compressibility*x(1))-coeff_B*abs(x(0))*x(0);
  double y = x(0) + beta * atan(e);
  double e2_1 = 1.+pow(e,2);
  double dy = dx1-beta*x(0)/e2_1;

  return (1./coeff_A)*(coeff_A*coeff_compressibility*x(0)-2.*coeff_B*dx1*abs(x(0)) -beta*(2.*pow(x(0), 2)*e + dx1*e2_1)/pow(e2_1,2) +l*dy+y);
}


int main(int argc, char *argv[]){
  ros::init(argc, argv, "sliding_node");
  ros::NodeHandle n;

  // Parameters
  ros::NodeHandle n_private("~");
  const double frequency = n_private.param<double>("frequency", 1.0);

  beta = n_private.param<double>("velocity_target", 0.05)*M_PI_2;
  l = n_private.param<double>("time_constant", 10.0);
  double limit_depth_regulation = n_private.param<double>("limit_depth_regulation", 1.0);
  double speed_volume_sink = n_private.param<double>("speed_volume_sink", 5.0);

  const double hysteresis_piston = n_private.param<double>("hysteresis_piston", 0.6);

  const double rho = n.param<double>("/rho", 1025.0);
  const double g = n.param<double>("/g", 9.81);
  const double m = n.param<double>("/m", 8.800);
  const double diam_collerette = n.param<double>("/diam_collerette", 0.24);
  const double compressibility_tick = n.param<double>("/compressibility_tick", 20.0);
  const double screw_thread = n.param<double>("/screw_thread", 1.75e-3);
  const double tick_per_turn = n.param<double>("/tick_per_turn", 48);
  const double piston_diameter = n.param<double>("/piston_diameter", 0.05);
  const double piston_ref_eq = n.param<double>("/piston_ref_eq", 2100);

  coeff_A = g*rho/m;
  const double Cf = M_PI*pow(diam_collerette/2.0, 2);
  coeff_B = 0.5*rho*Cf/m;
  tick_to_volume = (screw_thread/tick_per_turn)*pow(piston_diameter/2.0, 2)*M_PI;
  coeff_compressibility = compressibility_tick*tick_to_volume;

  // Subscriber
  ros::Subscriber kalman_sub = n.subscribe("/fusion/kalman", 1, kalman_callback);
  ros::Subscriber state_sub = n.subscribe("/driver/piston/state", 1, piston_callback);
  ros::Subscriber depth_set_point_sub = n.subscribe("/mission/set_point", 1, depth_set_point_callback);

  // Publisher
  ros::Publisher position_pub = n.advertise<seabot_piston_driver::PistonPosition>("/driver/piston/position", 1);
  ros::Publisher debug_pub = n.advertise<seabot_depth_regulation::RegulationDebug2>("debug", 1);

  seabot_piston_driver::PistonPosition position_msg;
  seabot_depth_regulation::RegulationDebug2 debug_msg;

  // Server
  ros::ServiceServer server_emergency = n.advertiseService("emergency", emergency_service);


  // Variables
  position_msg.position = 0.0;
  t_old = ros::WallTime::now() - ros::WallDuration(1);
  ros::Rate loop_rate(frequency);

  double piston_position_old = 0.;
  double u = 0.; // in m3
  double piston_set_point;

  // Main regulation loop
  ROS_INFO("[DepthRegulation2] Start Ok");
  while (ros::ok()){
    ros::spinOnce();

    if(depth_set_point>0.2 && !emergency){

      if(regulation_state == STATE_SURFACE){
        if(x(1)<limit_depth_regulation)
          u = -speed_volume_sink*tick_to_volume;
        else
          regulation_state = STATE_REGULATION;
      }
      else
        u = compute_u(x, depth_set_point);

      piston_set_point = x(3)/tick_to_volume + piston_ref_eq - u/tick_to_volume;

      // Mechanical limits (in = v_min, out = v_max)
      if((piston_switch_in && u<0) || (piston_switch_out && u>0))
        u = 0.0;

      /// ********************** Write command ****************** ///
      //  Hysteresis to limit motor movement
      if(abs(piston_position_old - piston_set_point)>hysteresis_piston){
        position_msg.position = round(piston_set_point);
        piston_position_old = piston_set_point;
      }

      // Debug msg
      debug_msg.u = u;
      debug_pub.publish(debug_msg);

      is_surface = false;
    }
    else{
      // Position set point
      regulation_state = STATE_SURFACE;
      position_msg.position = 0;
      is_surface = true;
    }
    position_pub.publish(position_msg);

    loop_rate.sleep();
  }

  return 0;
}
