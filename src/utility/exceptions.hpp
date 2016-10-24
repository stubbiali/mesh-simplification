/*
 * File:   exceptions.hpp
 * Author: bonelli
 *
 * Created on October 14, 2013, 11:44 AM
 */

#ifndef EXCEPTIONS_HPP
#define	EXCEPTIONS_HPP

#ifdef	__cplusplus
extern "C" {
#endif

    namespace std {

        class StringException : public exception {
        private:
            const string message;
        public:

            StringException(const string& message) throw () : message(message) {
            };

            virtual ~StringException() throw () {
            };

            virtual const char * what() throw () {
                return message.c_str();
            }
        };
    }


#ifdef	__cplusplus
}
#endif

#endif	/* EXCEPTIONS_HPP */

