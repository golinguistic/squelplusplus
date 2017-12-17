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

// Takes any map of value handlers and adds an entry
void registerValueHandler(map<std::string, std::function<std::string()>>* valueHandlers, std::string type, function<std::string()> handler){
    if(valueHandlers->count(type))
        return;
    
    valueHandlers->insert(std::pair<string, std::function<std::string()>>(type, handler)); 
}

// Get value handler for given type
function<std::string()> getValueHandler(std::string value, std::map<std::string, std::function<std::string()>>* valueHandlers){
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


