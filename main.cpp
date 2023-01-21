#include<cstdlib>
#include<iostream>
#include<string>

#include "mc_pixel_video.h"

int main() {
	// video.mp4 -> orig_video(ת�����Ƶ) orig_color(ԭ��Ƶ���е���ɫ)
	std::cout << "1: ��Ƶת�� & ��ɫ����" << std::endl;
	// orig_color blocks(���õķ���id) -> color_map(ԭ��ɫ��Ŀ����ɫ��Ӧ��ϵ) texture_map(Ŀ����ɫ�ͷ���Ķ�Ӧ, �ı�)
	std::cout << "2: ������ɫӳ��" << std::endl;
	// video color_map -> mapped_video
	std::cout << "3: ����ӳ������Ƶ����" << std::endl;
	// mapped_video -> video_preview.mp4
	std::cout << "4: ����Ԥ����Ƶ" << std::endl;
	// texture_map -> texture/
	std::cout << "5: ���ɲ��ʰ�" << std::endl;
	// mapped_video texture_map -> datapack/ 
	std::cout << "6: �������ݰ�(ȫԭʼ)" << std::endl;
	std::cout << "7: �������ݰ�(֡������)" << std::endl;
	std::cout << "8: �������ݰ�(֡������+��ѹ��)" << std::endl;
	int op;
	std::cin >> op;
	if (op == 1) {
		std::string filename;
		std::cout << "��Ƶ�ļ�����";
		std::cin >> filename;

		int w, h;
		std::cout << "ѹ������Ƶ��ߣ��ո������";
		std::cin >> w >> h;

		transcode_video(filename.c_str(), w, h);
	} else if (op == 2) {
		calc_color_map();
	} else if (op == 3) {
		gen_mapped_video();
	} else if (op == 4) {
		double frame_rate;
		std::cout << "֡�ʣ�";
		std::cin >> frame_rate;
		generate_preview_video(frame_rate);
	} else if (op == 5) {
		gen_texture();
	} else if (op == 6) {
		gen_mcfunction();
	} else if (op == 7) {
		int delay_tick;
		std::cout << "��֮֡����Ϸ������";
		std::cin >> delay_tick;
		gen_better_mcfunction(delay_tick);
	} else if (op == 8) {
		int delay_tick;
		std::cout << "��֮֡����Ϸ������";
		std::cin >> delay_tick;
		gen_super_mcfunction(delay_tick);
	}
	return 0;
}