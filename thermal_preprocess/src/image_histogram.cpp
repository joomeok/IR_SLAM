#include "image_histogram.h"

image_histo::image_histo() {}

void image_histo::add_imgs(const cv::Mat &img) {
    if (imgs.size() > 10) {
      imgs.erase(imgs.begin());
    }
    imgs.emplace_back(img);
    total_num = img.cols * img.rows * imgs.size();
  }

void image_histo::compute_histo() {
    float range[] = {0, max_range};
    const float *histRange = {range};
    for (int i = 0; i < imgs.size(); i++) {
      cv::calcHist(&imgs[i], 1, 0, cv::Mat(), this->histogram, 1, &hist_size,
                   &histRange, uniform, accumulate);
    }
  }

int image_histo::get_min() {
    int sum = 0;
    int local_min;
    for (int i = 0; i < histogram.rows; i++) {
      sum += histogram.at<float>(i);
      if (sum >= total_num * 0.02) {
        local_min = i * max_range / hist_size + (max_range / hist_size) / 2;
        break;
      }
    }
    return local_min;
  }

int image_histo::get_max() {
    int sum_inv = 0;
    int local_max;
    for (int i = 0; i < histogram.rows; i++) {
      sum_inv += histogram.at<float>(histogram.rows - i);

      if (sum_inv >= total_num * 0.02) {
        local_max = (histogram.rows - i) * max_range / hist_size -
                    (max_range / hist_size) / 2;
        break;
      }
    }
    return local_max;
  }

