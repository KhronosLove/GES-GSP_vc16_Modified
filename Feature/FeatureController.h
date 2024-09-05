//
//  FeatureController.h
//  UglyMan_Stitching
//
//  Created by uglyman.nothinglo on 2015/8/15.
//  Copyright (c) 2015 nothinglo. All rights reserved.
//

#ifndef __UglyMan_Stitiching__FeatureController__
#define __UglyMan_Stitiching__FeatureController__

#include "../Configure.h"

const int SIFT_DESCRIPTOR_DIM = 128;

class FeatureDescriptor {
public:
	void addDescriptor(const Mat& _descriptor);
	static double getDistance(const FeatureDescriptor& _descriptor1,
		const FeatureDescriptor& _descriptor2,
		const double _threshold);
	vector<Mat> data;
};

class FeatureController {
public:
	static void detect(const Mat& _grey_img,
		vector<Point2>& _feature_points,
		vector<FeatureDescriptor>& _feature_descriptors);

	int static detectAndCount(const Mat& _grey_img, vector<Point2>& _feature_points, vector<FeatureDescriptor>& _feature_descriptors);
	int static matchFeatures(const vector<FeatureDescriptor>& descriptors1, const vector<FeatureDescriptor>& descriptors2, vector<std::pair<int, int>>& matches);
	void static saveCountsToFile(const std::string& filename, const std::string& content);
private:
};

#endif /* defined(__UglyMan_Stitiching__FeatureController__) */
