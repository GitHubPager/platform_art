// Copyright 2011 Google Inc. All Rights Reserved.

#include "class_linker.h"
#include "common_test.h"
#include "compiler.h"
#include "compiler_test.h"
#include "dex_cache.h"
#include "dex_file.h"
#include "heap.h"
#include "object.h"
#include "scoped_ptr.h"

#include <stdint.h>
#include <stdio.h>
#include "gtest/gtest.h"

namespace art {

class CompilerTest : public CommonTest {
};

TEST_F(CompilerTest, CompileLibCore) {
  Compiler compiler;
  compiler.Compile(boot_class_path_);

  // All libcore references should resolve
  const DexFile* dex = java_lang_dex_file_.get();
  DexCache* dex_cache = class_linker_->FindDexCache(*dex);
  EXPECT_EQ(dex->NumStringIds(), dex_cache->NumStrings());
  for (size_t i = 0; i < dex_cache->NumStrings(); i++) {
    String* string = dex_cache->GetResolvedString(i);
    EXPECT_TRUE(string != NULL);
  }
  EXPECT_EQ(dex->NumTypeIds(), dex_cache->NumTypes());
  for (size_t i = 0; i < dex_cache->NumTypes(); i++) {
    Class* type = dex_cache->GetResolvedType(i);
    EXPECT_TRUE(type != NULL);
  }
  EXPECT_EQ(dex->NumMethodIds(), dex_cache->NumMethods());
  for (size_t i = 0; i < dex_cache->NumMethods(); i++) {
    // TODO: ClassLinker::ResolveMethod
    // Method* method = dex_cache->GetResolvedMethod(i);
    // EXPECT_TRUE(method != NULL);
  }
  EXPECT_EQ(dex->NumFieldIds(), dex_cache->NumFields());
  for (size_t i = 0; i < dex_cache->NumFields(); i++) {
    // TODO: ClassLinker::ResolveField
    // Field* field = dex_cache->GetResolvedField(i);
    // EXPECT_TRUE(field != NULL);
  }

}


#if defined(__arm__)
TEST_F(CompilerTest, BasicCodegen) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kFibonacciDex,
                               "kFibonacciDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("Fibonacci");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "fibonacci", "(I)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 10);
  LOG(INFO) << "Fibonacci[10] is " << result;

  ASSERT_EQ(55, result);
}

TEST_F(CompilerTest, UnopTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "unopTest", "(I)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 38);
  LOG(INFO) << "unopTest(38) == " << result;

  ASSERT_EQ(37, result);
}

#if 0 // Does filled array - needs load-time class resolution
TEST_F(CompilerTest, ShiftTest1) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "shiftTest1", "()I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m);

  ASSERT_EQ(0, result);
}
#endif

TEST_F(CompilerTest, ShiftTest2) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "shiftTest2", "()I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m);

  ASSERT_EQ(0, result);
}

TEST_F(CompilerTest, UnsignedShiftTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "unsignedShiftTest", "()I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m);

  ASSERT_EQ(0, result);
}

#if 0 // Fail subtest #3, long to int conversion w/ truncation.
TEST_F(CompilerTest, ConvTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "convTest", "()I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m);

  ASSERT_EQ(0, result);
}
#endif

TEST_F(CompilerTest, CharSubTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "charSubTest", "()I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m);

  ASSERT_EQ(0, result);
}

#if 0 // Needs array allocation & r9 to be set up with Thread*
TEST_F(CompilerTest, IntOperTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "intOperTest", "(II)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 70000, -3);

  ASSERT_EQ(0, result);
}
#endif

#if 0 // Needs array allocation & r9 to be set up with Thread*
TEST_F(CompilerTest, Lit16Test) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "lit16Test", "(I)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 77777);

  ASSERT_EQ(0, result);
}
#endif

#if 0 // Needs array allocation & r9 to be set up with Thread*
TEST_F(CompilerTest, Lit8Test) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "lit8Test", "(I)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, -55555);

  ASSERT_EQ(0, result);
}
#endif

#if 0 // Needs array allocation & r9 to be set up with Thread*
TEST_F(CompilerTest, Lit8Test) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "lit8Test", "(I)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, -55555);

  ASSERT_EQ(0, result);
}
#endif

#if 0 // Needs array allocation & r9 to be set up with Thread*
TEST_F(CompilerTest, IntShiftTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "intShiftTest", "(II)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 0xff00aa01, 8);

  ASSERT_EQ(0, result);
}
#endif

#if 0 // Needs array allocation & r9 to be set up with Thread*
TEST_F(CompilerTest, LongOperTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "longOperTest", "(LL)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 70000000000L, 3);

  ASSERT_EQ(0, result);
}
#endif

#if 0 // Needs array allocation & r9 to be set up with Thread*
TEST_F(CompilerTest, LongShiftTest) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "longShiftTest", "(LL)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 0xd5aa96deff00aa01, 8);

  ASSERT_EQ(0, result);
}
#endif

TEST_F(CompilerTest, SwitchTest1) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "switchTest", "(I)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, 1);

  ASSERT_EQ(1234, result);
}

TEST_F(CompilerTest, IntCompare) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "testIntCompare", "(IIII)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, -5, 4, 4, 0);

  ASSERT_EQ(1111, result);
}

TEST_F(CompilerTest, LongCompare) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "testLongCompare", "(JJJJ)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, -5LL, -4294967287LL, 4LL, 8LL);

  ASSERT_EQ(2222, result);
}

TEST_F(CompilerTest, FloatCompare) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "testFloatCompare", "(FFFF)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, -5.0f, 4.0f, 4.0f,
                                         (1.0f/0.0f) / (1.0f/0.0f));

  ASSERT_EQ(3333, result);
}

TEST_F(CompilerTest, DoubleCompare) {
  scoped_ptr<DexFile> dex_file(OpenDexFileBase64(kIntMathDex,
                               "kIntMathDex"));
  PathClassLoader* class_loader = AllocPathClassLoader(dex_file.get());

  Thread::Current()->SetClassLoaderOverride(class_loader);

  JNIEnv* env = Thread::Current()->GetJniEnv();

  jclass c = env->FindClass("IntMath");
  ASSERT_TRUE(c != NULL);

  jmethodID m = env->GetStaticMethodID(c, "testDoubleCompare", "(DDDD)I");
  ASSERT_TRUE(m != NULL);

  jint result = env->CallStaticIntMethod(c, m, -5.0, 4.0, 4.0,
                                         (1.0/0.0) / (1.0/0.0));

  ASSERT_EQ(4444, result);
}

#endif // Arm
}  // namespace art
