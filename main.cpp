#include <iostream>
#include <string>
#include <vector>

enum class TokenType {
  Operation,
  LeftBrace,
  RightBrace,
  Integer,
};

struct Token {
  TokenType Type;
  std::string Literal;

public:
  void Inspect() { std::cout << this->Literal; }
};

bool isInteger(const char &ch) {
  if (ch >= '0' && ch <= '9') {
    return true;
  }
  return false;
}

struct Tokenizer {
  int Position;

public:
  std::vector<Token> Tokenize(std::string input) {
    std::vector<Token> res;
    if (input[0] != '-') {
      res.push_back({TokenType::Operation, "+"});
    }
    while (this->Position < input.length()) {
      Token token;
      char ch = input[this->Position];
      if (ch == '+') {
        token.Type = TokenType::Operation;
        token.Literal = "+";
        res.push_back(token);
        this->Position++;
      } else if (ch == '-') {
        token.Type = TokenType::Operation;
        token.Literal = "-";
        res.push_back(token);
        this->Position++;
      } else if (ch == '*') {
        token.Type = TokenType::Operation;
        token.Literal = "*";
        res.push_back(token);
        this->Position++;
      } else if (ch == '/') {
        token.Type = TokenType::Operation;
        token.Literal = "/";
        res.push_back(token);
        this->Position++;
      } else if (ch == '(') {
        token.Type = TokenType::LeftBrace;
        token.Literal = "(";
        res.push_back(token);
        this->Position++;
        if (input[this->Position] != '-') {
          res.push_back({TokenType::Operation, "+"});
        }
      } else if (ch == ')') {
        token.Type = TokenType::RightBrace;
        token.Literal = ")";
        res.push_back(token);
        this->Position++;
      } else if (ch == ' ') {
        this->Position++;
      } else if (isInteger(ch)) {
        token.Type = TokenType::Integer;
        token.Literal = this->readInteger(input);
        res.push_back(token);
      } else {
        std::cout << "An error ocured while parsing input";
        std::abort();
      }
    }
    if (res[0].Literal != "-") {
    }
    return res;
  }

private:
  std::string readInteger(std::string &input) {
    int start = this->Position;
    int length = 0;
    while (isInteger(input[this->Position])) {
      this->Position++;
      length++;
    }
    return input.substr(start, length);
  }
};

struct Executer {
  int Position;
  std::vector<Token> &Tokenized;

public:
  int Execute() {
    int result = 0;
    int cur = 0;
    while (this->Position < this->Tokenized.size() &&
           this->Tokenized[this->Position].Type != TokenType::RightBrace) {
      if (this->Position == this->Tokenized.size() - 1 ||
          this->Tokenized[this->Position].Type != TokenType::Operation) {
        std::cout << "Invalid input, please check your expression";
      }
      std::string op = this->Tokenized[this->Position].Literal;
      this->Position++;
      int num = this->Compute();
      if (op == "+" || op == "-") {
        result += cur;
        cur = 0;
        if (op == "-") {
          cur -= num;
        } else {
          cur += num;
        }
      } else if (op == "*" || op == "/") {
        if (op == "*") {
          cur *= num;
        } else {
          cur /= num;
        }
      }
    }
    result += cur;
    this->Position++;
    return result;
  }

private:
  int Compute() {
    Token token = Tokenized[this->Position];
    this->Position++;
    if (token.Type == TokenType::Integer) {
      return std::stoi(token.Literal);
    } else if (token.Type == TokenType::LeftBrace) {
      return this->Execute();
    }
    std::cout << "Undefined token: " << token.Literal;
    std::abort();
  }
};

int main() {
  std::string input;
  std::cout << "Write your expression: ";
  std::getline(std::cin, input);
  Tokenizer tokenizer = {0};
  std::vector<Token> tokenized = tokenizer.Tokenize(input);
  for (auto el : tokenized) {
    el.Inspect();
  }
  std::cout << "\n";
  Executer executer = {0, tokenized};
  int result = executer.Execute();
  std::cout << "Result: " << result;
  return 0;
}
