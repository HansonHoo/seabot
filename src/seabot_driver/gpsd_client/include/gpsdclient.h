#ifndef GPSDCLIENT_H
#define GPSDCLIENT_H


#include <ros/ros.h>
#include <gpsd_client/GPSFix.h>
#include <gpsd_client/GPSStatus.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/NavSatStatus.h>
#include <libgpsmm.h>

#include <cmath>

class GPSDClient {
  public:
    GPSDClient():privnode("~"){}

    bool start();

    void step();

    void stop();

private:
    void process_data(struct gps_data_t* p);
    void process_data_gps(struct gps_data_t* p);
    void process_data_navsat(struct gps_data_t* p);

    void reset();

  private:
    ros::NodeHandle node;
    ros::NodeHandle privnode;
    ros::Publisher gps_fix_pub;
    gpsmm *gps = nullptr;

    ros::WallTime last_seen;

    bool use_gps_time = true;
    bool check_fix_by_variance = true;
    std::string frame_id = "gps";

    bool m_last_fix_state = true;
};

#endif // GPSDCLIENT_H
