#ifndef FACE_RECOGNITION_FACENET_H
#define FACE_RECOGNITION_FACENET_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cassert>
#include <numeric>
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <NvInfer.h>
#include <NvUffParser.h>
#include <NvInferPlugin.h>
#include <l2norm_helper.h>
#include "common.h"
#include "pBox.h"

using namespace nvinfer1;
using namespace nvuffparser;


class FaceNetClassifier {
private:
    int m_INPUT_C;
    int m_INPUT_H;
    int m_INPUT_W;
    Logger m_gLogger;
    DataType m_dtype;
    string m_uffFile;
    string m_engineFile;
    int m_batchSize;
    bool m_serializeEngine;
    int m_maxFacesPerScene;
    ICudaEngine *m_engine;
    IExecutionContext *m_context;
    float m_output[128];
    
    std::vector<cv::Mat> m_croppedFaces;
    
    float m_knownPersonThresh;
public:
    std::vector<float> m_embeddings;
    std::vector<struct KnownID> m_knownFaces;
    FaceNetClassifier(Logger gLogger, DataType dtype, const string uffFile, const string engineFile, int batchSize,
            bool serializeEngine, float knownPersonThreshold, int maxFacesPerScene=5);
    ~FaceNetClassifier();

    void createOrLoadEngine();
    void getCroppedFacesAndAlign(cv::Mat frame, std::vector<struct Bbox> outputBbox);
    void preprocessFaces();
    void doInference(float* inputData, float* output);
    void forwardPreprocessing(cv::Mat image, std::vector<struct Bbox> outputBbox, std::vector<struct Paths> paths, int nbFace);
    void forward(cv::Mat image, std::vector<struct Bbox> outputBbox);
    void featureMatching();
    void resetVariables();

};

float vectors_distance(const std::vector<float>& a, const std::vector<float>& b);
//inline bool fileExists(const std::string &name);
inline unsigned int elementSize(nvinfer1::DataType t);

#endif //FACE_RECOGNITION_FACENET_H
