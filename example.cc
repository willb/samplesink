/*
 * example.cc:  simple example for samplesink
 * 
 * Copyright © 2003--2008 William C. Benton 
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *		 http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "samplesink.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main (int c, char *v[]) {
	/* The samplesink class is templated on three types;
	   the second two are optional.  The first is the
	   type of your samples, the second is the type you
	   want to use for aggregations (mean, standard
	   deviation), and the third is the type you want to
	   use for counts.  The second defaults to double
	   and the third to long.  */

	samplesink<int> ss;

	for(int i = 0 ; i < 1000 ; i++) {
		int k = rand() % 21;
		// use the << operator or the put method to add samples
		ss << k;  // same as ss.put(k)
		cerr << "ss << " << k << "; count == " << ss.count() << "; min == " << ss.min() << "; max == " << ss.max() << "; mean == " << ss.mean() << "; dev == " << ss.stddev() << endl;
	}

	return 0;
}
