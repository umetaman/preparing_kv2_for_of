#include "ofApp.h"

bool ofApp::setupKinect()
{
	//Kinect
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)) {
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return false;
	}
	//open Kinect
	hResult = pSensor->Open();
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
		return false;
	}

	// Source
	//DepthSource
	hResult = pSensor->get_DepthFrameSource(&pDepthSource);
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
		return false;
	}
	//ColorSource
	hResult = pSensor->get_ColorFrameSource(&pColorSource);
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
		return false;
	}

	// Reader
	//DepthReader
	hResult = pDepthSource->OpenReader(&pDepthReader);
	if (FAILED(hResult)) {
		std::cerr << "Error : IDepthFrameSource::OpenReader()" << std::endl;
		return false;
	}
	//ColorReader
	hResult = pColorSource->OpenReader(&pColorReader);
	if (FAILED(hResult)) {
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
		return false;
	}

	// Description
	//Depth
	hResult = pDepthSource->get_FrameDescription(&pDepthDescription);
	if (FAILED(hResult)) {
		std::cerr << "Error : IDepthFrameSource::get_FrameDescription()" << std::endl;
		return false;
	}
	//Color
	hResult = pColorSource->get_FrameDescription(&pColorDescription);
	if (FAILED(hResult)) {
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
		return false;
	}

	//大きさを取得してバッファを確保する
	//Depth
	pDepthDescription->get_Width(&depthFrameWidth); // 512
	pDepthDescription->get_Height(&depthFrameHeight); // 424
	depthBufferSize = depthFrameWidth * depthFrameHeight * sizeof(unsigned short);
	depthBufferMat = cv::Mat(depthFrameHeight, depthFrameWidth, CV_16UC1);
	depthMat = cv::Mat(depthFrameHeight, depthFrameWidth, CV_8UC1);

	// Range ( Range of Depth is 500-8000[mm], Range of Detection is 500-4500[mm] ) 
	unsigned short min = 0;
	unsigned short max = 0;
	pDepthSource->get_DepthMinReliableDistance(&min); // 500
	pDepthSource->get_DepthMaxReliableDistance(&max); // 4500
	std::cout << "Range : " << min << " - " << max << std::endl;

	//Color
	pColorDescription->get_Width(&colorFrameWidth); //1920
	pColorDescription->get_Height(&colorFrameHeight); //1080
	colorBufferSize = colorFrameWidth * colorFrameHeight * 4 * sizeof(unsigned char);
	//1920 x 1080はでかすぎるので半分にする
	colorBufferMat = cv::Mat(colorFrameHeight, colorFrameWidth, CV_8UC4);
	colorMat = cv::Mat(colorFrameHeight / 2, colorFrameWidth / 2, CV_8UC4);

	return true;
}

void ofApp::updateKinect()
{
	// DepthFrame
	IDepthFrame* pDepthFrame = nullptr;
	hResult = pDepthReader->AcquireLatestFrame(&pDepthFrame);
	if (SUCCEEDED(hResult)) {
		hResult = pDepthFrame->AccessUnderlyingBuffer(&depthBufferSize, reinterpret_cast<UINT16**>(&depthBufferMat.data));
		if (SUCCEEDED(hResult)) {
			depthBufferMat.convertTo(depthMat, CV_8U, -255.0f / 8000.0f, 255.0f);
		}
	}
	SafeRelease(pDepthFrame);

	// ColorFrame
	IColorFrame* pColorFrame = nullptr;
	hResult = pColorReader->AcquireLatestFrame(&pColorFrame);
	if (SUCCEEDED(hResult)) {
		//変換
		hResult = pColorFrame->CopyConvertedFrameDataToArray(
			colorBufferSize, reinterpret_cast<BYTE*>(colorBufferMat.data), ColorImageFormat::ColorImageFormat_Bgra);
		if (SUCCEEDED(hResult)) {
			//縮小処理
			cv::resize(colorBufferMat, colorMat, cv::Size(), 0.5, 0.5);
		}
	}
	SafeRelease(pColorFrame);
}

//--------------------------------------------------------------
void ofApp::setup(){
	if (setupKinect() == false) {
		OF_EXIT_APP(0);
	}

	//oFで使える画像の領域を確保する。
	depthImage.allocate(depthFrameWidth, depthFrameHeight, OF_IMAGE_COLOR);
	colorImage.allocate(colorFrameWidth / 2, colorFrameHeight / 2, OF_IMAGE_COLOR);

	//ウィンドウの大きさを変更する
	ofSetWindowShape(colorImage.getWidth(), depthImage.getHeight() + colorImage.getHeight());
}

//--------------------------------------------------------------
void ofApp::update(){
	//Kinect
	updateKinect();

	cv::cvtColor(depthMat, rgbDepthMat, cv::COLOR_GRAY2RGB);
	cv::cvtColor(colorMat, rgbColorMat, cv::COLOR_BGRA2RGB);

	depthImage.setFromPixels(rgbDepthMat.ptr(), rgbDepthMat.cols, rgbDepthMat.rows, OF_IMAGE_COLOR, false);
	colorImage.setFromPixels(rgbColorMat.ptr(), rgbColorMat.cols, rgbColorMat.rows, OF_IMAGE_COLOR, false);
}

//--------------------------------------------------------------
void ofApp::draw(){
	//描画
	depthImage.draw(0, 0);
	colorImage.draw(0, depthImage.getHeight());
}

void ofApp::exit()
{
	SafeRelease(pDepthSource);
	SafeRelease(pColorSource);
	SafeRelease(pDepthReader);
	SafeRelease(pColorReader);
	SafeRelease(pDepthDescription);
	SafeRelease(pColorDescription);
	if (pSensor) {
		pSensor->Close();
	}
	SafeRelease(pSensor);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
