#ifndef OPENGLESDEMO_OPENGLESPlYAER_H
#define OPENGLESDEMO_OPENGLESPlYAER_H

#include <string>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

class OpenSLESPlayer {
public:
    OpenSLESPlayer();
    int32_t setDataSource(const std::string& pcmPath);
    int32_t prepare();
    void start();
    void stop();
    void release();
private:
    SLresult createEngine();
private:
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    SLObjectItf outputMixObject;
    SLObjectItf playerObject;
    SLPlayItf playerPlay;
    SLVolumeItf playerVolume;
};
#endif //OPENGLESDEMO_OPENGLESPlYAER_H
