#include <pro2cylinder.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <vector>
#include <sys/io.h>
#include "utils.hpp"

using namespace std;

int main(int argc, char **argv){
    //configure
    AlgCfg cfg;
    cfg.K << 369.502083,0,640,
            0,369.502083,360,
            0,0,1;
    cfg.R = 10;
    cfg.org_img_height = 720;
    cfg.org_img_width = 1280;
    Pro2Cylinder alg(cfg);

    //CalcDefaultParameter, you need to get input param with ui
    float pixel_per_meter, h_min, h_max;
    bool status = alg.CalcDefaultParameter(cfg.org_img_height/2*0.85-1, cfg.org_img_width/7, pixel_per_meter, h_min, h_max);
    if(!status){
        return -1;
    }

    //adjust parameter, you need to adjust params with ui, and wait for human decision
    //adjust parameter step1: prepare image
    string working_path = "/home/amov/Downloads/cv_test-master/cv_alg/project2cylinder/img/normal/";
    string org_img_path = working_path + "org_img/";
    vector<string> org_img_filename_vec;
	find_dir_file(org_img_path.c_str(), org_img_filename_vec);
    if(org_img_filename_vec.empty()){
        std::cout<<"no image!" << std::endl;
        return -1;
    }
    size_t _size = org_img_filename_vec[0].rfind(".");
    string filename_without_type = org_img_filename_vec[0].substr(0, _size);
    vector<string> str_vec;
    StringSplit(filename_without_type, '_', str_vec);
    if(str_vec.size() < 7){
        std::cout<<"filename format error: " << org_img_filename_vec[0] <<std::endl;
        return -1;
    }
    
    //adjust parameter step2: prepare point
    std::vector<std::vector<CvPoint>> project_point,org_point;
    project_point.resize(3);
    int project_image_width = 2 * M_PI * cfg.R * pixel_per_meter + 0.5;
    int project_image_height = (h_max - h_min) * pixel_per_meter + 0.5;

    project_point[0].resize(project_image_width);//top line
    project_point[1].resize(project_image_width);//bottom line
    project_point[2].resize(project_image_height);//left line
    for(int v = 0; v < project_image_width; ++v){
        project_point[0][v] = CvPoint(v,0);
        project_point[1][v] = CvPoint(v,project_image_height-1);
    }
    for(int u = 0; u < project_image_height; ++u){
        project_point[2][u] = CvPoint(0,u);
    }

    //adjust parameter step3: AdjustParam
    cv::Mat org_image = cv::imread(org_img_path + org_img_filename_vec[0]);
    // 000001_3.1182_3.3544_66.3635_-0.0016_-0.0014_2.2270.png  
    //  index  tx     ty      z       roll     pitch  yaw
    status = alg.AdjustParam(pixel_per_meter, h_min, h_max, atof(str_vec[1].c_str()), atof(str_vec[2].c_str()), 
                    atof(str_vec[4].c_str()), atof(str_vec[5].c_str()), atof(str_vec[6].c_str()), org_image, project_point, org_point);
    
    //adjust parameter step4: show AdjustParam effect
    int width = max(project_image_width, cfg.org_img_width);
    int height = max(project_image_height, cfg.org_img_height);
    cv::Mat image(height*2, width, CV_8UC1);
    cvtColor(image, image, cv::COLOR_GRAY2RGB);

    org_image.copyTo(image.rowRange(0, cfg.org_img_height).colRange(0,cfg.org_img_width));

    for(unsigned i = 0; i < org_point.size(); ++i){
        cv::Scalar scalar(255, 0, 0);
        if(i == 1){
            scalar = cv::Scalar(0, 255, 0);
        }else if(i==2){
            scalar = cv::Scalar(0, 0, 255);
        }
        for(unsigned j=0; j < org_point[i].size(); ++j){
            if(org_point[i][j].x < 0|| org_point[i][j].y < 0){
                continue;
            }
            CvPoint p_dst(project_point[i][j].x,project_point[i][j].y+height);
            cv::circle(image, org_point[i][j], 2, scalar);
            cv::circle(image, p_dst, 2, scalar);
        }
    }
    cv::imwrite(working_path+"AdjustParamEffect.png", image);

    //project image
    mkdir_path(working_path+"project_img");
    mkdir_path(working_path+"stitch_img");
    cv::Mat last_stitch_img;
    bool has_project_img = false;
    float h_begin,h_end;
    for(unsigned i=0; i<org_img_filename_vec.size(); ++i){
        std::cout<<org_img_filename_vec[i]<<std::endl;
        size_t _size = org_img_filename_vec[i].rfind(".");
	    string filename_without_type = org_img_filename_vec[i].substr(0, _size);
        vector<string> str_vec;
        StringSplit(filename_without_type, '_', str_vec);
        if(str_vec.size() < 7){
            std::cout<<"filename format error: " << org_img_filename_vec[i] <<std::endl;
            return -1;
        }
        float height = atof(str_vec[3].c_str());
        string project_img_path = working_path + "project_img/" + filename_without_type + "_" 
                                    + float2string(pixel_per_meter) + "_" + float2string(h_min) + "_" + float2string(h_max) + string(".png");
        cv::Mat input_image = cv::imread(org_img_path+org_img_filename_vec[i]);
        cv::Mat dst;
        bool result = alg.ProjectImage(pixel_per_meter, h_min, h_max, atof(str_vec[1].c_str()), atof(str_vec[2].c_str()), 
                        atof(str_vec[4].c_str()), atof(str_vec[5].c_str()), atof(str_vec[6].c_str()), input_image, dst);
        if(result){
            //save_project_img
            cv::imwrite(project_img_path, dst);
            //stitch img
            if(!has_project_img){
                last_stitch_img.create(dst.size(), CV_32FC3);
                dst.copyTo(last_stitch_img.rowRange(0, last_stitch_img.rows));
                has_project_img = true;
                h_begin = height + h_min;
                h_end = height + h_max;
            }else{
                if((height + h_min) > h_begin && (height + h_max) > h_end)
                {
                    if((height + h_min) > h_end){
                        std::cout << "warning!!! not enough image view, height: " << (height + h_min) << " " << h_end << std::endl;
                    }
                    int rows = (height + h_max - h_begin) * pixel_per_meter + 0.5;
                    cv::Mat current_stitch_img;
                    status = alg.StitchImage(0.1, rows, last_stitch_img, dst, current_stitch_img);
                    if(status){
                        last_stitch_img = current_stitch_img;
                        h_end = height + h_max;
                    }
                }
            }
        }
    }
    //save_stitch_img
    if(!last_stitch_img.empty()){
        string stich_filename = working_path + string("stitch_img/") 
                                + float2string(pixel_per_meter) + "_" + float2string(h_begin) + "_" + float2string(h_end) + string(".png");
        cv::imwrite(stich_filename, last_stitch_img);
    }
    return 0;
}