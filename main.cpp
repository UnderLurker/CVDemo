#include <cstdint>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

// 显示图片
void showImage(const std::string &name, const cv::Mat &image, int offsetX,
               int offsetY) {

  cv::namedWindow(name, cv::WindowFlags::WINDOW_FREERATIO);
  cv::imshow(name, image);
  cv::resizeWindow(name, cv::Size(image.cols / 5, image.rows / 5 + 30));
  cv::moveWindow(name, offsetX, offsetY);
}

// 反色
void reflectGray(const cv::Mat &gray, cv::Mat &result) {

  if (gray.channels() != 1)
    return;
  gray.copyTo(result);
  gray.forEach<uchar>([&result](uchar &p, const int position[]) {
    result.at<uchar>(position[0], position[1]) = 0xFF - p;
  });

  // for(int i=0;i<gray.rows;i++) {
  //   uchar* curRow = gray.ptr<uchar>(i);
  //   for(int j=0;j<gray.cols;j++) {
  //     result.at<uchar>(i, j) = 0xFF - *curRow++;
  //   }
  // }
}

// C接口调用封装回调函数中使用的数据
struct UserInfo {
  cv::Mat image;
};

// 滑轨回调
void onTrack(int value, void *userdata) {
  UserInfo info = *reinterpret_cast<UserInfo *>(userdata);
  cv::Mat show;
  // 调节亮度
  cv::add(info.image, cv::Scalar(value, value, value), show);
  showImage("src", show, 1000, 1000);
}

// void onClick(int state, void* userdata) {
//   UserInfo info = *reinterpret_cast<UserInfo *>(userdata);
// }

int main() {
  std::string imagePath = "../image/1.jpg";
  cv::Mat src = cv::imread(imagePath);
  if (src.empty()) {
    cout << "Cann't load picture." << endl;
    return -1;
  }

  showImage("origin", src, 1000, 1000);
  showImage("src", src, 1000, 1000);

  UserInfo info{src};
  int maxLight = 100;
  int lightness = 50;
  cv::createTrackbar("lightness", "src", &lightness, maxLight, onTrack, &info);
  // cv::createButton("reflect", onClick);

  cv::waitKey(0);
  cv::destroyAllWindows();
  return 0;
}
