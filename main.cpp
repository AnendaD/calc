#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

enum class TokenType : uint32_t {
  Plus = 1 << 0,
  Minus = 1 << 1,
  Star = 1 << 2,
  Slash = 1 << 3,
  LeftBrace = 1 << 4,
  RightBrace = 1 << 5,
  Number = 1 << 6,
};

struct Token {
  TokenType Type;
  std::string Literal;

public:
  void Inspect() { std::cout << this->Literal; }
};

bool isNumber(const char &ch) {
  if (ch >= '0' && ch <= '9') {
    return true;
  }
  return false;
}

bool isDot(const char &ch) {
  if (ch == '.') {
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
      res.push_back({TokenType::Plus, "+"});
    }
    while (this->Position < input.length()) {
      Token token;
      char ch = input[this->Position];
      if (ch == '+') {
        token.Type = TokenType::Plus;
        token.Literal = "+";
        res.push_back(token);
        this->Position++;
      } else if (ch == '-') {
        token.Type = TokenType::Minus;
        token.Literal = "-";
        res.push_back(token);
        this->Position++;
      } else if (ch == '*') {
        token.Type = TokenType::Star;
        token.Literal = "*";
        res.push_back(token);
        this->Position++;
      } else if (ch == '/') {
        token.Type = TokenType::Slash;
        token.Literal = "/";
        res.push_back(token);
        this->Position++;
      } else if (ch == '(') {
        token.Type = TokenType::LeftBrace;
        token.Literal = "(";
        res.push_back(token);
        this->Position++;
        if (input[this->Position] != '-') {
          res.push_back({TokenType::Plus, "+"});
        }
      } else if (ch == ')') {
        token.Type = TokenType::RightBrace;
        token.Literal = ")";
        res.push_back(token);
        this->Position++;
      } else if (ch == ' ') {
        this->Position++;
      } else if (isNumber(ch)) {
        token.Type = TokenType::Number;
        token.Literal = this->readNumber(input);
        res.push_back(token);
      } else {
        std::cout << "An error ocured while parsing input";
        std::abort();
      }
    }
    return res;
  }

private:
  std::string readNumber(std::string &input) {
    int start = this->Position;
    int length = 0;
    int dotCount = 0;
    while (isNumber(input[this->Position]) || isDot(input[this->Position])) {
      if (isDot(input[this->Position])) {
        dotCount++;
        if (dotCount > 1) {
          std::cout << "Invalid input";
          std::abort();
        }
      }
      this->Position++;
      length++;
    }
    return input.substr(start, length);
  }
};

struct Evaluator {
  int Position;
  std::vector<Token> &Tokenized;
  static constexpr uint32_t OperationMask = 0b1111;

public:
  double Evaluate() {
    double result = 0;
    double cur = 0;
    while (this->Position < this->Tokenized.size() &&
           this->Tokenized[this->Position].Type != TokenType::RightBrace) {
      if (this->Position == this->Tokenized.size() - 1 ||
          !(uint32_t(this->Tokenized[this->Position].Type) & OperationMask)) {
        std::cout << "Invalid input";
        std::abort();
      }
      Token op = this->Tokenized[this->Position];
      this->Position++;
      double num = this->Compute();
      if (op.Type == TokenType::Plus || op.Type == TokenType::Minus) {
        result += cur;
        cur = 0.0;
        if (op.Type == TokenType::Minus) {
          cur -= num;
        } else {
          cur += num;
        }
      } else if (op.Type == TokenType::Star || op.Type == TokenType::Slash) {
        if (op.Type == TokenType::Star) {
          cur *= num;
        } else {
          if (num == 0.0) {
            std::cout << "Invalid input: division dy zero!";
            std::abort();
          }
          cur /= num;
        }
      }
    }
    result += cur;
    this->Position++;
    return result;
  }

private:
  double Compute() {
    Token token = Tokenized[this->Position];
    this->Position++;
    if (token.Type == TokenType::Number) {
      return std::stod(token.Literal);
    } else if (token.Type == TokenType::LeftBrace) {
      return this->Evaluate();
    }
    std::cout << "Undefined token: " << token.Literal;
    std::abort();
  }
};

int main() {
  std::string input;
  std::cout << "Write your expression: ";
  std::getline(std::cin, input);
  if (input.empty()) {
    std::cout << "Empty input";
    std::abort();
  }
  Tokenizer tokenizer = {0};
  std::vector<Token> tokenized = tokenizer.Tokenize(input);
  for (auto el : tokenized) {
    el.Inspect();
  }
  std::cout << "\n";
  Evaluator evaluator = {0, tokenized};
  double result = evaluator.Evaluate();
  std::cout << "Result: " << result;
  return 0;
}
