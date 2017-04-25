//
// Created by dimitrije on 4/24/17.
//

#ifndef HOBBITREWRITE_JAVA_DOUBLE_PARSER_H
#define HOBBITREWRITE_JAVA_DOUBLE_PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <jni.h>

class java_double_parser {

private:
    JavaVM *jvm;
    JNIEnv *env;
    JavaVMInitArgs vm_args;
    JavaVMOption options[1];

    jint res;
    jclass cls;
    jmethodID mid;

    jstring r;

public:

    java_double_parser();
    ~java_double_parser();

    const char *toString(double d);

};


#endif //HOBBITREWRITE_JAVA_DOUBLE_PARSER_H
