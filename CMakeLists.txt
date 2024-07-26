cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(Flappy)
include("${VITASDK}/share/vita.cmake" REQUIRED)

find_package(SDL2 REQUIRED)

set(VITA_APP_NAME "Flappy Bird - Vita")
set(VITA_TITLEID  "VSDK00017")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(
  common
)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/Game.cpp
  src/Player.cpp
  src/Background.cpp
  src/Level.cpp
  src/Img.cpp
  src/SoundEffect.cpp
  common/SDL_FontCache.c
)

target_link_libraries(${PROJECT_NAME}

  # SDL2 libraries
  SDL2_mixer
  SDL2 
  SDL2_image
  SDL2_ttf

  modplug
  vorbisfile
  vorbis
  ogg


  # Audio and image libraries
  opusfile
  opus
  FLAC
  xmp

  # Font library
  freetype

  # Compression and Image libraries
  jpeg
  png
  z
  bz2
  
  # Vita system libraries
  SceDisplay_stub
  SceTouch_stub
  SceCtrl_stub
  SceAudio_stub
  SceAudioIn_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceMotion_stub
  SceHid_stub

  
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg0.png sce_sys/livearea/contents/bg0.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml

  FILE assets/font/SuperMario256.ttf assets/font/SuperMario256.ttf

  FILE assets/textures/bg1.png assets/textures/bg1.png
  FILE assets/textures/bg2.png assets/textures/bg2.png
  FILE assets/textures/death.png assets/textures/death.png
  FILE assets/textures/frame-1.png assets/textures/frame-1.png
  FILE assets/textures/frame-2.png assets/textures/frame-2.png
  FILE assets/textures/pipeBottom.png assets/textures/pipeBottom.png
  FILE assets/textures/pipeTop.png assets/textures/pipeTop.png

  FILE assets/sounds/collision.wav assets/sounds/collision.wav
  FILE assets/sounds/death.wav assets/sounds/death.wav
  FILE assets/sounds/flap.wav assets/sounds/flap.wav
  FILE assets/sounds/music.wav assets/sounds/music.wav
  FILE assets/sounds/point.wav assets/sounds/point.wav

)
