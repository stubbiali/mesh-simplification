#ifndef OPTIONPARSER_HPP
#define OPTIONPARSER_HPP

#include <vector>
#include <iterator>
#include <boost/program_options/errors.hpp>
#include <boost/program_options.hpp>

/*! By default boost allows passing arrays of values by declaring the option type as an std::vector and repeating the same option multiple times:
 * --myoption=2 --myoption=3 --myoption=5
 * We can instead use the struct VectorStruct to pass all the values to the same option:
 * --myoption=2 3 5
 */
template <class T>
struct VectorStruct
{
    VectorStruct() {};
    VectorStruct(std::vector<T> vec) : stdVector(vec) {};
    std::vector<T> stdVector;
};

//! Overloading of boost's validate function used by boost::program_options that handles vectors
template <class T>
void validate(boost::any& v,
    const std::vector<std::string>& values,
    VectorStruct<T>*, int) {
    VectorStruct<T> dvalues;
    for(std::vector<std::string>::const_iterator it = values.begin();
        it != values.end();
        ++it)
    {
        std::stringstream ss(*it);
        std::copy(std::istream_iterator<T>(ss), std::istream_iterator<T>(),
            back_inserter(dvalues.stdVector));
        if(!ss.eof()) {
            throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value, "Error parsing vectorial option");
        }
    }
    v = dvalues;
}


#endif
