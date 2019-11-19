# KinectV2をopenFrameworksで使う

Microsoftの公式のSDKを使って、openFrameworks上でKinectV2を使う。

おおまかな流れは次の通り。
1. KinectV2をオープン。
2. OpenCVのMatにRGBカメラ画像と深度画像を読みだす。
3. openFrameworksのofImageに変換する。

## 必要なもの
- [Kinect for Windows SDK 2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561)
- [OpenCV 4.1.1](https://opencv.org/releases/) or later
- [openFrameworks](https://openframeworks.cc/ja/) 0.10.1
- Windows10

SDKの仕様上、Windowsでのみ動作する。デバッグ、およびビルドはx64のみで行う。

openFrameworksに同梱されているaddonの「ofxOpenCv」は使わない。競合するのかは不明だが、うまくいかなかった。（cv::imshowに問題あり？）

## 使うまでの手順
1. Kinect for Windows SDK 2.0をインストールする。Kinect Studioで動作確認する。
2. OpenCVの公式サイトから、ビルド済みのDLLやlibファイルが入ったインストーラーをダウンロードして、展開する。
3. openFrameworksでプロジェクトを作成する。
4. プロジェクトのインクルードディレクトリ、追加のインクルードディレクトリ、ライブラリディレクトリ、依存ファイルを設定する。
```
プロジェクトのプロパティから

[VC++ディレクトリ > インクルードディレクトリ]
$(KINECTSDK20_DIR)\inc
<opencv_root>\build\include

[VC++ディレクトリ > ライブラリディレクトリ]
$(KINECTSDK20_DIR)\Lib\x64
<opencv_root>\build\x64\vc15\lib

[C/C++ > 追加のインクルードディレクトリ]
$(KINECTSDK20_DIR)\inc
<opencv_root>\build\include

[リンカー > 入力 > 追加の依存ファイル]
opencv_world411.lib
opencv_world411d.lib

```

5. このリポジトリのsrc以下のソースコードを参照して、いい感じのコードを書く。

## 参考
- [https://github.com/UnaNancyOwen/Kinect2Sample/tree/2200fb0c5acd2dbb458a85a587dc2909c6be8859](https://github.com/UnaNancyOwen/Kinect2Sample/tree/2200fb0c5acd2dbb458a85a587dc2909c6be8859)