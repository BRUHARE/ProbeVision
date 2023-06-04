#pragma once
//备注：以下‘图片’均是去畸变后的图片

#include <string>
#include <Eigen/Dense>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>


struct AlgCfg{             //算法配置
    double R;              //探测的半径（单位 米）
    Eigen::Matrix3d K;     //相机内参
    int org_img_height;    //原始图片行数
    int org_img_width;     //原始图片列数
};

class Pro2Cylinder{
public:
    Pro2Cylinder(const AlgCfg& c){
        cfg = c;
        K_inv = c.K.inverse();
    }
    /**
     * @brief  计算默认参数
     * @param  roi_R_pixel 输入参数：原始图片上感兴趣的外环半径(单位：像素)
     * @param  roi_r_pixel 输入参数：原始图片上感兴趣的内环半径(单位：像素)
     * @param  pixel_per_meter 输出参数：拼接后的图像的分辨率(单位：像素/米)
     * @param  h_min 输出参数：拼接后的图像首行代表的高度（高度是以当前相机为原点，坐标轴朝下，单位：米）
     * @param  h_max 输出参数：拼接后的图像末行代表的高度（高度是以当前相机为原点，坐标轴朝下，单位：米）
     * @return 执行状态
     */
    bool CalcDefaultParameter(int roi_R_pixel, int roi_r_pixel, float &pixel_per_meter, float &h_min, float &h_max);
    
    /**
     * @brief  调整参数
     * @param  pixel_per_meter 输入参数：拼接后的图像的分辨率(单位：像素/米)
     * @param  h_min 输入参数：拼接后的图像首行代表的高度（高度是以当前相机为原点，坐标轴朝下，单位：米）
     * @param  h_max 输入参数：拼接后的图像末行代表的高度（高度是以当前相机为原点，坐标轴朝下，单位：米）
     * @param  tx 输入参数：圆柱中心到相机的x轴偏移 (单位：米)
     * @param  ty 输入参数：圆柱中心到相机的y轴偏移 (单位：米)
     * @param  roll 输入参数：滚转角(R_圆柱_相机,RPY变换，弧度制)
     * @param  pitch 输入参数：俯仰角(R_圆柱_相机,RPY变换，弧度制)
     * @param  yaw 输入参数：方向角(R_圆柱_相机,RPY变换，弧度制)
     * @param  org_img 输入参数：原始图片
     * @param  project_point 输入参数：待变换的展开图片的多组像素坐标
     * @param  org_point 输出参数：变换后的原始图片的多组像素坐标
     * @return 执行状态
     */
    bool AdjustParam(float pixel_per_meter,double h_min, double h_max, double tx, double ty, double roll, double pitch, double yaw, const cv::Mat &org_img, 
                        const std::vector<std::vector<CvPoint>> &project_point, std::vector<std::vector<CvPoint>> &org_point);

    /**
     * @brief  展开原始图像
     * @param  pixel_per_meter 输入参数：拼接后的图像的分辨率(单位：像素/米)
     * @param  h_min 输入参数：拼接后的图像首行代表的高度（高度是以当前相机为原点，坐标轴朝下，单位：米）
     * @param  h_max 输入参数：拼接后的图像末行代表的高度（高度是以当前相机为原点，坐标轴朝下，单位：米）
     * @param  tx 输入参数：圆柱中心到相机的x轴偏移 (单位：米)
     * @param  ty 输入参数：圆柱中心到相机的y轴偏移 (单位：米)
     * @param  roll 输入参数：滚转角(R_圆柱_相机,RPY变换，弧度制)
     * @param  pitch 输入参数：俯仰角(R_圆柱_相机,RPY变换，弧度制)
     * @param  yaw 输入参数：方向角(R_圆柱_相机,RPY变换，弧度制)
     * @param  org_image 输入参数：原始图片
     * @param  out_image 输出参数：展开后的图像
     * @return 执行状态
     */
    bool ProjectImage(float pixel_per_meter, double h_min, double h_max, double tx, double ty, double roll, double pitch, double yaw, 
                        cv::Mat &org_image, cv::Mat &out_image);

    /**
     * @brief  拼接展开后的图像
     * @param  optimize_rate 输入参数：取值范围[0-1]重叠区域待优化的百分比,0代表不优化
     * @param  total_rows 输入参数：本次拼接完成后的图像行数
     * @param  last_stitch_img 输入参数：上一次的拼接图像
     * @param  new_img 输入参数：本次待拼接的图像 
     * @param  current_stitch_img 输出参数：拼接后的图像
     * @return 执行状态
     */
    bool StitchImage(float optimize_rate, int total_rows, cv::Mat &last_stitch_img,  cv::Mat &new_img, cv::Mat &current_stitch_img);

private:
    AlgCfg cfg;
    Eigen::Matrix3d K_inv;
};