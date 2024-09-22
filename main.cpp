//new
#include <iostream>
#include "./Stitching/NISwGSP_Stitching.h"
#include "./Debugger/TimeCalculator.h"
#include <opencv2/opencv.hpp>
#include <Python.h>
#include "pre.h"
#include "ImageRenamer.h"
#include "ImageCropper.h"
using namespace std;

int GRID_SIZE_w = 40;
int GRID_SIZE_h = 40;

int main(int argc, const char* argv[]) {
	Eigen::initParallel(); /* remember to turn off "Hardware Multi-Threading */
	CV_DNN_REGISTER_LAYER_CLASS(Crop, CropLayer);
	cout << "nThreads = " << Eigen::nbThreads() << endl;
	cout << "[#Images : " << argc - 1 << "]" << endl;

	string base_path = "C:/Users/kyy/Desktop/GES-GSP_vc16_modified/input-data/";

	string img_file[] = { "fused" };


	std::string input_path = "C:/Users/kyy/Desktop/GES-GSP_vc16_Modified/input-data/original/";
	std::string output_path = "C:/Users/kyy/Desktop/GES-GSP_vc16_Modified/input-data/fused/";

	fuse_images(input_path, output_path);

	ImageRenamer imageRenamer(output_path);
	imageRenamer.renameAndProcessImages();

	ImageCropper imageCropper(output_path);
	imageCropper.cropImages();

	int file_num = sizeof(img_file) / sizeof(img_file[0]);


	time_t start = clock();
	timecalculator timer;

	for (int i = 0; i < file_num; ++i) {
		cout << "i = " << i << ", [images : " << argv[i] << "]" << endl;

		multiimages multi_images(base_path, img_file[i], lines_filter_width, lines_filter_length);

		/* 2d */
		niswgsp_stitching niswgsp(multi_images);
		niswgsp.setweighttoalignmentterm(1);
		niswgsp.setweighttolocalsimilarityterm(0.75);
		niswgsp.setweighttoglobalsimilarityterm(6, 20, global_rotation_2d_method);
		niswgsp.setweighttocontentpreservingterm(1.5);
		mat blend_linear;
		vector<vector<point2> > original_vertices;
		if (run_type == 1) {
			blend_linear = niswgsp.solve_content(blend_linear, original_vertices);
		}
		else {
			blend_linear = niswgsp.solve(blend_linear, original_vertices);
		}
		time_t end = clock();
		cout << "time:" << double(end - start) / clocks_per_sec << endl;

		niswgsp.writeimage(blend_linear, blending_methods_name[blend_linear]);
		niswgsp.assessment(original_vertices);
	}


	return 0;
}
