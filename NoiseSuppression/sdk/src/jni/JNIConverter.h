#pragma once

#include "jni.h"
#include <string>

#define CHECK_EXCEPTION(env); \
    do { \
        if (env->ExceptionCheck()) { \
            env->ExceptionClear(); \
        } \
    }while(false)

#define DELETE_LOCAL_REF(env, object); \
    do { \
        if (object) { \
            env->DeleteLocalRef(object); \
            CHECK_EXCEPTION(env); \
            object = nullptr; \
        } \
    }while(false)

static const std::string ClassName(const char *name)
{
    const static std::string sPath = "com/seewo/ns/";
    return sPath + name;
}

class SdkConverter
{
public:
    SdkConverter();
    virtual ~SdkConverter();

protected:
    static bool CheckClass(jclass clazz, const std::string &className);
    static bool CheckMethod(jmethodID method, const std::string &className);

public:
    static jstring CreateString(JNIEnv *env, const std::string &str);
    static std::string CreateString(JNIEnv *env, jstring jstr);
};