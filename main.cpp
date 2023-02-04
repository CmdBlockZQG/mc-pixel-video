#include<cstdlib>
#include<iostream>
#include<string>

#include "mc_pixel_video.h"

int main() {
	// video.mp4 -> orig_video(转码的视频) orig_color(原视频所有的颜色)
	std::cout << "1: 视频转码 & 颜色分析" << std::endl;
	// orig_color blocks(可用的方块id) -> color_map(原颜色和目标颜色对应关系) texture_map(目标颜色和方块的对应, 文本)
	std::cout << "2: 计算颜色映射" << std::endl;
	// video color_map -> mapped_video
	std::cout << "3: 生成映射后的视频数据" << std::endl;
	// mapped_video -> video_preview.mp4
	std::cout << "4: 生成预览视频" << std::endl;
	// texture_map -> texture/
	std::cout << "5: 生成材质包" << std::endl;
	// mapped_video texture_map -> datapack/ 
	std::cout << "6: 生成数据包(全原始)" << std::endl;
	std::cout << "7: 生成数据包(帧间增量)" << std::endl;
	std::cout << "8: 生成数据包(帧间增量+行压缩)" << std::endl;
	std::cout << "9: 生成rcon播放器专用数据包(帧间增量+行压缩)" << std::endl;
	int op;
	std::cin >> op;
	if (op == 1) {
		std::string filename;
		std::cout << "视频文件名：";
		std::cin >> filename;

		int w, h;
		std::cout << "压缩后视频宽高，空格隔开：";
		std::cin >> w >> h;

		transcode_video(filename.c_str(), w, h);
	} else if (op == 2) {
		calc_color_map();
	} else if (op == 3) {
		gen_mapped_video();
	} else if (op == 4) {
		double frame_rate;
		std::cout << "帧率：";
		std::cin >> frame_rate;
		generate_preview_video(frame_rate);
	} else if (op == 5) {
		gen_texture();
	} else if (op == 6) {
		gen_mcfunction();
	} else if (op == 7) {
		int delay_tick;
		std::cout << "两帧之间游戏刻数：";
		std::cin >> delay_tick;
		gen_better_mcfunction(delay_tick);
	} else if (op == 8) {
		int delay_tick;
		std::cout << "两帧之间游戏刻数：";
		std::cin >> delay_tick;
		gen_super_mcfunction(delay_tick);
	} else if (op == 9) {
		gen_player_mcfunction();
	}
	return 0;
}