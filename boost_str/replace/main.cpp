#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

// uppercase formatter
/*
    Convert an input to upper case.
    Note, that this formatter can be used only on std::string inputs.
*/
inline string upcase_formatter(const iterator_range<string::const_iterator> &Replace)
{
    string Temp(Replace.begin(), Replace.end());
    to_upper(Temp);
    return Temp;
}

int main()
{
    // 以下函数是模板函数, 返回的类型是第一个参数字符串的类型, 第二个参数是范围, (第三个参数可以第一个参数类型不一样)
    // erase_range_copy: 删除一定范围的字符串, 返回一个修改后的字符串copy
    // replace_range_copy : 替换一定范围的字符串, 返回一个修改后的字符串copy
    // replace_first_copy: 替换第一个出现的字符串, 返回一个修改后的字符串copy
    // replace_all_copy: 全部替换一组字符串, 返回一个修:q改后的字符串copy
    // erase_all_copy: 全部删除一组字符串, 返回一个修改后的字符串copy
    // replace_nth: 替换第Nth个出现的字符Search, 替换为Format
    // find_format_all_copy: 通过Finder查找, 然后通过format替换, 返回一个修改后的字符串copy

    cout << "* Replace Example *" << endl << endl;

    string str1("abc___cde___efg");

    // Erase 6-9th characters from the string(删除第6到9的字符, 超出范围是会core的)
    cout << "str1 without 6th to 9th character:"
    << erase_range_copy(str1, make_iterator_range(str1.begin() + 6, str1.begin() + 9)) << endl;

    // Replace 6-9th character with '+++'
    cout << "str1 with 6th to 9th character replaced with '+++': "
    << replace_range_copy(str1, make_iterator_range(str1.begin() + 6, str1.begin() + 9), "+++") << endl;

    cout << "str1 with 'cde' replaced with 'XYZ': ";
    // Replace first 'cde' with 'XYZ'. Modify the input(替换完才会打印cout内容)
    replace_first_copy(ostream_iterator<char>(cout), str1, "cde", "XYZ");
    cout << endl;

    // Replace all '___'
    cout << "str1 with all '___' replaced with '---': " << replace_all_copy(str1, "___", "---") << endl;

    // Erase all '___'
    cout << "str1 without all '___': " << erase_all_copy(str1, "___") << endl;

    // replace third and 5th occurrence of _ in str1
    // note that nth argument is 0-based
    replace_nth(str1, "_", 4, "+");
    replace_nth(str1, "_", 2, "+");

    cout << "str1 with third and 5th occurrence of _ replace: " << str1 << endl;

    // Custom formatter examples
    string str2("abC-xxxx-AbC-xxxx-abc");

    // Find string 'abc' ignoring the case and convert it to upper case
    cout << "Upcase all 'abc'(s) in the str2: " <<

    find_format_all_copy(str2, first_finder("abc", is_iequal()), upcase_formatter);

    cout << endl;

    return 0;
}
