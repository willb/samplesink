/*
 * samplesink.hpp:  simple generic sample sink for aggregating many 
 * sample values and maintaining updated mean and variance estimates 
 * in constant space.
 * 
 * Copyright © 2008 William C. Benton 
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

#include <cstdlib>
#include <cmath>
#include <climits>
#include <cassert>

namespace support {	
	template <typename T>
	class math {
	public:
		static T sqrt(T val) { return std::sqrt(static_cast<long double>(val)); }
	};

	template <>
	class math<double> {
	public:
		static double sqrt(double val) { return std::sqrt(val); }
	};

	template <>
	class math<float> {
	public:
		static float sqrt(float val) { return std::sqrt(val); }
	};
}

namespace meta {
	template <typename T>
	struct limits {
		static const T min = static_cast<T>(0);
		static const T max = static_cast<T>(0);
		static const bool valid = false;
	};

	template <>
	struct limits<int> {
		static const int min=INT_MIN;
		static const int max=INT_MAX;
		static const bool valid = true;
	};

	template <>
	struct limits<unsigned int> {
		static const unsigned int min=0;
		static const unsigned int max=UINT_MAX;
		static const bool valid = true;
	};

	template <>
	struct limits<long> {
		static const long min=LONG_MIN;
		static const long max=LONG_MAX;
		static const bool valid = true;
	};

	template <>
	struct limits<unsigned long> {
		static const unsigned long min=0;
		static const unsigned long max=ULONG_MAX;
		static const bool valid = true;
	};

	template <>
	struct limits<long long> {
		static const long long min=LLONG_MIN;
		static const long long max=LLONG_MAX;
		static const bool valid = true;
	};

	template <>
	struct limits<unsigned long long> {
		static const unsigned long long min=0;
		static const unsigned long long max=ULLONG_MAX;
		static const bool valid = true;
	};
}

/*! 

*/
template <typename SampleType, typename AggregateType = double, typename CountType = SampleType>
class samplesink {
private:
	SampleType _min;
	SampleType _max;
	CountType _ct;
	AggregateType _var;
	AggregateType _stddev;
	AggregateType _xbar;
	AggregateType _sum_x2;
	
	void update_stats(SampleType sample);
	void update_estimates(SampleType sample);
public:
	samplesink();
	void put(SampleType sample);
	samplesink& operator<<(SampleType sample);
	SampleType min() const;
	SampleType max() const;
	CountType count() const;
	AggregateType mean() const;
	AggregateType stddev() const;
};

template<typename SampleType, typename AggregateType, typename CountType>
samplesink<SampleType, AggregateType, CountType>::samplesink() :
	_min(meta::limits<SampleType>::max), _max(meta::limits<SampleType>::min), _ct(0), _var(0), _stddev(0), _xbar(0), _sum_x2(0) {
		assert(meta::limits<SampleType>::valid);
}

template<typename SampleType, typename AggregateType, typename CountType>
inline void samplesink<SampleType, AggregateType, CountType>::put(SampleType sample) {
	update_stats(sample);
	update_estimates(sample);
}


template<typename SampleType, typename AggregateType, typename CountType>
samplesink<SampleType, AggregateType, CountType> &samplesink<SampleType, AggregateType, CountType>::operator<<(SampleType sample) {
	this->put(sample);
	return *this;
}

template<typename SampleType, typename AggregateType, typename CountType>
void samplesink<SampleType, AggregateType, CountType>::update_stats(SampleType sample) {
	if (sample < _min) { _min = sample; }
	if (sample > _max) { _max = sample; }
	_ct++;
}

template<typename SampleType, typename AggregateType, typename CountType>
void samplesink<SampleType, AggregateType, CountType>::update_estimates(SampleType sample) {
	AggregateType dev = sample - _xbar;
	_xbar = _xbar + (dev / _ct);
	_sum_x2 = _sum_x2 + (dev * (sample - _xbar));
	_var = _sum_x2 / _ct;
	_stddev = support::math<AggregateType>::sqrt(_var);
}

template<typename SampleType, typename AggregateType, typename CountType>
SampleType samplesink<SampleType, AggregateType, CountType>::min() const { return _min; }

template<typename SampleType, typename AggregateType, typename CountType>
SampleType samplesink<SampleType, AggregateType, CountType>::max() const { return _max; }

template<typename SampleType, typename AggregateType, typename CountType>
CountType samplesink<SampleType, AggregateType, CountType>::count() const { return _ct; }

template<typename SampleType, typename AggregateType, typename CountType>
AggregateType samplesink<SampleType, AggregateType, CountType>::mean() const { return _xbar; }

template<typename SampleType, typename AggregateType, typename CountType>
AggregateType samplesink<SampleType, AggregateType, CountType>::stddev() const { return _stddev; }

