#include <iostream>
#include "CImg.h"
#include <vector>

using namespace cimg_library;

extern const int NEIGHBOORS[26][3] = {
    {0, 0, -1}, {0, 0, 1}, {0, -1, 0}, {0, 1, 0}, {0, -1, -1}, {0, -1, 1}, {0, 1, -1}, {0, 1, 1}, {1, -1, 1},
    {-1, 0, 0}, {1, 0, 0}, {-1, -1, 0}, {1, -1, 0}, {-1, 1, 0}, {1, 1, 0}, {-1, 0, -1}, {-1, 0, 1}, {1, 1, -1},
    {1, 0, -1}, {1, 0, 1}, {-1, -1, -1}, {-1, -1, 1}, {-1, 1, -1}, {-1, 1, 1}, {1, -1, -1}, {1, 1, 1}
};

CImg<> segmentationVascularNetwork(CImg<> img, int seedVoxelX, int seedVoxelY, int seedVoxelZ);

int main(int argc, char ** argv) {

    CImg<unsigned char> image_read;

    if (argc < 2) {
        std::cout << "Usage : image.hdr" << std::endl;
        return EXIT_FAILURE;
    }

    //Initialisation
    //Lecture de l'image
    float voxelsize[3];
    image_read.load_analyze(argv[1], voxelsize);
	CImgDisplay image_disp(image_read, "display");
    CImgDisplay tmp(image_read, "draw_fill");

    bool redraw = false;
    int slice = 0;
    int depth = image_read.depth();
    int x = 0;
    int y = 0;
    int z = 0;
    CImg<> current_img = image_read;

    while (!image_disp.is_closed() && !image_disp.is_keyESC()) {

	    if(image_disp.wheel()) {
            const int scroll = image_disp.wheel();
            slice += scroll;
            if (slice < 0) {
                slice = 0;
            } else {
                if (slice >= (int)depth) {
                    slice = (int)depth - 1;
                }
            }
            image_disp.set_wheel();
            redraw = true;
        }

        if (redraw) {
            current_img = image_read.get_slice(slice);
            //mpr_img.resize(512, 512);
            image_disp.display(current_img);
            redraw = false;
		}

        if ( image_disp.button()&1 ) {
		    x = image_disp.mouse_x();
		    y = image_disp.mouse_y();
		    z = slice;
		    CImg<> t_img = segmentationVascularNetwork(current_img, x, y, z);
            tmp.display(t_img);
        }
        image_disp.wait();
    }

    return 0;
}

CImg<> segmentationVascularNetwork(CImg<> img, int m_x, int m_y, int m_z) {

    int width = img.width();
	int height = img.height();
	int depth = img.depth();

	CImg<> voxels_img(width, height, depth);
	int theta_beg = img(m_x, m_y, m_z);
	int current_theta = theta_beg;
	int theta_end = theta_beg - 30;
	std::vector< std::vector< std::vector<int> > > list_of_voxels_position(30);

	std::cout << "X = " << m_x << ", Y = " << m_y << ", Z = " << m_z << std::endl;

    int counter = 0;
    std::vector<int> seed_voxel_i(3);
    seed_voxel_i.push_back(m_x);
    seed_voxel_i.push_back(m_y);
    seed_voxel_i.push_back(m_z);
    list_of_voxels_position[0].push_back(seed_voxel_i);

	do {
        for (int inc = 0; inc < list_of_voxels_position[counter].size(); inc++) {
            int i = list_of_voxels_position[counter][inc][0];
            int j = list_of_voxels_position[counter][inc][1];
            int k = list_of_voxels_position[counter][inc][2];
            for (int n = 0; n < 26; n++) {
                int x = i + NEIGHBOORS[n][0];
                int y = j + NEIGHBOORS[n][1];
                int z = k + NEIGHBOORS[n][2];
                if ((x < width && x >= 0) && (y < height && y >= 0) && (z < depth && z >= 0)) {
                    float theta_loop = img(x, y, z);
                    if (current_theta < theta_loop) {
                        std::vector<int> position(3);
                        position.push_back(x);
                        position.push_back(y);
                        position.push_back(z);
                        list_of_voxels_position[counter++].push_back(position);
                    }
                }
            }
        }
        current_theta--;
        counter++;
	} while(current_theta > theta_end);

    int * color;
    color = (int*) malloc(sizeof(int) * 30);
    int t = theta_beg;
    for (int i = 0; i < 30; i++) {
        t--;
        color[i] = t;
    }
    CImg<> ta = img.draw_fill(m_x, m_y, m_z , color, 1, voxels_img, current_theta, 20);
    return ta;
}
