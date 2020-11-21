#include <iostream>
#include <vector>
#include <regex>

/**
 * RegEx
 */
const std::string INTEGER_DECLARATION_STATEMENT = "([a-z])=\\w+";
const std::string STRING_DECLARATION_STATEMENT = R"(([a-z])=\"([^)]+)\")";
const std::string STRING_DECLARATION = R"(\"([^)]+)\")";
const std::string INTEGER_DECLARATION = "^[0-9]*$";

/**
 * Remove character
 *
 * @param string
 * @param letter
 */
void removeChar(std::string& string, char letter)
{
    string.erase(remove(string.begin(), string.end(), letter), string.end());
}

/**
 * Type
 */
class Type {
public:
    std::string name;
    std::string value;
    std::string type;
};

/**
 * ValueResult
 */
class ValueResult {
public:
    std::string name;
    std::string value;
    bool founded = false;
};

/**
 * UndefinedVariableException
 */
struct UndefinedVariableException : public std::exception {
public:
    std::string identifier;

    UndefinedVariableException(std::string identifier) {
        this->identifier = identifier;
    };

    const char * what () const throw () {
        return "Undefined variable";
    };
};

/**
 * UnsupportedVariableException
 */
struct UnsupportedVariableException : public std::exception {
public:
    std::string identifier;

    UnsupportedVariableException(std::string identifier) {
        this->identifier = identifier;
    };

    const char * what () const throw () {
        return "Undefined variable";
    };
};

/**
 * Variables
 */
std::vector<Type> variables;

/**
 * Print
 *
 * @param var
 */
void print(const std::string& var)
{
    std::cout << var;
}

/**
 * Native functions
 */
std::map<std::string, std::function<void(std::string)>> nativeFunctions =
{
    { "print", [](std::string param) { return print(param); }},
    { "sum", [](std::string param) { return print(param); }},
};

/**
 * Detect statement
 *
 * @param code
 * @return
 */
std::vector<std::string> detectStatements(const std::string& code)
{
    std::vector<std::string> statements;
    std::stringstream streams(code);
    std::string statement;
    while (getline(streams, statement, ';')) {
        statements.push_back(statement);
    }
    return statements;
}

/**
 * Check regex over string
 *
 * @param str
 * @param pattern
 * @return
 */
bool checkRegexOverString(const std::string& str, const std::string& pattern) {
    return std::regex_match(str, std::regex(pattern));
}

/**
 * Assign variable
 *
 * @param statement
 */
void assignVariable(const std::string& statement)
{
    std::string name;
    std::string token = "=";
    std::string value;
    std::string type;
    bool tokenFounded = false;

    for (char letter : statement) {
        if (letter == '=') { tokenFounded = true; continue; }

        if (tokenFounded)
            value.push_back(letter);
        else
            name.push_back(letter);
    }

    if (checkRegexOverString(value, STRING_DECLARATION)) {
        type = "string";
        removeChar(value, '"');
    } else if(checkRegexOverString(value, INTEGER_DECLARATION)) {
        type = "integer";
    } else {
        throw UnsupportedVariableException(name);
    }

    Type variable;
    variable.name = name;
    variable.value = value;
    variable.type = type;

    variables.push_back(variable);
}

/**
 * Detect stack
 *
 * @param statement
 * @return
 */
std::vector<std::string> detectStack(const std::string& statement)
{
    std::string instruction;
    std::vector<std::string> stack;
    bool tokenFounded = false;
    for (char letter : statement) {
        if (letter == '(' or letter == ')') {
            tokenFounded = true;
        } else {
            instruction.push_back(letter);
        }

        if (tokenFounded) {
            stack.push_back(instruction);
            instruction.clear();
            tokenFounded = false;
        }
    }
    return stack;
}

/**
 * Get value
 *
 * @param identifier
 * @return
 */
std::string getValue(const std::string& identifier)
{
    Type type;
    int index = 0;
    std::string value;
    bool founded = false;

    while (!founded && index < variables.size()) {
        type = variables.at(index);
        if (type.name == identifier) {
            value = type.value;
            founded = true;
        }
        index++;
    }

    ValueResult vr;

    vr.name = identifier;
    vr.founded = founded;

    if (founded) { vr.value = value; } else { throw UndefinedVariableException(identifier); }

    return value;
}

/**
 * Run stack
 *
 * @param stack
 */
void runStack(std::vector<std::string> stack) {
    std::string method = stack.at(0);
    std::string parameter = stack.at(1);
    nativeFunctions[method](getValue(parameter));
}

/**
 * Execute method
 *
 * @param statement
 */
void executeMethod(std::string& statement)
{
    std::vector<std::string> stack = detectStack(statement);
    runStack(stack);
}

/**
 * Run statement
 *
 * @param statement
 */
void runStatement(std::string statement)
{
    statement.erase(remove(statement.begin(), statement.end(), ' '), statement.end());
    if (checkRegexOverString(statement, INTEGER_DECLARATION_STATEMENT) or
        checkRegexOverString(statement, STRING_DECLARATION_STATEMENT)) {
        assignVariable(statement);
    } else {
        executeMethod(statement);
    }
}

/**
 * Parse code
 *
 * @param code
 * @return
 */
void parseCode(std::string code)
{
    std::vector<std::string> statements = detectStatements(code);
    for (auto & statement : statements) {
        runStatement(statement);
    }
}

/**
 * Main
 * 
 * @return
 */
int main()
{
    std::string string;
    std::getline(std::cin, string);
    parseCode(string);
    return 0;
}
