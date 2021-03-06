//
//  FlurryX_android.cpp
//
//  Created by Elviss Strazdins (based on FlurryAnalticsX by Dmitry Matyukhin)
//

#include "FlurryX.h"

#include <platform/android/jni/JniHelper.h>
#include <android/log.h>
#include <jni.h>

using namespace cocos2d;

static const char* const LOG_TAG = "FlurryXJni";

static bool initialized = false;
static JniMethodInfo setAppVersionMethodInfo;
static JniMethodInfo setUseHttpsMethodInfo;
static JniMethodInfo logEvent1MethodInfo;
static JniMethodInfo logEvent3MethodInfo;
static JniMethodInfo logEvent4MethodInfo;
static JniMethodInfo logEvent5MethodInfo;
static JniMethodInfo endTimedEvent1MethodInfo;
static JniMethodInfo endTimedEvent2MethodInfo;
static JniMethodInfo onErrorMethodInfo;
static JniMethodInfo setUserIdMethodInfo;

static JniMethodInfo setLogEnabledMethodInfo;
static JniMethodInfo setLogLevelMethodInfo;
static JniMethodInfo setLogEventsMethodInfo;

static JniMethodInfo setAgeMethodInfo;
static JniMethodInfo setGenderMethodInfo;
static JniMethodInfo setReportLocationMethodInfo;
static JniMethodInfo setContinueSessionMillis;
static JniMethodInfo resetMethodInfo;
static jclass hashMapClass;
static jmethodID hashMapInitMethod;
static jmethodID hashMapPutMethod;

