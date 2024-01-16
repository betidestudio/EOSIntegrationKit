// Copyright (c) 2018 Nineva Studios

#include "AndroidJNICallUtils.h"
#include "AndroidJNIConvertor.h"

#if PLATFORM_ANDROID
FString AndroidJNICallUtils::CallStaticStringMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallStaticStringMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = FAndroidApplication::FindJavaClass(ClassName);

	jmethodID Method = FJavaWrapper::FindStaticMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	jstring Return = static_cast<jstring>(Env->CallStaticObjectMethodV(Class, Method, Args));
	va_end(Args);

	const char* UTFString = Env->GetStringUTFChars(Return, nullptr);
	FString Result(UTF8_TO_TCHAR(UTFString));
	Env->ReleaseStringUTFChars(Return, UTFString);

	Env->DeleteLocalRef(Class);

	return Result;
}

bool AndroidJNICallUtils::CallStaticBoolMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallStaticBoolMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = FAndroidApplication::FindJavaClass(ClassName);

	jmethodID Method = FJavaWrapper::FindStaticMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	bool Result = Env->CallStaticBooleanMethodV(Class, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);

	return Result;
}

int AndroidJNICallUtils::CallStaticIntMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallStaticIntMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = FAndroidApplication::FindJavaClass(ClassName);

	jmethodID Method = FJavaWrapper::FindStaticMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	int Result = Env->CallStaticIntMethodV(Class, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);

	return Result;
}

long AndroidJNICallUtils::CallStaticLongMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallStaticIntMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = FAndroidApplication::FindJavaClass(ClassName);

	jmethodID Method = FJavaWrapper::FindStaticMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	long Result = Env->CallStaticLongMethodV(Class, Method, Args);
	va_end(Args);



	Env->DeleteLocalRef(Class);

	return Result;
}

jobject AndroidJNICallUtils::CallStaticObjectMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallStaticObjectMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = FAndroidApplication::FindJavaClass(ClassName);

	jmethodID Method = FJavaWrapper::FindStaticMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	jobject Result = Env->CallStaticObjectMethodV(Class, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);

	return Result;
}

void AndroidJNICallUtils::CallStaticVoidMethod(const ANSICHAR* ClassName, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallStaticVoidMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	//UE_LOG(LogTemp, Warning, TEXT("JavaEnvironmentSet"));

	jclass Class = FAndroidApplication::FindJavaClass(ClassName);
	//UE_LOG(LogTemp, Warning, TEXT("ClassName Set"));

	jmethodID Method = FJavaWrapper::FindStaticMethod(Env, Class, MethodName, MethodSignature, bIsOptional);
	//UE_LOG(LogTemp, Warning, TEXT("methodIDSet"));

	va_list Args;
	va_start(Args, MethodSignature);
	Env->CallStaticVoidMethodV(Class, Method, Args);
	//UE_LOG(LogTemp, Warning, TEXT("AfterArgument"));
	va_end(Args);

	Env->DeleteLocalRef(Class);
	//UE_LOG(LogTemp, Warning, TEXT("DeleteRef"));
}

FString AndroidJNICallUtils::CallStringMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallStringMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = Env->GetObjectClass(object);

	jmethodID Method = FJavaWrapper::FindMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	jstring Return = static_cast<jstring>(Env->CallObjectMethodV(object, Method, Args));
	va_end(Args);

	const char* UTFString = Env->GetStringUTFChars(Return, nullptr);
	FString Result(UTF8_TO_TCHAR(UTFString));
	Env->ReleaseStringUTFChars(Return, UTFString);

	Env->DeleteLocalRef(Class);

	return Result;
}

bool AndroidJNICallUtils::CallBoolMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallBoolMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = Env->GetObjectClass(object);

	jmethodID Method = FJavaWrapper::FindMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	bool Result = Env->CallBooleanMethodV(object, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);

	return Result;
}

int AndroidJNICallUtils::CallIntMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallIntMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = Env->GetObjectClass(object);

	jmethodID Method = FJavaWrapper::FindMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	int Result = Env->CallIntMethodV(object, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);

	return Result;
}
jobject AndroidJNICallUtils::CallObjectMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallObjectMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = Env->GetObjectClass(object);

	jmethodID Method = FJavaWrapper::FindMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	jobject Result = Env->CallObjectMethodV(object, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);

	return Result;
}

void AndroidJNICallUtils::CallVoidMethod(const ANSICHAR* className, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallVoidMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	//UE_LOG(LogTemp, Warning, TEXT("JavaEnvironmentSet"));

	jclass Class = FAndroidApplication::FindJavaClass(className);
	//UE_LOG(LogTemp, Warning, TEXT("ClassName Set"));

	jmethodID Method = FJavaWrapper::FindMethod(Env, Class, MethodName, MethodSignature, bIsOptional);
	//UE_LOG(LogTemp, Warning, TEXT("methodIDSet"));

	va_list Args;
	va_start(Args, MethodSignature);
	jobject object = Env->NewObject(Class, Method, Args);
	//UE_LOG(LogTemp, Warning, TEXT("AfterArgument"));
	//va_end(Args);

	//va_list Args;
	//va_start(Args, MethodSignature);
	Env->CallVoidMethodV(object, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);
	//UE_LOG(LogTemp, Warning, TEXT("DeleteRef"));
}

void AndroidJNICallUtils::CallGameActivityVoidMethod(const ANSICHAR* className, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	//UE_LOG(LogTemp, Warning, TEXT("ZOOMPLUGIN => Method call CallVoidMethod [%s]"), *FString(MethodName));

	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	//UE_LOG(LogTemp, Warning, TEXT("JavaEnvironmentSet"));

	jclass Class = FAndroidApplication::FindJavaClass(className);
	//UE_LOG(LogTemp, Warning, TEXT("ClassName Set"));

	jmethodID Method = FJavaWrapper::FindMethod(Env, Class, MethodName, MethodSignature, bIsOptional);
	//UE_LOG(LogTemp, Warning, TEXT("methodIDSet"));

	va_list Args;
	va_start(Args, MethodSignature);
	//UE_LOG(LogTemp, Warning, TEXT("AfterArgument"));

	Env->CallVoidMethodV(FJavaWrapper::GameActivityThis, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);
	//UE_LOG(LogTemp, Warning, TEXT("DeleteRef"));
}

long AndroidJNICallUtils::CallLongMethod(jobject object, const ANSICHAR* MethodName, const ANSICHAR* MethodSignature, ...)
{
	bool bIsOptional = false;

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();

	jclass Class = Env->GetObjectClass(object);

	jmethodID Method = FJavaWrapper::FindMethod(Env, Class, MethodName, MethodSignature, bIsOptional);

	va_list Args;
	va_start(Args, MethodSignature);
	long Result = Env->CallLongMethodV(object, Method, Args);
	va_end(Args);

	Env->DeleteLocalRef(Class);

	return Result;
}
#endif // PLATFORM_ANDROID
