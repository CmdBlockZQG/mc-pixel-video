#include<cstdlib>
#include<iostream>
#include<fstream>

#include<opencv2/core.hpp>
#include<opencv2/videoio.hpp>

#include "mc_pixel_video.h"

void generate_preview_video(double frame_rate) {
	std::ifstream video_source_file("mapped_video", std::ios::in | std::ios::binary);
	int frame_width, frame_height, frame_count;
	video_source_file.read((char*)&frame_width, 4);
	video_source_file.read((char*)&frame_height, 4);
	video_source_file.read((char*)&frame_count, 4);

	cv::VideoWriter video;
	video.open(
		"video_preview.avi",
		cv::VideoWriter::fourcc('R', 'G', 'B', 'A'),
		frame_rate,
		cv::Size(frame_width, frame_height)
	);
	std::system("cls");

	int i, j;
	uchar* p;
	for (int k = 1; k <= frame_count; ++k) {
		std::system("cls");
		std::cout << "处理帧：" << k << "/" << frame_count << std::endl;

		cv::Mat frame = cv::Mat(frame_height, frame_width, CV_8UC3, cv::Scalar(0, 0, 0));
		p = frame.data;

		for (i = 0; i < frame_width * frame_height; ++i) {
			for (j = 0; j < 3; ++j) {
				video_source_file.read((char*)p++, 1);
			}
		}

		video.write(frame);
	}
	std::cout << "生成完成" << std::endl;
}