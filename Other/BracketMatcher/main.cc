#include <iostream>
#include <stack>
#include <unordered_map>

bool isMatching(char opening, char closing)
{
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}

bool isValidParentheses(const std::string &s)
{
    std::stack<char> parenthesesStack;
    std::unordered_map<char, char> closingMap = {
        {')', '('},
        {']', '['},
        {'}', '{'}};

    for (char ch : s)
    {
        if (closingMap.find(ch) != closingMap.end())
        {
            // 当前字符是右括号
            if (parenthesesStack.empty() || parenthesesStack.top() != closingMap[ch])
            {
                // 如果栈为空或栈顶元素与当前右括号不匹配，则括号不匹配
                return false;
            }
            parenthesesStack.pop(); // 匹配成功，弹出栈顶元素
        }
        else
        {
            // 当前字符是左括号，入栈
            parenthesesStack.push(ch);
        }
    }

    // 如果最后栈为空，说明所有括号都匹配
    return parenthesesStack.empty();
}

int main()
{
    std::string str1 = "{[()]}";
    std::string str2 = "{[()]}}";

    if (isValidParentheses(str1))
    {
        std::cout << str1 << " 匹配成功\n";
    }
    else
    {
        std::cout << str1 << " 匹配失败\n";
    }

    if (isValidParentheses(str2))
    {
        std::cout << str2 << " 匹配成功\n";
    }
    else
    {
        std::cout << str2 << " 匹配失败\n";
    }

    return 0;
}
