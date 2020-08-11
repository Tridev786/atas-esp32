#ifndef Camera_h
#define Camera_h
#include <TridentTD_LineNotify.h>
#include <FS.h>
#include <HTTPClient.h>
#include "Parameter.h"

class Camera
{
  public:
    Camera();
    void camera1Trigger();
    void camera2Trigger();
    void camera3Trigger();
    void camera4Trigger();
    void camera5Trigger();
    
  private:
    String IPcam1 = "192.168.0.106";
    String IPcam2 = "192.168.0.104";
    String IPcam3 = "";
    String IPcam4 = "";
    String IPcam5 = "";

    String IPCAM_USERNAME = "admin";
    String IPCAM_PASSWORD = "";
    Parameter parameter;

    void downloadAndSaveFile(String fileName, String  url);
    
    String tokens[2] = {"hE2QQoFwRy5aKIKOmSnCO4proUOPtS5f5UlbAZWTmTk",parameter.lineToken};
};
#endif
