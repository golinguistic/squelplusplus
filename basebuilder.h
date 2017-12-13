#include <string>
#include <map>
#include <vector>
using namespace std;
class basebuilder {
    
    struct QueryBuilderOptions{
        bool autoQuoteTableNames;
        // If true then field names will rendered inside quotes. The quote character used is configurable via the nameQuoteCharacter option.
        bool autoQuoteFieldNames;
        // If true then alias names will rendered inside quotes. The quote character used is configurable via the `tableAliasQuoteCharacter` and `fieldAliasQuoteCharacter` options.
        bool autoQuoteAliasNames;
        // If true then table alias names will rendered after AS keyword.
        bool useAsForTableAliasNames;
        // The quote character used for when quoting table and field names
        char nameQuoteCharacter;
        // The quote character used for when quoting table alias names
        char tableAliasQuoteCharacter;
        // The quote character used for when quoting table alias names
        char fieldAliasQuoteCharacter;
        // Custom value handlers where key is the value type and the value is the handler function
        map<string, void (*)(int)> valueHandlers;
        // Character used to represent a parameter value
        char parameterCharacter;
        // Numbered parameters returned from toParam() as $1, $2, etc.
        bool numberedParameters;
        // Numbered parameters prefix character(s)
        char numberedParametersPrefix;
        // Numbered parameters start at this number.
        short numberedParametersStartAt;
        // If true then replaces all single quotes within strings. The replacement string used is configurable via the `singleQuoteReplacement` option.
        bool replaceSingleQuotes;
        // The string to replace single quotes with in query strings
        string singleQuoteReplacement;
        // String used to join individual blocks in a query when it's stringified
        char separator;
        // Function for formatting string values prior to insertion into query string
        string stringFormatter;
        // Whether to prevent the addition of brackets () when nesting this query builder's output
        bool rawNesting;
    };
    
    basebuilder();
    
    std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
    
    std::vector<string> split(string str, string token){
        std::vector<string>result;
        while(str.size()){
            int index = str.find(token);
            if(index!=string::npos){
                result.push_back(str.substr(0,index));
                str = str.substr(index+token.size());
                if(str.size()==0)result.push_back(str);
            }else{
                result.push_back(str);
                str = "";
            }
        }
        return result;
    }
    
    std::string escapeValue(std::string v);
    std::string formatTableName(std::string item);
    std::string formatFieldAlias(std::string item);
    std::string formatTableAlias(std::string item);
    std::string formatFieldName(std::string item, bool ignorePeriodsForFieldNameQuotes);
    // std::string formatCustomValue(std::string item, function)
    
    
    QueryBuilderOptions defaults;
};