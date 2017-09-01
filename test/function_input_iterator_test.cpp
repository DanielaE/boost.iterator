// Copyright 2010 (c) Dean Michael Berris
// Distributed under the Boost Software License Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

#include <boost/config.hpp>
#include <boost/iterator/function_input_iterator.hpp>

namespace {

struct ones {
    typedef int result_type;
    result_type operator() () {
        return 1;
    }
};

int ones_function () {
    return 1;
}

struct counter {
    typedef int result_type;
    int n;
    explicit counter(int n_) : n(n_) { }
    result_type operator() () {
        return n++;
    }
};

} // namespace

using namespace std;

int main()
{
    // test the iterator with function objects
    ones ones_generator;
    vector<int> values(10);
    generate(values.begin(), values.end(), ones());
    
    vector<int> generated;
    copy(
        boost::make_function_input_iterator(ones_generator, 0),
        boost::make_function_input_iterator(ones_generator, 10),
        back_inserter(generated)
        );

    assert(values.size() == generated.size());
    assert(equal(values.begin(), values.end(), generated.begin()));
    cout << "function iterator test with function objects successful." << endl;

    // test the iterator with normal functions
    vector<int>().swap(generated);
    copy(
        boost::make_function_input_iterator(&ones_function, 0),
        boost::make_function_input_iterator(&ones_function, 10),
        back_inserter(generated)
        );

    assert(values.size() == generated.size());
    assert(equal(values.begin(), values.end(), generated.begin()));
    cout << "function iterator test with pointer to function successful." << endl;

    // test the iterator with a reference to a function
    vector<int>().swap(generated);
    copy(
        boost::make_function_input_iterator(ones_function, 0),
        boost::make_function_input_iterator(ones_function, 10),
        back_inserter(generated)
        );

    assert(values.size() == generated.size());
    assert(equal(values.begin(), values.end(), generated.begin()));
    cout << "function iterator test with reference to function successful." << endl;

    // test the iterator with a stateful function object
    counter counter_generator(42);
    vector<int>().swap(generated);
    copy(
        boost::make_function_input_iterator(counter_generator, 0),
        boost::make_function_input_iterator(counter_generator, 10),
        back_inserter(generated)
        );

    assert(generated.size() == 10);
    assert(counter_generator.n == 42 + 10);
    for(std::size_t i = 0; i != 10; ++i)
        assert(generated[i] == static_cast<int>(42 + i));
    cout << "function iterator test with stateful function object successful." << endl;

#if !defined(BOOST_NO_CXX11_LAMBDAS) && !defined(BOOST_NO_CXX11_AUTO_DECLARATIONS)
    // test the iterator with lambda expressions
    int num = 42;
    auto lambda_generator = [&num] { return num++; };
    vector<int>().swap(generated);
    copy(
        boost::make_function_input_iterator(lambda_generator, 0),
        boost::make_function_input_iterator(lambda_generator, 10),
        back_inserter(generated)
        );

    assert(generated.size() == 10);
    for(std::size_t i = 0; i != 10; ++i)
        assert(generated[i] == static_cast<int>(42 + i));
    cout << "function iterator test with lambda expressions successful." << endl;
#endif // BOOST_NO_CXX11_LAMBDAS

    return 0;
}
