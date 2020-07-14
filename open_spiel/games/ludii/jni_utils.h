// Copyright 2019 DeepMind Technologies Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_SPIEL_GAMES_LUDII_JNIUTILS_H_
#define OPEN_SPIEL_GAMES_LUDII_JNIUTILS_H_

#include <cstring>
#include <string>

#include "jni.h"  // NOLINT

namespace open_spiel {
namespace ludii {

/**
 * Implementation of this file partially based on:
 * 	https://gist.github.com/alexminnaar/90cf1ea3de45e79a1b14081d90d214b7
 * 	https://github.com/facebookincubator/Polygames/blob/master/games/ludii/jni_utils.h
 */
class JNIUtils {
 public:
  static JNIEnv *GetEnv();

  static void InitJVM(std::string jar_location);
  static void CloseJVM();

  static jclass LudiiGameWrapperClass();
  static jclass LudiiStateWrapperClass();

  /**
   * @return A string description of the version of Ludii that we're working with.
   */
  static const std::string LudiiVersion();

 private:
  static JavaVM *jvm;
  static JNIEnv *env;
  static jint res;

  /** Our LudiiGameWrapper class in Java */
  static jclass ludiiGameWrapperClass;

  /** Our LudiiStateWrapper class in Java */
  static jclass ludiiStateWrapperClass;

  /** Method ID for the ludiiVersion() method in Java */
  static jmethodID ludiiVersionMethodID;
};

}  // namespace ludii
}  // namespace open_spiel

#endif  // OPEN_SPIEL_GAMES_LUDII_JNIUTILS_H_
