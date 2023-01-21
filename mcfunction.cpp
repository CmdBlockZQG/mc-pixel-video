#include<cstdlib>
#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>

#include "mc_pixel_video.h"

std::map<int, std::string> color_block;

void read_texture_map() {
	std::ifstream texture_map_file("texture_map.txt", std::ios::in);

	int block_count;
	texture_map_file >> block_count;

	std::string block_id;
	int cur_color; // BGR
	for (int i = 0; i < block_count; ++i) {
		texture_map_file >> block_id >> cur_color;
		color_block[cur_color] = block_id;
	}

	texture_map_file.close();
}

int frame_width, frame_height, frame_count;

void generate_frame_function() {
	std::ifstream video_file("mapped_video", std::ios::in | std::ios::binary);
	video_file.read((char*)&frame_width, 4);
	video_file.read((char*)&frame_height, 4);
	video_file.read((char*)&frame_count, 4);

	std::system("cls");

	const int chunk_size = 100;

	for (int i = 0; i <= frame_count / chunk_size; ++i) {
		std::system(("mkdir mc-pixel-video-func\\data\\pixel-video\\functions_" + std::to_string(i)).c_str());
		std::ofstream chunk_func_file("mc-pixel-video-func/data/pixel-video/functions_" + std::to_string(i) + "/chunk" + std::to_string(i) + ".mcfunction", std::ios::out);
		for (int j = i * chunk_size; j < i * chunk_size + chunk_size && j < frame_count; ++j) {
			chunk_func_file << "function pixel-video:frame" + std::to_string(j) << std::endl;
		}
		chunk_func_file.close();
	}

	for (int k = 0; k < frame_count; ++k) {
		std::system("cls");
		std::cout << "生成帧：" << k + 1 << "/" << frame_count << std::endl;

		std::ofstream frame_func_file("mc-pixel-video-func/data/pixel-video/functions_" + std::to_string(k / chunk_size) + "/frame" + std::to_string(k) + ".mcfunction", std::ios::out);

		for (int j = frame_height - 1; j >= 0; --j) {
			for (int i = frame_width - 1; i >= 0; --i) {
				int cur_color = 0;
				unsigned char t;
				for (int k = 0; k < 3; ++k) {
					video_file.read((char*)&t, 1);
					cur_color = (cur_color << 8) | t;
				}

				frame_func_file << "setblock " << i << " " << j << " " << k << " " + color_block[cur_color] << std::endl;
			}
		}

		frame_func_file.close();
	}

	std::cout << "生成完成" << std::endl;
}

void generate_better_frame_function(int delay_tick) {
	std::ifstream video_file("mapped_video", std::ios::in | std::ios::binary);
	video_file.read((char*)&frame_width, 4);
	video_file.read((char*)&frame_height, 4);
	video_file.read((char*)&frame_count, 4);

	std::system("cls");

	const int chunk_size = 100;

	std::vector<int> last_frame(frame_height * frame_width, -1);

	std::system("mkdir mc-pixel-video-func\\data\\pixel-video\\functions_better");

	for (int k = 0; k < frame_count; ++k) {
		std::system("cls");
		std::cout << "生成帧：" << k + 1 << "/" << frame_count << std::endl;

		std::ofstream frame_func_file("mc-pixel-video-func/data/pixel-video/functions_better/frame" + std::to_string(k) + ".mcfunction", std::ios::out);

		for (int j = frame_height - 1; j >= 0; --j) {
			for (int i = frame_width - 1; i >= 0; --i) {
				int cur_color = 0;
				unsigned char t;
				for (int k = 0; k < 3; ++k) {
					video_file.read((char*)&t, 1);
					cur_color = (cur_color << 8) | t;
				}

				int cur_pixel_num = j * frame_width + i;
				if (cur_color == last_frame[cur_pixel_num]) {
					continue;
				}
				last_frame[cur_pixel_num] = cur_color;
				frame_func_file << "setblock " << i << " " << j << " " << 0 << " " + color_block[cur_color] << std::endl;
			}
		}

		frame_func_file << "schedule function pixel-video:frame" << std::to_string(k + 1) << " " << std::to_string(delay_tick) << "t";
		frame_func_file.close();
	}

	std::cout << "生成完成" << std::endl;
}

void generate_super_frame_function(int delay_tick) {
	std::ifstream video_file("mapped_video", std::ios::in | std::ios::binary);
	video_file.read((char*)&frame_width, 4);
	video_file.read((char*)&frame_height, 4);
	video_file.read((char*)&frame_count, 4);

	std::system("cls");

	const int chunk_size = 100;

	std::vector<int> last_frame(frame_height * frame_width, -1);

	std::system("mkdir mc-pixel-video-func\\data\\pixel-video\\functions_super");

	for (int k = 0; k < frame_count; ++k) {
		std::system("cls");
		std::cout << "生成帧：" << k + 1 << "/" << frame_count << std::endl;

		std::ofstream frame_func_file("mc-pixel-video-func/data/pixel-video/functions_super/frame" + std::to_string(k) + ".mcfunction", std::ios::out);

		for (int j = frame_height - 1; j >= 0; --j) {
			int last_i = -1, last_color = -1;
			for (int i = frame_width - 1; i >= 0; --i) {
				int cur_color = 0;
				unsigned char t;
				for (int k = 0; k < 3; ++k) {
					video_file.read((char*)&t, 1);
					cur_color = (cur_color << 8) | t;
				}

				int cur_pixel_num = j * frame_width + i;
				if (cur_color == last_frame[cur_pixel_num]) { // 与上一帧的相同会截断线段
					if (last_i != -1) {
						if (last_i == i + 1) {
							frame_func_file << "setblock " << last_i << " " << j << " 0 " + color_block[last_color] << std::endl;
						} else {
							frame_func_file << "fill " << last_i << " " << j << " 0 " << i + 1 << " " << j << " 0 " << color_block[last_color] << std::endl;
						}
						last_i = -1; // 没有线段头
					}
					continue;
				}
				last_frame[cur_pixel_num] = cur_color;
				// 这里的方块会被更新，计算线段
				if (last_i != -1) {
					if (last_color != cur_color) { // 颜色变化，线段断开
						if (last_i == i + 1) {
							frame_func_file << "setblock " << last_i << " " << j << " 0 " + color_block[last_color] << std::endl;
						} else {
							frame_func_file << "fill " << last_i << " " << j << " 0 " << i + 1 << " " << j << " 0 " << color_block[last_color] << std::endl;
						}
						last_i = i; // 新线段从现在开始
						last_color = cur_color;
					} // 不变 就不管了
				} else { // 没有线段头，当前就是线段头
					last_i = i;
					last_color = cur_color;
				}
			}
			if (last_i != -1) { // 有线段一直到结尾了
				if (last_i == 0) {
					frame_func_file << "setblock 0 " << j << " 0 " + color_block[last_color] << std::endl;
				} else {
					frame_func_file << "fill " << last_i << " " << j << " 0 0 " << j << " 0 " << color_block[last_color] << std::endl;
				}
			}
		}

		frame_func_file << "schedule function pixel-video:frame" << std::to_string(k + 1) << " " << std::to_string(delay_tick) << "t";
		frame_func_file.close();
	}

	std::cout << "生成完成" << std::endl;
}

void gen_mcfunction() {
	read_texture_map();
	generate_frame_function();
}

void gen_better_mcfunction(int delay_tick) {
	read_texture_map();
	generate_better_frame_function(delay_tick);
}

void gen_super_mcfunction(int delay_tick) {
	read_texture_map();
	generate_super_frame_function(delay_tick);
}