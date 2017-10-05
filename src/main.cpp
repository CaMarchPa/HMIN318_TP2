#include <iostream>
#include "CImg.h"
#include <vector>

using namespace cimg_library;

void segmentationVascularNetwork(CImg<> img, int seedVoxelX, int seedVoxelY, int seedVoxelZ);

std::vector<int> getNeighboors(CImg<> img, int x, int y, int z);

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
		    segmentationVascularNetwork(current_img, x, y, z);
        }
        
        image_disp.wait();
    }

    return 0;
}

void segmentationVascularNetwork(CImg<> img, int m_x, int m_y, int m_z) {
	
	std::vector<float> voxels;
	CImg<> voxels_img;
	int theta_beg = img(m_x, m_y, m_z);
	int current_theta = theta_beg;
	int theta_end = theta_beg - 30;
	int * N;
	N = (int*) malloc(sizeof(int) * 255);
	
	std::cout << "X = " << m_x << ", Y = " << m_y << ", Z = " << m_z << std::endl;
	int color = img.spectrum();
	//img.draw_fill(x, y, z , &color, 1, voxels_img, current_theta, 20);
	
	int width = img.width();
	int height = img.height();
	int depth = img.depth();
	
	do {
		for ( int j = 0; j < height; j++ ) {
			for ( int i = 0; i < width; i++ ) {
				for ( int k = 0; k < depth; k++ ) {
					
					// neighborhood
					std::vector<int> neighboors = std::vector<int> getNeighboors(img, i, j, k)
					int size_vect = neighboors.size();
					
					for (int inc = 0; inc < size_vect; inc++) {
						if (size_vect[inc] > current_theta) 
					}
				}
			}
		}
	} while(current_theta > theta_end);
	
	
	
	
	
	
	
	
	
}

std::vector<int> getNeighboors(CImg<> img, int x, int y, int z) {
	return NULL;
}
