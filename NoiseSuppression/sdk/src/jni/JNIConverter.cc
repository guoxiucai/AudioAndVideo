#include "JNIConverter.h"

SdkConverter::SdkConverter()
{
}

SdkConverter::~SdkConverter()
{
}

bool SdkConverter::CheckClass(jclass clazz, const std::string &className)
{
    if (clazz) {
        return true;
    }
    return false;
}

bool SdkConverter::CheckMethod(jmethodID method, const std::string &className)
{
    if (method) {
        return true;
    }
    return false;
}

jstring SdkConverter::CreateString(JNIEnv *env, const std::string &str)
{
    if (!env || str.empty()) {
        return nullptr;
    }

    jclass strClass = env->FindClass("java/lang/String");
    CHECK_EXCEPTION(env);
    jstring strEncode = env->NewStringUTF("UTF-8");
    CHECK_EXCEPTION(env);
    jmethodID strMethodID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    CHECK_EXCEPTION(env);

    jbyteArray byteArray = env->NewByteArray((jsize)str.length());
    CHECK_EXCEPTION(env);
    env->SetByteArrayRegion(byteArray, 0, (jsize)str.length(), (const jbyte*)str.c_str());
    CHECK_EXCEPTION(env);

    jstring jstr = (jstring)env->NewObject(strClass, strMethodID, byteArray, strEncode);
    CHECK_EXCEPTION(env);

    DELETE_LOCAL_REF(env, byteArray);
    DELETE_LOCAL_REF(env, strEncode);
    DELETE_LOCAL_REF(env, strClass);

    return jstr;
}

std::string SdkConverter::CreateString(JNIEnv *env, jstring jstr)
{
    std::string str = "";
    if (!env || !jstr) {
        return str;
    }

    jclass strClass = env->FindClass("java/lang/String");
    CHECK_EXCEPTION(env);
    jstring strEncode = env->NewStringUTF("UTF-8");
    CHECK_EXCEPTION(env);
    jmethodID getBytes = env->GetMethodID(strClass, "getBytes", "(Ljava/lang/String;)[B");
    CHECK_EXCEPTION(env);

    jbyteArray byteArray = (jbyteArray)env->CallObjectMethod(jstr, getBytes, strEncode);
    CHECK_EXCEPTION(env);

    jbyte *data = env->GetByteArrayElements(byteArray, JNI_FALSE);
    CHECK_EXCEPTION(env);

    int size = env->GetArrayLength(byteArray);
    CHECK_EXCEPTION(env);

    if (size > 0 && data) {
        str = std::string((const char *)data, size);
    }

    if (data) {
        env->ReleaseByteArrayElements(byteArray, data, JNI_ABORT);
        CHECK_EXCEPTION(env);
    }

    DELETE_LOCAL_REF(env, byteArray);
    DELETE_LOCAL_REF(env, strEncode);
    DELETE_LOCAL_REF(env, strClass);

    return str;
}