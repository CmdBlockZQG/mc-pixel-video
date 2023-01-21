#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>

#include<opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "mc_pixel_video.h"

void gen_texture() {
	std::ifstream texture_map_file("texture_map.txt", std::ios::in);

	int block_count;
	texture_map_file >> block_count;

	std::string block_id;
	int cur_color; // BGR
	for (int i = 0; i < block_count; ++i) {
		texture_map_file >> block_id >> cur_color;
		cv::Mat img(16, 16, CV_8UC3);

		uchar* p = img.data;
		uchar cur_color_bgr[3];

		cur_color_bgr[2] = cur_color & 255; // r
		cur_color >>= 8;
		cur_color_bgr[1] = cur_color & 255; // g
		cur_color >>= 8;
		cur_color_bgr[0] = cur_color & 255; // b

		for (int i = 0; i < 16 * 16; ++i) {
			for (int j = 0; j < 3; ++j) {
				*p++ = cur_color_bgr[j];
			}
		}

		cv::imwrite("mc-pixel-video-texture/assets/minecraft/textures/block/" + block_id + ".png", img);
	}

	texture_map_file.close();
}