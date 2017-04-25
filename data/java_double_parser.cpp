//
// Created by dimitrije on 4/24/17.
//

#include "java_double_parser.h"

java_double_parser::java_double_parser() {
    vm_args.version = JNI_VERSION_1_8;

    /* This option doesn't do anything, just to illustrate how to pass args to JVM. */
    vm_args.nOptions = 0;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_FALSE;

    /* load and initialize a Java VM, return a JNI interface pointer in env */
    res = JNI_CreateJavaVM(&jvm,(void**)&env,&vm_args);
    if (res < 0) {
        fprintf(stderr, "Can't create Java VM\n");
        exit(1);
    }

    /* invoke the Main.test method using the JNI */
    cls = (*env).FindClass("sun/misc/FloatingDecimal");
    if (cls == 0) {
        fprintf(stderr, "Can't find FloatingDecimal class\n");
        exit(1);
    }

    mid = (*env).GetStaticMethodID(cls, "toJavaFormatString", "(D)Ljava/lang/String;");
    if (mid==0) {
        fprintf(stderr, "No such method!\n");
        exit(1);
    }

    r = nullptr;
}

const char *java_double_parser::toString(double d) {

    // remove the previous guy
    if(r != nullptr) {
        env->DeleteGlobalRef(r);
    }

    r = (jstring)env->CallStaticObjectMethod(cls, mid, d);
    return env->GetStringUTFChars(r, 0);
}

java_double_parser::~java_double_parser() {
    (*jvm).DestroyJavaVM();
}
