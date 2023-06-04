#include <pro2cylinder.h>
#include <iostream>

using namespace std;
using namespace cv;

bool Pro2Cylinder::CalcDefaultParameter(int roi_R_pixel, int roi_r_pixel, float &pixel_per_meter, float &h_min, float &h_max){
    if(roi_R_pixel <= roi_r_pixel){
        std::cout << "CalcDefaultParameter failed, wrong params " << std::endl;
        return false;
    }
    int u = cfg.org_img_width/2;
    int v_min = cfg.org_img_height/2 - roi_R_pixel;
    int v_max = cfg.org_img_height/2 - roi_r_pixel;
    Eigen::Vector3d b_min = K_inv*Eigen::Vector3d(u, v_min, 1);
    Eigen::Vector3d b_max = K_inv*Eigen::Vector3d(u, v_max, 1);

    h_min = b_min(2)*cfg.R/sqrt(b_min(0)*b_min(0) + b_min(1)*b_min(1));
    h_max = b_max(2)*cfg.R/sqrt(b_max(0)*b_max(0) + b_max(1)*b_max(1));
    pixel_per_meter = (float)roi_R_pixel / cfg.R; 
    return true;
}

bool Pro2Cylinder::AdjustParam(float pixel_per_meter,double h_min, double h_max, double tx, double ty, double roll, double pitch, double yaw, const cv::Mat &org_img, 
                const std::vector<std::vector<CvPoint>> &project_point, std::vector<std::vector<CvPoint>> &org_point){
    if(org_img.empty() || project_point.empty() || h_min < 0 || h_max <= h_min){
        std::cout << "AdjustParam failed, wrong params " << std::endl;
        return false;
    }
    double a1 = -tx;
    double a2 = -ty;
    int project_image_width = 2 * M_PI * cfg.R * pixel_per_meter + 0.5;
    int project_image_height = (h_max - h_min) * pixel_per_meter + 0.5;

    //R_2 == R_w
    Eigen::Matrix3d R_21, R_12, K_R12;
    R_21 = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ())*
            Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY())*
            Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());
    R_12 = R_21.transpose();
    K_R12 = cfg.K * R_12;
    double k1 = K_R12(2,0);    
    double k2 = K_R12(2,1);    
    double k3 = K_R12(2,2);

    double theta,h,z;
    double delta_h = (h_max - h_min);
    Eigen::Vector3d point;
    org_point.resize(project_point.size());
    for(unsigned i = 0; i < project_point.size(); ++i){
        org_point[i].resize(project_point[i].size());
        for(unsigned j = 0; j < project_point[i].size(); ++j){
            theta = 2 * M_PI * (double)project_point[i][j].x / project_image_width;
            h = h_min + delta_h * (double)project_point[i][j].y / project_image_height;
            z = k1 * ( a1 + cfg.R * cos(theta)) + k2 * (a2 + cfg.R * sin(theta)) + k3 * h;
            point = K_R12 / z * Eigen::Vector3d(a1 + cfg.R*cos(theta), a2 + cfg.R*sin(theta), h);//column row
            if(point(0) < 0 || point(1) < 0 || point(0) > (cfg.org_img_width-1) || point(1) > (cfg.org_img_height-1)){
                org_point[i][j].x = -1;
                org_point[i][j].y = -1;
            }else{
                org_point[i][j].x = point(0);
                org_point[i][j].y = point(1); 
            }
        }
    } 
    return true; 
}

