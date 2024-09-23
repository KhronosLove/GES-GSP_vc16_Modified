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

	//fuse_images(input_path, output_path);

	ImageRenamer imageRenamer(output_path);
	imageRenamer.renameAndProcessImages();

	ImageCropper imageCropper(output_path);
	imageCropper.cropImages();

	int file_num = sizeof(img_file) / sizeof(img_file[0]);


	time_t start = clock();
	TimeCalculator timer;

	for (int i = 0; i < file_num; ++i) {
		cout << "i = " << i << ", [Images : " << argv[i] << "]" << endl;
		MultiImages multi_images(base_path, img_file[i], LINES_FILTER_WIDTH, LINES_FILTER_LENGTH);

		/* 2D */
		NISwGSP_Stitching niswgsp(multi_images);
		niswgsp.setWeightToAlignmentTerm(1);
		niswgsp.setWeightToLocalSimilarityTerm(0.75);
		niswgsp.setWeightToGlobalSimilarityTerm(6, 20, GLOBAL_ROTATION_2D_METHOD);
		niswgsp.setWeightToContentPreservingTerm(1.5);
		Mat blend_linear;
		vector<vector<Point2> > original_vertices;
		if (RUN_TYPE == 1) {
			blend_linear = niswgsp.solve_content(BLEND_LINEAR, original_vertices);
		}
		else {
			blend_linear = niswgsp.solve(BLEND_LINEAR, original_vertices);
		}
		time_t end = clock();
		cout << "Time:" << double(end - start) / CLOCKS_PER_SEC << endl;
		niswgsp.writeImage(blend_linear, BLENDING_METHODS_NAME[BLEND_LINEAR]);
		niswgsp.assessment(original_vertices);
	}



	return 0;
}
