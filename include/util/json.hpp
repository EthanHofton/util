// TODO: make json parcer
// #ifndef UTIL_JSON_HPP
// #define UTIL_JSON_HPP
//
// #include <any>
// #include <string> 
// #include <regex>
// #include <iterator>
//
// #include "tree.hpp"
// #include "algo.hpp"
//
// namespace util {
//
// class json {
// public:
//
//
//     // * build dom *  
//     bool parse(const std::string& t_json) {
//         return build_dom(t_json);
//     }
//
//     enum json_strucs {
//         object,
//         number,
//         string,
//         boolean,
//         array,
//         null,
//         none,
//     };
//
//     struct json_value {
//         json_strucs value_type;
//
//         // * object data *
//
//         // * array data *
//         
//         // * other value data *
//         std::any m_data; 
//     };
//
//     using dom_tree = util::tree<json_value>;
//
// private:
//
//     bool build_dom(std::string t_json) {
//         // * sanatize json string
//         // *  - remove newlines
//         // *  - remove spaces outside of strings
//         // * parse json:
//         // *  1. resolve outermost type
//         // *  2. examine type, add to dom
//         // *  3. if still unresolved, go to 1
//         // * NOTES:
//         // * regex to find object   : ^\{\}$|^\{(.*?):(.*?)\}$
//         // * regex to find array    : ^\[(.*)\]$
//         // * regex to find split csv: ^((?:[^,]+)+),?$
//         // * regex to find string   : ^\"(.*?)\"$
//         // * regex to find number   : ^([1-9]+)$
//         // * regex to find boolean  : ^true$|^false$
//         
//         // * sanatize
//         sanatize(t_json);
//
//         // * json can either be object or array
//         // * first resolve object 
//         
//         // * check if object
//         if (resolve_object(t_json, m_dom.begin())) {
//             return true;
//         }
//
//         // * check if the object is an array
//         if (resolve_array(t_json)) {
//             return true;
//         }
//
//         // * print error (fornow, TODO: make exception)
//         std::cout << "object or array expected" << std::endl;
//         return false;
//     }
//
//     void sanatize(std::string& t_json) {
//         // * remove json newlines
//         t_json.erase(std::remove(t_json.begin(), t_json.end(), '\n'), t_json.cend());
//     }
//
//     bool resolve_object(std::string& t_json, dom_tree::iterator pos) {
//         // * set the regex iterator
//         std::regex obj_regex("^\\{(.*)\\}$");
//         // * create the regex iterator
//         std::regex_iterator itr(t_json.begin(), t_json.end(), obj_regex);
//         
//         // * declare the iterator end
//         decltype(itr) end;
//
//         // * check if the regex iterator has found a match, if not, no object was found
//         if (itr == end) {
//             return false;
//         }
//
//         // * get contents of object
//         std::string contents = (*itr)[1];
//
//         
//
//
//         return true;        
//     }
//
//     bool resolve_array(std::string& t_json) {
//         // * set the regex array matcher
//         std::regex arr_regex("^\\[(.*)\\]$");
//         // * create the regex iterator
//         std::regex_iterator itr(t_json.begin(), t_json.end(), arr_regex);
//
//         // * declare the end iterator
//         decltype(itr) end;
//
//         // * check if match was found;
//         if (itr == end) {
//             return false;
//         }
//
//         // * first capture group is array contents
//         std::string contents = (*itr)[1];
//
//         std::cout << util::fmt("array contents: {}", contents) << std::endl;
//
//         return true;
//     }
//
//     bool resolve_string(std::string& t_json);
//     bool resolve_number(std::string& t_json);
//     bool resolve_boolean(std::string& t_json);
//     bool resolve_null(std::string& t_json);
//
//    dom_tree m_dom;
//
// };
//
// }
//
// #endif
