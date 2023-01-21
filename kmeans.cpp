#include<iostream>
#include<cstdlib>
#include<cmath>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<tuple>

#include <opencv2/opencv.hpp>

#include "mc_pixel_video.h"

int target_color_count;
std::vector<std::string> block_id;

int orig_color_count;
std::vector<std::pair<int, double>> orig_color;

std::vector<int> target_color;
int cluster[1 << 24] = { 0 };

void read_blocks_file() {
	std::ifstream blocks_file("blocks.txt", std::ios::in);

	blocks_file >> target_color_count;
	block_id.resize(target_color_count);

	for (int i = 0; i < target_color_count; ++i) {
		blocks_file >> block_id[i];
	}

	blocks_file.close();
}

void read_orig_color_file() {
	std::ifstream orig_color_file("orig_color", std::ios::in | std::ios::binary);
	orig_color_file.read((char*)&orig_color_count, 4);

	for (int i = 0; i < orig_color_count; ++i) {
		int cur_color = 0;
		unsigned char p;
		for (int j = 0; j < 3; ++j) { // BGR
			orig_color_file.read((char*)&p, 1);
			cur_color = (cur_color << 8) | p;
		}
		double weight;
		orig_color_file.read((char*)&weight, 8);
		orig_color.push_back(std::make_pair(cur_color, weight));
	}

	orig_color_file.close();
}

inline int color_dis(int x, int y) {
	int res = 0, t;
	t = (x & 255) - (y & 255);
	res += t * t;
	x >>= 8;
	y >>= 8;
	t = (x & 255) - (y & 255);
	res += t * t;
	x >>= 8;
	y >>= 8;
	t = (x & 255) - (y & 255);
	return res + t * t;
}

double kmean() {
	std::vector<std::tuple<double, double, double, double>> cluster_avg; // 总权值 R G B
	cluster_avg.resize(target_color_count);

	double loss_func = 0;
	for (auto cur_color_pair : orig_color) {
		int cur_color = cur_color_pair.first;
		double cur_weight = cur_color_pair.second;
		loss_func += cur_weight * color_dis(cur_color, target_color[cluster[cur_color]]);

		int cur_min_dis = 200000;
		int cur_cluster = -1;
		for (int i = 0; i < target_color_count; ++i) {
			int new_dis = color_dis(cur_color, target_color[i]);
			if (new_dis < cur_min_dis) {
				cur_cluster = i;
				cur_min_dis = new_dis;
			}
		}
		cluster[cur_color] = cur_cluster;
		std::get<0>(cluster_avg[cur_cluster]) += cur_weight;
		std::get<1>(cluster_avg[cur_cluster]) += (cur_color & 255) * cur_weight; // R
		cur_color >>= 8;
		std::get<2>(cluster_avg[cur_cluster]) += (cur_color & 255) * cur_weight; // G
		cur_color >>= 8;
		std::get<3>(cluster_avg[cur_cluster]) += (cur_color & 255) * cur_weight; // B
	}
	for (int i = 0; i < target_color_count; ++i) {
		double c = std::get<0>(cluster_avg[i]);
		if (!c) {
			continue;
		}
		int r = std::get<1>(cluster_avg[i]) / c,
			g = std::get<2>(cluster_avg[i]) / c,
			b = std::get<3>(cluster_avg[i]) / c;
		
		target_color[i] = (b << 16) | (g << 8) | r;
	}

	return loss_func;
}

void gen_color_map_file() {
	std::vector<std::vector<int>> cluster_color;
	cluster_color.resize(target_color_count);

	for (auto cur_color_pair : orig_color) {
		int& cur_color = cur_color_pair.first;
		cluster_color[cluster[cur_color]].push_back(cur_color);
	}

	std::ofstream color_map_file("color_map", std::ios::out | std::ios::binary);
	color_map_file.write((const char*)&target_color_count, 4);
	for (int i = 0; i < target_color_count; ++i) {
		unsigned char r, g, b;
		int cur_color = target_color[i];
		r = cur_color & 255;
		cur_color >>= 8;
		g = cur_color & 255;
		cur_color >>= 8;
		b = cur_color & 255;
		cur_color >>= 8;

		color_map_file.write((const char*)&b, 1);
		color_map_file.write((const char*)&g, 1);
		color_map_file.write((const char*)&r, 1);

		int cur_cluster_size = cluster_color[i].size();
		color_map_file.write((const char*)&cur_cluster_size, 4);

		for (int cur_color : cluster_color[i]) {
			r = cur_color & 255;
			cur_color >>= 8;
			g = cur_color & 255;
			cur_color >>= 8;
			b = cur_color & 255;
			cur_color >>= 8;

			color_map_file.write((const char*)&b, 1);
			color_map_file.write((const char*)&g, 1);
			color_map_file.write((const char*)&r, 1);
		}
	}

	color_map_file.close();
}

void gen_texture_map_file() {
	std::ofstream texture_map_file("texture_map.txt", std::ios::out);

	texture_map_file << target_color_count << std::endl;

	for (int i = 0; i < target_color_count; ++i) {
		texture_map_file << block_id[i] << " " << target_color[i] << std::endl;
	}

	texture_map_file.close();
}

void calc_color_map() {
	std::system("cls");

	std::cout << "读取方块和颜色数据...";
	read_blocks_file();
	read_orig_color_file();
	std::cout << "完成" << std::endl;

	std::random_shuffle(orig_color.begin(), orig_color.end());
	for (int i = 0; i < target_color_count; ++i) {
		target_color.push_back(orig_color[i].first);
	}

	double last_loss = 0, cur_loss;
	for (int i = 1; i <= 100; ++i) {
		cur_loss = kmean();
		std::cout << "第" << i << "次迭代，loss=" << std::fixed << std::setprecision(8) << cur_loss << std::endl;
		if (abs(cur_loss - last_loss) < 0.01) {
			std::cout << "损失函数收敛，聚类完成" << std::endl;
			break;
		}
		last_loss = cur_loss;
	}

	std::cout << "保存颜色映射数据...";
	gen_color_map_file();
	std::cout << "完成" << std::endl;

	std::cout << "保存方块映射数据...";
	gen_texture_map_file();
	std::cout << "完成" << std::endl;
}
