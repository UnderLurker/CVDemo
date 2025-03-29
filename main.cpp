#include <iostream>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void showImage(const std::string& name, const cv::Mat& image, int offsetX, int offsetY) {

  cv::namedWindow(name, cv::WindowFlags::WINDOW_NORMAL);
  cv::imshow(name, image);
  cv::resizeWindow(name, cv::Size(image.cols / 10, image.rows / 10));
  cv::moveWindow(name, offsetX, offsetY);
}

int main() {
  std::string imagePath = "../image/1.jpg";
  cv::Mat src = cv::imread(imagePath);
  if (src.empty()) {
    cout << "Cann't load picture." << endl;
    return -1;
  }

  cv::Mat gray, cloneGray(src.rows, src.cols, CV_8UC1);
  cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);

  for(int i=0;i<gray.rows;i++) {
    uchar* curRow = gray.ptr<uchar>(i);
    for(int j=0;j<gray.cols;j++) {
      cloneGray.at<uchar>(i, j) = 0xFF - *curRow++;
    }
  }

  showImage("gray", gray, 1000, 1000);
  auto rect = cv::getWindowImageRect("gray");
  showImage("cloneGray", cloneGray, 1000 + rect.width, 1000);

  
  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
