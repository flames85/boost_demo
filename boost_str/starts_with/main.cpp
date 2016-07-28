#include <string>
#include <iostream>
//#include <functional>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>


using namespace std;
using namespace boost;

int main() {

    cout << "* Predicate Example *" << endl << endl;
    string str1("123xxx321");
    string str2("abc");

    // Check if str1 starts with '123'
    cout << "str1 starts with \"123\": " <<
    (starts_with(str1, string("123")) ? "true" : "false") << endl;

    // Check if str1 ends with '123'
    cout << "str1 ends with \"123\": " <<
    (ends_with(str1, string("123")) ? "true" : "false") << endl;

    // Check if str1 containes 'xxx'
    cout << "str1 contains \"xxx\": " <<
    (contains(str1, string("xxx")) ? "true" : "false") << endl;


    // Check if str2 equals to 'abc'
    cout << "str2 equals \"abc\": " <<
    (equals(str2, string("abc")) ? "true" : "false") << endl;


    // is_punct: 标点
    // is_any_of: 任意顺序的字符
    // is_space: 空格

    // Classification functors and all predicate(判断是否全部是标点符号)
    if (all(";.,", is_punct())) {
        cout << "\";.,\" are all punctuation characters" << endl;
    }

    // Classification predicates can be combined(判断是否包含任意顺序的字符xabc且不包含空格)
    if (all("abcxxx", is_any_of("xabc") && !is_space())) {
        cout << "true" << endl;
    }

    cout << endl;

    return 0;
}