bool Pro2Cylinder::ProjectImage(float pixel_per_meter,double h_min, double h_max, double tx, double ty, double roll, double pitch, double yaw, cv::Mat &org_image, cv::Mat &out_image){
    if(org_image.empty()){
        std::cout << "org_image empty" << std::endl;
        return false;
    }
    if(org_image.rows != cfg.org_img_height
        ||org_image.cols != cfg.org_img_width){
        std::cout << "wrong image height or width:" << std::endl;
        return false;
    }
    if(h_min < 0 || h_max <= h_min){
        std::cout << "wrong image h_max or h_min: " << h_max << "  " << h_min << std::endl;
        return false;
    }
    double a1 = -tx;
    double a2 = -ty;
    int project_image_width = 2 * M_PI * cfg.R * pixel_per_meter + 0.5;
    int project_image_height = (h_max - h_min) * pixel_per_meter + 0.5;

    //R_2 == R_w
    Eigen::Matrix3d R_21, R_12, K_R12;
    R_21 = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ())*
            Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY())*
            Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());
    R_12 = R_21.transpose();
    K_R12 = cfg.K * R_12;
    double k1 = K_R12(2,0);    
    double k2 = K_R12(2,1);    
    double k3 = K_R12(2,2);    
    cv::Mat dst;
    dst.create(project_image_height, project_image_width, CV_32FC3);
    // Mat dst(project_image_height, project_image_width, CV_8UC3, Scalar(255, 255, 255));//row,column
	Mat map_x, map_y;
	map_x.create(dst.size(), CV_32FC1);
	map_y.create(dst.size(), CV_32FC1);
    double theta,h,z;
    double delta_h = (h_max - h_min);
    Eigen::Vector3d point;

    for (int u = 0; u < project_image_height; u++)//row
	{
		for (int v = 0; v < project_image_width; v++)//column
		{
            theta = 2 * M_PI * (double)v / project_image_width;
            h = h_min + delta_h * (double)u / project_image_height;
            z = k1 * ( a1 + cfg.R * cos(theta)) + k2 * (a2 + cfg.R * sin(theta)) + k3 * h;
            point = K_R12 / z * Eigen::Vector3d(a1 + cfg.R*cos(theta), a2 + cfg.R*sin(theta), h);//column row
            if(point(0) < 0 || point(1) < 0 || point(0) > (cfg.org_img_width-1) || point(1) > (cfg.org_img_height-1)){
                std::cout << "warning: wrong remap" << std::endl;
                continue;
            }
            map_x.at<float>(u, v) = point(0);//column,x
			map_y.at<float>(u, v) = point(1);//row,y
        }
    }

    remap(org_image, dst, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
    out_image.create(dst.rows, dst.cols, CV_32FC3);
    dst.copyTo(out_image.rowRange(0, dst.rows));

    //for debug
    // {
    //     int width = max(project_image_width, cfg.org_img_width);
    //     int height = max(project_image_height, cfg.org_img_height);
    //     cv::Mat image(height*2, width, CV_8UC1);
    //     cvtColor(image, image, cv::COLOR_GRAY2RGB);

    //     org_image.copyTo(image.rowRange(0, cfg.org_img_height).colRange(0,cfg.org_img_width));
    //     dst.copyTo(image.rowRange(height,height + project_image_height).colRange(0,project_image_width));

    //     for (int u = 0; u < project_image_height; u++)//row
    //     {
    //         for (int v = 0; v < project_image_width; v++)//column
    //         {
    //             if(u%100==0 && v%100 ==0){
    //                 CvPoint p_org(map_x.at<float>(u, v), map_y.at<float>(u, v)), p_dst(v,u+height);//column-x, row-y
    //                 cv::line(image, p_org, p_dst, cv::Scalar(0,0,255));
    //                 cv::circle(image, p_org, 5, cv::Scalar(0, 255, 0));
    //                 cv::circle(image, p_dst, 5, cv::Scalar(0, 255, 0));
    //             }
    //         }
    //     }
    //     cv::imwrite("./test.png", image);
    // }

    return true;
}

bool Pro2Cylinder::StitchImage(float optimize_rate, int total_rows, cv::Mat &last_stitch_img,  cv::Mat &new_img, cv::Mat &current_stitch_img){
    if(total_rows <= last_stitch_img.rows || last_stitch_img.cols != new_img.cols 
        || last_stitch_img.empty() || new_img.empty() || optimize_rate > 1){
        std::cout << "StitchImage failed, input param error" << std::endl;
        return false;
    }
    current_stitch_img.create(total_rows, last_stitch_img.cols, CV_32FC3);
    int start = total_rows - new_img.rows;
    last_stitch_img.copyTo(current_stitch_img.rowRange(0, last_stitch_img.rows));
    new_img.copyTo(current_stitch_img.rowRange(start, total_rows));

    if(optimize_rate <= 0 || last_stitch_img.rows < start){
        return true;
    }

    int processRows = max(1,(int)((last_stitch_img.rows - start + 1)*optimize_rate));
    float alpha = 0;

    for(int u = 0 ; u < processRows - 1; ++u){//row
        alpha = float(u) / processRows;
        for(int v = 0; v < last_stitch_img.cols; ++v){
            current_stitch_img.at<Vec3f>(u+start,v)[0] = last_stitch_img.at<Vec3f>(u+start,v)[0]*(1-alpha) + new_img.at<Vec3f>(u,v)[0]*alpha;
            current_stitch_img.at<Vec3f>(u+start,v)[1] = last_stitch_img.at<Vec3f>(u+start,v)[1]*(1-alpha) + new_img.at<Vec3f>(u,v)[1]*alpha;
            current_stitch_img.at<Vec3f>(u+start,v)[2] = last_stitch_img.at<Vec3f>(u+start,v)[2]*(1-alpha) + new_img.at<Vec3f>(u,v)[2]*alpha;
        }
    }
    return true;
}