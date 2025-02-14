/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <cstring>
#include <jni.h>
#include <cinttypes>
#include <android/log.h>
#include <gmath.h>
#include <gperf.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/resource.h>

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, "hello-libs::", __VA_ARGS__))


#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, "hello2-libs::", __VA_ARGS__))


/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   app/src/main/java/com/example/hellolibs/MainActivity.java
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellolibs_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    // Just for simplicity, we do this right away; correct way would do it in
    // another thread...

    void *p = malloc(32);
    auto ticks = GetTicks();

    for (auto exp = 0; exp < 32; ++exp) {
        volatile unsigned val = gpower(exp);
        (void) val;  // to silence compiler warning
    }
    ticks = GetTicks() - ticks;

    int i=5000;

    LOGW("about to malloc() 650Mb");
    p = malloc(1048560 * 650);
    memset(p, 0, 1048560 * 650 );

    LOGW("sleeping for 5s");
    sleep(5);

    struct rlimit lim_as, lim_data ;

    if( getrlimit(RLIMIT_AS, &lim_as) != 0 ){
        LOGW("RLIMIT_AS error %d", errno);
    }

    LOGW("Rlimit as cur=%ld, max=%ld", lim_as.rlim_cur, lim_as.rlim_max);
    if(getrlimit(RLIMIT_DATA, &lim_data) != 0 ){
        LOGW("RLIMIT_DATA error %d", errno);

    }

    LOGW("Rlimit data cur=%ld, max=%ld", lim_data.rlim_cur, lim_data.rlim_max);



    for( i=0; i < 12350; i++){
        LOGW("About to malloc() 1Mb loop-count %d", i);
        p = malloc(1048560);
        if( p == NULL ){
            LOGW("errno = %d", errno);
            LOGW("malloc failed at 650 + %d",i );


        }
        memset(p,0,1048560);
        LOGW("malloc loop: %d 'th Mb - sleeping for 0.02s", i);
        usleep(20);
    }

    LOGW("after malloc");

    LOGI("calculation time: %" PRIu64, ticks);

//    p = malloc(32);

    return env->NewStringUTF("Hello from malloc() buster");
}
