#include "basebuilder.h"
#include <string>
#include <algorithm>
#include <vector>
#include <functional>

using namespace std;

basebuilder::basebuilder() {
    
    defaults.autoQuoteTableNames = false;
    defaults.autoQuoteFieldNames = false;
    defaults.autoQuoteAliasNames = true;
    defaults.useAsForTableAliasNames = false;
    defaults.nameQuoteCharacter = '`';
    defaults.tableAliasQuoteCharacter = '`';
    defaults.fieldAliasQuoteCharacter = '"';
    defaults.parameterCharacter = '?';
    defaults.numberedParameters = false;
    defaults.numberedParametersPrefix = '$';
    defaults.numberedParametersStartAt = 1;
    defaults.replaceSingleQuotes = false;
    defaults.singleQuoteReplacement = "\'\'";
    defaults.separator = ' ';
    defaults.stringFormatter = "";
    defaults.rawNesting = false;
    
}

typedef tuple<bool, bool, string> handler_tuple;
typedef function<handler_tuple(string, string, map<string,string>)> handlerfunc;


// Takes any map of value handlers and adds an entry
void registerValueHandler(map<string, handlerfunc>* valueHandlers, string type, handlerfunc handler){
    if(valueHandlers->count(type))
        return;
    
    valueHandlers->insert(pair<string, handlerfunc>(type, handler)); 
}

// Get value handler for given type
handlerfunc getValueHandler(string value, map<string, handlerfunc>* valueHandlers){
    return valueHandlers->at(value);    
}

std::string basebuilder::escapeValue(std::string v){
    if (!defaults.replaceSingleQuotes)
        return v;
    else
        return basebuilder::ReplaceAll(v,"\'", defaults.singleQuoteReplacement);
}

std::string basebuilder::formatTableName(std::string item){
    
    if(defaults.autoQuoteTableNames){
        return defaults.nameQuoteCharacter + item + defaults.nameQuoteCharacter;
    }
    
    return item;
}

std::string basebuilder::formatFieldAlias(std::string item){
    if(defaults.autoQuoteAliasNames){
        return defaults.fieldAliasQuoteCharacter + item + defaults.fieldAliasQuoteCharacter;
    }
    
    return item;
}

std::string basebuilder::formatTableAlias(std::string item){
    if(defaults.autoQuoteAliasNames){
        item = defaults.tableAliasQuoteCharacter + item + defaults.tableAliasQuoteCharacter;
    }
    
    if(defaults.useAsForTableAliasNames)
        return "AS " + item;
    else
        return item;
}
// Tuple is Formatted, rawNesting, value
tuple<bool, bool, string>  basebuilder::formatCustomValue(string value, string param, map<string,string> options){
    auto customHandler = getValueHandler(value, &defaults.valueHandlers);
    
    if(customHandler){
        handler_tuple handlervalue = customHandler(value, param, options);
    }
    
    return tuple<bool, bool, string>(false,false,value);
}

std::string basebuilder::formatFieldName(std::string item, bool ignorePeriodsForFieldNameQuotes){
    if(defaults.autoQuoteFieldNames){
        
        char quoteChar = defaults.nameQuoteCharacter;
        
        if(ignorePeriodsForFieldNameQuotes)
            item = quoteChar + item + quoteChar;
        else{
            std::vector<string> splitup = basebuilder::split(item,".");
            for_each(splitup.begin(),splitup.end(), [&quoteChar](string &n){ quoteChar + n + quoteChar;});
        }
    }
    
    return item;
}


