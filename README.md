# Raw-Format-Convert-to-BMP

This application converts Raw files to Bitmap files according to file's width, height and bit-depth. Also you can choose output color type and Bayer Pattern

해당 어플리케이션은 Raw 파일을 Bayer Pattern에 따라 Bitmap 파일로 변환하여 저장한다. 또한 이미지의 Color Type을 설정할 수 있고 이러한 변환을 위해서 Raw 파일의 가로와 세로, 그리고 BitDepth가 필요하다. 해당 어플리케이션은 VC 9버전으로 작성되었다.

## 개발 환경

* OS : Windows 10 Pro 64bit (App: 32bit)
* CPU : Intel(R) Core(TM) i7-6500U CPU @ 2.50GHz 2.59 GHz
* RAM : 8.00GB
* IDE : Microsoft Visual Studio 2008 Version 9.0.30729.1 SP

## 사용 기술

C++, Window API, STL Vector, MFC, Image Processing

## 부가 설명

먼저 프로그램을 실행하면 MFC File Dialog를 통해 변환할 파일(RAW)을 불러오고
해당 파일의 정보를 CFiles 클래스 객체에 저장한다. 이후 해당 객체를 인자로 FileControl의 메서드를 실행하며 어플리케이션이 동작한다.

파일 컨트롤 클래스는 RAW 파일의 읽기와 저장 그리고 Interpolation 객체와 BmpFile 객체를 통해 파일의 보간과 비트맵 이미지로의 변환을 수행한다. 먼저 파일의 읽기 과정에서 해당 파일이 10bit depth일 경우 8bit 즉 Byte 단위로 정규화 변환을 수행하고 파일을 동적할당한 배열에 저장한다. 이후 Bilinear Interpolation을 수행한다.

이렇게 보간된 데이터에 출력(저장) Color Type에 따라 RGB to GrayScale 변환 여부를 결정하고 최종적으로 변환된 데이터에 헤더 파일과 비트맵 이미지 특성을 고려한 상하 반전을 수행하여 CFiles 클래스의 Result 객체에 할당하고 해당 객체를 통해 디스크에 저장한다.

## 실행 과정

1. 솔루션 생성 및 Source, Header Files Include
2. 솔루션 빌드 및 실행
3. RAW 파일 선택 후 해당 파일의 가로와 세로, 픽셀 당 비트 선택
4. 출력할 Color Type과 보간할 BayerPattern 선택

## 실행 결과

추후 업로드 예정입니다.

(클릭 시 유튜브로 이동합니다)
