#include <iostream>
#include <unistd.h>

#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/NavSatStatus.h>
#include <gnss_neom8l_driver/PoseCartesian.h>

#include "neom8l.h"

#include <proj_api.h>

using namespace std;

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "gnss_neom8l");
  ros::NodeHandle n;

  // Parameters
  ros::NodeHandle n_private("~");
  double offset_east = n_private.param<double>("offset_east", 0.0);
  double offset_north = n_private.param<double>("offset_north", 0.0);

  // Publishers
  ros::Publisher navSatFix_pub = n.advertise<sensor_msgs::NavSatFix>("fix", 1);
  ros::Publisher pose_pub = n.advertise<gnss_neom8l_driver::PoseCartesian>("pose", 1);

  // Sensor init
  NeoM8L sensor;
//  sensor.i2c_open();

  // Loop with sensor reading
  sensor_msgs::NavSatFix navSatFix_msg;
  gnss_neom8l_driver::PoseCartesian pose_msg;

//  ros::Rate loop_rate(frequency);
  while (ros::ok()){
    // ToDo
//    sensor.read_data();
    sensor.convert_data(); // Debug
    pose_msg.east = sensor.get_east();
    pose_msg.north = sensor.get_north();
    pose_msg.time_month = sensor.get_time_month();
    pose_pub.publish(pose_msg);

    if(sensor.get_time_data() != sensor.get_nmea_info().utc.sec){
      sensor.convert_data();

      // Lat Long
      navSatFix_msg.altitude = sensor.get_nmea_info().elv;
      navSatFix_msg.latitude = sensor.get_lat();
      navSatFix_msg.longitude = sensor.get_lon();
      navSatFix_msg.status.status = sensor.get_nmea_info().sig;
      navSatFix_msg.header.stamp = ros::Time::now();
      navSatFix_pub.publish(navSatFix_msg);

      // Local frame
      pose_msg.east = sensor.get_east() - offset_east;
      pose_msg.north = sensor.get_north() - offset_north;
      pose_msg.time_month = sensor.get_time_month();
      pose_pub.publish(pose_msg);

      sensor.update_time_data(); // To enable next message
      ros::spinOnce();
    }

    usleep(10000);
  }

  return 0;
}