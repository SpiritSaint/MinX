#include <iostream>
#include <vector>
#include <regex>

/**
 * RegEx of var declaration
 */
const std::string VAR_DECLARATION = "([a-z])=\\w+";

/**
 * Type
 */
class Type {
public:
    std::string name;
    std::string value;
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
    bool tokenFounded = false;

    for (char letter : statement) {
        if (letter == '=') { tokenFounded = true; continue; }

        if (tokenFounded)
            value.push_back(letter);
        else
            name.push_back(letter);
    }

    Type variable;
    variable.name = name;
    variable.value = value;

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
    while (!founded && index <= variables.size()) {
        type = variables.at(index);
        if (type.name == identifier) {
            value = type.value;
            founded = true;
        }
        index++;
    }
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
    if (checkRegexOverString(statement, VAR_DECLARATION)) {
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
    parseCode("a = awesome;c = 100;print(a);print(c);");
    return 0;
}
