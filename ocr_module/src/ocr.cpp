#include <iostream>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./ocr_executable <image_path>" << std::endl;
        return -1;
    }

    // 画像の読み込み
    cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    // グレースケール変換
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    
    // デバッグ: グレースケール画像の表示・保存
    cv::imwrite("debug_gray.png", gray); // 保存
    cv::waitKey(0); // キー入力待ちで表示維持

    // 二値化（Otsuの方法を使用）
    cv::Mat binary;
    cv::threshold(gray, binary, 0, 255, cv::THRESH_BINARY_INV + cv::THRESH_OTSU);

    // デバッグ: 二値化画像の表示・保存
    cv::imwrite("debug_binary.png", binary); // 保存
    cv::waitKey(0); // キー入力待ちで表示維持

    // 輪郭抽出
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 文字領域をクリップしてOCRを実行
    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    if (ocr->Init(NULL, "jpn")) {
        std::cerr << "Could not initialize tesseract." << std::endl;
        return -1;
    }

    for (const auto& contour : contours) {
        // 輪郭の外接矩形を取得
        cv::Rect boundingBox = cv::boundingRect(contour);

        // 文字領域をクリップ
        cv::Mat roi = image(boundingBox);

        // デバッグ: クリップされた文字領域の表示・保存
        cv::imwrite("debug_cropped.png", roi); // 保存
        cv::waitKey(0); // キー入力待ちで表示維持

        // OCR処理を実行
        ocr->SetImage(roi.data, roi.cols, roi.rows, 3, roi.step);
        std::cout << ocr->GetUTF8Text() << std::endl;
    }

    ocr->End();
    return 0;
}
