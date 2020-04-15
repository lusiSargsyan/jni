Connect C++ to Selenium hub using java middleware 

Required setup : 

1)openjdk 13.0.2 2020-01-14

2)docker

3)mvn


Create docker network for selenium grid .




        docker network create grid



 Create selenium hub container


           

     docker run -d -p 4444:4444 --net grid --name selenium-hub selenium/hub:3.141.59-20200326



Create selenium firefox node container


       

     docker run -d --net grid -e HUB_HOST=selenium-hub -v /dev/shm:/dev/shm selenium/node-firefox:3.141.59-zirconium




To make sure that grid is successfully running please navigate to http://localhost:4444/grid/console in your browser. Expected result should be like this 

![Grid console](https://github.com/lusiSargsyan/jni/blob/master/Screenshot%20from%202020-04-07%2015-09-00.png)


Java part

Create a simple java/mvn project.
Add required dependencies for Junit and Selenium and mvn plugins to create jar with dependencies 

```

<dependencies>
       <!-- https://mvnrepository.com/artifact/org.seleniumhq.selenium/selenium-server -->
       <dependency>
           <groupId>org.seleniumhq.selenium</groupId>
           <artifactId>selenium-server</artifactId>
           <version>3.141.59</version>
       </dependency>
       <!-- https://mvnrepository.com/artifact/junit/junit -->
       <dependency>
           <groupId>junit</groupId>
           <artifactId>junit</artifactId>
           <version>4.1</version>
       </dependency>
   </dependencies>
```

Mvn plugins

```
<build>
    <plugins>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-compiler-plugin</artifactId>
            <version>3.8.1</version>
            <configuration>
                <source>1.8</source>
                <target>1.8</target>
            </configuration>
        </plugin>
        <plugin>
            <groupId>org.apache.maven.plugins</groupId>
            <artifactId>maven-assembly-plugin</artifactId>
            <version>2.4.1</version>
            <configuration>
                <!-- get all project dependencies -->
                <descriptorRefs>
                    <descriptorRef>jar-with-dependencies</descriptorRef>
                </descriptorRefs>
            </configuration>
            <executions>
                <execution>
                    <id>make-assembly</id>
                    <!-- bind to the packaging phase -->
                    <phase>package</phase>
                    <goals>
                        <goal>single</goal>
                    </goals>
                </execution>
            </executions>
        </plugin>
    </plugins>
</build>
```

Create a java class TestJni.java

```
import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.Keys;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.remote.DesiredCapabilities;
import org.openqa.selenium.remote.RemoteWebDriver;
import java.net.MalformedURLException;
import java.net.URL;
 
public class TestJni {
 
    private static final String HUB_URL = "http://localhost:4444/wd/hub";
 
    public static String getResult(String url){
 
        StringBuffer writer = new StringBuffer();
 
        try {
            WebDriver driver = getFirefoxDriver();
            writer.append("driver was created\n");
            driver.get(url);
            writer.append("Requested ");
            writer.append(url);
            writer.append(" page was opened");
            driver.quit();
        } catch (Throwable e) {
            writer.append("Error occurred ");
            writer.append(e.getMessage());
         }
        return writer.toString();
    }
 
    private static WebDriver getFirefoxDriver() throws MalformedURLException {
        DesiredCapabilities capability = DesiredCapabilities.firefox();
        return new RemoteWebDriver(new URL(HUB_URL), capability);
    }
 
}
```
Build a testJni.jar


```
mvn clean install
```

C++ part

Create a connectToHub.cpp

```
#include <iostream>
#include "jni.h"
using namespace std;
 
int main(){
    JavaVM *jvm;
    JNIEnv *env;      
    JavaVMInitArgs vm_args; 
    JavaVMOption* options = new JavaVMOption[1];
    options[0].optionString = "-Djava.class.path={JAR_FILE_PATH}";
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
         jstring jstr = (env)->NewStringUTF("https://github.com/");
         jmethodID mid = env->GetStaticMethodID(cls, "getResult", "(Ljava/lang/String;)Ljava/lang/String;");
        if(mid != 0){
           jstring rv = (jstring) env->CallStaticObjectMethod(cls, mid,jstr);
           const char *strReturn = env->GetStringUTFChars( rv, 0);
           cout << strReturn;
        } else {
           printf("method not found");
        }
     } else {
         printf("class not found");
     }
    /* We are done. */
    jvm->DestroyJavaVM();
}
```
Compile file giving needed libraries and dependencies.



We need to give a jvm and jni.h files location 

Compile c++
```
g++ -g -I/usr/lib/jvm/jdk-11.0.5/include -I/usr/lib/jvm/jdk-11.0.5/include/linux/ -L/usr/bin/java -L/usr/lib/jvm/jdk-11.0.5/lib/server/ createJvm.cpp -o createJvm -ljvm
```

Run executable file 


As a result we will get a message in C++ console 

Requested page https://github.com/ was opened
