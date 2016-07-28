//#define BOOST_SPIRIT_NO_REGEX_LIB

#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/home/classic/actor.hpp>

using namespace std;
using namespace boost::spirit::classic;

//struct my_enum : public grammar<my_enum>
//{
//    template <typename ScannerT>
//    struct definition
//    {
//        definition(my_enum const& self)
//        {
//            enum_specifier = enum_p >> '{' >> enum_list >> '}';
//            enum_p = str_p("enum");
//            enum_list = +id_p >> *(',' >> +id_p);
//            id_p = range_p('a','z');
//        }
//
//        rule<ScannerT> enum_specifier, enum_p, enum_list, id_p;
//        rule<ScannerT> const& start() const { return enum_specifier; }
//    };
//};
//
//string input = "enum { ah, bk  }";
//
//int main ()
//{
//    my_enum e;
//    int status = parse(input.c_str(), e, space_p).hit;
//    cout << status << endl;
//    return 0;
//}
//


int main(int argc, char* argv[])
{
    // 逗号分割的, 个数
    int count = 0;
    rule<> r1 = list_p.direct(int_p[increment_a(count)], ch_p(','));
    parse("2,3,4", r1);

    // 都好分割的, 解析到vector里
    vector<int> intVector;
    rule<> r2 = list_p.direct(int_p[push_back_a(intVector)], ch_p(','));
    parse("2,3,4", r2);


    // 查找固定字符串个数
    count = 0;
    rule<> r3 = *(str_p("Jhon") [ increment_a(count) ] | anychar_p);
    parse ("Jame, Jhon, Bake", r3);

//    boost::spirit::rule<> r3 = (str_p >> '=' >> int_p) ;
//    boost::spirit::parse_info<> info = boost::spirit::parse(("4=3"), r3);
//    if (info.hit)
//    {
//        cout << "说明匹配到了,字符串中包含一个浮点数" << endl;
//    }
//    if (info.full)
//    {
//        cout << "说明整个字符串都匹配完了。否则就说明在这个字符串的某个位置停止匹配了，因为没有合适的规则" << endl;;
//    }
//    if (NULL != info.stop)
//    {
//        printf(info.stop); //匹配没有完全的话，打印出是在哪个位置中断了。这个是字符串指针类型
//    }
//    printf("%d\n", info.length);//一共匹配了多少个字符串
    return 0;
}