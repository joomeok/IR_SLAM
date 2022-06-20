#pragma once
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "image_histogram.h"

using namespace sensor_msgs;
cv::Mat Normalize(const cv::Mat &img, int min, int max);

class thermal_node 
{
    public:
     thermal_node(ros::NodeHandle nh_) {
         left_sub = nh_.subscribe<sensor_msgs::Image>("/thermal_14bit/left/image_raw",1,&thermal_node::left_callback,this);
         right_sub = nh_.subscribe<sensor_msgs::Image>("/thermal_14bit/right/image_raw",1,&thermal_node::right_callback,this);
         left_pub = nh_.advertise<sensor_msgs::Image>("/thermal_preprocessed/left",1);
         right_pub = nh_.advertise<sensor_msgs::Image>("/thermal_preprocessed/right",1);
         std::cout << "Node made!" << std::endl;
     }
     
      void left_callback(const ImageConstPtr& left_image){
        cv_bridge::CvImagePtr cv_ptr_left = cv_bridge::toCvCopy(left_image, sensor_msgs::image_encodings::TYPE_16UC1);
        cv::Mat cropped_left = cv_ptr_left->image(cv::Range(0,400),cv::Range(0,640));
        H_left.add_imgs(cropped_left);
        H_left.compute_histo();
        int left_min = H_left.get_min();
        int left_max = H_left.get_max();
        
        cv::Mat normalized_left = Normalize(cropped_left,left_min,left_max);

        sensor_msgs::ImagePtr normalized_left_ptr = cv_bridge::CvImage(std_msgs::Header(), "mono8", normalized_left).toImageMsg();
        left_pub.publish(normalized_left_ptr);
     } 

      void right_callback(const ImageConstPtr& right_image ){
        cv_bridge::CvImagePtr cv_ptr_right = cv_bridge::toCvCopy(right_image, sensor_msgs::image_encodings::TYPE_16UC1);
        cv::Mat cropped_right = cv_ptr_right->image(cv::Range(0,400),cv::Range(0,640));
        H_right.add_imgs(cropped_right);
        H_right.compute_histo();
        int right_min = H_right.get_min();
        int right_max = H_right.get_max(); 

        cv::Mat normalized_right = Normalize(cropped_right,right_min,right_max);
        sensor_msgs::ImagePtr normalized_right_ptr = cv_bridge::CvImage(std_msgs::Header(), "mono8", normalized_right).toImageMsg();
  
        right_pub.publish(normalized_right_ptr);

     }
    private:
     ros::Subscriber left_sub;
     ros::Subscriber right_sub;
     ros::Publisher left_pub;
     ros::Publisher right_pub;
     image_histo H_left;
     image_histo H_right;
};

cv::Mat Normalize(const cv::Mat &img,int min, int max){
    int rows = img.rows;
    int cols = img.cols;
    cv::Mat normalized_ = cv::Mat(rows,cols,CV_8UC1);

    for(int j=0;j<rows;j++){
        for (int i =0;i<cols;i++){
            double pixel_val = (static_cast<double>(img.at<ushort>(j,i)) - min) / (max - min); 
            if (pixel_val<0) pixel_val = 0;
            else if (pixel_val >1) pixel_val = 1; 
            normalized_.at<uchar>(j,i) = 255*pixel_val;
        }
    }
    return normalized_;
}


