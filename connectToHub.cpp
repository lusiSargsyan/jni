#include <iostream>
#include "jni.h"
using namespace std;

int main(){
    JavaVM *jvm;
    JNIEnv *env;       
    JavaVMInitArgs vm_args;  
    JavaVMOption* options = new JavaVMOption[1];
    options[0].optionString = "-Djava.class.path=JAR_FILE_LOCATION/testJni.jar";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    /* load and initialize a Java VM, return a JNI interface
    * pointer in env */
    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    delete options;
    jclass cls = env->FindClass( "TestJni");
      if(cls !=0)                                                          
     { 
         jstring jstr = (env)->NewStringUTF("https://www.github.com/");
         jmethodID mid = env->GetStaticMethodID(cls, "getResult", "(Ljava/lang/String;)Ljava/lang/String;");
        if(mid != 0){
  	   jstring rv = (jstring) env->CallStaticObjectMethod(cls, mid,jstr);
           const char *strReturn = env->GetStringUTFChars( rv, 0);
           cout << strReturn;

  	 }else{
	   printf("method not found" );
	 }
     }else{
         printf("class not found" );
     }
/* We are done. */
    jvm->DestroyJavaVM();
}
