#include <ros/ros.h>
#include "thermal_node.h"
int main(int argc, char **argv){
    ros::init(argc,argv,"thermal_preprocess_node");
    ros::NodeHandle nh;
    thermal_node therm(nh);
    ros::spin();
    return 0;

}
