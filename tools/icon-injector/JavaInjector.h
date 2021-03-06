/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _JAVA_INJECTOR_H_
#define _JAVA_INJECTOR_H_

#include "Injector.h"
#include <string>

namespace MoSync {
	
/**
 * The java injector. Used to inject an icon into a J2ME jar-file.
 */
class JavaInjector : public Injector {
public:
	void inject(const Icon* icon, const std::map<std::string, std::string>& params);
};

}

#endif // _JAVA_INJECTOR_H_