static bool init()
{
	bool result = true;
	
	if (!initialized)
	{
		if (!JniHelper::getStaticMethodInfo(setAppVersionMethodInfo, "com/flurry/android/FlurryAgent", "setVersionName", "(Ljava/lang/String;)V") ||
			!JniHelper::getStaticMethodInfo(setUseHttpsMethodInfo, "com/flurry/android/FlurryAgent", "setUseHttps", "(Z)V") ||
			!JniHelper::getStaticMethodInfo(logEvent1MethodInfo, "com/flurry/android/FlurryAgent", "logEvent", "(Ljava/lang/String;)V") ||
			!JniHelper::getStaticMethodInfo(logEvent3MethodInfo, "com/flurry/android/FlurryAgent", "logEvent", "(Ljava/lang/String;Ljava/util/Map;)V") ||
			!JniHelper::getStaticMethodInfo(logEvent4MethodInfo, "com/flurry/android/FlurryAgent", "logEvent", "(Ljava/lang/String;Z)V") ||
			!JniHelper::getStaticMethodInfo(logEvent5MethodInfo, "com/flurry/android/FlurryAgent", "logEvent", "(Ljava/lang/String;Ljava/util/Map;Z)V") ||
			!JniHelper::getStaticMethodInfo(onErrorMethodInfo, "com/flurry/android/FlurryAgent", "onError", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Throwable;)V") ||
			!JniHelper::getStaticMethodInfo(setUserIdMethodInfo, "com/flurry/android/FlurryAgent", "setUserId", "(Ljava/lang/String;)V") ||
			
			!JniHelper::getStaticMethodInfo(setLogEnabledMethodInfo, "com/flurry/android/FlurryAgent", "setLogEnabled", "(Z)V") ||
			!JniHelper::getStaticMethodInfo(setLogLevelMethodInfo, "com/flurry/android/FlurryAgent", "setLogLevel", "(I)V") ||
			!JniHelper::getStaticMethodInfo(setLogEventsMethodInfo, "com/flurry/android/FlurryAgent", "setLogEvents", "(Z)V") ||
			
			!JniHelper::getStaticMethodInfo(setAgeMethodInfo, "com/flurry/android/FlurryAgent", "setAge", "(I)V") ||
			!JniHelper::getStaticMethodInfo(setGenderMethodInfo, "com/flurry/android/FlurryAgent", "setGender", "(B)V") ||
			!JniHelper::getStaticMethodInfo(setReportLocationMethodInfo, "com/flurry/android/FlurryAgent", "setReportLocation", "(Z)V") ||
			!JniHelper::getStaticMethodInfo(setContinueSessionMillis, "com/flurry/android/FlurryAgent", "setContinueSessionMillis", "(J)V") ||
			!JniHelper::getStaticMethodInfo(endTimedEvent1MethodInfo, "com/flurry/android/FlurryAgent", "endTimedEvent", "(Ljava/lang/String)V") ||
			!JniHelper::getStaticMethodInfo(endTimedEvent2MethodInfo, "com/flurry/android/FlurryAgent", "endTimedEvent", "(Ljava/lang/String;Ljava/util/Map)V") ||
			!JniHelper::getStaticMethodInfo(resetMethodInfo, "com/flurry/android/FlurryAgent", "reset", "()V"))
		{
			result = false;
		}
		
		hashMapClass = logEvent3MethodInfo.env->FindClass("java/util/HashMap");
		hashMapInitMethod = logEvent3MethodInfo.env->GetMethodID(hashMapClass, "<init>", "()V");
		hashMapPutMethod = logEvent3MethodInfo.env->GetMethodID(hashMapClass, "put",
																"(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		
		if (!hashMapClass || !hashMapInitMethod || !hashMapPutMethod)
		{
			result = false;
		}
		
		initialized = true;
	}
	
	return result;
}

void jsetAppVersion(const char *version)
{
	jstring stringArg = setAppVersionMethodInfo.env->NewStringUTF(version);
	setAppVersionMethodInfo.env->CallStaticVoidMethod(setAppVersionMethodInfo.classID, setAppVersionMethodInfo.methodID, stringArg);
	setAppVersionMethodInfo.env->DeleteLocalRef(stringArg);
}

const char* jgetFlurryAgentVersion()
{
	return NULL;
}

void jsetLogEnabled(bool value)
{
	setLogEnabledMethodInfo.env->CallStaticVoidMethod(setLogEnabledMethodInfo.classID, setLogEnabledMethodInfo.methodID, value);
}

void jsetLogLevel(int level)
{
	setLogLevelMethodInfo.env->CallStaticVoidMethod(setLogLevelMethodInfo.classID, setLogLevelMethodInfo.methodID, level);
}

void jsetContinueSessionMillis(long millis)
{
	setContinueSessionMillis.env->CallStaticVoidMethod(setContinueSessionMillis.classID, setContinueSessionMillis.methodID, millis);
}

void jsetSecureTransportEnabled(bool value)
{
	setUseHttpsMethodInfo.env->CallStaticVoidMethod(setUseHttpsMethodInfo.classID, setUseHttpsMethodInfo.methodID, value);
}

/*
 start session, attempt to send saved sessions to server
 */
//	void jstartSession(const char *apiKey)
//  {
//		better to start session in java code in onStart and onStop
//	}

/*
 log events or errors after session has started
 */
void jlogEvent1(const char* eventName)
{
	jstring stringArg = logEvent1MethodInfo.env->NewStringUTF(eventName);
	logEvent1MethodInfo.env->CallStaticVoidMethod(logEvent1MethodInfo.classID, logEvent1MethodInfo.methodID, stringArg);
	logEvent1MethodInfo.env->DeleteLocalRef(stringArg);
}

void jlogEvent3(const char* eventName, CCDictionary* parameters)
{	
	jobject hashMap = logEvent3MethodInfo.env->NewObject(hashMapClass, hashMapInitMethod);
	
	CCArray* keys = parameters->allKeys();
	
	CCObject* object;
	
	CCARRAY_FOREACH(keys, object)
	{
		CCString* key = (CCString*)object;
		
		CCString* value = (CCString*)parameters->objectForKey(key->getCString());
		
		jstring stringKey = logEvent3MethodInfo.env->NewStringUTF(key->getCString());
		jstring stringValue = logEvent3MethodInfo.env->NewStringUTF(value->getCString());
		
		logEvent3MethodInfo.env->CallObjectMethod(hashMap, hashMapPutMethod, stringKey, stringValue);
		
		logEvent3MethodInfo.env->DeleteLocalRef(stringKey);
		logEvent3MethodInfo.env->DeleteLocalRef(stringValue);
	}
	
	
	logEvent3MethodInfo.env->CallStaticVoidMethod(logEvent3MethodInfo.classID, logEvent3MethodInfo.methodID, hashMap);
	logEvent3MethodInfo.env->DeleteLocalRef(hashMap);
}

void jlogError(const char* errorID, const char* message)
{	
	jstring stringError = onErrorMethodInfo.env->NewStringUTF(errorID);
	jstring stringMessage = onErrorMethodInfo.env->NewStringUTF(message);
	jstring stringClass = onErrorMethodInfo.env->NewStringUTF("JNI");
	onErrorMethodInfo.env->CallStaticVoidMethod(onErrorMethodInfo.classID, onErrorMethodInfo.methodID, stringError, stringMessage, stringClass);
	onErrorMethodInfo.env->DeleteLocalRef(stringError);
	onErrorMethodInfo.env->DeleteLocalRef(stringMessage);
	onErrorMethodInfo.env->DeleteLocalRef(stringClass);
}

/*
 start or end timed events
 */
void jlogEvent4(const char* eventName, bool timed)
{
	jstring stringArg = logEvent4MethodInfo.env->NewStringUTF(eventName);
	logEvent4MethodInfo.env->CallStaticVoidMethod(logEvent4MethodInfo.classID, logEvent4MethodInfo.methodID, stringArg, timed);
	logEvent4MethodInfo.env->DeleteLocalRef(stringArg);
}

void jlogEvent5(const char* eventName, CCDictionary* parameters, bool timed)
{	
	jobject hashMap = logEvent5MethodInfo.env->NewObject(hashMapClass, hashMapInitMethod);
	
	CCArray* keys = parameters->allKeys();
	
	CCObject* object;
	
	CCARRAY_FOREACH(keys, object)
	{
		CCString* key = (CCString*)object;
		
		CCString* value = (CCString*)parameters->objectForKey(key->getCString());
		
		jstring stringKey = logEvent5MethodInfo.env->NewStringUTF(key->getCString());
		jstring stringValue = logEvent5MethodInfo.env->NewStringUTF(value->getCString());
		
		logEvent5MethodInfo.env->CallObjectMethod(hashMap, hashMapPutMethod, stringKey, stringValue);
		
		logEvent5MethodInfo.env->DeleteLocalRef(stringKey);
		logEvent5MethodInfo.env->DeleteLocalRef(stringValue);
	}
	
	logEvent5MethodInfo.env->CallStaticVoidMethod(logEvent5MethodInfo.classID, logEvent5MethodInfo.methodID, hashMap, timed);
	logEvent5MethodInfo.env->DeleteLocalRef(hashMap);
}

void jendTimedEvent(const char* eventName)
{
	jstring stringArg = endTimedEvent1MethodInfo.env->NewStringUTF(eventName);
	endTimedEvent1MethodInfo.env->CallStaticVoidMethod(endTimedEvent1MethodInfo.classID, endTimedEvent1MethodInfo.methodID, stringArg);
	endTimedEvent1MethodInfo.env->DeleteLocalRef(stringArg);
}

void jendTimedEvent(const char* eventName, CCDictionary* parameters)
{
	jstring stringArg = endTimedEvent2MethodInfo.env->NewStringUTF(eventName);
	
	jobject hashMap = endTimedEvent2MethodInfo.env->NewObject(hashMapClass, hashMapInitMethod);
	
	CCArray* keys = parameters->allKeys();
	
	CCObject* object;
	
	CCARRAY_FOREACH(keys, object)
	{
		CCString* key = (CCString*)object;
		
		CCString* value = (CCString*)parameters->objectForKey(key->getCString());
		
		jstring stringKey = endTimedEvent2MethodInfo.env->NewStringUTF(key->getCString());
		jstring stringValue = endTimedEvent2MethodInfo.env->NewStringUTF(value->getCString());
		
		endTimedEvent2MethodInfo.env->CallObjectMethod(hashMap, hashMapPutMethod, stringKey, stringValue);
		
		endTimedEvent2MethodInfo.env->DeleteLocalRef(stringKey);
		endTimedEvent2MethodInfo.env->DeleteLocalRef(stringValue);
	}
	
	endTimedEvent2MethodInfo.env->CallStaticVoidMethod(endTimedEvent2MethodInfo.classID, endTimedEvent2MethodInfo.methodID, stringArg, hashMap);
	endTimedEvent2MethodInfo.env->DeleteLocalRef(stringArg);
	endTimedEvent2MethodInfo.env->DeleteLocalRef(hashMap);
}

/*
 set user info
 */
void jsetUserID(const char* userID)
{
	jstring stringArg = setUserIdMethodInfo.env->NewStringUTF(userID);
	setUserIdMethodInfo.env->CallStaticVoidMethod(setUserIdMethodInfo.classID, setUserIdMethodInfo.methodID, stringArg);
	setUserIdMethodInfo.env->DeleteLocalRef(stringArg);
}

void jsetAge(int age)
{
	setAgeMethodInfo.env->CallStaticVoidMethod(setAgeMethodInfo.classID, setAgeMethodInfo.methodID, age);
}

void jsetGender(char gender)
{
	setGenderMethodInfo.env->CallStaticVoidMethod(setGenderMethodInfo.classID, setGenderMethodInfo.methodID, gender);
}

void jsetEventLogLevel(int level)
{
	setLogLevelMethodInfo.env->CallStaticVoidMethod(setLogLevelMethodInfo.classID, setLogLevelMethodInfo.methodID, level);
}

void jsetEventLoggingEnabled(bool value)
{
	setLogEventsMethodInfo.env->CallStaticVoidMethod(setLogEventsMethodInfo.classID, setLogEventsMethodInfo.methodID, value);
}

void jreset()
{
	resetMethodInfo.env->CallStaticVoidMethod(resetMethodInfo.classID, resetMethodInfo.methodID);
}

void jsetReportLocation(bool reportLocation)
{
	setReportLocationMethodInfo.env->CallStaticVoidMethod(setReportLocationMethodInfo.classID, setReportLocationMethodInfo.methodID, reportLocation);
}

void FlurryX::setAppVersion(const char *version)
{
	init();
	jsetAppVersion(version);
}

const char* FlurryX::getFlurryAgentVersion()
{
	init();
	return jgetFlurryAgentVersion();
}

void FlurryX::setShowErrorInLogEnabled(bool value)
{
	init();
	jsetLogLevel(value?6:0); // Log.ERROR
}

void FlurryX::setDebugLogEnabled(bool value)
{
	init();
	jsetLogEnabled( value );
}

void FlurryX::setSessionContinueMillis(long millis)
{
	init();
	jsetContinueSessionMillis(millis);
}

void FlurryX::setSecureTransportEnabled(bool value)
{
	init();
	jsetSecureTransportEnabled( value );
}

/*
 start session, attempt to send saved sessions to server 
 */
void FlurryX::startSession(const char* apiKey)
{
	// FIX: Better start session in java
//	jstartSession(apiKey);
}

/*
 log events or errors after session has started
 */
void FlurryX::logEvent(const char* eventName)
{
	init();
	jlogEvent1(eventName);
}

void FlurryX::logEvent(const char* eventName, const char* paramName, const char* paramValue)
{
	init();
    CCDictionary* params = new CCDictionary();
    CCString* value = new CCString( paramValue );
    std::string strKey = paramName;
    params->setObject(value, strKey);

    logEvent( eventName, params );
    
    CC_SAFE_RELEASE_NULL( value );
    CC_SAFE_RELEASE_NULL( params );
}

void FlurryX::logEvent(const char* eventName, CCDictionary* parameters)
{
	init();
	jlogEvent3(eventName, parameters);
}

void FlurryX::logError(const char* errorID, const char *message)
{
	init();
	jlogError(errorID, message);
}

/* 
 start or end timed events
 */
void FlurryX::logEvent(const char* eventName, bool timed)
{
	init();
	jlogEvent4(eventName, timed);
}

void FlurryX::logEvent(const char* eventName, CCDictionary* parameters, bool timed)
{
	init();
	jlogEvent5(eventName, parameters, timed);
}

void FlurryX::endTimedEvent(const char* eventName)
{
	init();
	jendTimedEvent(eventName);
}

void FlurryX::endTimedEvent(const char* eventName, CCDictionary* parameters)
{
	init();
	jendTimedEvent(eventName, parameters);
}

/*
 set user info
 */
void FlurryX::setUserID(const char* userID)
{
	init();
	jsetUserID(userID);
}

void FlurryX::setAge(int age)
{
	init();
	jsetAge(age);
}

void FlurryX::setGender(Gender gender)
{
	init();
	jsetGender(gender);
}

/*
 set location information
 */
void FlurryX::setLatitude(double latitude, double longitude, float horizontalAccuracy, float verticalAccuracy)
{

}

void FlurryX::setReportLocation(bool reportLocation)
{
	init();
	jsetReportLocation(reportLocation);
}

/*
 optional session settings that can be changed after start session
 */
void FlurryX::setSessionReportsOnCloseEnabled(bool sendSessionReportsOnClose)
{
	
}

void FlurryX::setSessionReportsOnPauseEnabled(bool setSessionReportsOnPauseEnabled)
{
	
}

void FlurryX::setEventLoggingEnabled(bool value)
{
	init();
	jsetEventLoggingEnabled(value);
}

void FlurryX::reset()
{
	init();
	jreset();
}
