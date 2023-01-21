#include<cstdlib>
#include<iostream>
#include<fstream>
#include<set>
#include<map>

#include<opencv2/core.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/imgproc.hpp>

#include "mc_pixel_video.h"

void transcode_video(const char* video_filename, int target_frame_width, int target_frame_height) {
	cv::VideoCapture capture;
	capture.open(video_filename);
	if (!capture.isOpened()) {
		std::cout << "无法读取视频文件";
		return;
	}

	// 视频基础信息
	const int source_frame_width = capture.get(cv::CAP_PROP_FRAME_WIDTH);
	const int source_frame_height = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	const int frame_count = capture.get(cv::CAP_PROP_FRAME_COUNT);

	std::ofstream video_file("video", std::ios::out | std::ios::binary);
	video_file.write((const char*)&target_frame_width, 4);
	video_file.write((const char*)&target_frame_height, 4);
	video_file.write((const char*)&frame_count, 4);

	std::map<int, long long> orig_color_map; // 原视频颜色集合,值是颜色像素总数

	std::system("cls");
	int cur_frame = 0;

	cv::Mat source_frame;
	uchar* p;
	uchar val;
	int i, j;
	while (capture.read(source_frame)) {
		std::system("cls");
		std::cout << "处理帧：" << ++cur_frame << "/" << frame_count << std::endl;

		cv::Mat target_frame = cv::Mat(target_frame_height, target_frame_width, CV_8UC3);
		cv::resize(source_frame, target_frame, target_frame.size(), 0, 0, cv::INTER_AREA);

		p = target_frame.data;
		for (i = 0; i < target_frame_width * target_frame_height; ++i) {
			int cur_color = 0;
			for (j = 0; j < 3; ++j) {
				val = *p++;
				video_file.write((const char*)&val, 1);

				cur_color = (cur_color << 8) | val;
			}
			++orig_color_map[cur_color];
		}
	}

	video_file.close();
	std::cout << "视频转码完成 正在统计颜色值信息..." << std::endl;

	std::ofstream orig_color_file("orig_color", std::ios::out | std::ios::binary);
	const int orig_color_count = orig_color_map.size();
	orig_color_file.write((const char*)&orig_color_count, 4);

	long long total_pixel_count = (long long)target_frame_width * (long long)target_frame_height * (long long)frame_count;

	for (auto i = orig_color_map.begin(); i != orig_color_map.end(); ++i) {
		int cur_color = i->first;
		uchar r = cur_color & 255;
		cur_color >>= 8;
		uchar g = cur_color & 255;
		cur_color >>= 8;
		uchar b = cur_color & 255;

		orig_color_file.write((const char*)&b, 1);
		orig_color_file.write((const char*)&g, 1);
		orig_color_file.write((const char*)&r, 1);

		double weight = (i->second + 0.0) / total_pixel_count; // 所有颜色总权值为1
		orig_color_file.write((const char*)&weight, 8);
	}
	orig_color_file.close();
	std::cout << "颜色值统计完成" << std::endl;
}