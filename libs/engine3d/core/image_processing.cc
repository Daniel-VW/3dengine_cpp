// Copyright (C) 2024 twyleg
#include "image_processing.h"

#include <engine3d/engine/engine.h>

#include <opencv2/opencv.hpp>
#include <fmt/core.h>

#include <optional>

namespace HTM = engine3d::engine::homogeneous_transformation_matrix;

namespace engine3d::core {

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

ImageProcessing::ImageProcessing(

	engine3d::ui::ImageModel& imageModel,
	engine3d::ui::ParameterModel& parameterModel,
		QObject* parent) :
	QThread(parent),
	mImageModel(imageModel),
	mParameterModel(parameterModel)
{}

ImageProcessing::~ImageProcessing() {}

void ImageProcessing::run() {

	int frame_width = 640;
	int frame_height = 480;
	double lane_parameters[10];

	cv::Mat camera_frame;
    cv::Mat engine_frame;

	engine3d::engine::Engine engine(frame_width, frame_height);

    bool running = true;

    while(running) {
		double lane_parameters[] = {mParameterModel.getWidth(), mParameterModel.getLength(), mParameterModel.getStripeNumber(), mParameterModel.getStripeLength(), mParameterModel.getLineDistance(), mParameterModel.getStripedLine(), mParameterModel.getLeftLine(), mParameterModel.getRightLine()};

		camera_frame = cv::Mat(frame_height, frame_width, CV_8UC3, cv::Scalar(255,255,255));

		engine_frame = engine.run(camera_frame, getParameters(), lane_parameters);

		QImage img((uchar*)engine_frame.data, engine_frame.cols, engine_frame.rows, QImage::Format_RGB888);
		mImageModel.setImage(QPixmap::fromImage(img));

		msleep(1000 / 60.0);

    }

}


 HTM::Matrix::Parameter ImageProcessing::getParameters() {

    HTM::Matrix::Parameter parameter;

	parameter.vehicle_to_camera_parameter = {
		(mParameterModel.getCameraSystemTranslationX()-10000)/1000.0,
		(mParameterModel.getCameraSystemTranslationY()-10000)/1000.0,
		(mParameterModel.getCameraSystemTranslationZ()-10000)/1000.0,
		DEG_TO_RAD(mParameterModel.getCameraSystemRotationRoll()/10.0),
		DEG_TO_RAD(mParameterModel.getCameraSystemRotationPitch()/10.0),
		DEG_TO_RAD(mParameterModel.getCameraSystemRotationYaw()/10.0)
	};
 
	parameter.vehicle_to_cube_parameter = {
		(mParameterModel.getCubeSystemTranslationX()-10000)/1000.0,
		(mParameterModel.getCubeSystemTranslationY()-10000)/1000.0,
		(mParameterModel.getCubeSystemTranslationZ()-10000)/1000.0,
		DEG_TO_RAD(mParameterModel.getCubeSystemRotationRoll()/10.0),
		DEG_TO_RAD(mParameterModel.getCubeSystemRotationPitch()/10.0),
		DEG_TO_RAD(mParameterModel.getCubeSystemRotationYaw()/10.0)
	};

    parameter.ui_parameter = {
		static_cast<bool>(mParameterModel.getCubeSystemNormals()),
		static_cast<bool>(mParameterModel.getCubeSystemPoints()),
		static_cast<bool>(mParameterModel.getCubeSystemFaces())
    };

    return parameter;
}

}
