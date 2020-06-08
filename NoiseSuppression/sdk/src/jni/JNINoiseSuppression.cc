#include "JNINoiseSuppression.h"
#include "../noise_suppression.h"

static JavaVM *g_JavaVM = nullptr;

JNINativeMethod g_Methods[] = {
    { "WebRtcNsCreate", "()J", (void*)Java_com_seewo_ns_NoiseSuppression_WebRtcNsCreate},
    { "WebRtcNsFree", "(J)V", (void*)Java_com_seewo_ns_NoiseSuppression_WebRtcNsFree},
    { "WebRtcNsInit", "(JI)I", (void*)Java_com_seewo_ns_NoiseSuppression_WebRtcNsInit},
    { "WebRtcNsSetPolicy", "(JI)I", (void*)Java_com_seewo_ns_NoiseSuppression_WebRtcNsSetPolicy}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    g_JavaVM = vm;
    JNIEnv *pEnv = NULL;
    if (vm->GetEnv((void**)&pEnv, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    jclass nativeClass = pEnv->FindClass(ClassName("NoiseSuppressionSDK").c_str());
    CHECK_EXCEPTION(pEnv);
    if (nullptr == nativeClass) {
        return JNI_ERR;
    }
    jint ret = pEnv->RegisterNatives(nativeClass, g_Methods, sizeof(g_Methods) / sizeof(g_Methods[0]));
    CHECK_EXCEPTION(pEnv);
    if (ret != JNI_OK) {
        return JNI_ERR;
    }    

    return JNI_VERSION_1_6;
}

JNIEXPORT jlong JNICALL Java_com_seewo_ns_NoiseSuppression_WebRtcNsCreate(JNIEnv *, jclass)
{
    NsHandle *hNS = WebRtcNs_Create(); 
    return (long)hNS;
}

JNIEXPORT void JNICALL Java_com_seewo_ns_NoiseSuppression_WebRtcNsFree(JNIEnv *, jclass, jlong nsHandler)
{
    NsHandle *hNS = (NsHandle *)nsHandler;
    WebRtcNs_Free(hNS);
}

JNIEXPORT jint JNICALL Java_com_seewo_ns_NoiseSuppression_WebRtcNsInit(JNIEnv *, jclass, jlong nsHandler, jint sf) 
{
    NsHandle *hNS = (NsHandle *)nsHandler;
    return WebRtcNs_Init(hNS, sf);
}

JNIEXPORT jint JNICALL Java_com_seewo_ns_NoiseSuppression_WebRtcNsSetPolicy(JNIEnv *, jclass, jlong nsHandler, jint mode)
{
    NsHandle *hNS = (NsHandle *)nsHandler;
    return WebRtcNs_set_policy(hNS, mode);
}