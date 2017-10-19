#pragma once

#include "common.h"

void drawLines(cv::Mat& image, const std::vector<Line>& lines, const cv::Scalar &color = cv::Scalar(1.0f, 1.0f, 1.0f));
void drawLines(cv::Mat& image, const std::vector<Point>& lines, const cv::Scalar &color = cv::Scalar(1.0f, 1.0f, 1.0f));
void drawPoints(cv::Mat& image, const std::vector<Point>& points,
                const cv::Scalar &color = cv::Scalar(1.0f, 1.0f, 1.0f), float radius = 2.0f);
void drawHull(cv::Mat& image, const std::vector<Point>& points, const std::vector<Point>& hull = {});
