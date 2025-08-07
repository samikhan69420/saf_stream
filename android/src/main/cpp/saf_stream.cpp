#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define LOG_TAG "SafStream"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_fluttercavalry_saf_1stream_NativeFileReader_nativeInit(
        JNIEnv *env,
        jobject /* this */,
        jint fd,
        jlong fileSize) {
    
    void* mapped = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        LOGE("mmap failed: %d", errno);
        return 0;
    }
    
    return reinterpret_cast<jlong>(mapped);
}

JNIEXPORT void JNICALL
Java_com_fluttercavalry_saf_1stream_NativeFileReader_nativeRead(
        JNIEnv *env,
        jobject /* this */,
        jlong handle,
        jlong offset,
        jbyteArray buffer,
        jint length) {
    
    void* mapped = reinterpret_cast<void*>(handle);
    char* data = static_cast<char*>(mapped) + offset;
    
    env->SetByteArrayRegion(buffer, 0, length, reinterpret_cast<jbyte*>(data));
}

JNIEXPORT void JNICALL
Java_com_fluttercavalry_saf_1stream_NativeFileReader_nativeClose(
        JNIEnv *env,
        jobject /* this */,
        jlong handle,
        jlong fileSize) {
    
    void* mapped = reinterpret_cast<void*>(handle);
    munmap(mapped, fileSize);
}

}
