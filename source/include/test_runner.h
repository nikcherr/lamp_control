#pragma once
#include <iostream>
#include <string>
#include <sstream>

template<typename T, typename U>
void AssertEqual(const T& t, const U& u, const std::string& hint){
    std::ostringstream s;
    if(t != u){
        s << "Assertion is failed: " << t << " != " << u << ". Hint: " << hint << std::endl;
        throw(std::runtime_error(s.str()));
    }
}

inline void Assert(bool b, const std::string& hint){
    AssertEqual(b, true, hint);
}

class TestRunner{
public:
    template<typename TypeFunc>
    void RunTest(TypeFunc func, const std::string& func_name){
        try{
            func();
            std::cerr << func_name << " OK" << std::endl;
        }catch(std::runtime_error& e){
            ++test_fail;
            std::cerr << func_name << " " << e.what() << std::endl;
        }
    }
    ~TestRunner(){
        if(test_fail > 0){
            std::cerr << "test";
            (test_fail > 1) ? std::cerr << "s " : std::cerr << " ";
            std::cerr << "failed. Terminate." << std::endl;
            exit(-1);
        }
    }
private:
    int test_fail = 0;
};

#define RUN_TEST(tr, func){		 		\
    tr.RunTest(func, #func); 			\
}

#define ASSERT_EQUAL(x, y){				\
    std::ostringstream s;				\
    s << #x << " != " << #y << ", " 	\
    << __FILE__ << ": " << __LINE__;	\
    AssertEqual(x, y, s.str()); 		\
}

#define ASSERT(b){						\
    std::ostringstream s;				\
    s << #b << " is false, " <<			\
    __FILE__ << ": " << __LINE__;		\
    Assert(b, s.str());					\
}
