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

#ifndef _CPP_BACKEND_H_
#define _CPP_BACKEND_H_

#include "Backend.h"
#include <string>

class CPPBackend : public Backend {
protected:

	// a bit messy this function...
	void emit(const BasesMap& bases, std::fstream& stream);
};

#endif // _CPP_BACKEND_H_