#include<iostream>
#include<cstdlib>
#include<fstream>

#include "mc_pixel_video.h"

int color_map[1 << 24];

void read_color_map_file() {
	std::ifstream color_map_file("color_map", std::ios::in | std::ios::binary);
	int target_color_count;
	color_map_file.read((char*)&target_color_count, 4);
	for (int i = 0; i < target_color_count; ++i) {
		unsigned char r, g, b;
		color_map_file.read((char*)&b, 1);
		color_map_file.read((char*)&g, 1);
		color_map_file.read((char*)&r, 1);

		int cur_target_color = (b << 16) | (g << 8) | r;

		int cur_cluster_size;
		color_map_file.read((char*)&cur_cluster_size, 4);

		for (int j = 0; j < cur_cluster_size; ++j) {
			color_map_file.read((char*)&b, 1);
			color_map_file.read((char*)&g, 1);
			color_map_file.read((char*)&r, 1);
			int cur_orig_color = (b << 16) | (g << 8) | r;
			color_map[cur_orig_color] = cur_target_color;
		}
	}
	color_map_file.close();
}

void gen_mapped_video() {
	std::cout << "读取颜色映射...";
	read_color_map_file();
	std::cout << "完成";

	std::ifstream video_source_file("video", std::ios::in | std::ios::binary);
	int frame_width, frame_height, frame_count;
	video_source_file.read((char*)&frame_width, 4);
	video_source_file.read((char*)&frame_height, 4);
	video_source_file.read((char*)&frame_count, 4);

	std::ofstream video_file("mapped_video", std::ios::out | std::ios::binary);
	video_file.write((const char*)&frame_width, 4);
	video_file.write((const char*)&frame_height, 4);
	video_file.write((const char*)&frame_count, 4);

	int i, j;
	int cur_color = 0;
	unsigned char r, g, b;

	for (int k = 1; k <= frame_count; ++k) {
		std::system("cls");
		std::cout << "处理帧：" << k << "/" << frame_count << std::endl;
		for (i = 0; i < frame_width * frame_height; ++i) {
			video_source_file.read((char*)&b, 1);
			video_source_file.read((char*)&g, 1);
			video_source_file.read((char*)&r, 1);

			cur_color = color_map[(b << 16) | (g << 8) | r];

			r = cur_color & 255;
			cur_color >>= 8;
			g = cur_color & 255;
			cur_color >>= 8;
			b = cur_color & 255;

			video_file.write((const char*)&b, 1);
			video_file.write((const char*)&g, 1);
			video_file.write((const char*)&r, 1);
		}
	}

	video_source_file.close();
	video_file.close();
}