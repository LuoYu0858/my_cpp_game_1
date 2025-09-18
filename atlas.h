#ifndef PLANTSVSPLANTS_ATLAS_H
#define PLANTSVSPLANTS_ATLAS_H

#include <vector>
#include <graphics.h>

// 图集类
class Atlas {
public:
    Atlas() = default;
    ~Atlas() = default;

    void load_from_file(LPCTSTR path_template, int num) {
        img_list.clear();
        img_list.resize(num);

        TCHAR path_file[255];
        for (int i = 0; i < num; ++i) {
            _stprintf_s(path_file, path_template, i + 1);
            loadimage(&img_list[i], path_file);
        }
    }

    void clear() {
        img_list.clear();
    }

    int get_size() {
        return (int)img_list.size();
    }

    IMAGE* get_image(int idx) {
        return idx < 0 or idx >= img_list.size() ? nullptr : &img_list[idx];
    }

    void add_image(const IMAGE& img) {
        img_list.push_back(img);
    }

private:
    std::vector<IMAGE> img_list;    // 图片列表, 存放一组图片资源的所有图片
};

#endif //PLANTSVSPLANTS_ATLAS_H
