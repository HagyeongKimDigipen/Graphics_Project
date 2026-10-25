# Graphics_Project

C++로 작성한 실시간 그래픽스 프로젝트 모음입니다. DigiPen Institute of Technology 컴퓨터과학 과정의 그래픽스 과제를 바탕으로 진행했습니다. 라이팅, 환경 매핑, 디퍼드 렌더링, 충돌 판정, 3D 변환, DirectX 11 렌더링 파이프라인까지 직접 구현했습니다.

## 프로젝트

| 폴더 | 주제 | 주요 기술 |
|---|---|---|
| [`lighting`](./lighting) | 다중 광원 실시간 라이팅 | OpenGL, GLSL, 조명 모델 3종, CPU/GPU UV 생성, OBJ 로딩, ImGui |
| [`reflaction`](./reflaction) | 반사, 굴절, Fresnel 환경 매핑 | OpenGL, GLSL, FBO, UBO(최대 16개 광원), Skybox |
| [`hybrid_rendering`](./hybrid_rendering) | 디퍼드 라이팅 기반 하이브리드 렌더링 | OpenGL 4.0, G-buffer, Deferred Lighting, 디버그 시각화 |
| [`collision_geomeryPrimitives`](./collision_geomeryPrimitives) | 3D 충돌 판정 시각화 | 기하 프리미티브 6종, 충돌 케이스 14종, GLM, ImGui |
| [`project1`](./project1) | 3D 변환과 카메라 | Affine 변환, 투영, 카메라, Cube/Frustum/Snub Dodecahedron 메시 |
| [`directx_classproject`](./directx_classproject) | 소형 엔진형 DirectX 11 프로젝트 | Win32, DirectX 11, 버퍼 추상화, 입력 처리, 커스텀 수학 타입 |

## 프로젝트별 요약

### lighting
다중 광원과 조명 모델 3종을 셰이더로 전환하며 비교하는 실시간 라이팅 프로젝트입니다. OBJ 모델을 불러와 노멀을 계산하고, 텍스처 좌표를 CPU와 GPU 양쪽에서 생성합니다. 광원과 재질은 ImGui로 실시간 조절합니다.

### reflaction
GLSL로 반사, 굴절, Fresnel 환경 매핑을 구현했습니다. FBO로 주변 장면을 렌더링해 환경 맵으로 쓰고, 최대 16개 광원을 UBO로 묶어 셰이더에 전달합니다.

### hybrid_rendering
G-buffer에 위치, 노멀, 재질 정보를 기록한 뒤 조명을 한 번에 계산하는 디퍼드 라이팅 구조입니다. G-buffer 내용을 화면에 띄우는 디버그 시각화를 포함합니다.

### collision_geomeryPrimitives
구, AABB, 광선, 평면, 삼각형, 점 6종의 프리미티브와 14가지 충돌 판정 케이스를 구현했습니다. 충돌 여부는 셰이더 색상으로 즉시 표시되고, WASD로 카메라를 움직이며 확인할 수 있습니다.

### project1
Affine 변환, 투영 행렬, 카메라 클래스를 직접 작성하고 Cube, Frustum, Snub Dodecahedron 메시를 렌더링합니다.

### directx_classproject
Win32 창 위에 DirectX 11 렌더링 파이프라인을 구성했습니다. Graphics, Model, Camera, Vertex/Index/Constant Buffer, 키보드와 마우스 입력을 역할별 클래스로 나눴습니다.

## 실행 환경

- Windows, Visual Studio (각 폴더의 `.sln` 파일)
- 라이브러리: GLFW, GLEW, GLM, ImGui (각 폴더의 `ThirdParty` 포함), DirectX 11
- 조작 방법은 각 폴더의 `README.txt`에 있습니다.

## 참고

DigiPen 과제 프레임워크를 바탕으로 작성한 코드입니다. 일부 파일에는 DigiPen 저작권 헤더가 포함되어 있습니다.
