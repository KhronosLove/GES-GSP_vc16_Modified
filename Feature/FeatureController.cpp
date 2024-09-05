//
//  FeatureController.cpp
//  UglyMan_Stitching
//
//  Created by uglyman.nothinglo on 2015/8/15.
//  Copyright (c) 2015 nothinglo. All rights reserved.
//

#include "FeatureController.h"


//Adds a new descriptor to the data vector, which stores the descriptors associated with a feature point.
void FeatureDescriptor::addDescriptor(const Mat& _descriptor) {
	//add the _descriptor (a matrix representing the SIFT descriptor) to the data vector
	data.emplace_back(_descriptor);
}
//
//Computes the Euclidean distance between two sets of feature descriptors.
//This distance measures how similar two feature points are, with a lower distance indicating higher similarity.
double FeatureDescriptor::getDistance(const FeatureDescriptor& _descriptor1,
	const FeatureDescriptor& _descriptor2,
	const double _threshold) {
	const vector<Mat>& data1 = _descriptor1.data;
	const vector<Mat>& data2 = _descriptor2.data;
	double result = FLT_MAX;

	for (int i = 0; i < data1.size(); ++i) {
		for (int j = 0; j < data2.size(); ++j) {
			double distance = 0;
			for (int k = 0; k < SIFT_DESCRIPTOR_DIM; ++k) {
				distance += ((data1[i].at<vl_sift_pix>(k) - data2[j].at<vl_sift_pix>(k)) *
					(data1[i].at<vl_sift_pix>(k) - data2[j].at<vl_sift_pix>(k)));

				/* at<vl_sift_pix>(k) == at<vl_sift_pix>(0, k) */

				if (distance >= _threshold) {
					break;
				}
			}
			result = min(result, distance);
		}
	}
	return result;
}


//Detects feature points in a grayscale image (_grey_img) and computes their corresponding SIFT descriptors.
void FeatureController::detect(const Mat& _grey_img,
	vector<Point2>& _feature_points,
	vector<FeatureDescriptor>& _feature_descriptors) {
#ifndef DP_NO_LOG
	if (_feature_points.empty() == false) {
		_feature_points.clear();
		printError("F(detect) feature points is not empty");
	}
	if (_feature_descriptors.empty() == false) {
		_feature_descriptors.clear();
		printError("F(detect) feature descriptors is not empty");
	}
#endif

	Mat grey_img_float = _grey_img.clone();
	grey_img_float.convertTo(grey_img_float, CV_32FC1);


	const int  width = _grey_img.cols;
	const int height = _grey_img.rows;


	VlSiftFilt* vlSift = vl_sift_new(width, height,
		log2(min(width, height)),
		SIFT_LEVEL_COUNT,
		SIFT_MINIMUM_OCTAVE_INDEX);

	vl_sift_set_peak_thresh(vlSift, SIFT_PEAK_THRESH);

	vl_sift_set_edge_thresh(vlSift, SIFT_EDGE_THRESH);

	if (vl_sift_process_first_octave(vlSift, (vl_sift_pix const*)grey_img_float.data) != VL_ERR_EOF) {

		do {
			vl_sift_detect(vlSift);
			for (int i = 0; i < vlSift->nkeys; ++i) {


				_feature_points.emplace_back(vlSift->keys[i].x, vlSift->keys[i].y);

				double angles[4];
				FeatureDescriptor descriptor;
				int angleCount = vl_sift_calc_keypoint_orientations(vlSift, angles, &vlSift->keys[i]);
				for (int j = 0; j < angleCount; ++j) {
					Mat descriptor_array(1, SIFT_DESCRIPTOR_DIM, CV_32FC1);
					vl_sift_calc_keypoint_descriptor(vlSift, (vl_sift_pix*)descriptor_array.data, &vlSift->keys[i], angles[j]);
					descriptor.addDescriptor(descriptor_array);
				}
				_feature_descriptors.emplace_back(descriptor);
			}	//saveKeypointsToFile("keypoints.txt", _feature_points, _feature_descriptors);
				
		} while (vl_sift_process_next_octave(vlSift) != VL_ERR_EOF);
	}
	vl_sift_delete(vlSift);
}

