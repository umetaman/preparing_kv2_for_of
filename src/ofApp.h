#pragma once

#include "ofMain.h"
#include <opencv2/opencv.hpp>
#include <Kinect.h>
#include <cstdlib>

template<class Interface>
inline void SafeRelease(Interface*& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL) {
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

class ofApp : public ofBaseApp{

	public:
		bool setupKinect();
		void updateKinect();

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		int depthFrameWidth = 0;
		int depthFrameHeight = 0;
		unsigned int depthBufferSize;

		int colorFrameWidth = 0;
		int colorFrameHeight = 0;
		unsigned int colorBufferSize;

		//KinectV2
		IKinectSensor* pSensor;
		IColorFrameSource* pColorSource;
		IColorFrameReader* pColorReader;
		IDepthFrameSource* pDepthSource;
		IDepthFrameReader* pDepthReader;
		IFrameDescription* pDepthDescription;
		IFrameDescription* pColorDescription;

		//�����̐�������
		HRESULT hResult = S_OK;

		//OpenCv�̉�f�z��
		cv::Mat depthBufferMat;
		cv::Mat depthMat;
		cv::Mat colorBufferMat;
		cv::Mat colorMat;

		//openFrameworks��draw�Ŋm�F�p�ɕϊ�
		//OpenCv��BGR���f�t�H���g�Ȃ̂ŁARGB�ɕϊ����Ă�����
		cv::Mat rgbDepthMat;
		cv::Mat rgbColorMat;

		//openFrameworks�Ŏg����摜
		ofImage depthImage;
		ofImage colorImage;
};
