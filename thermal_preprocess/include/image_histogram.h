#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class image_histo {
private:
  std::vector<cv::Mat> imgs;
  cv::Mat histogram;
  int hist_size = 256;
  float max_range = 8192;
  bool uniform = true;
  bool accumulate = true;
  int total_num;

public:
  image_histo();
  void add_imgs(const cv::Mat &img);
  void compute_histo();
  int get_min();
  int get_max();
};
